// ref :https://github.com/armel/M5AirQuality  functions.h->viewBattery()
// #include "../include/display/battery/display_battery.h"
// #include <M5Unified.h>

// View battery
void viewBattery()
{
    char buf[8];

    uint8_t batteryLevel;
    boolean batteryCharging;

    // On left, view battery level
    batteryLevel = map(getBatteryLevel(), 0, 100, 0, 17);
    batteryCharging = isCharging();

    // Serial.printf(">>> %d %d %d %d\n", batteryLevel, batteryLevelOld,
    // batteryCharging, batteryCharginglOld);

    // 电量有变化才更新显示
    //  if (batteryLevel != batteryLevelOld || batteryCharging != batteryCharginglOld)
    //  {
    //  M5.Displays(display).drawFastHLine(0, 20, 320, TFT_BLACK);

    batteryLevelOld = batteryLevel;
    batteryCharginglOld = batteryCharging;

    M5.Displays(display).drawRect(293, 4, 21, 12, TFT_WHITE);
    M5.Displays(display).drawRect(313, 7, 4, 6, TFT_WHITE);

    for (uint8_t i = 0; i < batteryLevel; i++)
    {
        M5.Displays(display).drawGradientVLine(295 + i, 6, 8, TFT_GREEN, TFT_DARKGREEN);
    }

    M5.Displays(display).drawFastVLine(300, 5, 10, TFT_SCREEN_BG);
    M5.Displays(display).drawFastVLine(306, 5, 10, TFT_SCREEN_BG);

    M5.Displays(display).fillRect(260, 4, 32, 12, TFT_RED);

    if (batteryCharging)
    {
        M5.Displays(display).setTextColor(TFT_WHITE);
        M5.Displays(display).setFont(&arial6pt7b);
        M5.Displays(display).setTextDatum(CR_DATUM);
        M5.Displays(display).setTextPadding(0);
        snprintf(buf, 8, "%d%s", getBatteryLevel(), "+"); // 充电显示+
    }
    else
    {
        M5.Displays(display).setTextColor(TFT_WHITE);
        M5.Displays(display).setFont(0);
        M5.Displays(display).setTextDatum(CR_DATUM);
        M5.Displays(display).setTextPadding(0);
        snprintf(buf, 8, "%d%s", getBatteryLevel(), "%");
    }
    M5.Displays(display).fillRect(260, 4, 32, 12, TFT_SCREEN_BG);
    M5.Displays(display).drawString(buf, 290, 11);
    // }
}
