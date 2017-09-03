/***************************************************************************//**
* \file Name: RTC_1.h
* \version 1.10
*
* \brief
*  This file provides constants and parameter values for the RTC_P4 Component.
*
********************************************************************************
* Copyright 2015-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_RTC_P4_RTC_1_H)
#define CY_RTC_P4_RTC_1_H

#include <cytypes.h>
#include <CyLib.h>


/*******************************************************************************
*    Data types definition
*******************************************************************************/

typedef enum
{
    RTC_1_DST_DATE_RELATIVE = 0u,
    RTC_1_DST_DATE_FIXED = 1u
}RTC_1_DST_DATETYPE_ENUM;

/**
* \addtogroup group_structures
* @{
*/

/**
* This is the data structure that is used to save the current time and date 
* (RTC_currentTimeDate), and Alarm time and date (RTC_alarmCfgTimeDate).
*/
typedef struct
{
    uint32 time;
    uint32 date;
    uint32 dayOfWeek;
    uint32 status;
}RTC_1_DATE_TIME;

/**
* This is the data structure that is used to save time and date values for 
* Daylight Savings Time Start and Stop (RTC_dstTimeDateStart and 
* RTC_dstTimeDateStop).
*/
typedef struct
{
    uint32 hour;
    uint32 dayOfWeek;
    uint32 dayOfMonth;
    uint32 weekOfMonth;
    uint32 month;
    uint8  timeFormat;
}RTC_1_DST_TIME;

/** @} structures */

/*******************************************************************************
*    Enumerated Types and Parameters
*******************************************************************************/
/* Time Format setting constants */
#define RTC_1__HOUR_12 1
#define RTC_1__HOUR_24 0

/*  Date Format setting constants */
#define RTC_1__MM_DD_YYYY 0
#define RTC_1__DD_MM_YYYY 1
#define RTC_1__YYYY_MM_DD 2

/* Date Type setting constants */
#define RTC_1__RELATIVE 0
#define RTC_1__FIXED 1

/* DST Hour setting constants */
#define RTC_1__H0000 0
#define RTC_1__H0100 1
#define RTC_1__H0200 2
#define RTC_1__H0300 3
#define RTC_1__H0400 4
#define RTC_1__H0500 5
#define RTC_1__H0600 6
#define RTC_1__H0700 7
#define RTC_1__H0800 8
#define RTC_1__H0900 9
#define RTC_1__H1000 10
#define RTC_1__H1100 11
#define RTC_1__H1200 12
#define RTC_1__H1300 13
#define RTC_1__H1400 14
#define RTC_1__H1500 15
#define RTC_1__H1600 16
#define RTC_1__H1700 17
#define RTC_1__H1800 18
#define RTC_1__H1900 19
#define RTC_1__H2000 20
#define RTC_1__H2100 21
#define RTC_1__H2200 22
#define RTC_1__H2300 23

/* DST DayOfWeek setting constants */
#define RTC_1__SUNDAY 1
#define RTC_1__MONDAY 2
#define RTC_1__TUESDAY 3
#define RTC_1__WEDNESDAY 4
#define RTC_1__THURSDAY 5
#define RTC_1__FRIDAY 6
#define RTC_1__SATURDAY 7

/* DST Month setting constants */
#define RTC_1__JAN 1
#define RTC_1__FEB 2
#define RTC_1__MAR 3
#define RTC_1__APR 4
#define RTC_1__MAY 5
#define RTC_1__JUN 6
#define RTC_1__JUL 7
#define RTC_1__AUG 8
#define RTC_1__SEP 9
#define RTC_1__OCT 10
#define RTC_1__NOV 11
#define RTC_1__DEC 12

/* DST WeekOfMonth setting constants */
#define RTC_1__FIRST 1
#define RTC_1__SECOND 2
#define RTC_1__THIRD 3
#define RTC_1__FOURTH 4
#define RTC_1__FIFTH 5
#define RTC_1__LAST 6


#define RTC_1_INITIAL_DATA_FORMAT     (0u)
#define RTC_1_INITIAL_TIME_FORMAT     (1u)
#define RTC_1_INITIAL_UPDATE_MODE     (0u)
#define RTC_1_INITIAL_SECOND          (0u)
#define RTC_1_INITIAL_MINUTE          (0u)
#define RTC_1_INITIAL_HOUR            (0u)
#define RTC_1_INITIAL_DAY             (1u)
#define RTC_1_INITIAL_MONTH           (1u)
#define RTC_1_INITIAL_YEAR            (1970u)

#define RTC_1_INITIAL_ALARM_STATUS    (0u)

#define RTC_1_INITIAL_DST_STATUS      (0u)
#define RTC_1_INITIAL_DST_DATE_TYPE   (0u)
#define RTC_1_INITIAL_DST_START_MONTH (3uL)
#define RTC_1_INITIAL_DST_START_WOM   (6uL)
#define RTC_1_INITIAL_DST_START_DOM   (22uL)
#define RTC_1_INITIAL_DST_START_DOW   (1uL)
#define RTC_1_INITIAL_DST_START_HRS   (0uL)
#define RTC_1_INITIAL_DST_STOP_MONTH  (10uL)
#define RTC_1_INITIAL_DST_STOP_DOM    (22uL)
#define RTC_1_INITIAL_DST_STOP_DOW    (1uL)
#define RTC_1_INITIAL_DST_STOP_WOM    (6uL)
#define RTC_1_INITIAL_DST_STOP_HRS    (0uL)

/* Day of week definitions */
#define RTC_1_SUNDAY     (RTC_1__SUNDAY)
#define RTC_1_MONDAY     (RTC_1__MONDAY)
#define RTC_1_TUESDAY    (RTC_1__TUESDAY)
#define RTC_1_WEDNESDAY  (RTC_1__WEDNESDAY)
#define RTC_1_THURSDAY   (RTC_1__THURSDAY)
#define RTC_1_FRIDAY     (RTC_1__FRIDAY)
#define RTC_1_SATURDAY   (RTC_1__SATURDAY)

