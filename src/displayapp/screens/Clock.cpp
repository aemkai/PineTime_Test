#include <cstdio>

#include <libs/date/includes/date/date.h>
#include "DateTimeController.h" //#include "components/datetime/DateTimeController.h"  -> ohne Zusatzpfad in https://github.com/AppKaki/lvgl-wasm/tree/master/clock
#include <libs/lvgl/lvgl.h>
#include "Clock.h"
#include "../DisplayApp.h"
#include "BatteryIcon.h"
#include "BleIcon.h"
#include "Symbols.h"
//#include "NotificationManager.h" //#include "components/ble/NotificationManager.h"  -> nicht in https://github.com/AppKaki/lvgl-wasm/tree/master/clock
//#include "NotificationIcon.h" //#include "NotificationIcon.h"  -> nicht in https://github.com/AppKaki/lvgl-wasm/tree/master/clock

/* Definition currentDateTime 
CtsData currentDateTime;
	currentDateTime.year = m_dateTimeController.Year();
	currentDateTime.month = static_cast<u_int8_t>(m_dateTimeController.Month());
	currentDateTime.dayofmonth = m_dateTimeController.Day();
	currentDateTime.hour = m_dateTimeController.Hours();
	currentDateTime.minute = m_dateTimeController.Minutes();
	currentDateTime.second = m_dateTimeController.Seconds();
	currentDateTime.millis = 0;
 Extra Ende */

using namespace Pinetime::Applications::Screens;
extern lv_font_t jetbrains_mono_extrabold_compressed;
extern lv_font_t jetbrains_mono_bold_20;
extern lv_font_t jetbrains_mono_42;
extern lv_font_t jetbrains_mono_76;
extern lv_style_t* LabelBigStyle;

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  Clock* screen = static_cast<Clock *>(obj->user_data);
  screen->OnObjectEvent(obj, event);
}

