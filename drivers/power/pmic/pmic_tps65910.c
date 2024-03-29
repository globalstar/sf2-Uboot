/*
 * (C) Copyright 2011-2013
 * Texas Instruments, <www.ti.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <i2c.h>
#include <power/tps65910.h>

/*
 * tps65910_set_i2c_control() - Set the TPS65910 to be controlled via the I2C
 * 				interface.
 * @return:		       0 on success, not 0 on failure
 */
int tps65910_set_i2c_control(void)
{
	int ret;
	uchar buf;

	/* VDD1/2 voltage selection register access by control i/f */
	ret = i2c_read(TPS65910_CTRL_I2C_ADDR, TPS65910_DEVCTRL_REG, 1,
		       &buf, 1);

	if (ret)
		return ret;

	buf |= TPS65910_DEVCTRL_REG_SR_CTL_I2C_SEL_CTL_I2C;

	return i2c_write(TPS65910_CTRL_I2C_ADDR, TPS65910_DEVCTRL_REG, 1,
			 &buf, 1);
}

/*
 * tps65910_set_dev_on() - Set the TPS65910 DEV_ON bit of the DEVCTRL_REG

 * @return:		       0 on success, not 0 on failure
 */
int tps65910_set_dev_on(void)
{
	int ret;
	uchar buf;

	ret = i2c_read(TPS65910_CTRL_I2C_ADDR, TPS65910_DEVCTRL_REG, 1,
		       &buf, 1);

	if (ret)
		return ret;

	buf |= TPS65910_DEVCTRL_REG_DEV_ON;

	return i2c_write(TPS65910_CTRL_I2C_ADDR, TPS65910_DEVCTRL_REG, 1,
			 &buf, 1);
}

/*
 * tps65910_clear_dev_on() - Clear the TPS65910 DEV_ON bit of the DEVCTRL_REG

 * @return:		       0 on success, not 0 on failure
 */
int tps65910_clear_dev_on(void)
{
	int ret;
	uchar buf;

	ret = i2c_read(TPS65910_CTRL_I2C_ADDR, TPS65910_DEVCTRL_REG, 1,
		       &buf, 1);

	if (ret)
		return ret;

	buf &= ~TPS65910_DEVCTRL_REG_DEV_ON;

	return i2c_write(TPS65910_CTRL_I2C_ADDR, TPS65910_DEVCTRL_REG, 1,
			 &buf, 1);
}

/*
 * tps65910_get_devctrl_reg() - Read the TPS65910 DEVCTRL_REG register
 * @return:		       0 to 0x7F on success, 0x80 on failure
 */
int tps65910_get_devctrl_reg(void)
{
	int ret;
	uchar buf;

	ret = i2c_read(TPS65910_CTRL_I2C_ADDR, TPS65910_DEVCTRL_REG, 1,
		       &buf, 1);

	if (ret)
		return 0x80;

	return buf;
}

/*
 * tps65910_get_bck1_reg() - Read the TPS65910 BCK1_REG register
 * @return:		       0 to 0xFF on success, 0 on failure
 */
int tps65910_get_bck1_reg(void)
{
	int ret;
	uchar buf;

	ret = i2c_read(TPS65910_CTRL_I2C_ADDR, TPS65910_BCK1_REG, 1,
		       &buf, 1);

	if (ret)
		return 0x0;

	return buf;
}

/*
 * tps65910_set_bck1_reg() - Set the TPS65910 BCK1_REG register
 * @buf:	               value to set BCK1_REG
 * @return:		       0 on success, not 0 on failure
 */
int tps65910_set_bck1_reg(unsigned char value)
{
	uchar buf = value;


	return i2c_write(TPS65910_CTRL_I2C_ADDR, TPS65910_BCK1_REG, 1,
			 &buf, 1);
}

/*
 * tps65910_voltage_update() - Voltage switching for MPU frequency switching.
 * @module:		       mpu - 0, core - 1
 * @vddx_op_vol_sel:	       vdd voltage to set
 * @return:		       0 on success, not 0 on failure
 */
int tps65910_voltage_update(unsigned int module, unsigned char vddx_op_vol_sel)
{
	uchar buf;
	unsigned int reg_offset;
	int ret;

	if (module == MPU)
		reg_offset = TPS65910_VDD1_OP_REG;
	else
		reg_offset = TPS65910_VDD2_OP_REG;

	/* Select VDDx OP   */
	ret = i2c_read(TPS65910_CTRL_I2C_ADDR, reg_offset, 1, &buf, 1);
	if (ret)
		return ret;

	buf &= ~TPS65910_OP_REG_CMD_MASK;

	ret = i2c_write(TPS65910_CTRL_I2C_ADDR, reg_offset, 1, &buf, 1);
	if (ret)
		return ret;

	/* Configure VDDx OP  Voltage */
	ret = i2c_read(TPS65910_CTRL_I2C_ADDR, reg_offset, 1, &buf, 1);
	if (ret)
		return ret;

	buf &= ~TPS65910_OP_REG_SEL_MASK;
	buf |= vddx_op_vol_sel;

	ret = i2c_write(TPS65910_CTRL_I2C_ADDR, reg_offset, 1, &buf, 1);
	if (ret)
		return ret;

	ret = i2c_read(TPS65910_CTRL_I2C_ADDR, reg_offset, 1, &buf, 1);
	if (ret)
		return ret;

	if ((buf & TPS65910_OP_REG_SEL_MASK) != vddx_op_vol_sel)
		return 1;

	return 0;
}