/* DST WeekOfMonth setting constants definitions */
#define RTC_1_FIRST      (RTC_1__FIRST)
#define RTC_1_SECOND     (RTC_1__SECOND)
#define RTC_1_THIRD      (RTC_1__THIRD)
#define RTC_1_FOURTH     (RTC_1__FOURTH)
#define RTC_1_FIFTH      (RTC_1__FIFTH)
#define RTC_1_LAST       (RTC_1__LAST)

/* Month definitions */
#define RTC_1_JANUARY    (RTC_1__JAN)
#define RTC_1_FEBRUARY   (RTC_1__FEB)
#define RTC_1_MARCH      (RTC_1__MAR)
#define RTC_1_APRIL      (RTC_1__APR)
#define RTC_1_MAY        (RTC_1__MAY)
#define RTC_1_JUNE       (RTC_1__JUN)
#define RTC_1_JULY       (RTC_1__JUL)
#define RTC_1_AUGUST     (RTC_1__AUG)
#define RTC_1_SEPTEMBER  (RTC_1__SEP)
#define RTC_1_OCTOBER    (RTC_1__OCT)
#define RTC_1_NOVEMBER   (RTC_1__NOV)
#define RTC_1_DECEMBER   (RTC_1__DEC)

#define RTC_1_DAYS_PER_WEEK       (7u)

/* Number of days in month definitions */
#define RTC_1_DAYS_IN_JANUARY     (31u)
#define RTC_1_DAYS_IN_FEBRUARY    (28u)
#define RTC_1_DAYS_IN_MARCH       (31u)
#define RTC_1_DAYS_IN_APRIL       (30u)
#define RTC_1_DAYS_IN_MAY         (31u)
#define RTC_1_DAYS_IN_JUNE        (30u)
#define RTC_1_DAYS_IN_JULY        (31u)
#define RTC_1_DAYS_IN_AUGUST      (31u)
#define RTC_1_DAYS_IN_SEPTEMBER   (30u)
#define RTC_1_DAYS_IN_OCTOBER     (31u)
#define RTC_1_DAYS_IN_NOVEMBER    (30u)
#define RTC_1_DAYS_IN_DECEMBER    (31u)

#define RTC_1_MONTHS_PER_YEAR     (12uL)

#define RTC_1_HOURS_PER_DAY       (24uL)
#define RTC_1_HOURS_PER_HALF_DAY  (12uL)

#define RTC_1_SECONDS_PER_MINUTE  (60uL)
#define RTC_1_SECONDS_PER_HOUR    (3600uL)
#define RTC_1_SECONDS_PER_DAY     (24uL * 3600uL)

#define RTC_1_SECONDS_PER_LEAP_YEAR    (366uL * 24uL * 3600uL)
#define RTC_1_SECONDS_PER_NONLEAP_YEAR (365uL * 24uL * 3600uL)

#define RTC_1_UNIX_TIME_PM   ((12uL * 3600uL) + 1uL)

/* AM/PM status definitions */
#define RTC_1_AM                 (0u)
#define RTC_1_PM                 (1u)

/* Time format definitions */
#define RTC_1_12_HOURS_FORMAT    (RTC_1__HOUR_12)
#define RTC_1_24_HOURS_FORMAT    (RTC_1__HOUR_24)

/* UNIX time begins in 1970 year.  */
#define RTC_1_YEAR_0             (1970u)

/* Definition of date register fields */
#if(RTC_1_INITIAL_DATA_FORMAT == RTC_1__MM_DD_YYYY)
    #define RTC_1_10_MONTH_OFFSET   (28u)
    #define RTC_1_MONTH_OFFSET      (24u)
    #define RTC_1_10_DAY_OFFSET     (20u)
    #define RTC_1_DAY_OFFSET        (16u)
    #define RTC_1_1000_YEAR_OFFSET  (12u)
    #define RTC_1_100_YEAR_OFFSET   (8u)
    #define RTC_1_10_YEAR_OFFSET    (4u)
    #define RTC_1_YEAR_OFFSET       (0u)
#elif(RTC_1_INITIAL_DATA_FORMAT == RTC_1__DD_MM_YYYY)
    #define RTC_1_10_MONTH_OFFSET   (20u)
    #define RTC_1_MONTH_OFFSET      (16u)
    #define RTC_1_10_DAY_OFFSET     (28u)
    #define RTC_1_DAY_OFFSET        (24u)
    #define RTC_1_1000_YEAR_OFFSET  (12u)
    #define RTC_1_100_YEAR_OFFSET   (8u)
    #define RTC_1_10_YEAR_OFFSET    (4u)
    #define RTC_1_YEAR_OFFSET       (0u)
#else
    #define RTC_1_10_MONTH_OFFSET   (12u)
    #define RTC_1_MONTH_OFFSET      (8u)
    #define RTC_1_10_DAY_OFFSET     (4u)
    #define RTC_1_DAY_OFFSET        (0u)
    #define RTC_1_1000_YEAR_OFFSET  (28u)
    #define RTC_1_100_YEAR_OFFSET   (24u)
    #define RTC_1_10_YEAR_OFFSET    (20u)
    #define RTC_1_YEAR_OFFSET       (16u)
#endif /* (RTC_1_INITIAL_DATA_FORMAT == RTC_1__MM_DD_YYYY) */

#define RTC_1_10_MONTH_MASK          (0x00000001uL << RTC_1_10_MONTH_OFFSET)
#define RTC_1_MONTH_MASK             (0x0000000FuL << RTC_1_MONTH_OFFSET)
#define RTC_1_10_DAY_MASK            (0x00000003uL << RTC_1_10_DAY_OFFSET)
#define RTC_1_DAY_MASK               (0x0000000FuL << RTC_1_DAY_OFFSET)
#define RTC_1_1000_YEAR_MASK         (0x00000003uL << RTC_1_1000_YEAR_OFFSET)
#define RTC_1_100_YEAR_MASK          (0x0000000FuL << RTC_1_100_YEAR_OFFSET)
#define RTC_1_10_YEAR_MASK           (0x0000000FuL << RTC_1_10_YEAR_OFFSET)
#define RTC_1_YEAR_MASK              (0x0000000FuL << RTC_1_YEAR_OFFSET)

