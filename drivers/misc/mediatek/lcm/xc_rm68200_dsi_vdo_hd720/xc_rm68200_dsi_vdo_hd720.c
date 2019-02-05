/*
 * Copyright (C) 2019,
 * parthibx24 <e.inxpired@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0+
 */

#include "lcm_drv.h"

/* Local Constants */
#define LCM_NAME "xc_rm68200_dsi_vdo_hd720"
#define LCM_ID (0x8000)
#define FRAME_WIDTH  (720)
#define FRAME_HEIGHT (1280)

/* REGFLAG_DELAY, used to trigger MDELAY,
 * REGFLAG_END_OF_TABLE, used to mark the end of LCM_setting_table.
 * their values dosen't matter until they,
 * match with any lcm registers.
 */
#define REGFLAG_DELAY (0xFF)
#define REGFLAG_END_OF_TABLE (0xDD)

/* Local Variables */
#define SET_RESET_PIN(v) (lcm_util.set_reset_pin((v)))
#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))

#define LCM_DBG_TAG "[LCM]"

#ifdef BUILD_LK
#define LCM_LOGD(str, args...) print(LCM_DBG_TAG "[%s][%s] " str, LCM_NAME, __func__, ##args)
#else
#define LCM_LOGD(str, args...) pr_info(LCM_DBG_TAG "[%s][%s] " str, LCM_NAME, __func__, ##args)
#endif

/* Local Functions */
#define dsi_set_cmdq_V3(para_tbl,size,force_update)         lcm_util.dsi_set_cmdq_V3(para_tbl,size,force_update)
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define read_reg_v2(cmd, buffer, buffer_size)	            lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)
#define write_regs(addr, pdata, byte_nums)	                lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)   lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define wrtie_cmd(cmd)	lcm_util.dsi_write_cmd(cmd)

/* LCM Driver Implementations */

static LCM_UTIL_FUNCS lcm_util = { 0 };

struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};

