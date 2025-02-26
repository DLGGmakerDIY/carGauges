

void dis_env()
{
    float co2, temperature, humidity;
    static float co2Old = 0;
    static float temperatureOld = 0;
    static float humidityOld = 0;
    static int16_t co2Last[20] = {1000};
    int16_t co2Max = 0;
    int16_t co2Min = 10000;
    char strTempInt[8] = "25";
    char strTempFloat[8] = "25.0";
    char strHumidityFloat[8] = "50";
    uint8_t shift = 0;

    uint8_t data[12], counter;

    size_t n = sizeof(co2Last) / sizeof(co2Last[0]);

    Serial.println(n);

    if (settingsMode == false)
    {
        settingLock = true;
#ifdef USE_ENV
        // 复位传感器
        // if (resetSensor == true)
        // {
        //     // Reinit Sensor
        //     // initSensor();
        //     initCO2();
        //     resetSensor = false;
        // }

        if (scd4x.update()) // readMeasurement will return true when
                            // fresh data is available
        {
            co2 = scd4x.getCO2();
            temperature = scd4x.getTemperature();
            humidity = scd4x.getHumidity();
            Serial.printf("co2 %02f, temperature %02f, humidity %02f\n", co2, temperature, humidity);
        }

        // // Floating point conversion according to datasheet
        // co2 = (float)((uint16_t)data[0] << 8 | data[1]);
        // // Convert T in deg C
        // temperature = -45 + 175 * (float)((uint16_t)data[3] << 8 | data[4]) / 65535 - temperatureOffset;
        // // Convert RH in %
        // humidity = 100 * (float)((uint16_t)data[6] << 8 | data[7]) / 65535;

#else
        co2 = 1234;
        temperature = 22.5;
        // temperature = random(-10, 90);
        humidity = 56.7;
#endif

        if (unit == FAHRENHEIT)
        {
            temperature = (temperature * 1.8) + 32;
        }

        if (temperature < -9 || temperature > 99)
        {
            sprintf(strTempInt, "%3d", (int)temperature);
        }
        else
        {
            sprintf(strTempInt, "%2d", (int)temperature);
            shift = 10;
        }

        sprintf(strTempFloat, "%.2f", temperature);

        Serial.println(strTempFloat);
        Serial.println(strTempInt);

        sprintf(strHumidityFloat, "%.2f", humidity);

        Serial.printf("co2 %02f, temperature %02f, temperature offset %02f, humidity %02f\n", co2, temperature,
                      temperatureOffset, humidity);

        if (temperature > -50)
        {
            // View result
            for (uint8_t i = 0; i < (n - 1); i++)
            {
                co2Last[i] = co2Last[i + 1];
            }
            co2Last[(n - 1)] = int(co2);

            // View co2
            M5.Displays(display).setFont(&digital_7__mono_24pt7b);
            M5.Displays(display).setTextDatum(CL_DATUM);

            if (int(co2) < 1000)
            {
                M5.Displays(display).setTextPadding(120);
            }
            else
            {
                M5.Displays(display).setTextPadding(150);
            }

            M5.Displays(display).setTextColor(TFT_PINK, TFT_SCREEN_BG);
            M5.Displays(display).drawString(String(int(co2)), 90, 46);

            // View + or - and legend
            measureSprite.clear();
            measureSprite.fillRect(0, 0, 26, 40, TFT_SCREEN_BG);
            measureSprite.setFont(&digital_7__mono_24pt7b);
            measureSprite.setTextColor(TFT_PINK);

            if (co2Old < co2)
            {
                measureSprite.drawString("+", 2, 4);
            }
            else if (co2Old > co2)
            {
                measureSprite.drawString("-", 2, 4);
            }
            else
            {
                measureSprite.drawString("=", 2, 4);
            }
            co2Old = co2;

            measureSprite.setFont(&arial6pt7b);
            measureSprite.setTextColor(TFT_PINK);
            measureSprite.drawString("ppm", 0, 0);

            if (co2 < 1000)
            {
                measureSprite.pushSprite(165, 30, TFT_TRANSPARENT);
            }
            else
            {
                measureSprite.pushSprite(185, 30, TFT_TRANSPARENT);
            }

            for (uint8_t i = 0; i < n; i++)
            {
                if (co2Last[i] != 0)
                {
                    co2Max = max(co2Last[i], co2Max);
                    co2Min = min(co2Last[i], co2Min);
                }
            }

            for (uint8_t i = 0; i < n; i++)
            {
                uint8_t j = map(co2Last[i], co2Min, co2Max, 1, 40);

                if (j > 40)
                {
                    j = 40;
                }

                // Serial.printf("%d %d %d %d\n", co2Min, co2Max, co2Last[i], j);
                M5.Displays(display).drawFastVLine(220 + (i * 4), 62 - 40, 40, TFT_SCREEN_BG);
                M5.Displays(display).drawFastVLine(220 + (i * 4) + 1, 62 - 40, 40, TFT_SCREEN_BG);

                if (co2Last[i] != 0)
                {
                    M5.Displays(display).drawFastVLine(220 + (i * 4), 62 - j, j,
                                                       M5.Displays(display).color565(255, 128 - (i * 4), 128 - (i * 4)));
                    M5.Displays(display).drawFastVLine(220 + (i * 4) + 1, 62 - j, j,
                                                       M5.Displays(display).color565(255, 128 - (i * 4), 128 - (i * 4)));
                }
            }

            M5.Displays(display).fillRect(220, 63, 78, 1, M5.Displays(display).color565(255, 128, 128));

            // View temperature
            M5.Displays(display).setFont(&arial6pt7b);
            M5.Displays(display).setTextPadding(60);
            M5.Displays(display).setTextColor(TFT_SKYBLUE, TFT_SCREEN_BG);
            M5.Displays(display).drawString(strTempFloat, 90, 218);

            tempSprite.clear();
            tempSprite.fillRect(0, 0, 90, 40, TFT_SCREEN_BG);
            tempSprite.setFont(&digital_7__mono_24pt7b);
            tempSprite.setTextPadding(60);
            tempSprite.setTextColor(TFT_SKYBLUE, TFT_SCREEN_BG);
            tempSprite.drawString(strTempInt, 0 + shift, 0);
            tempSprite.pushSprite(80, 170, TFT_TRANSPARENT);

            // View + or - and legend
            measureSprite.clear();
            measureSprite.fillRect(0, 0, 26, 40, TFT_SCREEN_BG);
            measureSprite.setFont(&digital_7__mono_24pt7b);
            measureSprite.setTextColor(TFT_SKYBLUE, TFT_SCREEN_BG);

            if (temperatureOld < temperature)
            {
                measureSprite.drawString("+", 2, 4);
            }
            else if (temperatureOld > temperature)
            {
                measureSprite.drawString("-", 2, 4);
            }
            else
            {
                measureSprite.drawString("=", 2, 4);
            }
            temperatureOld = temperature;

            measureSprite.setFont(&arial6pt7b);
            measureSprite.setTextColor(TFT_SKYBLUE, TFT_SCREEN_BG);
            measureSprite.drawString("o", 0, 0);
            if (unit == FAHRENHEIT)
            {
                measureSprite.drawString("F", 8, 5);
                measureSprite.pushSprite(150 - shift, 174, TFT_TRANSPARENT);
            }
            else
            {
                measureSprite.drawString("C", 8, 5);
                measureSprite.pushSprite(150 - shift, 174, TFT_TRANSPARENT);
            }

            // View humidity
            M5.Displays(display).setFont(&arial6pt7b);
            M5.Displays(display).setTextPadding(44);
            M5.Displays(display).setTextColor(TFT_ORANGE, TFT_SCREEN_BG);
            M5.Displays(display).drawString(strHumidityFloat, 250, 218);

            M5.Displays(display).setFont(&digital_7__mono_24pt7b);
            M5.Displays(display).drawString(String(int(humidity)), 250, 190);

            // View + or - and legend
            measureSprite.clear();
            measureSprite.fillRect(0, 0, 26, 40, TFT_SCREEN_BG);
            measureSprite.setFont(&digital_7__mono_24pt7b);
            measureSprite.setTextColor(TFT_ORANGE, TFT_SCREEN_BG);

            if (humidityOld < humidity)
            {
                measureSprite.drawString("+", 2, 4);
            }
            else if (humidityOld > humidity)
            {
                measureSprite.drawString("-", 2, 4);
            }
            else
            {
                measureSprite.drawString("=", 2, 4);
            }
            humidityOld = humidity;

            measureSprite.setFont(&arial6pt7b);
            measureSprite.setTextColor(TFT_ORANGE, TFT_SCREEN_BG);
            measureSprite.drawString("%", 0, 2);

            measureSprite.pushSprite(300, 174, TFT_TRANSPARENT);

            // Bar空气质量滑块
            M5.Displays(display).fillRect(0, 100 - 16, 320, 18, TFT_SCREEN_BG);
            // M5.Displays(display).fillRect(0, 100, 320, 2, TFT_SCREEN_BG);

            uint16_t trgX;
            if (co2 < 1000)
            {
                // M5.Displays(display).fillRect(16 + 64 * 0 + 8 * 0, 100, 64, 2, TFT_WHITE);
                trgX = map(co2, 0, 1000, 16 + 64 * 0 + 8 * 0, 16 + 64 * 0 + 8 * 0 + 64);
            }
            else if (co2 < 2000)
            {
                // M5.Displays(display).fillRect(16 + 64 * 1 + 8 * 1, 100, 64, 2, TFT_WHITE);
                trgX = map(co2, 1000, 2000, 16 + 64 * 1 + 8 * 1, 16 + 64 * 1 + 8 * 1 + 64);
            }
            else if (co2 < 3000)
            {
                // M5.Displays(display).fillRect(16 + 64 * 2 + 8 * 2, 100, 64, 2, TFT_WHITE);
                trgX = map(co2, 2000, 3000, 16 + 64 * 2 + 8 * 2, 16 + 64 * 2 + 8 * 2 + 64);
            }
            else
            {
                // M5.Displays(display).fillRect(16 + 64 * 3 + 8 * 3, 100, 64, 2, TFT_WHITE);
                trgX = map(co2, 3000, 5000, 16 + 64 * 3 + 8 * 3, 16 + 64 * 3 + 8 * 3 + 64);
            }
            M5.Displays(display).fillTriangle(trgX, 100, trgX - 16, 100 - 16, trgX + 16, 100 - 16, TFT_RED);

            //   if (alert == 0)
            //   {
            //     if (co2 < 1000)
            //     {
            //       m5goColor = CRGB::Green;
            //     }
            //     else if (co2 < 2000)
            //     {
            //       m5goColor = CRGB::Yellow;
            //     }
            //     else if (co2 < 3000)
            //     {
            //       m5goColor = CRGB::Orange;
            //     }
            //     else
            //     {
            //       m5goColor = CRGB::Red;
            //     }
            //   }
            //   else
            //   {
            //     if (co2 >= alert)
            //     {
            //       m5goColor = CRGB::Red;
            //     }
            //     else
            //     {
            //       m5goColor = CRGB::Black;
            //     }
            //   }
        }
        settingLock = false;
        // Wait for next measurement
    }
}