#define  RTC_1_MONTH_FULL_MASK       (RTC_1_10_MONTH_MASK  | RTC_1_MONTH_MASK)
#define  RTC_1_DAY_FULL_MASK         (RTC_1_10_DAY_MASK    | RTC_1_DAY_MASK)
#define  RTC_1_YEAR_FULL_MASK        (RTC_1_1000_YEAR_MASK | RTC_1_100_YEAR_MASK |\
                                                 RTC_1_10_YEAR_MASK   | RTC_1_YEAR_MASK)


/* Definition of time register fields */
#define RTC_1_TIME_FORMAT_OFFSET     (23u)
#define RTC_1_PERIOD_OF_DAY_OFFSET   (22u)
#define RTC_1_10_HOURS_OFFSET        (20u)
#define RTC_1_HOURS_OFFSET           (16u)
#define RTC_1_10_MINUTES_OFFSET      (12u)
#define RTC_1_MINUTES_OFFSET         (8u)
#define RTC_1_10_SECONDS_OFFSET      (4u)
#define RTC_1_SECONDS_OFFSET         (0u)

#define RTC_1_TIME_FORMAT_MASK       (0x00000001uL << RTC_1_TIME_FORMAT_OFFSET)
#define RTC_1_PERIOD_OF_DAY_MASK     (0x00000001uL << RTC_1_PERIOD_OF_DAY_OFFSET)
#define RTC_1_10_HOURS_MASK          (0x00000003uL << RTC_1_10_HOURS_OFFSET)
#define RTC_1_HOURS_MASK             (0x0000000FuL << RTC_1_HOURS_OFFSET)
#define RTC_1_10_MINUTES_MASK        (0x00000007uL << RTC_1_10_MINUTES_OFFSET)
#define RTC_1_MINUTES_MASK           (0x0000000FuL << RTC_1_MINUTES_OFFSET)
#define RTC_1_10_SECONDS_MASK        (0x00000007uL << RTC_1_10_SECONDS_OFFSET)
#define RTC_1_SECONDS_MASK           (0x0000000FuL << RTC_1_SECONDS_OFFSET)

#define RTC_1_HOURS_FULL_MASK        (RTC_1_10_HOURS_MASK   | RTC_1_HOURS_MASK)
#define RTC_1_MINUTES_FULL_MASK      (RTC_1_10_MINUTES_MASK | RTC_1_MINUTES_MASK)
#define RTC_1_SECONDS_FULL_MASK      (RTC_1_10_SECONDS_MASK | RTC_1_SECONDS_MASK)

#define RTC_1_ALARM_SEC_MASK         (0x00000001uL)
#define RTC_1_ALARM_MIN_MASK         (0x00000002uL)
#define RTC_1_ALARM_HOUR_MASK        (0x00000004uL)
#define RTC_1_ALARM_DAYOFWEEK_MASK   (0x00000008uL)
#define RTC_1_ALARM_DAYOFMONTH_MASK  (0x00000010uL)
#define RTC_1_ALARM_MONTH_MASK       (0x00000020uL)
#define RTC_1_ALARM_YEAR_MASK        (0x00000040uL)

#define RTC_1_STATUS_DST_OFFSET      (1u)
#define RTC_1_STATUS_ALARM_OFFSET    (2u)
#define RTC_1_STATUS_LY_OFFSET       (3u)
#define RTC_1_STATUS_AM_PM_OFFSET    (4u)

/* Definition of the RTC status values. */
#define RTC_1_STATUS_DST             (1uL << RTC_1_STATUS_DST_OFFSET)
#define RTC_1_STATUS_LY              (1uL << RTC_1_STATUS_LY_OFFSET)
#define RTC_1_STATUS_AM_PM           (1uL << RTC_1_STATUS_AM_PM_OFFSET)

/* Number of bits per one BCD digit. */
#define RTC_1_BCD_NUMBER_SIZE        (4u)
#define RTC_1_BCD_ONE_DIGIT_MASK     (0x0000000FuL)

/*******************************************************************************
*    Function Prototypes
*******************************************************************************/
/**
* \addtogroup group_api
* @{
*/
void   RTC_1_Start(void);
void   RTC_1_Stop(void);
void   RTC_1_Init(void);
void   RTC_1_SetUnixTime(uint64 unixTime);
uint64 RTC_1_GetUnixTime(void);
void   RTC_1_SetPeriod(uint32 ticks, uint32 refOneSecTicks);
uint32 RTC_1_GetPeriod(void);
uint32 RTC_1_GetRefOneSec(void);
void   RTC_1_SetDateAndTime(uint32 inputTime, uint32 inputDate);
void   RTC_1_GetDateAndTime(RTC_1_DATE_TIME* dateTime);
uint32 RTC_1_GetTime(void);
uint32 RTC_1_GetDate(void);
void   RTC_1_SetAlarmDateAndTime(const RTC_1_DATE_TIME* alarmTime);
void   RTC_1_GetAlarmDateAndTime(RTC_1_DATE_TIME* alarmTimeDate);
void   RTC_1_SetAlarmMask(uint32 mask);
uint32 RTC_1_GetAlarmMask(void);
uint32 RTC_1_ReadStatus(void);
uint32 RTC_1_GetAlarmStatus(void);
void   RTC_1_ClearAlarmStatus(void);
void   RTC_1_SetDSTStartTime(const RTC_1_DST_TIME* dstStartTime,
                                              RTC_1_DST_DATETYPE_ENUM type);
void   RTC_1_SetDSTStopTime(const RTC_1_DST_TIME* dstStopTime,
                                             RTC_1_DST_DATETYPE_ENUM type);
uint32 RTC_1_ConvertBCDToDec(uint32 bcdNum);
uint32 RTC_1_ConvertDecToBCD(uint32 decNum);
void   RTC_1_Update(void);
void*  RTC_1_SetAlarmHandler(void (*CallbackFunction)(void));