Clock::Clock(DisplayApp* app,
        Controllers::DateTime& dateTimeController,
        Controllers::Battery& batteryController,
        Controllers::Ble& bleController) : Screen(app), currentDateTime{{}},
                                           dateTimeController{dateTimeController}, batteryController{batteryController},
                                           bleController{bleController} {
  displayedChar[0] = 0;
  displayedChar[1] = 0;
  displayedChar[2] = 0;
  displayedChar[3] = 0;
  displayedChar[4] = 0;
  displayedChar[5] = 0;
  displayedChar[6] = 0;
						   
  batteryIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(batteryIcon, Symbols::batteryFull);
  lv_obj_align(batteryIcon, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, -5, 2);

  batteryPlug = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(batteryPlug, Symbols::plug);
  lv_obj_align(batteryPlug, batteryIcon, LV_ALIGN_OUT_LEFT_MID, -5, 0);

  bleIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(bleIcon, Symbols::bluetooth);
  lv_obj_align(bleIcon, batteryPlug, LV_ALIGN_OUT_LEFT_MID, -5, 0);

  //notificationIcon = lv_label_create(lv_scr_act(), NULL);
  //lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(false));
  //lv_obj_align(notificationIcon, nullptr, LV_ALIGN_IN_TOP_LEFT, 10, 0);

  label_date = lv_label_create(lv_scr_act(), nullptr);

  lv_obj_align(label_date, lv_scr_act(), LV_ALIGN_IN_LEFT_MID, 0, 60);

  label_time = lv_label_create(lv_scr_act(), nullptr);
  //lv_obj_set_style_local_text_font(label_time, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_extrabold_compressed);
  lv_obj_set_style_local_text_font(label_time, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_76);

  label_sec = lv_label_create(lv_scr_act(), nullptr);
  //lv_obj_set_style_local_text_font(label_time, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_extrabold_compressed);
  lv_obj_set_style_local_text_font(label_sec, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_42);
  lv_obj_set_style_local_text_color(label_sec, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xCCCC00));


  lv_obj_align(label_time, lv_scr_act(), LV_ALIGN_IN_LEFT_MID, 0, 0);
  lv_obj_align(label_sec, lv_scr_act(), LV_ALIGN_IN_RIGHT_MID, 0, 0);
						   
  backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  backgroundLabel->user_data = this;
  lv_obj_set_click(backgroundLabel, true);
  lv_obj_set_event_cb(backgroundLabel, event_handler);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text(backgroundLabel, "");						   
						   
  heartbeatIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(heartbeatIcon, Symbols::heartBeat);
  lv_obj_align(heartbeatIcon, lv_scr_act(), LV_ALIGN_IN_BOTTOM_LEFT, 5, -2);

  heartbeatValue = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(heartbeatValue, "0");
  lv_obj_align(heartbeatValue, heartbeatIcon, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

  heartbeatBpm = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(heartbeatBpm, "TEST");
  lv_obj_align(heartbeatBpm, heartbeatValue, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

  stepValue = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(stepValue, "0");
  lv_obj_align(stepValue, lv_scr_act(), LV_ALIGN_IN_BOTTOM_RIGHT, -5, -2);

  stepIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(stepIcon, Symbols::shoe);
  lv_obj_align(stepIcon, stepValue, LV_ALIGN_OUT_LEFT_MID, -5, 0);
}

Clock::~Clock() {
  lv_obj_clean(lv_scr_act());
}

bool Clock::Refresh()
{
  batteryPercentRemaining = batteryController.PercentRemaining();
  if (batteryPercentRemaining.IsUpdated())
  {
    auto batteryPercent = batteryPercentRemaining.Get();
    lv_label_set_text(batteryIcon, BatteryIcon::GetBatteryIcon(batteryPercent));
    auto isCharging = batteryController.IsCharging() || batteryController.IsPowerPresent();
    lv_label_set_text(batteryPlug, BatteryIcon::GetPlugIcon(isCharging));
  }

  bleState = bleController.IsConnected();
  if (bleState.IsUpdated())
  {
    if(bleState.Get() == true)
    {
      lv_label_set_text(bleIcon, BleIcon::GetIcon(true));
    } else {
      lv_label_set_text(bleIcon, BleIcon::GetIcon(false));
    }
  }
  lv_obj_align(batteryIcon, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, -5, 5);
  lv_obj_align(batteryPlug, batteryIcon, LV_ALIGN_OUT_LEFT_MID, -5, 0);
  lv_obj_align(bleIcon, batteryPlug, LV_ALIGN_OUT_LEFT_MID, -5, 0);

/*  notificationState = notificatioManager.AreNewNotificationsAvailable();
  if(notificationState.IsUpdated())
  {
    if(notificationState.Get() == true)
      lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(true));
    else
      lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(false));
  }
*/
  currentDateTime = dateTimeController.CurrentDateTime(); // -> wird in https://github.com/AppKaki/lvgl-wasm/blob/master/clock/DateTimeController.cpp berechnet

  if(currentDateTime.IsUpdated())
    {
	auto newDateTime = currentDateTime.Get();

	auto dp = date::floor<date::days>(newDateTime);
	auto time = date::make_time(newDateTime-dp);
	auto yearMonthDay = date::year_month_day(dp);

	auto year = (int)yearMonthDay.year();
	auto month = static_cast<Pinetime::Controllers::DateTime::Months>((unsigned)yearMonthDay.month());
	auto day = (unsigned)yearMonthDay.day();
	auto dayOfWeek = static_cast<Pinetime::Controllers::DateTime::Days>(date::weekday(yearMonthDay).iso_encoding());

	auto hour = time.hours().count();
	auto minute = time.minutes().count();
	auto second = time.seconds().count();

	char secondsChar[3];
	sprintf(secondsChar, "%02d", static_cast<int>(second));
	char minutesChar[3];
	sprintf(minutesChar, "%02d", static_cast<int>(minute));

	char hoursChar[3];
	sprintf(hoursChar, "%02d", static_cast<int>(hour));

	char timeStr[5];
	sprintf(timeStr, "%c%c:%c%c", hoursChar[0],hoursChar[1],minutesChar[0], minutesChar[1]);

	char secondsStr[3];	  
	  
    	if (hoursChar[0] != displayedChar[0] || hoursChar[1] != displayedChar[1] || minutesChar[0] != displayedChar[2] || minutesChar[1] != displayedChar[3]  || secondsChar[0] != displayedChar[4] || secondsChar[1] != displayedChar[5])
	{
		displayedChar[0] = hoursChar[0];
		displayedChar[1] = hoursChar[1];
		displayedChar[2] = minutesChar[0];
		displayedChar[3] = minutesChar[1];
		displayedChar[4] = secondsChar[0];
		displayedChar[5] = secondsChar[1];	  

	  	//char timeStr[6];
		//char timeStr[4];
	  	//char secondsStr[3];
      	}
	  
	// sprintf(timeStr, "%c%c:%c%c:%c%c", hoursChar[0], hoursChar[1], minutesChar[0], minutesChar[1], secondsChar[0], secondsChar[1]);
	// lv_label_set_text(label_time, timeStr);

	// if (settingsController.GetClockType() == Controllers::Settings::ClockType::H12) {
	// lv_obj_align(label_time, lv_scr_act(), LV_ALIGN_IN_RIGHT_MID, 0, 0);
	// } else {
	// lv_obj_align(label_time, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);
	// }
	sprintf(timeStr, "%c%c:%c%c", hoursChar[0], hoursChar[1], minutesChar[0], minutesChar[1]);
	  lv_label_set_text(label_time, timeStr);
	sprintf(secondsStr, "%c%c", secondsChar[0], secondsChar[1]);	  
	lv_label_set_text(label_sec, secondsStr);

	lv_obj_align(label_time, lv_scr_act(), LV_ALIGN_CENTER, 0, -35);
	lv_obj_align(label_sec, label_time, LV_ALIGN_OUT_BOTTOM_MID, 85, 10);
		  
	 	  

	if ((year != currentYear) || (month != currentMonth) || (dayOfWeek != currentDayOfWeek) || (day != currentDay)) 
	{
		char dateStr[22];
		//sprintf(dateStr, "%s %d.%02d.%d", dateTimeController.DayOfWeekToString(), day, month, year);		
		sprintf(dateStr, "%s %d.%02d.%d", DayOfWeekToString(dayOfWeek), day, month, year);		

		lv_label_set_text(label_date, dateStr);
		lv_obj_align(label_date, lv_scr_act(), LV_ALIGN_CENTER, 0, 60);

		currentYear = year;
		currentMonth = month;
		currentDayOfWeek = dayOfWeek;
		currentDay = day;
	}
  }

  // TODO heartbeat = heartBeatController.GetValue();
  if(heartbeat.IsUpdated())
  {
    char heartbeatBuffer[4];
    sprintf(heartbeatBuffer, "%d", heartbeat.Get());
    lv_label_set_text(heartbeatValue, heartbeatBuffer);
    lv_obj_align(heartbeatIcon, lv_scr_act(), LV_ALIGN_IN_BOTTOM_LEFT, 5, -2);
    lv_obj_align(heartbeatValue, heartbeatIcon, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_align(heartbeatBpm, heartbeatValue, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
  }

  // TODO stepCount = stepController.GetValue();
  if(stepCount.IsUpdated()) 
  {
    char stepBuffer[5];
    sprintf(stepBuffer, "1234");  //sprintf(stepBuffer, "%lu", stepCount.Get());
    lv_label_set_text(stepValue, stepBuffer);
    lv_obj_align(stepValue, lv_scr_act(), LV_ALIGN_IN_BOTTOM_RIGHT, -5, -2);
    lv_obj_align(stepIcon, stepValue, LV_ALIGN_OUT_LEFT_MID, -5, 0);
  }

  return running;
}



const char *Clock::MonthShortToString(Pinetime::Controllers::DateTime::Months month) {
  return Clock::MonthsString[(uint8_t) month];
}

const char *Clock::MonthShortToStringLow(Pinetime::Controllers::DateTime::Months month) {
  return Clock::MonthsStringLow[(uint8_t) month];
}

const char *Clock::MonthsToStringLow(Pinetime::Controllers::DateTime::Months month) {
  return Clock::MonthsLow[(uint8_t) month];
}

const char *Clock::DayOfWeekToString(Pinetime::Controllers::DateTime::Days dayOfWeek) {
  return Clock::DaysString[(uint8_t) dayOfWeek];
}

const char *Clock::DayOfWeekShortToString(Pinetime::Controllers::DateTime::Days dayOfWeek) {
  return Clock::DaysStringShort[(uint8_t) dayOfWeek];
}

const char *Clock::DayOfWeekToStringLow(Pinetime::Controllers::DateTime::Days dayOfWeek) {
  return Clock::DaysStringLow[(uint8_t) dayOfWeek];
}

const char *Clock::DayOfWeekShortToStringLow(Pinetime::Controllers::DateTime::Days dayOfWeek) {
  return Clock::DaysStringShortLow[(uint8_t) dayOfWeek];
}


char const *Clock::DaysStringLow[] = {"--", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag", "Sonntag"};

char const *Clock::DaysStringShortLow[] = {"--", "Mon", "Die", "Mit", "Don", "Fre", "Sam", "Son"};

char const *Clock::DaysStringShort[] = {"--", "MON", "DIE", "MIT", "DON", "FRE", "SAM", "SON"};

char const *Clock::DaysString[] = {"--", "MONTAG", "DIENSTAG", "MITTWOCH", "DONNERSTAG", "FREITAG", "SAMSTAG", "SONNTAG"};

char const *Clock::MonthsString[] = {"--", "JAN", "FEB", "MÄR", "APR", "MAI", "JUN", "JUL", "AUG", "SEP", "OKT", "NOV", "DEZ"};

char const *Clock::MonthsStringLow[] = {"--", "Jan", "Feb", "Mär", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dez"};

char const *Clock::MonthsLow[] = {
  "--", "Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember"};




void Clock::OnObjectEvent(lv_obj_t *obj, lv_event_t event) {
  if(obj == backgroundLabel) {
    if (event == LV_EVENT_CLICKED) {

      running = false;
    }
  }
}

bool Clock::OnButtonPushed() {
  running = false;
  return false;
}