static struct LCM_setting_table lcm_initialization_setting[] =
{
    {0xFE,1,{0x01}},
    {0x00,1,{0x0A}},
    {0x24,1,{0xC0}},
    {0x25,1,{0x53}},
    {0x26,1,{0x00}},
    {0x27,1,{0x0A}},
    {0x29,1,{0x0A}},
    {0x2B,1,{0xE5}},
    {0x16,1,{0x52}},
    {0x2F,1,{0x44}},
    {0x34,1,{0x45}},
    {0x1B,1,{0x00}},
    {0x12,1,{0x08}},
    {0x1A,1,{0x06}},
    {0x52,1,{0x8A}},
    {0x53,1,{0x00}},
    {0x54,1,{0x8A}},
    {0x55,1,{0x00}},
    {0x5F,1,{0x12}},
    {0xFE,1,{0x03}},                                                                                                                                                                           
    {0x01,1,{0x14}},                                                                                                                                                                           
    {0x02,1,{0x01}},                                                                                                                                                                           
    {0x03,1,{0x00}},
    {0x04,1,{0x00}},
    {0x05,1,{0x00}},
    {0x06,1,{0x00}},
    {0x07,1,{0x05}},
    {0x08,1,{0x14}},
    {0x09,1,{0x06}},
    {0x0A,1,{0x00}},
    {0x0B,1,{0x00}},
    {0x0C,1,{0x00}},
    {0x0D,1,{0x00}},
    {0x0E,1,{0x0A}},
    {0x0F,1,{0x0B}},
    {0x10,1,{0x0C}},
    {0x11,1,{0x0D}},
    {0x12,1,{0x00}},
    {0x13,1,{0x7D}},
    {0x14,1,{0x00}},
    {0x15,1,{0xC5}},
    {0x16,1,{0x08}},
    {0x17,1,{0x0E}},
    {0x18,1,{0x0F}},
    {0x19,1,{0x10}},
    {0x1A,1,{0x11}},
    {0x1B,1,{0x00}},
    {0x1C,1,{0x7D}},
    {0x1D,1,{0x00}},
    {0x1E,1,{0x85}},
    {0x1F,1,{0x08}},
    {0x20,1,{0x00}},
    {0x21,1,{0x00}},
    {0x22,1,{0x0A}},
    {0x23,1,{0x10}},
    {0x24,1,{0x12}},
    {0x25,1,{0x2D}},
    {0x26,1,{0x00}},
    {0x27,1,{0x14}},
    {0x28,1,{0x16}},
    {0x29,1,{0x2D}},
    {0x2A,1,{0x00}},
    {0x2B,1,{0x00}},
    {0x2C,1,{0x00}},
    {0x2D,1,{0x00}},
    {0x2E,1,{0x00}},
    {0x2F,1,{0x00}},
    {0x30,1,{0x00}},
    {0x31,1,{0x00}},
    {0x32,1,{0x00}},
    {0x33,1,{0x00}},
    {0x34,1,{0x00}},
    {0x35,1,{0x00}},
    {0x36,1,{0x00}},
    {0x37,1,{0x00}},
    {0x38,1,{0x00}},
    {0x39,1,{0x00}},
    {0x3A,1,{0x00}},
    {0x3B,1,{0x00}},
    {0x3C,1,{0x00}},
    {0x3D,1,{0x00}},
    {0x3E,1,{0x00}},
    {0x3F,1,{0x00}},
    {0x40,1,{0x00}},
    {0x41,1,{0x00}},
    {0x42,1,{0x00}},
    {0x43,1,{0x00}},
    {0x44,1,{0x00}},
    {0x45,1,{0x00}},
    {0x46,1,{0x00}},
    {0x47,1,{0x00}},
    {0x48,1,{0x00}},
    {0x49,1,{0x00}},
    {0x4A,1,{0x00}},
    {0x4B,1,{0x00}},
    {0x4C,1,{0x00}},
    {0x4D,1,{0x00}},
    {0x4E,1,{0x00}},
    {0x4F,1,{0x00}},
    {0x50,1,{0x00}},
    {0x51,1,{0x00}},
    {0x52,1,{0x00}},
    {0x53,1,{0x00}},
    {0x54,1,{0x00}},
    {0x55,1,{0x00}},
    {0x56,1,{0x00}},
    {0x57,1,{0x00}},
    {0x58,1,{0x00}},
    {0x59,1,{0x00}},
    {0x5A,1,{0x00}},
    {0x5B,1,{0x00}},
    {0x5C,1,{0x00}},
    {0x5D,1,{0x00}},
    {0x5E,1,{0x00}},
    {0x5F,1,{0x00}},
    {0x60,1,{0x00}},
    {0x61,1,{0x00}},
    {0x62,1,{0x00}},
    {0x63,1,{0x00}},
    {0x64,1,{0x00}},
    {0x65,1,{0x00}},
    {0x66,1,{0x00}},
    {0x67,1,{0x00}},
    {0x68,1,{0x00}},
    {0x69,1,{0x00}},
    {0x6A,1,{0x00}},
    {0x6B,1,{0x00}},
    {0x6C,1,{0x00}},
    {0x6D,1,{0x00}},
    {0x6E,1,{0x00}},
    {0x6F,1,{0x00}},
    {0x70,1,{0x00}},
    {0x71,1,{0x00}},
    {0x72,1,{0x00}},
    {0x73,1,{0x00}},
    {0x74,1,{0x00}},
    {0x75,1,{0x00}},
    {0x76,1,{0x00}},
    {0x77,1,{0x00}},
    {0x78,1,{0x00}},
    {0x79,1,{0x00}},
    {0x7A,1,{0x00}},
    {0x7B,1,{0x00}},
    {0x7C,1,{0x00}},
    {0x7D,1,{0x00}},
    {0x7E,1,{0x8B}},
    {0x7F,1,{0x09}},
    {0x80,1,{0x0F}},
    {0x81,1,{0x0D}},
    {0x82,1,{0x05}},
    {0x83,1,{0x07}},
    {0x84,1,{0x3F}},
    {0x85,1,{0x3F}},
    {0x86,1,{0x3F}},
    {0x87,1,{0x3F}},
    {0x88,1,{0x3F}},
    {0x89,1,{0x3F}},
    {0x8A,1,{0x3F}},
    {0x8B,1,{0x3F}},
    {0x8C,1,{0x3F}},
    {0x8D,1,{0x3F}},
    {0x8E,1,{0x3F}},
    {0x8F,1,{0x3F}},
    {0x90,1,{0x3F}},
    {0x91,1,{0x01}},
    {0x92,1,{0x1C}},
    {0x93,1,{0x1D}},
    {0x94,1,{0x1D}},
    {0x95,1,{0x1C}},
    {0x96,1,{0x00}},
    {0x97,1,{0x3F}},
    {0x98,1,{0x3F}},
    {0x99,1,{0x3F}},
    {0x9A,1,{0x3F}},
    {0x9B,1,{0x3F}},
    {0x9C,1,{0x3F}},
    {0x9D,1,{0x3F}},
    {0x9E,1,{0x3F}},
    {0x9F,1,{0x3F}},
    {0xA0,1,{0x3F}},
    {0xA2,1,{0x3F}},
    {0xA3,1,{0x3F}},
    {0xA4,1,{0x3F}},
    {0xA5,1,{0x06}},
    {0xA6,1,{0x04}},
    {0xA7,1,{0x0C}},
    {0xA9,1,{0x0E}},
    {0xAA,1,{0x08}},
    {0xAB,1,{0x0A}},
    {0xAC,1,{0x07}},
    {0xAD,1,{0x05}},
    {0xAE,1,{0x0D}},
    {0xAF,1,{0x0F}},
    {0xB0,1,{0x09}},
    {0xB1,1,{0x8B}},
    {0xB2,1,{0x3F}},
    {0xB3,1,{0x3F}},
    {0xB4,1,{0x3F}},
    {0xB5,1,{0x3F}},
    {0xB6,1,{0x3F}},
    {0xB7,1,{0x3F}},
    {0xB8,1,{0x3F}},
    {0xB9,1,{0x3F}},
    {0xBA,1,{0x3F}},
    {0xBB,1,{0x3F}},
    {0xBC,1,{0x3F}},
    {0xBD,1,{0x3F}},
    {0xBE,1,{0x3F}},
    {0xBF,1,{0x00}},
    {0xC0,1,{0x1D}},
    {0xC1,1,{0x1C}},
    {0xC2,1,{0x1C}},
    {0xC3,1,{0x1D}},
    {0xC4,1,{0x01}},
    {0xC5,1,{0x3F}},
    {0xC6,1,{0x3F}},
    {0xC7,1,{0x3F}},
    {0xC8,1,{0x3F}},
    {0xC9,1,{0x3F}},
    {0xCA,1,{0x3F}},
    {0xCB,1,{0x3F}},
    {0xCC,1,{0x3F}},
    {0xCD,1,{0x3F}},
    {0xCE,1,{0x3F}},
    {0xCF,1,{0x3F}},
    {0xD0,1,{0x3F}},
    {0xD1,1,{0x3F}},
    {0xD2,1,{0x0A}},
    {0xD3,1,{0x08}},
    {0xD4,1,{0x0E}},
    {0xD5,1,{0x0C}},
    {0xD6,1,{0x04}},
    {0xD7,1,{0x06}},
    {0xDC,1,{0x02}},
    {0xDE,1,{0x11}},
    {0xFE,1,{0x0E}},
    {0x1B,1,{0x00}},
    {0x1C,1,{0x00}},
    {0x01,1,{0x75}},
    {0xFE,1,{0x04}},
    {0x60,1,{0x00}},
    {0x61,1,{0x19}},
    {0x62,1,{0x1D}},
    {0x63,1,{0x10}},
    {0x64,1,{0x07}},
    {0x65,1,{0x14}},
    {0x66,1,{0x0E}},
    {0x67,1,{0x0A}},
    {0x68,1,{0x16}},
    {0x69,1,{0x0C}},
    {0x6A,1,{0x0D}},
    {0x6B,1,{0x08}},
    {0x6C,1,{0x0F}},
    {0x6D,1,{0x0F}},
    {0x6E,1,{0x0A}},
    {0x6F,1,{0x00}},
    {0x70,1,{0x00}},
    {0x71,1,{0x19}},
    {0x72,1,{0x1D}},
    {0x73,1,{0x10}},
    {0x74,1,{0x07}},
    {0x75,1,{0x14}},
    {0x76,1,{0x0E}},
    {0x77,1,{0x0A}},
    {0x78,1,{0x16}},
    {0x79,1,{0x0C}},
    {0x7A,1,{0x0D}},
    {0x7B,1,{0x08}},
    {0x7C,1,{0x0F}},
    {0x7D,1,{0x0F}},
    {0x7E,1,{0x0A}},
    {0x7F,1,{0x00}},
    {0xFE,1,{0x00}},
    {0x11,0,{}},
    {REGFLAG_DELAY,120,{}},
    {0x29,0,{}},
    {REGFLAG_DELAY,20,{}},
    {REGFLAG_END_OF_TABLE, 0, {}}
};