static uint32 RTC_1_ConstructDate(uint32 month, uint32 day, uint32 year);
static uint32 RTC_1_ConstructTime(uint32 timeFormat, uint32 stateAmPm, uint32 hour, uint32 min, uint32 sec);
/** @} api */

static uint32 RTC_1_GetTimeFormat(uint32 inputTime);
static uint32 RTC_1_SetTimeFormat(uint32 inputTime, uint32 timeFormat);

/**
* \addtogroup group_api
* @{
*/
static uint32 RTC_1_LeapYear(uint32 year);
static uint32 RTC_1_IsBitSet(uint32 var, uint32 mask);
static uint32 RTC_1_GetSecond(uint32 inputTime);
static uint32 RTC_1_GetMinutes(uint32 inputTime);
static uint32 RTC_1_GetHours(uint32 inputTime);
static uint32 RTC_1_GetAmPm(uint32 inputTime);
static uint32 RTC_1_GetDay(uint32 date);
static uint32 RTC_1_GetMonth(uint32 date);
static uint32 RTC_1_GetYear(uint32 date);
/** @} api */

static uint32 RTC_1_SetSecond(uint32 inputTime, uint32 inputSecond);
static uint32 RTC_1_SetMinutes(uint32 inputTime, uint32 inputMinute);
static uint32 RTC_1_SetHours(uint32 inputTime, uint32 inputHours);
static uint32 RTC_1_SetAmPm(uint32 inputTime, uint32 periodOfDay);
static uint32 RTC_1_SetDay(uint32 inputDate, uint32 inputDay);
static uint32 RTC_1_SetMonth(uint32 inputDate, uint32 inputMonth);
static uint32 RTC_1_SetYear(uint32 inputDate, uint32 inputYear);

uint64 RTC_1_ConstructUnixAlarmTime(const RTC_1_DATE_TIME* alarmTime, uint8 alarmCfgMask);
uint64 RTC_1_GetDstUnixTime(const RTC_1_DST_TIME* dstTime);
uint64 RTC_1_GetNexAlarmTime(uint64 curUnixTime, uint8 alarmCfgMask);

static uint32 RTC_1_GetNextMinSec(uint32 curVal);
static uint32 RTC_1_GetNextHour(uint32 curVal);
static uint32 RTC_1_GetNextDay(uint32 curYear, uint32 curMonth, uint32 curDay, uint32 alarmCfgMask);
static uint32 RTC_1_GetNextMonth(uint32 curMonth);
static uint32 RTC_1_GetNextYear(uint32 curYear);
static uint32 RTC_1_GetDayOfWeek(uint32 day, uint32 month, uint32 year);

uint32 RTC_1_DaysInMonth(uint32 month, uint32 year);
uint32 RTC_1_DaysBeforeMonth(uint32 month, uint32 year);

/**
* \addtogroup group_api
* @{
*/
void   RTC_1_UnixToDateTime(RTC_1_DATE_TIME* dateTime, uint64 unixTime, uint32 timeFormat);
uint64 RTC_1_DateTimeToUnix(uint32 inputDate, uint32 inputTime);
/** @} api */

uint32 RTC_1_RelativeToFixed(uint32 dayOfWeek, uint32 weekOfMonth, uint32 month, uint32 year);


#if defined(CYDEV_RTC_SOURCE_WDT)
    #if((0u != CYDEV_WDT_GENERATE_ISR) && (0u == RTC_1_INITIAL_UPDATE_MODE))
        static void RTC_1_CySysRtcSetCallback(uint32 wdtNumber);
        static void RTC_1_CySysRtcResetCallback(uint32 wdtNumber);
    #endif /* ((0u != CYDEV_WDT_GENERATE_ISR) && (0u == RTC_1_INITIAL_UPDATE_MODE)) */
#endif /* (CYDEV_RTC_SOURCE_WDT) */

