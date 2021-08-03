#include <cstdio>

#include <libs/date/includes/date/date.h>
#include "DateTimeController.h" //#include "components/datetime/DateTimeController.h"  -> ohne Zusatzpfad in https://github.com/AppKaki/lvgl-wasm/tree/master/clock
#include <libs/lvgl/lvgl.h>
#include <../lvgl.h>
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
                                           bleController{bleController} 
{
	seconds_old = 0;
	
	label_date = lv_label_create(lv_scr_act(), nullptr);
	lv_obj_align(label_date, lv_scr_act(), LV_ALIGN_CENTER, 20, 85);
	lv_obj_set_style_local_text_color(label_date, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x999999));	

	backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
	backgroundLabel->user_data = this;
	lv_obj_set_click(backgroundLabel, true);
	lv_obj_set_event_cb(backgroundLabel, event_handler);
	lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
	lv_obj_set_size(backgroundLabel, 240, 240);
	lv_obj_set_pos(backgroundLabel, 0, 0);
	lv_label_set_text(backgroundLabel, "");						   


						   
						   

	/* Binary Watch */
	/****************/

	/* 2 Möglichkeiten, Anzeige zu beeinflussen:
		-> border width: leerer Ring (=1) vs. gefüllt (=size/2)
		-> set_value: grau (val=min) vs. farbig (val=max)
	*/	
	/********************/
	/* Generate Circles */
	/********************/
	
	// Generate Circles //
	
	// for seconds //
	// from top down
	for (uint8_t i = 0; i < NR_SECBARS; i++)
	{	
		secBar[i] = lv_bar_create(lv_scr_act(), nullptr);
		lv_obj_set_style_local_bg_color(secBar[i], LV_BAR_PART_BG, LV_STATE_DEFAULT, LED_COL_SEC_OFF);
		lv_obj_set_style_local_line_color(secBar[i], LV_ARC_PART_INDIC, LV_STATE_DEFAULT, LED_COL_SEC_ON);			
		lv_obj_set_size(secBar[i], 230, 5);	
		lv_bar_set_range(secBar[i], 0, 15);		// range = 1/4 minute
		//lv_bar_set_anim_time(secBar[i], 100);
		lv_bar_set_value(secBar[i], 0, LV_ANIM_OFF);
		
		if (i == 0)	// positioning first bar absolute
		{
			lv_obj_align(secBar[i], lv_scr_act(), LV_ALIGN_IN_LEFT_MID, 5, (LED_SIZE1));
		}
		else						// positioning following bars relative to predecessor (underneath)
		{
			lv_obj_align(secBar[i], secBar[i-1], LV_ALIGN_OUT_BOTTOM_MID, 0, SECBAR_SPACE_V);
		}
	}		   

}

Clock::~Clock() {
  lv_obj_clean(lv_scr_act());
}

bool Clock::Refresh()
{

	currentDateTime = dateTimeController.CurrentDateTime();

	if (currentDateTime.IsUpdated()) 
	{
		auto newDateTime = currentDateTime.Get();

		auto dp = date::floor<date::days>(newDateTime);
		auto time = date::make_time(newDateTime - dp);
		auto yearMonthDay = date::year_month_day(dp);

		auto year = (int) yearMonthDay.year();
		auto month = static_cast<Pinetime::Controllers::DateTime::Months>((unsigned) yearMonthDay.month());
		auto day = (unsigned) yearMonthDay.day();
		auto dayOfWeek = static_cast<Pinetime::Controllers::DateTime::Days>(date::weekday(yearMonthDay).iso_encoding());

		int hour = time.hours().count();
		auto minute = time.minutes().count();
		auto second = time.seconds().count();


		// Binary Watch //
		//////////////////
	
		// seconds //
		if (seconds_old != second)
		{
			if (second == 0)
			{
				lv_bar_set_value(secBar[0], 0, LV_ANIM_OFF);		
				lv_bar_set_value(secBar[1], 0, LV_ANIM_OFF);		
				lv_bar_set_value(secBar[2], 0, LV_ANIM_OFF);		
				lv_bar_set_value(secBar[3], 0, LV_ANIM_OFF);		
				
			}
			else if (second < 16)
			{
				lv_bar_set_value(secBar[0], second, LV_ANIM_OFF);		
			}
			else if (second < 31)
			{
				lv_bar_set_value(secBar[1], second-16, LV_ANIM_OFF);		
			}
			else if (second < 46)
			{
				lv_bar_set_value(secBar[2], second-31, LV_ANIM_OFF);		
			}
			else if (second < 60)
			{
				lv_bar_set_value(secBar[3], second-46, LV_ANIM_OFF);		
			}
				
			
			
			seconds_old = second;
		}
		// end binary watch //	
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


