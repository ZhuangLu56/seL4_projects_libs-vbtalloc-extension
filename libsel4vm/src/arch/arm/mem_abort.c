/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <autoconf.h>
#include <stdio.h>
#include <stdlib.h>

#include <sel4vm/guest_vm.h>
#include <sel4vm/boot.h>
#include "sel4vm/guest_memory.h"
#include "vm.h"
#include "mem_abort.h"
#include "fault.h"
#include "guest_memory.h"
#include <sel4/benchmark_utilisation_types.h>
#include <sel4vm/guest_vm_util.h>
#include <sel4vm/guest_vcpu_fault.h>

#define CONN2(a, b) a##b
#define CONN3(a, b, c) a##b##c
#ifdef CONFIG_LIB_SEL4VM_DEFER_MEMORY_MAP
#define FAULT_GET_WIDTH CONN3(fault, _get_width_size, _stage_2)
#else
#define FAULT_GET_WIDTH CONN2(fault, _get_width_size)
#endif

static int unhandled_memory_fault(vm_t *vm, vm_vcpu_t *vcpu, fault_t *fault)
{
    uintptr_t addr = fault_get_address(fault);
    size_t fault_size = FAULT_GET_WIDTH(fault);
    
    memory_fault_result_t fault_result = vm->mem.unhandled_mem_fault_handler(vm, vcpu, addr, fault_size,
                                                                             vm->mem.unhandled_mem_fault_cookie);
    switch (fault_result) {
    case FAULT_HANDLED:
        return 0;
    case FAULT_RESTART:
        restart_fault(fault);
        return 0;
    case FAULT_IGNORE:
        return ignore_fault(fault);
    case FAULT_ERROR:
        print_fault(fault);
        abandon_fault(fault);
        return -1;
    default:
        break;
    }
    return -1;
}

int handle_page_fault(vm_t *vm, vm_vcpu_t *vcpu, fault_t *fault)
{
    uintptr_t addr = fault_get_address(fault);
    size_t fault_size = FAULT_GET_WIDTH(fault);
    int err;

    memory_fault_result_t fault_result = vm_memory_handle_fault(vm, vcpu, addr, fault_size);
    switch (fault_result) {
    case FAULT_HANDLED:
        return 0;
    case FAULT_RESTART:
        restart_fault(fault);
        return 0;
    case FAULT_IGNORE:
        return ignore_fault(fault);
    case FAULT_ERROR:
        print_fault(fault);
        abandon_fault(fault);
        return -1;
    case FAULT_UNHANDLED:
        if (vm->mem.unhandled_mem_fault_handler) {
            err = unhandled_memory_fault(vm, vcpu, fault);
            if (err) {
                return -1;
            }
            return 0;
        }
    default:
        break;
        /* We don't have a memory reservation for the faulting address
         * We move onto the rest of the page fault handler */
    }

    print_fault(fault);
    abandon_fault(fault);
    return -1;
}

int handle_hvc_fault(vm_t *vm, fault_t *fault, vm_vcpu_t *vcpu)
{   
    vspace_t *vspace;
    int err;
    seL4_UserContext regs;
    seL4_CPtr tcb;
    uintptr_t addr = fault_get_address(fault);

    tcb = vm_get_vcpu_tcb(vcpu);
    err = seL4_TCB_ReadRegisters(tcb, false, 0, sizeof(regs) / sizeof(regs.pc), &regs);
    assert(!err);
    vspace = vm_get_vspace(vm);
    vspace->unmap_pages(vspace, (void *)regs.r0, 1, PAGE_BITS_4K, vm->vka);
    advance_vcpu_fault(vcpu);
    return 0;
}

int vm_guest_mem_abort_handler(vm_vcpu_t *vcpu)
{
    int err;
    fault_t *fault;
    fault = vcpu->vcpu_arch.fault;
    err = new_memory_fault(fault);
    if (err) {
        ZF_LOGE("Failed to initialise new fault");
        return -1;
    }
#ifdef CONFIG_KERNEL_BENCHMARK
    uint64_t *ipcbuffer = (uint64_t *)&(seL4_GetIPCBuffer()->msg[0]);
    seL4_BenchmarkResetThreadUtilisation(simple_get_tcb(vcpu->vm->simple));
    seL4_BenchmarkResetLog();
#endif

    if (fault->type < 2) {
        err = handle_page_fault(vcpu->vm, vcpu, fault);
    } 
    else {
        err = handle_hvc_fault(vcpu->vm, fault, vcpu);
    }
    
#ifdef CONFIG_KERNEL_BENCHMARK
    seL4_BenchmarkFinalizeLog();
    seL4_BenchmarkGetThreadUtilisation(simple_get_tcb(vcpu->vm->simple));
#endif
    if (err) {
        return VM_EXIT_HANDLE_ERROR;
    }
#ifdef CONFIG_KERNEL_BENCHMARK
    vcpu->vm->page_fault_num += 1;
    vcpu->vm->page_fault_tcb_utilisation += ipcbuffer[BENCHMARK_TCB_UTILISATION];
    vcpu->vm->page_fault_kernel_utilisation += ipcbuffer[BENCHMARK_TCB_KERNEL_UTILISATION];
    printf("user: %llu, kernel: %llu\n",
            vcpu->vm->page_fault_tcb_utilisation / vcpu->vm->page_fault_num,
            vcpu->vm->page_fault_kernel_utilisation / vcpu->vm->page_fault_num);
#endif
    return VM_EXIT_HANDLED;
}
