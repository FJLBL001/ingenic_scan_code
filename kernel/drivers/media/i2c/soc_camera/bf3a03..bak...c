/*
 * bf3a03 Camera Driver
 *
 * Copyright (C) 2014, Ingenic Semiconductor Inc.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

//#define DEBUG	1
#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/videodev2.h>
#include <media/v4l2-chip-ident.h>
#include <media/v4l2-subdev.h>
#include <media/soc_camera.h>
#include <media/soc_mediabus.h>


#define REG_CHIP_ID_HIGH        0xfc
#define REG_CHIP_ID_LOW         0xfd

#define CHIP_ID_HIGH            0x3a
#define CHIP_ID_LOW				0x03

/* Private v4l2 controls */
#define V4L2_CID_PRIVATE_BALANCE  (V4L2_CID_PRIVATE_BASE + 0)
#define V4L2_CID_PRIVATE_EFFECT  (V4L2_CID_PRIVATE_BASE + 1)

#define REG14				0x1e
#define REG14_HFLIP_IMG		0x60 /* Horizontal mirror image ON/OFF */
#define REG14_VFLIP_IMG     0x50 /* Vertical flip image ON/OFF */

/* whether sensor support high resolution (> vga) preview or not */
#define SUPPORT_HIGH_RESOLUTION_PRE		1

/*
 * Struct
 */
struct regval_list {
	u8 reg_num;
	u8 value;
};

struct mode_list {
	u8 index;
	const struct regval_list *mode_regs;
};

/* Supported resolutions */
enum bf3a03_width {
	W_QVGA	= 320,
	W_VGA	= 640,
};

enum bf3a03_height {
	H_QVGA	= 240,
	H_VGA	= 480,
};

struct bf3a03_win_size {
	char *name;
	enum bf3a03_width width;
	enum bf3a03_height height;
	const struct regval_list *regs;
};


struct bf3a03_priv {
	struct v4l2_subdev subdev;
	struct bf3a03_camera_info *info;
	enum v4l2_mbus_pixelcode cfmt_code;
	const struct bf3a03_win_size *win;
	int	model;
	u8 balance_value;
	u8 effect_value;
	u16	flag_vflip:1;
	u16	flag_hflip:1;
};

static inline int bf3a03_write_reg(struct i2c_client * client, unsigned char addr, unsigned char value)
{
	return i2c_smbus_write_byte_data(client, addr, value);
}
static inline char bf3a03_read_reg(struct i2c_client *client, unsigned char addr)
{
	char ret;
	ret = i2c_smbus_read_byte_data(client, addr);
	if (ret < 0)
		return ret;

	return ret;
}
/*
 * Registers settings
 */

#define ENDMARKER { 0xff, 0xff }

static const struct regval_list bf3a03_init_regs[] = {
{0x09,0x55},
{0x15,0x02},
{0x1e,0x40},
{0x06,0x78},
{0x21,0x00},
{0x3e,0x37},
{0x29,0x2b},
{0x27,0x98},
{0x16,0x25},
{0xd9,0x24},
{0xdf,0x2f},
{0x20,0x00},
{0xd2,0x18},
{0x11,0x10},
{0x1b,0x31},
{0x2f,0x4c},
{0x2b,0x00},
{0x92,0x09},
{0xcc,0x77},
{0xcd,0x7b},
{0xce,0x82},
{0xcf,0x98},
{0xc3,0xa0},
{0x3a,0x00},
{0x4a,0x98},
{0x12,0x00},
{0x13,0x08},
{0x01,0x00},
{0x02,0x00},
{0x8c,0x02},
{0x8d,0x4c},
{0x87,0x16},
{0x13,0x15},
{0x35,0x46},
{0x65,0x38},
{0x66,0x42},
{0x70,0x0f},
{0x3b,0x00},
{0x71,0x0c},
{0x73,0x27},
{0x75,0x88},
{0x76,0xd8},
{0x77,0x0a},
{0x78,0xff},
{0x79,0x14},
{0x7a,0x04},
{0x9e,0x04},
{0x7d,0x2a},
{0x39,0xa0},
{0x3f,0xa0},
{0x90,0x20},
{0x5f,0x00},
{0x40,0x22},
{0x41,0x23},
{0x42,0x28},
{0x43,0x25},
{0x44,0x1d},
{0x45,0x17},
{0x46,0x13},
{0x47,0x12},
{0x48,0x10},
{0x49,0x0d},
{0x4b,0x0b},
{0x4c,0x0b},
{0x4e,0x09},
{0x4f,0x07},
{0x50,0x06},
{0x24,0x40},
{0x97,0x40},
{0x25,0x88},
{0x81,0x00},
{0x82,0x18},
{0x83,0x30},
{0x84,0x20},
{0x85,0x38},
{0x86,0xff},
{0x94,0xf2},
{0x80,0x92},
{0x98,0xbf},
{0x89,0x15},
{0x8e,0x2c},
{0x8f,0x82},
{0x8a,0x32},
{0x8b,0x7a},
{0x96,0xb5},
{0x9a,0x30},
{0xf7,0x8a},
{0x5a,0xec},
{0x51,0x90},
{0x52,0x10},
{0x53,0x8d},
{0x54,0x88},
{0x57,0x82},
{0x58,0x8d},
{0x5a,0x7c},
{0x51,0x80},
{0x52,0x04},
{0x53,0x8d},
{0x54,0x88},
{0x57,0x82},
{0x58,0x8d},
{0x5a,0x6c},
{0x51,0x93},
{0x52,0x04},
{0x53,0x8a},
{0x54,0x88},
{0x57,0x02},
{0x58,0x8d},
{0xb0,0xa0},
{0xb1,0x26},
{0xb2,0x1c},
{0xb4,0xfd},
{0xb0,0x30},
{0xb1,0x80},
{0xb2,0x80},
{0xb4,0xf1},
{0x3c,0x40},
{0x56,0x48},
{0x4d,0x40},
{0x59,0x40},
{0xb4,0xe1},
{0x56,0x40},
{0x6a,0x81},
{0x23,0x33},
{0xa2,0x0b},
{0xa3,0x26},
{0xa4,0x08},
{0xa5,0x26},
{0xa7,0x1a},
{0xa8,0x14},
{0xa9,0x14},
{0xaa,0x14},
{0xab,0x16},
{0xac,0x44},
{0xad,0xf0},
{0xae,0x57},
{0xc5,0x66},
{0xc7,0x38},
{0xc8,0x0d},
{0xc9,0x16},
{0xd3,0x09},
{0xd4,0x15},
{0xd0,0x00},
{0xd1,0x00},
{0x69,0x20},
{0xf1,0x29},
	ENDMARKER,
};