/*******************************************************************************
* Function Name: RTC_1_IsBitSet
****************************************************************************//**
*
* \brief
*  Checks the state of a bit passed through parameter.
*
* \param
*  var The variable to be checked.
* \param
*  mask The mask for a bit to be checked.
*
* \return
*  0u - Bit is not set.
* \return
*  1u - Bit is set.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_1_IsBitSet(uint32 var, uint32 mask)
{
    return ((mask == (var & mask)) ? 1Lu : 0Lu);
}


/*******************************************************************************
* Function Name: RTC_1_LeapYear
****************************************************************************//**
*
* \brief
* Checks whether the year passed through the parameter is
* leap or no.
*
* \param
*  year The year to be checked.
*
*
* \return
*  0u - The year is not leap.
* \return
*  1u - The year is leap.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_1_LeapYear(uint32 year)
{
    uint32 retVal;

    if(((0u == (year % 4Lu)) && (0u != (year % 100Lu))) || (0u == (year % 400Lu)))
    {
        retVal = 1uL;
    }
    else
    {
        retVal = 0uL;
    }

    return(retVal);
}


/*******************************************************************************
* Function Name: RTC_1_GetSecond
****************************************************************************//**
*
* \brief
*  Returns the seconds value from the time value that is passed as a/the parameter.
*
* \param
*  time The time value.
*
* \return
*  The seconds value.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_1_GetSecond(uint32 inputTime)
{
    uint32 retVal;

    retVal  = ((inputTime & RTC_1_10_SECONDS_MASK) >> RTC_1_10_SECONDS_OFFSET) * 10u;
    retVal += (inputTime & RTC_1_SECONDS_MASK) >> RTC_1_SECONDS_OFFSET;

    return (retVal);
}


/*******************************************************************************
* Function Name: RTC_1_GetMinutes
****************************************************************************//**
*
* \brief
*  Returns the minutes value from the time value that is passed as a/the 
* parameter.
*
* \param
*  time The time value.
*
* \return
*  The minutes value.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_1_GetMinutes(uint32 inputTime)
{
    uint32 retVal;

    retVal  = ((inputTime & RTC_1_10_MINUTES_MASK) >> RTC_1_10_MINUTES_OFFSET) * 10u;
    retVal += (inputTime & RTC_1_MINUTES_MASK) >> RTC_1_MINUTES_OFFSET;

    return (retVal);
}


/*******************************************************************************
* Function Name: RTC_1_GetHours
****************************************************************************//**
*
* \brief
*  Returns the hours value from the time value that is passed as a/the parameter.
*
* \param
*  time The time value.
*
* \return
*  The hours value.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_1_GetHours(uint32 inputTime)
{
    uint32 retVal;

    retVal  = ((inputTime & RTC_1_10_HOURS_MASK) >> RTC_1_10_HOURS_OFFSET) * 10u;
    retVal += (inputTime & RTC_1_HOURS_MASK) >> RTC_1_HOURS_OFFSET;

    return (retVal);
}


/*******************************************************************************
* Function Name: RTC_1_GetAmPm
****************************************************************************//**
*
* \brief
*  Returns the AM/PM status from the time value that is passed as parameter.
*
* \param
*  time The time value.
*
* \return
*  RTC_1_AM - AM.
* \return
*  RTC_1_PM - PM.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_1_GetAmPm(uint32 inputTime)
{
    return (RTC_1_IsBitSet(inputTime, RTC_1_PERIOD_OF_DAY_MASK));
}


/*******************************************************************************
* Function Name: RTC_1_GetDay
****************************************************************************//**
*
* \brief
*  Returns the day value from the date value that is passed as parameter.
*
* \param
*  date The date value.
*
* \return
*  The day value.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_1_GetDay(uint32 date)
{
    uint32 retVal;

    retVal  = ((date & RTC_1_10_DAY_MASK) >> RTC_1_10_DAY_OFFSET) * 10u;
    retVal += (date & RTC_1_DAY_MASK) >> RTC_1_DAY_OFFSET;

    return (retVal);
}


/*******************************************************************************
* Function Name: RTC_1_GetMonth
****************************************************************************//**
*
* \brief
*  Returns the month value from the date value that is passed as parameter.
*
* \param
*  date The date value.
*
* \return
*  The month value.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_1_GetMonth(uint32 date)
{
    uint32 retVal;

    retVal  = ((date & RTC_1_10_MONTH_MASK) >> RTC_1_10_MONTH_OFFSET) * 10u;
    retVal += (date & RTC_1_MONTH_MASK) >> RTC_1_MONTH_OFFSET;

    return (retVal);
}


/*******************************************************************************
* Function Name: RTC_1_GetYear
****************************************************************************//**
*
* \brief
*  Returns the year value from the date value that is passed as parameter.
*
* \param
*  date The date value.
*
* \return
*  The year value.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_1_GetYear(uint32 date)
{
    uint32 retVal;

    retVal  = ((date & RTC_1_1000_YEAR_MASK) >> RTC_1_1000_YEAR_OFFSET) * 1000u;
    retVal += ((date & RTC_1_100_YEAR_MASK) >> RTC_1_100_YEAR_OFFSET) * 100u;
    retVal += ((date & RTC_1_10_YEAR_MASK) >> RTC_1_10_YEAR_OFFSET) * 10u;
    retVal += (date & RTC_1_YEAR_MASK) >> RTC_1_YEAR_OFFSET;

    return (retVal);
}


/*******************************************************************************
* Function Name: RTC_1_SetSecond
****************************************************************************//**
*\internal
* \brief
*  Updates the time with the new second value.
*
* \param
* time The current date.
* \param
* second The seconds value to be set to the time variable.
*
* \return
*  The updated time variable.
*\endinternal
*******************************************************************************/
static CY_INLINE uint32 RTC_1_SetSecond(uint32 inputTime, uint32 inputSecond)
{
    inputTime &= ~(RTC_1_SECONDS_FULL_MASK);

    inputTime |= (inputSecond / 10u) << RTC_1_10_SECONDS_OFFSET;
    inputTime |= (inputSecond % 10u) << RTC_1_SECONDS_OFFSET;

    return(inputTime);
}


/*******************************************************************************
* Function Name: RTC_1_SetMinutes
****************************************************************************//**
*\internal
* \brief
*  Updates the time with the new minute value.
*
* \param
*  time The current date.
* \param
*  minutes The minutes value to be set to the time variable.
*
* \return
*  The updated time variable.
* \endinternal
*******************************************************************************/
static CY_INLINE uint32 RTC_1_SetMinutes(uint32 inputTime, uint32 inputMinute)
{
    inputTime &= ~(RTC_1_MINUTES_FULL_MASK);

    inputTime |= (inputMinute / 10u) << RTC_1_10_MINUTES_OFFSET;
    inputTime |= (inputMinute % 10u) << RTC_1_MINUTES_OFFSET;

    return(inputTime);
}


/*******************************************************************************
* Function Name: RTC_1_SetHours
****************************************************************************//**
*\internal
* \brief
*  Updates the time with the new hour value.
*
* \param
* time The current date.
* \param
* hours The hours value to be set to the time variable.
*
* \return
*  The updated time variable.
*\endinternal
*******************************************************************************/
static CY_INLINE uint32 RTC_1_SetHours(uint32 inputTime, uint32 inputHours)
{
    inputTime &= ~(RTC_1_HOURS_FULL_MASK);

    inputTime |= (inputHours / 10u) << RTC_1_10_HOURS_OFFSET;
    inputTime |= (inputHours % 10u) << RTC_1_HOURS_OFFSET;

    return(inputTime);
}


/*******************************************************************************
* Function Name: RTC_1_SetAmPm
****************************************************************************//**
*\internal
* \brief
*  Updates the time variable with the AmPm status.
*
* \param
* time The current date.
* \param
* periodOfDay The AmPm status to be set to the time variable.
*
* \return
*  The updated time variable.
* \endinternal
*******************************************************************************/
static CY_INLINE uint32 RTC_1_SetAmPm(uint32 inputTime, uint32 periodOfDay)
{
    if(0u != periodOfDay)
    {
        inputTime &= ~(RTC_1_PERIOD_OF_DAY_MASK);
    }
    else
    {
        inputTime |= RTC_1_PERIOD_OF_DAY_MASK;
    }

    return(inputTime);
}


