/*
 * Copyright 2014, NICTA
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(NICTA_BSD)
 */

#include <sel4arm-vmm/exynos/device_map.h>
#include <sel4arm-vmm/devices.h>

const struct device dev_i2c1 = {
    .devid = DEV_CUSTOM,
    .name = "i2c1",
    .pstart = I2C1_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};


const struct device dev_i2c2 = {
    .devid = DEV_CUSTOM,
    .name = "i2c2",
    .pstart = I2C2_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};


const struct device dev_i2c4 = {
    .devid = DEV_CUSTOM,
    .name = "i2c4",
    .pstart = I2C4_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};

const struct device dev_i2chdmi = {
    .devid = DEV_CUSTOM,
    .name = "i2c_hdmi",
    .pstart = I2C_HDMI_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};


const struct device dev_usb2_ohci = {
    .devid = DEV_CUSTOM,
    .name = "usb2.OHCI",
    .pstart = USB2_HOST_OHCI_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};


const struct device dev_usb2_ehci = {
    .devid = DEV_CUSTOM,
    .name = "usb2.EHCI",
    .pstart = USB2_HOST_EHCI_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};


const struct device dev_usb2_ctrl = {
    .devid = DEV_CUSTOM,
    .name = "usb2.ctrl",
    .pstart = USB2_HOST_CTRL_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};

const struct device dev_ps_cmu_core = {
    .devid = DEV_CUSTOM,
    .name = "CMU_CORE",
    .pstart = CMU_CORE_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};

const struct device dev_ps_cmu_cpu = {
    .devid = DEV_CUSTOM,
    .name = "CMU_CPU",
    .pstart = CMU_CPU_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};

const struct device dev_ps_cmu_cdrex = {
    .devid = DEV_CUSTOM,
    .name = "CMU_CDREX",
    .pstart = CMU_CDREX_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};

const struct device dev_ps_cmu_mem = {
    .devid = DEV_CUSTOM,
    .name = "CMU_MEM",
    .pstart = CMU_MEM_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};

const struct device dev_ps_cmu_isp = {
    .devid = DEV_CUSTOM,
    .name = "CMU_ISP",
    .pstart = CMU_ISP_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};

const struct device dev_ps_cmu_acp = {
    .devid = DEV_CUSTOM,
    .name = "CMU_ACP",
    .pstart = CMU_ACP_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};

const struct device dev_ps_chip_id = {
    .devid = DEV_CUSTOM,
    .name = "chipid",
    .pstart = CHIP_ID_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};

/* Video */
const struct device dev_ps_tx_mixer = {
    .devid = DEV_CUSTOM,
    .name = "tv_mixer",
    .pstart = TV_MIXER_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};

const struct device dev_ps_hdmi0 = {
    .devid = DEV_CUSTOM,
    .name = "hdmi0",
    .pstart = HDMI_0_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};

const struct device dev_ps_hdmi1 = {
    .devid = DEV_CUSTOM,
    .name = "hdmi1",
    .pstart = HDMI_1_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};

const struct device dev_ps_hdmi2 = {
    .devid = DEV_CUSTOM,
    .name = "hdmi2",
    .pstart = HDMI_2_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};

const struct device dev_ps_hdmi3 = {
    .devid = DEV_CUSTOM,
    .name = "hdmi3",
    .pstart = HDMI_3_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};

const struct device dev_ps_hdmi4 = {
    .devid = DEV_CUSTOM,
    .name = "hdmi4",
    .pstart = HDMI_4_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};

const struct device dev_ps_hdmi5 = {
    .devid = DEV_CUSTOM,
    .name = "hdmi5",
    .pstart = HDMI_5_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};

const struct device dev_ps_hdmi6 = {
    .devid = DEV_CUSTOM,
    .name = "hdmi6",
    .pstart = HDMI_6_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};

/* DMA */
const struct device dev_ps_mdma0 = {
    .devid = DEV_CUSTOM,
    .name = "MDMA0",
    .pstart = NS_MDMA0_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};

const struct device dev_ps_mdma1 = {
    .devid = DEV_CUSTOM,
    .name = "MDMA1",
    .pstart = NS_MDMA1_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};

const struct device dev_ps_pdma0 = {
    .devid = DEV_CUSTOM,
    .name = "PDMA0",
    .pstart = PDMA0_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};

const struct device dev_ps_pdma1 = {
    .devid = DEV_CUSTOM,
    .name = "PDMA1",
    .pstart = PDMA1_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};


/* Timers */
const struct device dev_wdt_timer = {
    .devid = DEV_WDT_TIMER,
    .name = "wdt",
    .pstart = WDT_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};

const struct device dev_ps_pwm_timer = {
    .devid = DEV_CUSTOM,
    .name = "pwm",
    .pstart = PWM_PADDR,
    .size = 0x1000,
    .handle_page_fault = NULL,
    .priv = NULL
};