/* qvga Center crop */
static const struct regval_list bf3a03_qvga_regs[] = {
		//subsample
	{0x12,0x10},
	ENDMARKER,
};

/* qvga zoom */
/*static const struct regval_list bf3a03_qvga_regs[] = {
	{0xfe,0x00},
	{0xfd,0x01},
	// crop window
	{0xfe,0x00},
	{0x90,0x01},
	{0x91,0x00},
	{0x92,0x00},
	{0x93,0x00},
	{0x94,0x00},
	{0x95,0x00},
	{0x96,0xf0},
	{0x97,0x01},
	{0x98,0x40},
	//subsample
	{0xfe,0x00},
	{0x99,0x55},
	{0x9a,0x06},
	{0x9b,0x00},
	{0x9c,0x00},
	{0x9d,0x01},
	{0x9e,0x00},
	{0x9f,0x00},
	{0xa0,0x00},
	{0xa1,0x01},
	{0xa2,0x00},
	// AWB
	{0xfe,0x00},
	{0xec,0x01},
	{0xed,0x02},
	{0xee,0x30},
	{0xef,0x48},
	{0xfe,0x01},
	{0x74,0x00},
	//// AEC
	{0xfe,0x01},
	{0x01,0x04},
	{0x02,0x60},
	{0x03,0x02},
	{0x04,0x48},
	{0x05,0x18},
	{0x06,0x4c},
	{0x07,0x14},
	{0x08,0x36},
	{0x0a,0xc0},
	{0x21,0x14},
	{0xfe,0x00},
	//// gamma
	{0xfe,0x00},
	{0xc3,0x11},
	{0xc4,0x20},
	{0xc5,0x30},
	{0xfe,0x00},
	ENDMARKER,
};*/

/* vga center crop */
static const struct regval_list bf3a03_vga_regs[] = {
	{0x12,0x00},
	ENDMARKER,
};

/* vga zoom */
/*static const struct regval_list bf3a03_vga_regs[] = {
	{0xfe,0x00},
	{0xfd,0x01},
	//// crop window
	{0xfe,0x00},
	{0x90,0x01},
	{0x91,0x00},
	{0x92,0x00},
	{0x93,0x00},
	{0x94,0x00},
	{0x95,0x01},
	{0x96,0xe0},
	{0x97,0x02},
	{0x98,0x80},
	//subsample
	{0xfe,0x00},
	{0x99,0x55},
	{0x9a,0x06},
	{0x9b,0x00},
	{0x9c,0x00},
	{0x9d,0x01},
	{0x9e,0x23},
	{0x9f,0x00},
	{0xa0,0x00},
	{0xa1,0x01},
	{0xa2,0x23},
	//// AWB
	{0xfe,0x00},
	{0xec,0x01},
	{0xed,0x02},
	{0xee,0x30},
	{0xef,0x48},
	{0xfe,0x01},
	{0x74,0x00},
	//// AEC
	{0xfe,0x01},
	{0x01,0x04},
	{0x02,0x60},
	{0x03,0x02},
	{0x04,0x48},
	{0x05,0x18},
	{0x06,0x4c},
	{0x07,0x14},
	{0x08,0x36},
	{0x0a,0xc0},
	{0x21,0x14},
	{0xfe,0x00},
	//// gamma
	{0xfe,0x00},
	{0xc3,0x11},
	{0xc4,0x20},
	{0xc5,0x30},
	{0xfe,0x00},
	ENDMARKER,
};*/

