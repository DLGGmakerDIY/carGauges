// #include "sys_time.h"

#ifndef SYS_TIME_H
#define SYS_TIME_H

#include <Arduino.h>
/***************************************************************************************
** Function name:      编译器获取win系统时间
** Description:
    month:
    year:
***************************************************************************************/
// Function to convert month to the number equivalent
uint8_t getMonthNum(const char *p)
{
    const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    char month[4] = {};
    strncpy(month, p, 3); // copy first 3 characters (month)
    return (strstr(month_names, month) - month_names) / 3 + 1;
}
/***************************************************************************************
** Function name:      GetDayOfWeek
** Description:        获取日期（日,月,年）是星期几
    d:                  日
    m:                  月
    y:                  年
***************************************************************************************/
int GetDayOfWeek(int d, int m, int y)
{
    if (m < 3)
    {
        m += 12;
        y--;
    }
    int h = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 + 5 * y / 100 + y / 400) % 7;
    return ((h + 5) % 7) + 1;
}
static uint8_t conv2d(const char *p);                                                                      // Forward declaration needed for IDE 1.6.x
uint8_t pc_Hours = conv2d(__TIME__), pc_Minutes = conv2d(__TIME__ + 3), pc_Seconds = conv2d(__TIME__ + 6); // Get H, M, S from compile time编译器获取win系统时间
uint8_t pc_Year_u8 = conv2d(__DATE__ + 9);                                                                 // 24
uint8_t pc_Month = getMonthNum(__DATE__);                                                                  // 3月打印3
uint8_t pc_Date = conv2d(__DATE__ + 4);
// GPT "Zeller's Congruence" 算法
uint8_t pc_weekDay = GetDayOfWeek(conv2d(__DATE__ + 4), getMonthNum(__DATE__), conv2d(__DATE__ + 9) + 2000);
// Function to extract numbers from compile time string
static uint8_t conv2d(const char *p)
{
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}

#endif