/*******************************************************************************
* Function Name: RTC_1_SetDay
****************************************************************************//**
*\internal
* \brief
*  Updates the date variable with the new day value.
*
* \param
* date The current date.
* \param
* day The month to be set to the date variable.
*
* \return
*  The updated date variable.
*\endinternal
*******************************************************************************/
static CY_INLINE uint32 RTC_1_SetDay(uint32 inputDate, uint32 inputDay)
{
    inputDate &= ~(RTC_1_DAY_FULL_MASK);

    inputDate |= (inputDay / 10u) << RTC_1_10_DAY_OFFSET;
    inputDate |= (inputDay % 10u) << RTC_1_DAY_OFFSET;

    return(inputDate);
}


/*******************************************************************************
* Function Name: RTC_1_SetMonth
****************************************************************************//**
*\internal
* \brief
*  Updates the date variable with the new month value.
*
* \param
* date The current date.
* \param
* month The month that to be set to the date variable.
*
* \return
*  The updated date variable.
*\endinternal
*******************************************************************************/
static CY_INLINE uint32 RTC_1_SetMonth(uint32 inputDate, uint32 inputMonth)
{
    inputDate &= ~(RTC_1_MONTH_FULL_MASK);

    inputDate |= (inputMonth / 10u) << RTC_1_10_MONTH_OFFSET;
    inputDate |= (inputMonth % 10u) << RTC_1_MONTH_OFFSET;

    return(inputDate);
}


/*******************************************************************************
* Function Name: RTC_1_SetYear
****************************************************************************//**
*\internal
* \brief
*  Updates the date variable with the new year value.
*
* \param
* date The current date.
* \param
* year The year to be set to the date variable.
*
* \return
* The updated date variable.
*\endinternal
*******************************************************************************/
static CY_INLINE uint32 RTC_1_SetYear(uint32 inputDate, uint32 inputYear)
{
    inputDate  &= ~(RTC_1_YEAR_FULL_MASK);

    inputDate |= (inputYear / 1000u) << RTC_1_1000_YEAR_OFFSET;
    inputYear %= 1000u;

    inputDate |= (inputYear / 100u) << RTC_1_100_YEAR_OFFSET;
    inputYear %= 100u;

    inputDate |= (inputYear / 10u) << RTC_1_10_YEAR_OFFSET;
    inputDate |= (inputYear % 10u) << RTC_1_YEAR_OFFSET;

    return(inputDate);
}


/*******************************************************************************
* Function Name: RTC_1_GetNextMinSec
****************************************************************************//**
*\internal
* \brief
* This is an internal function that calculates the value of the next Second/Minute
* that follows after the current Minute/Second.
*
* \param
* curVal The current Second/Minute value.
*
* \return
* Returns the Second/Minute which follows after the current Second/Minute.
*\endinternal
*******************************************************************************/
static CY_INLINE uint32 RTC_1_GetNextMinSec(uint32 curVal)
{
    return((curVal < 59u) ? (curVal + 1u) : 0u);
}


/*******************************************************************************
* Function Name: RTC_1_GetNextHour
****************************************************************************//**
*\internal
* \brief
*  This is an internal function that calculates the value of the next Hour
*  that follows after the current Hour.
*
* \param
* curVal The current Hour value.
*
* \return
*  Returns the Hour which follows after the current Hour.
* \endinternal
*******************************************************************************/
static CY_INLINE uint32 RTC_1_GetNextHour(uint32 curVal)
{
    return((curVal < 23u) ? (curVal + 1u) : 0u);
}


/*******************************************************************************
* Function Name: RTC_1_GetNextDay
****************************************************************************//**
* \internal
* \brief
*  This is an internal function that calculates the value of the next Day
*  that follows after the current Day.
*
* \param
*  curYear The current year.
* \param
*  curMonth The current month.
* \param
*  curDay The current day.
* \param
*  alarmCfgMask Alarm Config Mask.
*
* \return
*  Returns the day which follows after the current Day.
* \endinternal
*******************************************************************************/
static CY_INLINE uint32 RTC_1_GetNextDay(uint32 curYear, uint32 curMonth, uint32 curDay, uint32 alarmCfgMask)
{
    uint32 daysInMonth;
    uint32 tmpVal;

    daysInMonth = RTC_1_DaysInMonth(curMonth, curYear);

    if(0u != (alarmCfgMask & RTC_1_ALARM_DAYOFWEEK_MASK))
    {
        tmpVal = curDay + RTC_1_DAYS_PER_WEEK;
        tmpVal = (tmpVal > daysInMonth) ? (tmpVal - daysInMonth) : tmpVal;
    }
    else
    {
        tmpVal = (curDay < daysInMonth) ? (curDay + 1u) : 1u;
    }

    return(tmpVal);
}


/*******************************************************************************
* Function Name: RTC_1_GetNextMonth
****************************************************************************//**
* \internal
* \brief
*  This is an internal function that calculates the value of the next month
*  that follows after the current month.
*
* \param
*  curMonth The current month.
*
* \return
*  Returns the month which follows after the current month.
* \endinternal
*******************************************************************************/
static CY_INLINE uint32 RTC_1_GetNextMonth(uint32 curMonth)
{
    return((curMonth < 12u) ? (curMonth + 1u) : 1u);
}


/*******************************************************************************
* Function Name: RTC_1_GetNextYear
****************************************************************************//**
* \internal
* \brief
*  This is an internal function that calculates the value of the next year
*  that follows after the current year.
*
* \param
*  curYear The current year.
*
* \return
*  Returns the year which follows after the current year.
* \endinternal
*******************************************************************************/
static CY_INLINE uint32 RTC_1_GetNextYear(uint32 curYear)
{
    return(curYear + 1u);
}