/*static const struct regval_list bf3a03_720P_regs[] = {
	// 720P init
	 {0xfe,0x00},
	 {0xb6,0x01},
	 {0xfd,0x00},

  //subsample
	 {0xfe,0x00},
	 {0x99,0x55},
	 {0x9a,0x06},
	 {0x9b,0x00},
	 {0x9c,0x00},
	 {0x9d,0x01},
	 {0x9e,0x23},
	 {0x9f,0x00},
	 {0xa0,0x00},
	 {0xa1,0x01},
	 {0xa2,0x23},
	 //crop window
	 {0x90,0x01},
	 {0x91,0x00},
	 {0x92,0x78},
	 {0x93,0x00},
	 {0x94,0x00},
	 {0x95,0x02},
	 {0x96,0xd0},
	 {0x97,0x05},
	 {0x98,0x00},
	 // AWB
	 {0xfe,0x00},
	 {0xec,0x02},
	 {0xed,0x04},
	 {0xee,0x60},
	 {0xef,0x90},
	 {0xfe,0x01},
	 {0x74,0x01},
	 // AEC
	 {0xfe,0x01},
	 {0x01,0x08},
	 {0x02,0xc0},
	 {0x03,0x04},
	 {0x04,0x90},
	 {0x05,0x30},
	 {0x06,0x98},
	 {0x07,0x28},
	 {0x08,0x6c},
	 {0x0a,0xc2},
	 {0x21,0x15},
	 {0xfe,0x00},
	 //banding setting 20fps fixed///
	 {0xfe,0x00},
	 {0x03,0x03},
	 {0x04,0xe8},
	 {0x05,0x01},
	 {0x06,0x56},
	 {0x07,0x00},
	 {0x08,0x32},
	 {0xfe,0x01},
	 {0x25,0x00},
	 {0x26,0xfa},
	 {0x27,0x04},
	 {0x28,0xe2}, //20fps
	 {0x29,0x04},
	 {0x2a,0xe2}, //16fps   5dc
	 {0x2b,0x04},
	 {0x2c,0xe2}, //16fps  6d6  5dc
	 {0x2d,0x04},
	 {0x2e,0xe2}, //8fps	bb8
	 {0x3c,0x00}, //8fps
	 {0xfe,0x00},
	ENDMARKER,
};
*/  

static const struct regval_list bf3a03_wb_auto_regs[] = {
	ENDMARKER,
};

static const struct regval_list bf3a03_wb_incandescence_regs[] = {
	ENDMARKER,
};

static const struct regval_list bf3a03_wb_daylight_regs[] = {
	ENDMARKER,
};

static const struct regval_list bf3a03_wb_fluorescent_regs[] = {
	ENDMARKER,
};

static const struct regval_list bf3a03_wb_cloud_regs[] = {
	ENDMARKER,
};

static const struct mode_list bf3a03_balance[] = {
	{0, bf3a03_wb_auto_regs}, {1, bf3a03_wb_incandescence_regs},
	{2, bf3a03_wb_daylight_regs}, {3, bf3a03_wb_fluorescent_regs},
	{4, bf3a03_wb_cloud_regs},
};


static const struct regval_list bf3a03_effect_normal_regs[] = {
	ENDMARKER,
};

static const struct regval_list bf3a03_effect_grayscale_regs[] = {
	ENDMARKER,
};

static const struct regval_list bf3a03_effect_sepia_regs[] = {
	ENDMARKER,
};

static const struct regval_list bf3a03_effect_colorinv_regs[] = {
	ENDMARKER,
};

static const struct regval_list bf3a03_effect_sepiabluel_regs[] = {
	ENDMARKER,
};

static const struct mode_list bf3a03_effect[] = {
	{0, bf3a03_effect_normal_regs}, {1, bf3a03_effect_grayscale_regs},
	{2, bf3a03_effect_sepia_regs}, {3, bf3a03_effect_colorinv_regs},
	{4, bf3a03_effect_sepiabluel_regs},
};

#define bf3a03_SIZE(n, w, h, r) \
	{.name = n, .width = w , .height = h, .regs = r }

static const struct bf3a03_win_size bf3a03_supported_win_sizes[] = {
	bf3a03_SIZE("QVGA", W_QVGA, H_QVGA, bf3a03_qvga_regs),
	bf3a03_SIZE("VGA", W_VGA, H_VGA, bf3a03_vga_regs),
//	bf3a03_SIZE("720P", W_720P, H_720P, bf3a03_720P_regs),
};

#define N_WIN_SIZES (ARRAY_SIZE(bf3a03_supported_win_sizes))

static const struct regval_list bf3a03_yuv422_regs[] = {
	ENDMARKER,
};

static const struct regval_list bf3a03_rgb565_regs[] = {
	ENDMARKER,
};

static enum v4l2_mbus_pixelcode bf3a03_codes[] = {
	V4L2_MBUS_FMT_YUYV8_2X8,
	V4L2_MBUS_FMT_YUYV8_1_5X8,
	V4L2_MBUS_FMT_JZYUYV8_1_5X8,
};

/*
 * Supported controls
 */
