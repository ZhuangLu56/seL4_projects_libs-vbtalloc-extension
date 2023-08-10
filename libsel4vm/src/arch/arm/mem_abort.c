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

static int unhandled_memory_fault(vm_t *vm, vm_vcpu_t *vcpu, fault_t *fault)
{
    uintptr_t addr = fault_get_address(fault);

    size_t fault_size;
    if (config_set(CONFIG_LIB_SEL4VM_DEFER_MEMORY_MAP)) {
        fault_size = fault_get_width_size_stage_2(fault);
    } else {
        fault_size = fault_get_width_size(fault);
    }
    
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
    int err;
    uintptr_t addr = fault_get_address(fault);

    size_t fault_size;
    if (config_set(CONFIG_LIB_SEL4VM_DEFER_MEMORY_MAP)) {
        fault_size = fault_get_width_size_stage_2(fault);
    } else {
        fault_size = fault_get_width_size(fault);
    }

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
    err = handle_page_fault(vcpu->vm, vcpu, fault);
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
    printf("kernel: %llu, user: %llu\n",
            vcpu->vm->page_fault_tcb_utilisation / vcpu->vm->page_fault_num,
            vcpu->vm->page_fault_kernel_utilisation / vcpu->vm->page_fault_num);
#endif
    return VM_EXIT_HANDLED;
}
