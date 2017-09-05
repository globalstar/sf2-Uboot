/*
 */
#include <common.h>
#include <command.h>
#include <rtc.h>
#include <asm/io.h>
#include <asm/arch/hardware_am33xx.h>
#include "pmic_power_en.h"

u8 rtc_read(unsigned int reg)
{
	return readb(RTC_BASE + reg);
}

u32 rtc_readl(unsigned int reg)
{
	return readl(RTC_BASE + reg);
}

void rtc_write(unsigned int reg, u8 val)
{
	writeb(val, RTC_BASE + reg);
}

void rtc_writel(unsigned int reg, u32 val)
{
	writel(val, RTC_BASE + reg);
}


int turn_on_pmic_power_en(void)
{
  u32 val;

  val = rtc_readl(AM335X_RTC_PMIC_REG);

  val = 0;  /* disable PWR_ENABLE_EN and clear wakeup bits */
  rtc_writel(AM335X_RTC_PMIC_REG, val);

  val = AM335X_RTC_PMIC_EXT_WKUP_EN(0);  /* enable external wakeup pin */
  val |= AM335X_RTC_PMIC_EXT_WKUP_POL(0); /* set active low to wakeup since signal is tied to GND */
  rtc_writel(AM335X_RTC_PMIC_REG, val);

  val |= AM335X_RTC_PMIC_PWR_ENABLE_EN;  /* enable the signal to trigger wakeup event */
  rtc_writel(AM335X_RTC_PMIC_REG, val);
  
  return 0;
}