static const struct v4l2_queryctrl bf3a03_controls[] = {
	{
		.id		= V4L2_CID_PRIVATE_BALANCE,
		.type		= V4L2_CTRL_TYPE_MENU,
		.name		= "whitebalance",
		.minimum	= 0,
		.maximum	= 4,
		.step		= 1,
		.default_value	= 0,
	}, {
		.id		= V4L2_CID_PRIVATE_EFFECT,
		.type		= V4L2_CTRL_TYPE_MENU,
		.name		= "effect",
		.minimum	= 0,
		.maximum	= 4,
		.step		= 1,
		.default_value	= 0,
	},
	{
		.id		= V4L2_CID_VFLIP,
		.type		= V4L2_CTRL_TYPE_BOOLEAN,
		.name		= "Flip Vertically",
		.minimum	= 0,
		.maximum	= 1,
		.step		= 1,
		.default_value	= 0,
	}, {
		.id		= V4L2_CID_HFLIP,
		.type		= V4L2_CTRL_TYPE_BOOLEAN,
		.name		= "Flip Horizontally",
		.minimum	= 0,
		.maximum	= 1,
		.step		= 1,
		.default_value	= 0,
	},
};

/*
 * Supported balance menus
 */
static const struct v4l2_querymenu bf3a03_balance_menus[] = {
	{
		.id		= V4L2_CID_PRIVATE_BALANCE,
		.index		= 0,
		.name		= "auto",
	}, {
		.id		= V4L2_CID_PRIVATE_BALANCE,
		.index		= 1,
		.name		= "incandescent",
	}, {
		.id		= V4L2_CID_PRIVATE_BALANCE,
		.index		= 2,
		.name		= "fluorescent",
	},  {
		.id		= V4L2_CID_PRIVATE_BALANCE,
		.index		= 3,
		.name		= "daylight",
	},  {
		.id		= V4L2_CID_PRIVATE_BALANCE,
		.index		= 4,
		.name		= "cloudy-daylight",
	},

};

/*
 * Supported effect menus
 */
static const struct v4l2_querymenu bf3a03_effect_menus[] = {
	{
		.id		= V4L2_CID_PRIVATE_EFFECT,
		.index		= 0,
		.name		= "none",
	}, {
		.id		= V4L2_CID_PRIVATE_EFFECT,
		.index		= 1,
		.name		= "mono",
	}, {
		.id		= V4L2_CID_PRIVATE_EFFECT,
		.index		= 2,
		.name		= "sepia",
	},  {
		.id		= V4L2_CID_PRIVATE_EFFECT,
		.index		= 3,
		.name		= "negative",
	}, {
		.id		= V4L2_CID_PRIVATE_EFFECT,
		.index		= 4,
		.name		= "aqua",
	},
};


/*
 * General functions
 */
static struct bf3a03_priv *to_bf3a03(const struct i2c_client *client)
{
	return container_of(i2c_get_clientdata(client), struct bf3a03_priv,
			    subdev);
}

static int bf3a03_write_array(struct i2c_client *client,
			      const struct regval_list *vals)
{
	int ret = 0;
	/* u8 my_num; */
	/* struct regval_list *vals_old = vals; */
	while ((vals->reg_num != 0xff) || (vals->value != 0xff)) {
		ret = i2c_smbus_write_byte_data(client,
						vals->reg_num, vals->value);

//		msleep(100);
//		my_num = bf3a03_read_reg(client, vals->reg_num);
//		if (my_num != vals->value)
//		{
//			printk("vals_old->reg_num is 0x%x, vals_old->value is 0x%x, ret is 0x%x\n", vals->reg_num, vals->value, my_num);
//		}
		if (ret < 0)
			return ret;
		vals++;
	}
	return ret;
}

static int bf3a03_mask_set(struct i2c_client *client,
			   u8  reg, u8  mask, u8  set)
{
	s32 val = i2c_smbus_read_byte_data(client, reg);
	if (val < 0)
		return val;

	val &= ~mask;
	val |= set & mask;

	dev_vdbg(&client->dev, "masks: 0x%02x, 0x%02x", reg, val);

	return i2c_smbus_write_byte_data(client, reg, val);
}

 /* current use hardware reset */
#if 0
static int bf3a03_reset(struct i2c_client *client)
{
	int ret;
	ret = i2c_smbus_write_byte_data(client, 0x12, 0x80);
	if (ret)
		goto err;

	msleep(5);
err:
	dev_dbg(&client->dev, "%s: (ret %d)", __func__, ret);
	return ret;
}
#endif

/*
 * soc_camera_ops functions
 */
static int bf3a03_s_stream(struct v4l2_subdev *sd, int enable)
{
	return 0;
}

#if 0
static int bf3a03_set_bus_param(struct soc_camera_device *icd,
				unsigned long flags)
{
	struct soc_camera_link *icl = to_soc_camera_link(icd);
	unsigned long width_flag = flags & SOCAM_DATAWIDTH_MASK;

	/* Only one width bit may be set */
	if (!is_power_of_2(width_flag))
		return -EINVAL;

	if (icl->set_bus_param)
		return icl->set_bus_param(icl, width_flag);

	/*
	 * Without board specific bus width settings we support only the
	 * sensors native bus width which are tested working
	 */
	if (width_flag & SOCAM_DATAWIDTH_8)
		return 0;

	return 0;
}