/*******************************************************************************
* Function Name: RTC_1_SetTimeFormat
****************************************************************************//**
* \internal
* \brief
*  Updates the "Time Format" value in the variable that contains time in the
*  "HH:MM:SS" format.
*
* \param
*  inputTime The current value of the time in the "HH:MM:SS" format.
*
* \return
*  Returns the updated value of the time in the "HH:MM:SS" format.
* \endinternal
*******************************************************************************/
static CY_INLINE uint32 RTC_1_SetTimeFormat(uint32 inputTime, uint32 timeFormat)
{
    inputTime &= ~(RTC_1_TIME_FORMAT_MASK);

    if((uint32)RTC_1_12_HOURS_FORMAT != timeFormat)
    {
        inputTime |= RTC_1_TIME_FORMAT_MASK;
    }

    return(inputTime);
}


/*******************************************************************************
* Function Name: RTC_1_GetTimeFormat
****************************************************************************//**
* \internal
* \brief
*  Reads the time format from the variable that contains time in the
*  "HH:MM:SS" format.
*
* \param
*  inputTime The current value of the time in the "HH:MM:SS" format.
*
* \return
*  Returns the time format that is stored in the variable that contains time
*  in the "HH:MM:SS" format.
* \endinternal
*******************************************************************************/
static CY_INLINE uint32 RTC_1_GetTimeFormat(uint32 inputTime)
{
    return ((0uL != (inputTime & (1uL << RTC_1_TIME_FORMAT_OFFSET))) ? (uint32)RTC_1_12_HOURS_FORMAT :
                                                                                  (uint32)RTC_1_24_HOURS_FORMAT);
}


/*******************************************************************************
* Function Name: RTC_1_ConstructTime
****************************************************************************//**
*
* \brief
*  Returns the time in the format used in APIs from individual elements
*  passed (hour, min, sec etc)
*
* \param
* timeFormat The 12/24 hours time format <br>
* RTC_1_24_HOURS_FORMAT - The 24 hours format. <br>
* RTC_1_12_HOURS_FORMAT - The 12 hours format.
*
* \param
* stateAmPm  The AM/PM status <br>
* RTC_1_AM - AM. <br>
* RTC_1_PM - PM.
*
* \param
*  hour The hour.
*
* \param
*  min The minute.
*
* \param
*  sec The second.
*
* \return
*  Time in the format used in API.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_1_ConstructTime(uint32 timeFormat, uint32 stateAmPm, uint32 hour, uint32 min, uint32 sec)
{
    uint32 retVal;

    retVal  = timeFormat << RTC_1_TIME_FORMAT_OFFSET;
    retVal |= stateAmPm << RTC_1_PERIOD_OF_DAY_OFFSET;

    retVal |= (hour / 10u) << RTC_1_10_HOURS_OFFSET;
    retVal |= (hour % 10u) << RTC_1_HOURS_OFFSET;

    retVal |= (min / 10u) << RTC_1_10_MINUTES_OFFSET;
    retVal |= (min % 10u) << RTC_1_MINUTES_OFFSET;

    retVal |= (sec / 10u) << RTC_1_10_SECONDS_OFFSET;
    retVal |= (sec % 10u) << RTC_1_SECONDS_OFFSET;

    return (retVal);
}


/*******************************************************************************
* Function Name: RTC_1_ConstructDate
****************************************************************************//**
*
* \brief
*  Returns the date in the format used in APIs from individual elements
*  passed (day. Month and year)
*
* \param
*  month The month.
*
* \param
*  day The day.
*
* \param
*  year The year.
*
* \return
*  The date in the format used in API.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_1_ConstructDate(uint32 month, uint32 day, uint32 year)
{
    uint32 retVal;
    uint32 tmpVal = year;

    retVal  = (month / 10u) << RTC_1_10_MONTH_OFFSET;
    retVal |= (month % 10u) << RTC_1_MONTH_OFFSET;

    retVal |= (day / 10u) << RTC_1_10_DAY_OFFSET;
    retVal |= (day % 10u) << RTC_1_DAY_OFFSET;

    retVal |= (year / 1000u) << RTC_1_1000_YEAR_OFFSET;
    tmpVal %= 1000u;

    retVal |= (tmpVal / 100u) << RTC_1_100_YEAR_OFFSET;
    tmpVal %= 100u;

    retVal |= (tmpVal / 10u) << RTC_1_10_YEAR_OFFSET;
    retVal |= (tmpVal % 10u) << RTC_1_YEAR_OFFSET;

    return (retVal);
}


/*******************************************************************************
* Function Name: RTC_1_GetDayOfWeek
****************************************************************************//**
* \internal
*
* \brief
*  Returns a day of the week for a year, month, and day of month that are passed
*  through parameters. Zeller's congruence is used to calculate the day of
*  the week.
*
*  For the Georgian calendar, Zeller's congruence is:
*  h = (q + [13 * (m + 1)] + K + [K/4] + [J/4] - 2J) mod 7
*
*  h - The day of the week (0 = Saturday, 1 = Sunday, 2 = Monday, ..., 6 = Friday).
*  q - The day of the month.
*  m - The month (3 = March, 4 = April, 5 = May, ..., 14 = February)
*  K - The year of the century (year \mod 100).
*  J - The zero-based century (actually [year/100]) For example, the zero-based
*  centuries for 1995 and 2000 are 19 and 20 respectively (not to be
*  confused with the common ordinal century enumeration which indicates
*  20th for both cases).
*
* \note In this algorithm January and February are counted as months 13 and 14
* of the previous year.
*
* \param
*  day The day of the month(1..31)
*
* \param
*  month The month of the year
* RTC_1_JANUARY <br>
* RTC_1_FEBRUARY <br>
* RTC_1_MARCH <br>
* RTC_1_APRIL <br>
* RTC_1_MAY <br>
* RTC_1_JUNE <br>
* RTC_1_JULY <br>
* RTC_1_AUGUST <br>
* RTC_1_SEPTEMBER <br>
* RTC_1_OCTOBER <br>
* RTC_1_NOVEMBER <br>
* RTC_1_DECEMBER <br>
*
* \param
*  year The year value.
*
* \return
* Returns a day of the week
* RTC_1_SUNDAY <br>
* RTC_1_MONDAY <br>
* RTC_1_TUESDAY <br>
* RTC_1_WEDNESDAY <br>
* RTC_1_THURSDAY <br>
* RTC_1_FRIDAY <br>
* RTC_1_SATURDAY <br>
*
* \endinternal
*******************************************************************************/
static CY_INLINE uint32 RTC_1_GetDayOfWeek(uint32 day, uint32 month, uint32 year)
{
    uint32 retVal;

    /* Converts month number from regular convention
     * (1=January,..., 12=December) to convention required for this
     * algorithm(January and February are counted as months 13 and 14 of
     * previous year).
    */
    if(month < (uint32)RTC_1_MARCH)
    {
        month = 12u + month;
        year--;
    }

    /* Calculates Day of Week using Zeller's congruence algorithms.
    */
    retVal = (day + (((month + 1u) * 26u) / 10u) + year + (year / 4u) + (6u * (year / 100u)) + (year / 400u)) % 7u;

    /* Makes correction for Saturday. Saturday number should be 7 instead of 0. */
    if(0u == retVal)
    {
        retVal = (uint32)RTC_1_SATURDAY;
    }

    return(retVal);
}


