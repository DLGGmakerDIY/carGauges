
// View UI
void viewGUI()
{
  M5.Displays(display).setBrightness(map(brightness, 1, 100, 1, 254));

  M5.Displays(display).fillScreen(TFT_SCREEN_BG);
  M5.Displays(display).drawPng(co2, sizeof(co2), 16, 16, 64, 64);
  M5.Displays(display).drawPng(temperature, sizeof(temperature), 16, 160, 64, 64);
  M5.Displays(display).drawPng(humidity, sizeof(humidity), 176, 160, 64, 64);

  M5.Displays(display).fillRect(16 + 64 * 0 + 8 * 0, 110, 64, 10, TFT_GREEN);
  M5.Displays(display).fillRect(16 + 64 * 1 + 8 * 1, 110, 64, 10, TFT_YELLOW);
  M5.Displays(display).fillRect(16 + 64 * 2 + 8 * 2, 110, 64, 10, TFT_ORANGE);
  M5.Displays(display).fillRect(16 + 64 * 3 + 8 * 3, 110, 64, 10, TFT_RED);

  M5.Displays(display).setFont(&arial6pt7b);
  M5.Displays(display).setTextColor(TFT_WHITE, TFT_SCREEN_BG);
  M5.Displays(display).setTextDatum(CC_DATUM);
  M5.Displays(display).setTextPadding(20);

  M5.Displays(display).drawString("GOOD", 50, 130);
  M5.Displays(display).drawString("OK", 155, 130);
  M5.Displays(display).drawString("BAD", 265, 130);

  M5.Displays(display).setTextDatum(CL_DATUM);
  M5.Displays(display).setTextColor(TFT_PINK, TFT_SCREEN_BG);
  M5.Displays(display).drawString("CO2 Air Quality", 90, 21);
  M5.Displays(display).setTextColor(TFT_SKYBLUE, TFT_SCREEN_BG);
  M5.Displays(display).drawString("Temperature", 90, 165); // 温度标签
  M5.Displays(display).setTextColor(TFT_ORANGE, TFT_SCREEN_BG);
  M5.Displays(display).drawString("Humidity", 250, 165);
  M5.Displays(display).setTextColor(TFT_WHITE, TFT_SCREEN_BG);
  // M5.Displays(display).drawString("V" + String(VERSION) + " by " + String(AUTHOR), 222, 232);//右下角作者签名
  // M5.Displays(display).drawString("DLGG fork form" + String(AUTHOR), 180, 232); //fork
}

// Init Led
void initLed()
{
  // for (uint8_t j = 0; j < NUM_LEDS; j++)
  // {
  //   leds[j] = CRGB::Black;
  // }

  // FastLED.setBrightness(32);

  // FastLED.show();
}

// Init CO2 .src ex:test\M5\unit\Unit_CO2_M5Core.ino
void initCO2()
{
  if (!scd4x.begin(&Wire, SCD4X_I2C_ADDR, 32, 33, 400000U))
  {
    Serial.println("Couldn't find SCD4X");
    M5.Displays(display).drawString("Couldn't find CO2 Sensor", 160, 90);
    while (1)
      delay(1);
  }

  uint16_t error;
  // stop potentially previously started measurement
  error = scd4x.stopPeriodicMeasurement();
  if (error)
  {
    Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
  }

  // Start Measurement
  error = scd4x.startPeriodicMeasurement();
  if (error)
  {
    Serial.print("Error trying to execute startPeriodicMeasurement(): ");
  }
  Serial.println("Waiting for first measurement... (5 sec)");
}

// Init sensor SCD4x
void initSensor()
{
  Serial.begin(115200);
  Wire.end();
  // while (!Serial)
  //   ;

  // Serial.printf(">>> %d\n", port);

  // Init I2C
  switch (port)
  {
  case I2C_PORT_A:
    Wire.begin(); // Port A
    break;
  case I2C_PORT_C_M5GO2:
    Wire.begin(14, 13); // Port C available on M5GO2 for Core2
    break;
  case I2C_PORT_C_M5GO3:
    Wire.begin(17, 18); // Port C available on M5GO3 for CoreS3
    break;
  }

  // Wait until sensors are ready, > 1000 ms according to datasheet
  delay(1000);

  // Start SCD4x measurement in periodic mode, will update every 5s
  Wire.beginTransmission(SCD_ADDRESS);
  Wire.write(0x21);
  Wire.write(0xb1);
  Wire.endTransmission();

  // Wait for first measurement to be finished
  M5.Displays(display).setFont(&arial6pt7b);
  M5.Displays(display).setTextColor(TFT_WHITE, TFT_SCREEN_BG);
  M5.Displays(display).setTextDatum(CC_DATUM);
  M5.Displays(display).setTextPadding(180);

  for (uint8_t i = 0; i < 5; i++)
  {
    // leds[4 - i] = CRGB::DarkBlue;
    // leds[5 + i] = CRGB::DarkBlue;

    // FastLED.show();

    M5.Displays(display).drawString("Please wait - Init CO2 Sensor", 160, 90);
    delay(500);
    M5.Displays(display).drawString("", 160, 90);
    delay(500);
  }
}

// Get temperature offset
float getTemperatureOffset()
{
  uint8_t data[12], counter;
  counter = 0;

  Wire.beginTransmission(SCD_ADDRESS);
  Wire.write(0x23);
  Wire.write(0x16);
  Wire.endTransmission();

  Wire.requestFrom(SCD_ADDRESS, 12);

  while (Wire.available())
  {
    data[counter++] = Wire.read();
  }

  // Wait for first measurement to be finished
  delay(1000);

  // Return result
  return 175 * (float)((uint16_t)data[0] << 8 | data[1]) / 65536;
}

// Fade all led
void fadeall()
{
  // for (uint8_t i = 0; i < NUM_LEDS; i++)
  // {
  //   leds[i].nscale8(100);
  // }
}

// Led task
void led(void *pvParameters)
{
  // for (;;)
  // {
  //   // First slide the led in one direction
  //   for (uint8_t i = 0; i < NUM_LEDS; i++)
  //   {
  //     leds[i] = m5goColor;
  //     ;
  //     // Show the leds
  //     FastLED.show();
  //     fadeall();
  //     vTaskDelay(pdMS_TO_TICKS(50));
  //   }

  //   // Now go in the other direction
  //   for (uint8_t i = 0; i < NUM_LEDS; i++)
  //   {
  //     leds[(NUM_LEDS)-1 - i] = m5goColor;
  //     ;
  //     FastLED.show();
  //     fadeall();
  //     vTaskDelay(pdMS_TO_TICKS(50));
  //   }
  // }
}

// Clear Data
void clearData()
{
}

// Clean some options settings
char **cleanSettings()
{
  int size = sizeof(settingsStandard) / sizeof(settingsStandard[0]);
  char **tmp = (char **)malloc((size - 1) * sizeof(char *));
  int j = 0;
  int k = 0;
  for (int i = 0; i < size; i++)
  {
    if (strcmp(settingsStandard[i], "HDMI") == 0 && M5.getDisplayCount() == 1)
      continue;
    else
    {
      tmp[j++] = settingsStandard[i];
      k++;
    }
  }

  settingsLength = k;

  return tmp;
}