static unsigned long bf3a03_query_bus_param(struct soc_camera_device *icd)
{
	struct soc_camera_link *icl = to_soc_camera_link(icd);
	unsigned long flags = SOCAM_PCLK_SAMPLE_FALLING | SOCAM_MASTER |
		SOCAM_VSYNC_ACTIVE_HIGH | SOCAM_HSYNC_ACTIVE_HIGH |
		SOCAM_DATA_ACTIVE_HIGH;

	if (icl->query_bus_param)
		flags |= icl->query_bus_param(icl) & SOCAM_DATAWIDTH_MASK;
	else
		flags |= SOCAM_DATAWIDTH_8;

	return soc_camera_apply_sensor_flags(icl, flags);
}
#endif
static int bf3a03_g_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl)
{
	struct i2c_client  *client = v4l2_get_subdevdata(sd);
	struct bf3a03_priv *priv = to_bf3a03(client);

	switch (ctrl->id) {
	case V4L2_CID_VFLIP:
		ctrl->value = priv->flag_vflip;
		break;
	case V4L2_CID_HFLIP:
		ctrl->value = priv->flag_hflip;
		break;
	case V4L2_CID_PRIVATE_BALANCE:
		ctrl->value = priv->balance_value;
		break;
	case V4L2_CID_PRIVATE_EFFECT:
		ctrl->value = priv->effect_value;
		break;
	default:
		break;
	}
	return 0;
}

static int bf3a03_s_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl)
{
	struct i2c_client  *client = v4l2_get_subdevdata(sd);
	struct bf3a03_priv *priv = to_bf3a03(client);
	int ret = 0;
	int i = 0;
	u8 value;

	int balance_count = ARRAY_SIZE(bf3a03_balance);
	int effect_count = ARRAY_SIZE(bf3a03_effect);

	switch (ctrl->id) {
	case V4L2_CID_PRIVATE_BALANCE:
		if(ctrl->value > balance_count)
			return -EINVAL;

		for(i = 0; i < balance_count; i++) {
			if(ctrl->value == bf3a03_balance[i].index) {
				ret = bf3a03_write_array(client,
						bf3a03_balance[ctrl->value].mode_regs);
				priv->balance_value = ctrl->value;
				break;
			}
		}
		break;

	case V4L2_CID_PRIVATE_EFFECT:
		if(ctrl->value > effect_count)
			return -EINVAL;

		for(i = 0; i < effect_count; i++) {
			if(ctrl->value == bf3a03_effect[i].index) {
				ret = bf3a03_write_array(client,
						bf3a03_effect[ctrl->value].mode_regs);
				priv->effect_value = ctrl->value;
				break;
			}
		}
		break;

	case V4L2_CID_VFLIP:
		value = ctrl->value ? REG14_VFLIP_IMG : 0x00;
		priv->flag_vflip = ctrl->value ? 1 : 0;
		ret = bf3a03_mask_set(client, REG14, REG14_VFLIP_IMG, value);
		break;

	case V4L2_CID_HFLIP:
		value = ctrl->value ? REG14_HFLIP_IMG : 0x00;
		priv->flag_hflip = ctrl->value ? 1 : 0;
		ret = bf3a03_mask_set(client, REG14, REG14_HFLIP_IMG, value);
		break;

	default:
		dev_err(&client->dev, "no V4L2 CID: 0x%x ", ctrl->id);
		return -EINVAL;
	}

	return ret;
}

static int bf3a03_g_chip_ident(struct v4l2_subdev *sd,
			       struct v4l2_dbg_chip_ident *id)
{
	id->ident    = SUPPORT_HIGH_RESOLUTION_PRE;
	id->revision = 0;

	return 0;
}

static int bf3a03_querymenu(struct v4l2_subdev *sd,
					struct v4l2_querymenu *qm)
{
	switch (qm->id) {
	case V4L2_CID_PRIVATE_BALANCE:
		memcpy(qm->name, bf3a03_balance_menus[qm->index].name,
				sizeof(qm->name));
		break;

	case V4L2_CID_PRIVATE_EFFECT:
		memcpy(qm->name, bf3a03_effect_menus[qm->index].name,
				sizeof(qm->name));
		break;
	}

	return 0;
}

#ifdef CONFIG_VIDEO_ADV_DEBUG
static int bf3a03_g_register(struct v4l2_subdev *sd,
			     struct v4l2_dbg_register *reg)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	int ret;

	reg->size = 1;
	if (reg->reg > 0xff)
		return -EINVAL;

	ret = i2c_smbus_read_byte_data(client, reg->reg);
	if (ret < 0)
		return ret;

	reg->val = ret;

	return 0;
}

static int bf3a03_s_register(struct v4l2_subdev *sd,
			     struct v4l2_dbg_register *reg)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	if (reg->reg > 0xff ||
	    reg->val > 0xff)
		return -EINVAL;

	return i2c_smbus_write_byte_data(client, reg->reg, reg->val);
}
#endif