static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
int i;
    for(i = 0; i < count; i++) {

        switch (table[i].cmd) {
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
            case REGFLAG_END_OF_TABLE :
                break;
            default:
                dsi_set_cmdq_V2(table[i].cmd, table[i].count, table[i].para_list, force_update);
       	}
    }
}

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}

static void lcm_get_params(LCM_PARAMS *params) {

    memset(params, 0, sizeof(LCM_PARAMS));

    params->type = 2;

    params->width = FRAME_WIDTH;
    params->height = FRAME_HEIGHT;

    params->dsi.mode = 1;
    params->dsi.PS = 2;
    params->dsi.LANE_NUM = 3;
    params->dsi.packet_size = 256;
    params->dsi.PLL_CLOCK = 244;

    params->dsi.intermediat_buffer_num = 0;

    params->dsi.noncont_clock = 1;
    params->dsi.noncont_clock_period = 1;

    params->dsi.data_format.color_order = 0;
    params->dsi.data_format.trans_seq = 0;
    params->dsi.data_format.padding = 0;
    params->dsi.data_format.format = 2;

    params->dsi.horizontal_sync_active = 8;
    params->dsi.horizontal_backporch = 16;
    params->dsi.horizontal_frontporch = 16;
    params->dsi.horizontal_active_pixel = FRAME_WIDTH;

    params->dsi.vertical_sync_active = 4;
    params->dsi.vertical_backporch = 12;
    params->dsi.vertical_frontporch = 16;
    params->dsi.vertical_active_line = FRAME_HEIGHT;

    params->dbi.te_mode = 0;
    params->dbi.te_edge_polarity = 0;

    params->dsi.esd_check_enable = 1;
    params->dsi.customization_esd_check_enable = 1;
    params->dsi.lcm_esd_check_table[0].cmd = 0x0A;
    params->dsi.lcm_esd_check_table[0].count = 1;
    params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9Cu;

}

