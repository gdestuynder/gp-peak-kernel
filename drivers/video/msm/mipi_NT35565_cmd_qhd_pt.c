/* Copyright (c) 2012, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "msm_fb.h"
#include "mipi_dsi.h"
#include "mipi_NT35565.h"

static struct msm_panel_info pinfo;

static struct mipi_dsi_phy_ctrl dsi_cmd_mode_phy_db = {
	/* regulator */
	{0x03, 0x01, 0x01, 0x00},
	/* timing */
	{0xca, 0xa1, 0x36, 0x42, 0xb8, 0xb2, 0x48, 
	0xbc, 0x48, 0x38, 0x40}, 
	/* phy ctrl */
	{0x7f, 0x00, 0x00, 0x00}, 
	/* strength */ 
	{0xee, 0x02, 0x86, 0x00}, 
	/* pll control */ 
	{0x40, 0xf9, 0xb1, 0xda, 0x00, 0x50, 0x48, 0x63, 
	0x30, 0x07, 0x03,
	0x05, 0x14, 0x03, 0x0, 0x0, 0x54, 0x06, 0x10, 0x04, 0x0},
};

static int mipi_cmd_nt35565_qhd_pt_init(void)
{
	int ret;

	if (msm_fb_detect_client("mipi_cmd_nt35565_qhd"))
		return 0;

	pinfo.xres = 540;
	pinfo.yres = 960;
	pinfo.type = MIPI_CMD_PANEL;
	pinfo.pdest = DISPLAY_1;
	pinfo.wait_cycle = 0;
	pinfo.bpp = 24;
	pinfo.lcdc.h_back_porch = 50;
	pinfo.lcdc.h_front_porch = 50;
	pinfo.lcdc.h_pulse_width = 20;
	pinfo.lcdc.v_back_porch = 11;
	pinfo.lcdc.v_front_porch = 10;
	pinfo.lcdc.v_pulse_width = 5;

	pinfo.lcdc.border_clr = 0;	/* blk */
	pinfo.lcdc.underflow_clr = 0xff;	/* blue */
	pinfo.lcdc.hsync_skew = 0;
	pinfo.bl_max = 20;
	pinfo.bl_min = 0;
	pinfo.fb_num = 2;

	pinfo.clk_rate = 499000000;
	pinfo.lcd.refx100 = 6100; /* adjust refx100 to prevent tearing */

	pinfo.mipi.mode = DSI_CMD_MODE;
	pinfo.mipi.dst_format = DSI_CMD_DST_FORMAT_RGB888;
	pinfo.mipi.vc = 0;
	pinfo.mipi.rgb_swap = DSI_RGB_SWAP_RGB;
	pinfo.mipi.data_lane0 = TRUE;
	pinfo.mipi.data_lane1 = TRUE;
	pinfo.mipi.t_clk_post = 0x22;
	pinfo.mipi.t_clk_pre = 0x3f;
	pinfo.mipi.stream = 0; /* dma_p */
	pinfo.mipi.mdp_trigger = DSI_CMD_TRIGGER_SW;
	pinfo.mipi.dma_trigger = DSI_CMD_TRIGGER_SW;
	pinfo.mipi.te_sel = 1; /* TE from vsync gpio */
	pinfo.mipi.interleave_max = 1;
	pinfo.mipi.insert_dcs_cmd = TRUE;
	pinfo.mipi.wr_mem_continue = 0x3c;
	pinfo.mipi.wr_mem_start = 0x2c;
	pinfo.mipi.dsi_phy_db = &dsi_cmd_mode_phy_db;
	pinfo.mipi.tx_eot_append = 0x01;
	pinfo.mipi.rx_eot_ignore = 0x0;
	pinfo.mipi.dlane_swap = 0x01;

	ret = mipi_nt35565_device_register(&pinfo, MIPI_DSI_PRIM,
						MIPI_DSI_PANEL_QHD_PT);
	if (ret)
		pr_err("%s: failed to register device!\n", __func__);

	return ret;
}

module_init(mipi_cmd_nt35565_qhd_pt_init);