/* Select the nearest higher resolution for capture */
static const struct bf3a03_win_size *bf3a03_select_win(u32 *width, u32 *height)
{
	int i, default_size = ARRAY_SIZE(bf3a03_supported_win_sizes) - 1;

	for (i = 0; i < ARRAY_SIZE(bf3a03_supported_win_sizes); i++) {
		if (bf3a03_supported_win_sizes[i].width  >= *width &&
		    bf3a03_supported_win_sizes[i].height >= *height) {
			*width = bf3a03_supported_win_sizes[i].width;
			*height = bf3a03_supported_win_sizes[i].height;
			return &bf3a03_supported_win_sizes[i];
		}
	}

	*width = bf3a03_supported_win_sizes[default_size].width;
	*height = bf3a03_supported_win_sizes[default_size].height;
	return &bf3a03_supported_win_sizes[default_size];
}
#if 0
static int bf3a03_init(struct v4l2_subdev *sd, u32 val)
{
	int ret;
	struct i2c_client  *client = v4l2_get_subdevdata(sd);
	struct bf3a03_priv *priv = to_bf3a03(client);

	int bala_index = priv->balance_value;
	int effe_index = priv->effect_value;

	/* initialize the sensor with default data */
	ret = bf3a03_write_array(client, bf3a03_init_regs);
	ret = bf3a03_write_array(client, bf3a03_balance[bala_index].mode_regs);
	ret = bf3a03_write_array(client, bf3a03_effect[effe_index].mode_regs);
	if (ret < 0)
		goto err;

	dev_info(&client->dev, "%s: Init default", __func__);
	return 0;

err:
	dev_err(&client->dev, "%s: Error %d", __func__, ret);
	return ret;
}


static int bf3a03_set_params(struct i2c_client *client, u32 *width, u32 *height,
			     enum v4l2_mbus_pixelcode code)
{
	struct bf3a03_priv *priv = to_bf3a03(client);
	const struct regval_list *selected_cfmt_regs;
	int ret;

	/* select win */
	priv->win = bf3a03_select_win(width, height);

	/* select format */
	priv->cfmt_code = 0;
	switch (code) {
	case V4L2_MBUS_FMT_YUYV8_2X8:
		dev_dbg(&client->dev, "%s: Selected cfmt YUV422", __func__);
		selected_cfmt_regs = bf3a03_rgb565_regs;
		break;
	default:
	case V4L2_MBUS_FMT_YUYV8_1_5X8:
		dev_dbg(&client->dev, "%s: Selected cfmt YUV422", __func__);
		selected_cfmt_regs = bf3a03_yuv422_regs;
	}

	/* initialize the sensor with default data */
	dev_info(&client->dev, "%s: Init default", __func__);
	ret = bf3a03_write_array(client, bf3a03_init_regs);
	if (ret < 0)
		goto err;

	/* set size win */
	ret = bf3a03_write_array(client, priv->win->regs);
	if (ret < 0)
		goto err;

	priv->cfmt_code = code;
	*width = priv->win->width;
	*height = priv->win->height;

	return 0;

err:
	dev_err(&client->dev, "%s: Error %d", __func__, ret);
	bf3a03_reset(client);
	priv->win = NULL;

	return ret;
}
#endif

static int bf3a03_g_fmt(struct v4l2_subdev *sd,
			struct v4l2_mbus_framefmt *mf)
{
	struct i2c_client  *client = v4l2_get_subdevdata(sd);
	struct bf3a03_priv *priv = to_bf3a03(client);

	mf->width = priv->win->width;
	mf->height = priv->win->height;
	mf->code = priv->cfmt_code;

	mf->colorspace = V4L2_COLORSPACE_JPEG;
	mf->field	= V4L2_FIELD_NONE;

	return 0;
}

static int bf3a03_s_fmt(struct v4l2_subdev *sd,
			struct v4l2_mbus_framefmt *mf)
{
	/* current do not support set format, use unify format yuv422i */
	struct i2c_client  *client = v4l2_get_subdevdata(sd);
	struct bf3a03_priv *priv = to_bf3a03(client);
	int ret;


	priv->win = bf3a03_select_win(&mf->width, &mf->height);
	/* set size win */
	ret = bf3a03_write_array(client, priv->win->regs);
	if (ret < 0) {
		dev_err(&client->dev, "%s: Error\n", __func__);
		return ret;
	}

	return 0;
}

static int bf3a03_try_fmt(struct v4l2_subdev *sd,
			  struct v4l2_mbus_framefmt *mf)
{
	const struct bf3a03_win_size *win;
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	/*
	 * select suitable win
	 */
	win = bf3a03_select_win(&mf->width, &mf->height);

	if(mf->field == V4L2_FIELD_ANY) {
		mf->field = V4L2_FIELD_NONE;
	} else if (mf->field != V4L2_FIELD_NONE) {
		dev_err(&client->dev, "Field type invalid.\n");
		return -ENODEV;
	}