static void lcm_init(void)
{
    LCM_LOGD("!");

    SET_RESET_PIN(1);
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(50);
    SET_RESET_PIN(1);
    MDELAY(120);

    push_table(lcm_initialization_setting,
        sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}

static unsigned int lcm_compare_id(void) {

    unsigned int data_array[16];
    unsigned int id = 0;
    unsigned char buffer[3];

    SET_RESET_PIN(1);
	MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(10);
    SET_RESET_PIN(1);
    MDELAY(10);

    data_array[0] = 0x33700;
    dsi_set_cmdq(data_array, 1, 1);

    read_reg_v2(0x04, buffer, 3);

    id = (buffer[0] << 8) | buffer[1];
    LCM_LOGD("Synced id is 0x%2x", id);

    return (LCM_ID == id) ? 1 : 0;
}

static void lcm_suspend(void)
{

    unsigned int data_array[16];

    data_array[0] = 0x11500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(100);

    data_array[0] = 0x280500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(50);

    data_array[0] = 0x100500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(50);

    data_array[0] = 0x14F1500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(50);

    // For debuging lcm_compare_id
    if(lcm_compare_id())
        LCM_LOGD("yay! lcm id is correct.");

}

/* Get LCM Driver Hooks */
LCM_DRIVER xc_rm68200_dsi_vdo_hd720_lcm_drv =
{
    .name           = LCM_NAME,
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_init,
    .compare_id     = lcm_compare_id,
};
