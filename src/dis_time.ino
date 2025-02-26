// #include <M5Unified.h>
#include "Free_Fonts.h"
void init_time()
{
    // 编译器时间配置时间
}

void dis_time()
{
    char buf[128] = {0};
    static const char *WEEK_DAYS[] = {"日", "一", "二", "三", "四", "五", "六"};

    auto dt = M5.Rtc.getDateTime();

    // Serial.printf("%04d/%02d/%02d (%s)  %02d:%02d:%02d\n", dt.date.year, dt.date.month,
    //               dt.date.date, WEEK_DAYS[dt.date.weekDay], dt.time.hours, dt.time.minutes, dt.time.seconds);

    sprintf(buf, "%d年%d月%d日 星期%s", dt.date.year, dt.date.month,
            dt.date.date, WEEK_DAYS[dt.date.weekDay]);

    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);

    // 显示年月日星期
    M5.Display.setFont(&fonts::efontCN_24); // 正
    M5.Display.setTextDatum(MC_DATUM);      // 文字居中  160, 120
    M5.Display.drawString(buf, 160, 70);
    // M5.Display.drawString(buf, 40, 70);

    // 显示时分秒
    struct tm timeInfo;
    timeInfo.tm_hour = dt.time.hours;  // 小时 (0-23)
    timeInfo.tm_min = dt.time.minutes; // 分钟 (0-59)
    timeInfo.tm_sec = dt.time.seconds; // 秒 (0-59)
    strftime(buf, 36, "%T", &timeInfo);
    M5.Display.setFont(&fonts::FreeSans24pt7b);
    M5.Display.setTextDatum(MC_DATUM); // 文字居中  160, 120
    M5.Display.drawString(buf, 160, 120);
    // M5.Display.drawString(buf, 70, 120);
}