	switch (mf->code) {
	case V4L2_MBUS_FMT_YUYV8_2X8:
	case V4L2_MBUS_FMT_YUYV8_1_5X8:
	case V4L2_MBUS_FMT_JZYUYV8_1_5X8:
		mf->colorspace = V4L2_COLORSPACE_JPEG;
		break;

	default:
		mf->code = V4L2_MBUS_FMT_YUYV8_2X8;
		break;
	}

	return 0;
}

static int bf3a03_enum_fmt(struct v4l2_subdev *sd, unsigned int index,
			   enum v4l2_mbus_pixelcode *code)
{
	if (index >= ARRAY_SIZE(bf3a03_codes))
		return -EINVAL;

	*code = bf3a03_codes[index];
	return 0;
}

static int bf3a03_g_crop(struct v4l2_subdev *sd, struct v4l2_crop *a)
{
	return 0;
}

static int bf3a03_cropcap(struct v4l2_subdev *sd, struct v4l2_cropcap *a)
{
	return 0;
}


/*
 * Frame intervals.  Since frame rates are controlled with the clock
 * divider, we can only do 30/n for integer n values.  So no continuous
 * or stepwise options.  Here we just pick a handful of logical values.
 */
#if 0
static int bf3a03_frame_rates[] = { 30, 15, 10, 5, 1 };

static int bf3a03_enum_frameintervals(struct v4l2_subdev *sd,
		struct v4l2_frmivalenum *interval)
{
	if (interval->index >= ARRAY_SIZE(bf3a03_frame_rates))
		return -EINVAL;
	interval->type = V4L2_FRMIVAL_TYPE_DISCRETE;
	interval->discrete.numerator = 1;
	interval->discrete.denominator = bf3a03_frame_rates[interval->index];
	return 0;
}


/*
 * Frame size enumeration
 */
static int bf3a03_enum_framesizes(struct v4l2_subdev *sd,
		struct v4l2_frmsizeenum *fsize)
{
	int i;
	int num_valid = -1;
	__u32 index = fsize->index;

	for (i = 0; i < N_WIN_SIZES; i++) {
		const struct bf3a03_win_size *win = &bf3a03_supported_win_sizes[index];
		if (index == ++num_valid) {
			fsize->type = V4L2_FRMSIZE_TYPE_DISCRETE;
			fsize->discrete.width = win->width;
			fsize->discrete.height = win->height;
			return 0;
		}
	}

	return -EINVAL;
}
#endif
static int bf3a03_video_probe(struct i2c_client *client)
{
	unsigned char retval_high = 0, retval_low = 0;
	/* struct v4l2_subdev *subdev = i2c_get_clientdata(client); */
	int ret = 0;

	struct soc_camera_subdev_desc *ssdd = soc_camera_i2c_to_desc(client);

	ret = soc_camera_power_on(&client->dev, ssdd);
	if (ret < 0)
		return ret;

	/*
	 * check and show product ID and manufacturer ID
	 */
//	client->addr = 0;
//	while(1){
//	client->addr++;
//	if (client->addr > 0xff)
//		break;
//	printk("client->addr is 0x%x\n\n", client->addr);
	retval_high = bf3a03_read_reg(client, REG_CHIP_ID_HIGH);
	if (retval_high != CHIP_ID_HIGH) {
		dev_err(&client->dev, "read sensor %s chip_id high %x is error\n",
				client->name, retval_high);
		return -1;
//	}else
//		printk("\n\n\n\nok\n\n\n\n");
//	msleep(300);
	}
	retval_low = bf3a03_read_reg(client, REG_CHIP_ID_LOW);
	if (retval_low != CHIP_ID_LOW) {
		dev_err(&client->dev, "read sensor %s chip_id low %x is error\n",
				client->name, retval_low);
		return -1;
	}
	dev_info(&client->dev, "read sensor %s id high:0x%x,low:%x successed!\n",
			client->name, retval_high, retval_low);

	ret = soc_camera_power_off(&client->dev, ssdd);

	return 0;
}

static int bf3a03_s_power(struct v4l2_subdev *sd, int on)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct soc_camera_subdev_desc *ssdd = soc_camera_i2c_to_desc(client);
	struct bf3a03_priv *priv = to_bf3a03(client);
	int ret;



	int bala_index = priv->balance_value;
	int effe_index = priv->effect_value;


	if (!on)
		return soc_camera_power_off(&client->dev, ssdd);

	ret = soc_camera_power_on(&client->dev, ssdd);
	if (ret < 0)
		return ret;


	///* initialize the sensor with default data */
	ret = bf3a03_write_array(client, bf3a03_init_regs);
	if (ret < 0)
		goto err;
	ret = bf3a03_write_array(client, bf3a03_balance[bala_index].mode_regs);
	if (ret < 0)
		goto err;
	ret = bf3a03_write_array(client, bf3a03_effect[effe_index].mode_regs);
	if (ret < 0)
		goto err;


	dev_dbg(&client->dev, "%s: Init default", __func__);
	return 0;

err:
	dev_err(&client->dev, "%s: Error %d", __func__, ret);
	return ret;

}

