/*
 * Copyright (C) 2017 Álvaro Fernández Rojas <noltari@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <led.h>
#include <asm/io.h>
#include <dm/lists.h>
#include <i2c.h>

DECLARE_GLOBAL_DATA_PTR;

struct aw2013_led_priv {
	bool led_state;
	int  reg_on;
};

static enum led_state_t aw2013_led_get_state(struct udevice *dev)
{
	struct aw2013_led_priv *priv = dev_get_priv(dev);

	if(priv->led_state == LEDST_ON)
		return LEDST_ON;
	else
		return LEDST_OFF;
	return 0;
}

static int aw2013_led_set_state(struct udevice *dev, enum led_state_t state)
{
	struct aw2013_led_priv *priv = dev_get_priv(dev);
	int ret = 0;

	debug("%s %s:%d reg_on = 0x%x\n",dev->name,__func__,__LINE__,priv->reg_on);

	switch (state) {
	case LEDST_OFF:
		priv->led_state = state;
		break;
	case LEDST_ON:
		priv->led_state = state;
		break;
	case LEDST_TOGGLE:
		if (priv->led_state == LEDST_ON)
			priv->led_state = LEDST_OFF;
		else
			priv->led_state = LEDST_ON;
		break;
	default:
		return -ENOSYS;
	}

	if( priv->led_state == LEDST_ON )
		ret = dm_i2c_reg_write(dev->parent,priv->reg_on,0xff);
	else
		ret = dm_i2c_reg_write(dev->parent,priv->reg_on,0);

	if ( ret < 0 )
		printf("%s:%d error",__func__,__LINE__);

	return ret;
}

static const struct led_ops aw2013_led_ops = {
	.get_state = aw2013_led_get_state,
	.set_state = aw2013_led_set_state,
};

static int init_aw2013(struct udevice *dev)
{
	int ret;
	ret = dm_i2c_reg_write(dev,0,0x55); //reset
	ret |= dm_i2c_reg_write(dev,1,1);    //enable
	ret |= dm_i2c_reg_write(dev,0x30,7); //enable chanel
	udelay(1000);
	return ret;
}

static int aw2013_led_probe(struct udevice *dev)
{
	int ret = 0;
	struct aw2013_led_priv *priv = dev_get_priv(dev);
	if ( !strcmp(dev->name, "aw2013@45")) {
		return  init_aw2013(dev);
	}

	if(!priv) {
		printf("error %s:%d\n",__func__,__LINE__);
	}

	if (!strcmp(dev->name,"aw2013,green")) {
		ret=dm_i2c_reg_write(dev->parent,0x32,1); //set current
		priv->reg_on = 0x35;
	}else if (!strcmp(dev->name,"aw2013,blue")){
		ret=dm_i2c_reg_write(dev->parent,0x33,1); //set current
		priv->reg_on = 0x36;
	}else{
		ret=dm_i2c_reg_write(dev->parent,0x31,1); //set current
		priv->reg_on = 0x34;
	}

	udelay(1000);
	debug("led: %s:%d name:%s ,reg_on:0x%x \n",
	      __func__,__LINE__,dev->name,priv->reg_on);
	return ret;
}

static int aw2013_led_bind(struct udevice *parent)
{
	struct udevice *dev;
	ofnode node;
	int ret;

	debug("led: %s -- %d parentname--%s\n",
	      __func__,__LINE__,parent->name);
	dev_for_each_subnode(node, parent) {
		struct led_uc_plat *uc_plat;
		const char *label;

		label = ofnode_read_string(node,"aw2013,name");
		if (!label) {
			debug("%s: node %s has no label\n", __func__,
			      ofnode_get_name(node));
			return -EINVAL;
		}

		ret = device_bind_driver_to_node(parent, "aw2013-led",
						 ofnode_get_name(node), node, &dev);
		if (ret)
			return ret;
		uc_plat = dev_get_uclass_platdata(dev);
		uc_plat->label = label;
	}
	return 0;
}

static const struct udevice_id aw2013_led_ids[] = {
	{ .compatible = "awinic,aw2013" },
	{ /* sentinel */ }
};

U_BOOT_DRIVER(aw2013_led) = {
	.name = "aw2013-led",
	.id = UCLASS_LED,
	.of_match = aw2013_led_ids,
	.ops = &aw2013_led_ops,
	.bind = aw2013_led_bind,
	.probe = aw2013_led_probe,
	.priv_auto_alloc_size = sizeof(struct aw2013_led_priv),
};