#if defined(CYDEV_RTC_SOURCE_WDT)
    #if((0u != CYDEV_WDT_GENERATE_ISR) && (0u == RTC_1_INITIAL_UPDATE_MODE))

        /*******************************************************************************
        * Function Name: RTC_1_CySysRtcSetCallback
        ****************************************************************************//**
        * \internal
        * \brief
        *  This is an internal function that registers a callback for the
        *  RTC_1_Update() function by address "0".
        *
        * \param
        *  wdtNumber The number of the WDT or WCO timer to be used to pull the
        *  RTC_1_Update() function.
        *
        *  The callback registered before by address "0" is replaced
        *  by the RTC_1_Update() function.
        * \endinternal
        *******************************************************************************/
        static CY_INLINE void RTC_1_CySysRtcSetCallback(uint32 wdtNumber)
        {
            #if((0u != CY_IP_WCO_WDT_EN) && (0u != CY_IP_SRSSLT))
                (void)CySysTimerSetInterruptCallback(wdtNumber, RTC_1_Update);
            #else
                (void)CySysWdtSetIsrCallback(wdtNumber, RTC_1_Update);
            #endif /* ((0u != CY_IP_WCO_WDT_EN) && (0u != CY_IP_SRSSLT)) */
        }


        /*******************************************************************************
        * Function Name: RTC_1_CySysRtcResetCallback
        ****************************************************************************//**
        * \internal
        * \brief
        *  This is an internal function that clears a callback by address "0".
        *
        * \param
        *  wdtNumber The number of the WDT or WCO timer to be cleared callback for.
        *
        *  The callback registered before by address "0" is replaced
        *  by the NULL pointer.
        * \endinternal
        *******************************************************************************/
        static CY_INLINE void RTC_1_CySysRtcResetCallback(uint32 wdtNumber)
        {
            #if((0u != CY_IP_WCO_WDT_EN) && (0u != CY_IP_SRSSLT))
                (void)CySysTimerSetInterruptCallback(wdtNumber, (void *)0);
            #else
                (void)CySysWdtSetIsrCallback(wdtNumber, (void *)0);
            #endif /* ((0u != CY_IP_WCO_WDT_EN) && (0u != CY_IP_SRSSLT)) */
        }

    #endif /* ((0u != CYDEV_WDT_GENERATE_ISR) && (0u == RTC_1_INITIAL_UPDATE_MODE)) */
#endif /* (CYDEV_RTC_SOURCE_WDT) */

/*******************************************************************************
*    External Variables
*******************************************************************************/

/**
* \addtogroup group_globals
* @{
*/

/**
* Indicates whether the RTC has been initialized; The variable is initialized to
* 0 and set to 1 the first time RTC_Start() is called. This allows the component
* to restart without reinitialization after the first call to the RTC_Start() 
* routine.
*/
extern uint8  RTC_1_initVar;

/** The DST start/stop status */
extern uint8  RTC_1_dstStatus;

/**
* The uint64 variable represents the standard Unix time (number of seconds 
* elapsed from January 1, 1970 00:00 hours UTC) in 64-bit 
*/
extern volatile uint64 RTC_1_unixTime;

/** The values for the time and date of the DST start */
extern RTC_1_DST_TIME RTC_1_dstStartTime;

/** The values for the time and date of the DST stop */
extern RTC_1_DST_TIME RTC_1_dstStopTime;

/**
* The last updated time and date values are stored in this structure (update 
* happens in Get time/date APIs)
*/
extern RTC_1_DATE_TIME RTC_1_currentTimeDate;

/** The alarm time and date values are stored in this variable */
extern RTC_1_DATE_TIME RTC_1_alarmCfgTimeDate;

/**
* This variable is used to mask alarm events; mask seconds alarm, mask minutes 
* alarm, and so on. It will have bit masks for each time item masking that item 
* for alarm generation
*/
extern uint32  RTC_1_alarmCfgMask;

/**
* This variable is used to indicate current active alarm status per time item 
* used in the alarm; whether seconds alarm is active, minute's alarm is active, 
* and so on. It will have bit masks for each time item (seconds, minutes, hours,
* day, and so on) showing the status
*/
extern uint32 RTC_1_alarmCurStatus;

/** @} globals */

extern void (*RTC_1_alarmCallbackPtr)(void);

extern const uint16 RTC_1_daysBeforeMonthTbl[RTC_1_MONTHS_PER_YEAR];
extern const uint8  RTC_1_daysInMonthTbl[RTC_1_MONTHS_PER_YEAR];


#endif /* CY_RTC_P4_RTC_1_H */

/* [] END OF FILE */
