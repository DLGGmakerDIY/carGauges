#define USE_ENV // 注释则使用模拟温湿度数据

#include "carGauges.h"
#include "tools.h"
#include "images.h"
#include "font.h"
#include "functions.h"
#include "display_battery.h"
#include "menu.h"
#include "tasks.h"
#include <SimpleTimer.h> //jfturcot/SimpleTimer
SimpleTimer timer;
void Simple_timer()
{
  // Serial.println("Simple_timer_1");
  // Serial.println(dis_state);
  // view battery
  viewBattery();

  if (dis_state == 1)
  {
    dis_time();
  }
  else if (dis_state == 2)
  {
    dis_env();
  }
}

// Setup
void setup()
{
  // Init M5
  M5Unified_init();

  offsetX = (M5.Displays(display).width() - 320) / 2;
  offsetY = (M5.Displays(display).height() - 240) / 2;

  // Clean settings
  settings = cleanSettings();

  // Sprite
  measureSprite.setColorDepth(24);
  measureSprite.createSprite(26, 40);
  tempSprite.setColorDepth(24);
  tempSprite.createSprite(90, 40);

  // view UI
  viewGUI();

  // view battery
  viewBattery();

  // Init CO2
#ifdef USE_ENV
  initCO2();
#endif

  // init_time();

  // dis_state = 1;
  // dis_time();

  dis_state = 2;
  dis_env();

  timer.setInterval(1000L, Simple_timer);
}

// Main loop
void loop()
{

  timer.run();
  if (M5.BtnA.wasReleased())
  {
    M5.Lcd.clear();
    dis_state = 1;
    // timer.setInterval(1000L, Simple_timer);
    dis_time();
  }
  else if (M5.BtnB.wasReleased())
  {
    M5.Lcd.clear();
    dis_state = 2;
    // timer.setInterval(5000L, Simple_timer);
    viewGUI();
    dis_env();
  }
  else if (M5.BtnC.wasReleased())
  {
    M5.Power.powerOff();
  }

  M5.update();
  delay(10);
}

void M5Unified_init()
{
  auto cfg = M5.config();

  cfg.clear_display = true; // default=true. clear the screen when begin.
  cfg.internal_imu = true;  // default=true. use internal IMU.
  cfg.internal_rtc = true;  // default=true. use internal RTC.
  cfg.internal_spk = true;  // default=true. use internal speaker.
  cfg.internal_mic = true;  // default=true. use internal microphone.
  cfg.external_imu = false; // default=false. use Unit Accel & Gyro.
  cfg.external_rtc = false; // default=false. use Unit RTC.

  cfg.external_display.module_display = false; // default=true. use ModuleDisplay
  cfg.external_display.atom_display = false;   // default=true. use AtomDisplay
  cfg.external_display.unit_oled = false;      // default=true. use UnitOLED
  cfg.external_display.unit_lcd = false;       // default=true. use UnitLCD
  cfg.external_display.unit_rca = false;       // default=false. use UnitRCA VideoOutput
  cfg.external_display.module_rca = false;     // default=false. use ModuleRCA VideoOutput

  M5.begin(cfg);
}
