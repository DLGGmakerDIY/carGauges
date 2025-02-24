// #include <M5Unified.h>
#include "sys_time.h"
#include "Free_Fonts.h"
void init_time()
{
    // 编译器时间配置时间
    struct tm timeInfo;
    timeInfo.tm_hour = pc_Hours;                 // 小时 (0-23)
    timeInfo.tm_min = pc_Minutes;                // 分钟 (0-59)
    timeInfo.tm_sec = pc_Seconds;                // 秒 (0-59)
    timeInfo.tm_year = pc_Year_u8 + 2000 - 1900; // 年份 (当前年份 - 1900)
    timeInfo.tm_mon = pc_Month - 1;              // 月份 (从0开始，0 代表 1 月)
    timeInfo.tm_mday = pc_Date;                  // 日期 (1-31)
    timeInfo.tm_wday = pc_weekDay;               // 星期 (0-6, 0 = Sunday)
                                                 // timeInfo.tm_yday = 0;           // 一年中的第几天
                                                 // timeInfo.tm_isdst = 0;          // 是否是夏令时

    // 设置 RTC 时间
    M5.Rtc.setDateTime(&timeInfo);
}

void dis_time()
{
    M5.Display.setTextColor(TFT_YELLOW);
    M5.Display.setFreeFont(FF36);      // 正不倾斜
    M5.Display.setTextDatum(MC_DATUM); // 文字居中  160, 120

    auto dt = M5.Rtc.getDateTime();

    String h, m, s;
    if (dt.time.hours < 10)
        h = "0" + String(dt.time.hours);
    else
        h = String(dt.time.hours);
    if (dt.time.minutes < 10)
        m = "0" + String(dt.time.minutes);
    else
        m = String(dt.time.minutes);
    if (dt.time.seconds < 10)
        s = "0" + String(dt.time.seconds);
    else
        s = String(dt.time.seconds);
    M5.Lcd.clear();
    M5.Lcd.drawString(h + ":" + m + ":" + s, 160, 120);
    // clockFlush(dt.time.hours, dt.time.minutes, dt.time.seconds);
}