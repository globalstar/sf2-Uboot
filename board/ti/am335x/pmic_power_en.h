/*
 */
#include <asm/arch/hardware_am33xx.h>

/*
 * TI OMAP Real Time Clock interface for Linux
 *
 * Copyright (C) 2003 MontaVista Software, Inc.
 * Author: George G. Davis <gdavis@mvista.com> or <source@mvista.com>
 *
 * Copyright (C) 2006 David Brownell (new RTC framework)
 * Copyright (C) 2014 Johan Hovold <johan@kernel.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */


/* RTC registers */
#define AM335X_RTC_SECONDS_REG		0x00
#define AM335X_RTC_MINUTES_REG		0x04
#define AM335X_RTC_HOURS_REG		0x08
#define AM335X_RTC_DAYS_REG		0x0C
#define AM335X_RTC_MONTHS_REG		0x10
#define AM335X_RTC_YEARS_REG		0x14
#define AM335X_RTC_WEEKS_REG		0x18

#define AM335X_RTC_ALARM_SECONDS_REG	0x20
#define AM335X_RTC_ALARM_MINUTES_REG	0x24
#define AM335X_RTC_ALARM_HOURS_REG	0x28
#define AM335X_RTC_ALARM_DAYS_REG	0x2c
#define AM335X_RTC_ALARM_MONTHS_REG	0x30
#define AM335X_RTC_ALARM_YEARS_REG	0x34

#define AM335X_RTC_CTRL_REG		0x40
#define AM335X_RTC_STATUS_REG		0x44
#define AM335X_RTC_INTERRUPTS_REG		0x48

#define AM335X_RTC_COMP_LSB_REG		0x4c
#define AM335X_RTC_COMP_MSB_REG		0x50
#define AM335X_RTC_OSC_REG		0x54

#define AM335X_RTC_SCRATCH0_REG		0x60
#define AM335X_RTC_SCRATCH1_REG		0x64
#define AM335X_RTC_SCRATCH2_REG		0x68

#define AM335X_RTC_KICK0_REG		0x6c
#define AM335X_RTC_KICK1_REG		0x70

#define AM335X_RTC_IRQWAKEEN		0x7c

#define AM335X_RTC_ALARM2_SECONDS_REG	0x80
#define AM335X_RTC_ALARM2_MINUTES_REG	0x84
#define AM335X_RTC_ALARM2_HOURS_REG	0x88
#define AM335X_RTC_ALARM2_DAYS_REG	0x8c
#define AM335X_RTC_ALARM2_MONTHS_REG	0x90
#define AM335X_RTC_ALARM2_YEARS_REG	0x94

#define AM335X_RTC_PMIC_REG		0x98

/* AM335X_RTC_CTRL_REG bit fields: */
#define AM335X_RTC_CTRL_SPLIT		BIT(7)
#define AM335X_RTC_CTRL_DISABLE		BIT(6)
#define AM335X_RTC_CTRL_SET_32_COUNTER	BIT(5)
#define AM335X_RTC_CTRL_TEST		BIT(4)
#define AM335X_RTC_CTRL_MODE_12_24	BIT(3)
#define AM335X_RTC_CTRL_AUTO_COMP		BIT(2)
#define AM335X_RTC_CTRL_ROUND_30S		BIT(1)
#define AM335X_RTC_CTRL_STOP		BIT(0)

/* AM335X_RTC_STATUS_REG bit fields: */
#define AM335X_RTC_STATUS_POWER_UP	BIT(7)
#define AM335X_RTC_STATUS_ALARM2		BIT(7)
#define AM335X_RTC_STATUS_ALARM		BIT(6)
#define AM335X_RTC_STATUS_1D_EVENT	BIT(5)
#define AM335X_RTC_STATUS_1H_EVENT	BIT(4)
#define AM335X_RTC_STATUS_1M_EVENT	BIT(3)
#define AM335X_RTC_STATUS_1S_EVENT	BIT(2)
#define AM335X_RTC_STATUS_RUN		BIT(1)
#define AM335X_RTC_STATUS_BUSY		BIT(0)

/* AM335X_RTC_INTERRUPTS_REG bit fields: */
#define AM335X_RTC_INTERRUPTS_IT_ALARM2	BIT(4)
#define AM335X_RTC_INTERRUPTS_IT_ALARM	BIT(3)
#define AM335X_RTC_INTERRUPTS_IT_TIMER	BIT(2)

/* AM335X_RTC_OSC_REG bit fields: */
#define AM335X_RTC_OSC_32KCLK_EN		BIT(6)
#define AM335X_RTC_OSC_SEL_32KCLK_SRC	BIT(3)
#define AM335X_RTC_OSC_OSC32K_GZ_DISABLE	BIT(4)

/* AM335X_RTC_IRQWAKEEN bit fields: */
#define AM335X_RTC_IRQWAKEEN_ALARM_WAKEEN	BIT(1)

/* AM335X_RTC_PMIC bit fields: */
#define AM335X_RTC_PMIC_PWR_ENABLE_EN	BIT(16)
#define AM335X_RTC_PMIC_EXT_WKUP_EN(x)	BIT(x)
#define AM335X_RTC_PMIC_EXT_WKUP_POL(x)	BIT(4 + x)

/* AM335X_RTC_KICKER values */
#define	KICK0_VALUE			0x83e70b13
#define	KICK1_VALUE			0x95a4f1e0

/*
 * RTC_PMIC register access
 */

#define RTC_PMIC	(RTC_BASE + 0x098)
#define RTC_STATUS	(RTC_BASE + 0x044)