static int bf3a03_g_mbus_config(struct v4l2_subdev *sd,
		struct v4l2_mbus_config *cfg)
{

	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct soc_camera_subdev_desc *ssdd = soc_camera_i2c_to_desc(client);

	cfg->flags = V4L2_MBUS_PCLK_SAMPLE_FALLING | V4L2_MBUS_MASTER |
		    V4L2_MBUS_VSYNC_ACTIVE_HIGH | V4L2_MBUS_HSYNC_ACTIVE_HIGH |
			    V4L2_MBUS_DATA_ACTIVE_HIGH;
	cfg->type = V4L2_MBUS_PARALLEL;

	cfg->flags = soc_camera_apply_board_flags(ssdd, cfg);

	return 0;
}

#if 0
static struct soc_camera_ops bf3a03_ops = {
	.set_bus_param		= bf3a03_set_bus_param,
	.query_bus_param	= bf3a03_query_bus_param,
	.controls		= bf3a03_controls,
	.num_controls		= ARRAY_SIZE(bf3a03_controls),
};
#endif
static struct v4l2_subdev_core_ops bf3a03_subdev_core_ops = {
//	.init		= bf3a03_init,
	.s_power 	= bf3a03_s_power,
	.g_ctrl		= bf3a03_g_ctrl,
	.s_ctrl		= bf3a03_s_ctrl,
	.g_chip_ident	= bf3a03_g_chip_ident,
	.querymenu	= bf3a03_querymenu,
#ifdef CONFIG_VIDEO_ADV_DEBUG
	.g_register	= bf3a03_g_register,
	.s_register	= bf3a03_s_register,
#endif
};

static struct v4l2_subdev_video_ops bf3a03_subdev_video_ops = {
	.s_stream	= bf3a03_s_stream,
	.g_mbus_fmt	= bf3a03_g_fmt,
	.s_mbus_fmt	= bf3a03_s_fmt,
	.try_mbus_fmt	= bf3a03_try_fmt,
	.cropcap	= bf3a03_cropcap,
	.g_crop		= bf3a03_g_crop,
	.enum_mbus_fmt	= bf3a03_enum_fmt,
	.g_mbus_config  = bf3a03_g_mbus_config,
#if 0
	.enum_framesizes = bf3a03_enum_framesizes,
	.enum_frameintervals = bf3a03_enum_frameintervals,
#endif
};

static struct v4l2_subdev_ops bf3a03_subdev_ops = {
	.core	= &bf3a03_subdev_core_ops,
	.video	= &bf3a03_subdev_video_ops,
};

/*
 * i2c_driver functions
 */

static int bf3a03_probe(struct i2c_client *client,
			const struct i2c_device_id *did)
{
	struct bf3a03_priv *priv;
	struct soc_camera_subdev_desc *ssdd = soc_camera_i2c_to_desc(client);
	struct i2c_adapter *adapter = to_i2c_adapter(client->dev.parent);
	int ret = 0,default_wight = 640,default_height = 480;

	if (!ssdd) {
		dev_err(&client->dev, "bf3a03: missing platform data!\n");
		return -EINVAL;
	}


	if(!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE
			| I2C_FUNC_SMBUS_BYTE_DATA)) {
		dev_err(&client->dev, "client not i2c capable\n");
		return -ENODEV;
	}

	priv = kzalloc(sizeof(struct bf3a03_priv), GFP_KERNEL);
	if (!priv) {
		dev_err(&adapter->dev,
			"Failed to allocate memory for private data!\n");
		return -ENOMEM;
	}


	v4l2_i2c_subdev_init(&priv->subdev, client, &bf3a03_subdev_ops);

	priv->win = bf3a03_select_win(&default_wight, &default_height);

	priv->cfmt_code  =  V4L2_MBUS_FMT_YUYV8_2X8;


	ret = bf3a03_video_probe(client);
	if (ret) {
		kfree(priv);
	}
	return ret;
}

static int bf3a03_remove(struct i2c_client *client)
{
	struct bf3a03_priv       *priv = to_bf3a03(client);
//	struct soc_camera_device *icd = client->dev.platform_data;

//	icd->ops = NULL;
	kfree(priv);
	return 0;
}

static const struct i2c_device_id bf3a03_id[] = {
	{ "bf3a03",  0 },
	{ }
};


MODULE_DEVICE_TABLE(i2c, bf3a03_id);

static struct i2c_driver bf3a03_i2c_driver = {
	.driver = {
		.name = "bf3a03",
	},
	.probe    = bf3a03_probe,
	.remove   = bf3a03_remove,
	.id_table = bf3a03_id,
};

/*
 * Module functions
 */
static int __init bf3a03_module_init(void)
{
	return i2c_add_driver(&bf3a03_i2c_driver);
}

static void __exit bf3a03_module_exit(void)
{
	i2c_del_driver(&bf3a03_i2c_driver);
}

module_init(bf3a03_module_init);
module_exit(bf3a03_module_exit);

MODULE_DESCRIPTION("camera sensor bf3a03 driver");
MODULE_AUTHOR("cxtan <cxtan@ingenic.com>");
MODULE_LICENSE("GPL");
