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


void rtc_unlock(void)
{
	rtc_writel(AM335X_RTC_KICK0_REG, KICK0_VALUE);
	rtc_writel(AM335X_RTC_KICK1_REG, KICK1_VALUE);
}

void rtc_lock(void)
{
	rtc_writel(AM335X_RTC_KICK0_REG, 0);
	rtc_writel(AM335X_RTC_KICK1_REG, 0);
}

void rtc_wait_not_busy(void)
{
	int count;
	u8 status;

	/* BUSY may stay active for 1/32768 second (~30 usec) */
	for (count = 0; count < 50; count++) {
		status = rtc_read(AM335X_RTC_STATUS_REG);
		if (!(status & AM335X_RTC_STATUS_BUSY))
			break;
		udelay(1);
	}
	/* now we have ~15 usec to read/write various registers */
}

static int am335x_rtc_set_alarm(u8 year, u8 month, u8 day, u8 hour, u8 min, u8 sec)
{
	rtc_wait_not_busy();
	rtc_write(AM335X_RTC_ALARM_SECONDS_REG, sec);
	rtc_write(AM335X_RTC_ALARM_MINUTES_REG, min);
	rtc_write(AM335X_RTC_ALARM_HOURS_REG, hour);
	rtc_write(AM335X_RTC_ALARM_DAYS_REG, day);
	rtc_write(AM335X_RTC_ALARM_MONTHS_REG, month);
	rtc_write(AM335X_RTC_ALARM_YEARS_REG, year);
 
	return 0;
}

static int am335x_rtc_set_time(u8 year, u8 month, u8 day, u8 hour, u8 min, u8 sec, u8 week)
{
	rtc_wait_not_busy();
	rtc_unlock();
	rtc_write(AM335X_RTC_YEARS_REG, year);
	rtc_write(AM335X_RTC_MONTHS_REG, month);
	rtc_write(AM335X_RTC_WEEKS_REG, week);
	rtc_write(AM335X_RTC_DAYS_REG, day);
	rtc_write(AM335X_RTC_HOURS_REG, hour);
	rtc_write(AM335X_RTC_MINUTES_REG, min);
	rtc_write(AM335X_RTC_SECONDS_REG, sec);
	rtc_lock();

	return 0;
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
  

#if 0
  rtc_write(AM335X_RTC_CTRL_REG, 0); /* Make sure RTC is enabled */
  am335x_rtc_set_alarm(17,9,4,8,44,2);
  am335x_rtc_set_time( 17,9,4,8,44,0,1);
#endif
  
  return 0;
}

