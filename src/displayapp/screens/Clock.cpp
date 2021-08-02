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
                                           bleController{bleController} {
	displayedChar[0] = 0;
	displayedChar[1] = 0;
	displayedChar[2] = 0;
	displayedChar[3] = 0;
	displayedChar[4] = 0;
	displayedChar[5] = 0;
	displayedChar[6] = 0;

	
	

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
	
	// for minutes //
	/////////////////
	
	// for minutes //
	/////////////////
/*	
	minLED5 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(minLED5, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_MIN_OFF);
	lv_obj_set_style_local_radius(minLED5, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_style_local_border_width(minLED5, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_RING1);
	lv_obj_set_style_local_line_color(minLED5,LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_RING);	
	lv_obj_set_size(minLED5, LED_SIZE1, LED_SIZE1);
	lv_obj_align(minLED5, lv_scr_act(), LV_ALIGN_IN_LEFT_MID, LED_SPACE_H1/2, (5+(LED_SIZE1/2+LED_SPACE_V1)));		// absolut setzen (LV_ALIGN_IN_LEFT_MID, 5, 5) ist i.O., wenn Zeit noch 20 px höher -> LV_ALIGN_IN_RIGHT_MID, 0, -35);)
	//lv_obj_align(minLED5, label_time, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10); 	// relativ zu Uhrzeit setzen an linker unterer Kante, dann 10 px drunter
	
	minLED4 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(minLED4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_MIN_OFF);
	lv_obj_set_style_local_radius(minLED4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_style_local_border_width(minLED4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_RING1);
	lv_obj_set_style_local_line_color(minLED4,LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_RING);	
	lv_obj_set_size(minLED4, LED_SIZE1, LED_SIZE1);
	lv_obj_align(minLED4, minLED5, LV_ALIGN_OUT_RIGHT_MID, LED_SPACE_H1, 0);

	minLED3 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(minLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_MIN_OFF);
	lv_obj_set_style_local_radius(minLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_style_local_border_width(minLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_RING1);
	lv_obj_set_style_local_line_color(minLED3,LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_RING);	
	lv_obj_set_size(minLED3, LED_SIZE1, LED_SIZE1);
	lv_obj_align(minLED3, minLED4, LV_ALIGN_OUT_RIGHT_MID, LED_SPACE_H1, 0);		
	
	minLED2 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(minLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_MIN_OFF);
	lv_obj_set_style_local_radius(minLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_style_local_border_width(minLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_RING1);
	lv_obj_set_style_local_line_color(minLED2,LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_RING);
	lv_obj_set_size(minLED2, LED_SIZE1, LED_SIZE1);
	lv_obj_align(minLED2, minLED3, LV_ALIGN_OUT_RIGHT_MID, LED_SPACE_H1, 0);		
	
	minLED1 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(minLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_MIN_OFF);
	lv_obj_set_style_local_radius(minLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_style_local_border_width(minLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_RING1);
	lv_obj_set_style_local_line_color(minLED1,LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_RING);		
	lv_obj_set_size(minLED1, LED_SIZE1, LED_SIZE1);
	lv_obj_align(minLED1, minLED2, LV_ALIGN_OUT_RIGHT_MID, LED_SPACE_H1, 0);		
	
	minLED0 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(minLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_MIN_OFF);
	lv_obj_set_style_local_radius(minLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_style_local_border_width(minLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_RING1);
	lv_obj_set_style_local_line_color(minLED0,LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_RING);		
	lv_obj_set_size(minLED0, LED_SIZE1, LED_SIZE1);
	lv_obj_align(minLED0, minLED1, LV_ALIGN_OUT_RIGHT_MID, LED_SPACE_H1, 0);		
	
	// for hours //
	///////////////
	
	hourLED4 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(hourLED4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_HOUR_OFF);
	lv_obj_set_style_local_radius(hourLED4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_style_local_border_width(hourLED4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_RING1);
	lv_obj_set_style_local_line_color(hourLED4,LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_RING);		
	lv_obj_set_size(hourLED4, LED_SIZE1, LED_SIZE1);
	lv_obj_align(hourLED4, minLED4, LV_ALIGN_OUT_BOTTOM_MID, 0, (-(2*LED_SIZE1+LED_SPACE_V1)));	// 1/4 Kreis über minLED4

	hourLED3 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(hourLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_HOUR_OFF);
	lv_obj_set_style_local_radius(hourLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_style_local_border_width(hourLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_RING1);
	lv_obj_set_style_local_line_color(hourLED3,LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_RING);			
	lv_obj_set_size(hourLED3, LED_SIZE1, LED_SIZE1);
	lv_obj_align(hourLED3, hourLED4, LV_ALIGN_OUT_RIGHT_MID, LED_SPACE_H1, 0);		
	
	hourLED2 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(hourLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_HOUR_OFF);
	lv_obj_set_style_local_radius(hourLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_style_local_border_width(hourLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_RING1);
	lv_obj_set_style_local_line_color(hourLED2,LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_RING);			
	lv_obj_set_size(hourLED2, LED_SIZE1, LED_SIZE1);
	lv_obj_align(hourLED2, hourLED3, LV_ALIGN_OUT_RIGHT_MID, LED_SPACE_H1, 0);		
	
	hourLED1 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(hourLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_HOUR_OFF);
	lv_obj_set_style_local_radius(hourLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_style_local_border_width(hourLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_RING1);
	lv_obj_set_style_local_line_color(hourLED1,LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_RING);			
	lv_obj_set_size(hourLED1, LED_SIZE1, LED_SIZE1);
	lv_obj_align(hourLED1, hourLED2, LV_ALIGN_OUT_RIGHT_MID, LED_SPACE_H1, 0);		
	
	hourLED0 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(hourLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_HOUR_OFF);
	lv_obj_set_style_local_radius(hourLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_style_local_border_width(hourLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_RING1);
	lv_obj_set_style_local_line_color(hourLED0,LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_RING);			
	lv_obj_set_size(hourLED0, LED_SIZE1, LED_SIZE1);
	lv_obj_align(hourLED0, hourLED1, LV_ALIGN_OUT_RIGHT_MID, LED_SPACE_H1, 0);	
*/

	// for days //
	/////////////////
	
	dayLED4 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(dayLED4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_DAY_OFF);
	lv_obj_set_style_local_radius(dayLED4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_style_local_border_width(dayLED4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_RING2);
	lv_obj_set_style_local_line_color(dayLED4,LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_RING);	
	lv_obj_set_size(dayLED4, LED_SIZE2, LED_SIZE2);
	lv_obj_align(dayLED4, lv_scr_act(), LV_ALIGN_CENTER, -(2,5*LED_SIZE2), ((LED_SIZE1));		// absolut setzen (LV_ALIGN_IN_LEFT_MID, 5, 5) ist i.O., wenn Zeit noch 20 px höher -> LV_ALIGN_IN_RIGHT_MID, 0,

	dayLED3 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(dayLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_DAY_OFF);
	lv_obj_set_style_local_radius(dayLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_style_local_border_width(dayLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_RING2);
	lv_obj_set_style_local_line_color(dayLED3,LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_RING);	
	lv_obj_set_size(dayLED3, LED_SIZE2, LED_SIZE2);
	lv_obj_align(dayLED3, dayLED4, LV_ALIGN_OUT_RIGHT_MID, LED_SPACE_H2, 0);		
	
	dayLED2 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(dayLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_DAY_OFF);
	lv_obj_set_style_local_radius(dayLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_style_local_border_width(dayLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_RING2);
	lv_obj_set_style_local_line_color(dayLED2,LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_RING);
	lv_obj_set_size(dayLED2, LED_SIZE2, LED_SIZE2);
	lv_obj_align(dayLED2, dayLED3, LV_ALIGN_OUT_RIGHT_MID, LED_SPACE_H2, 0);		
	
	dayLED1 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(dayLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_DAY_OFF);
	lv_obj_set_style_local_radius(dayLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_style_local_border_width(dayLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_RING2);
	lv_obj_set_style_local_line_color(dayLED1,LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_RING);		
	lv_obj_set_size(dayLED1, LED_SIZE2, LED_SIZE2);
	lv_obj_align(dayLED1, dayLED2, LV_ALIGN_OUT_RIGHT_MID, LED_SPACE_H2, 0);		
	
	dayLED0 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(dayLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_DAY_OFF);
	lv_obj_set_style_local_radius(dayLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_style_local_border_width(dayLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_RING2);
	lv_obj_set_style_local_line_color(dayLED0,LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_RING);		
	lv_obj_set_size(dayLED0, LED_SIZE2, LED_SIZE2);
	lv_obj_align(dayLED0, dayLED1, LV_ALIGN_OUT_RIGHT_MID, LED_SPACE_H2, 0);		
	
	// for months //
	///////////////
	


	monLED3 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(monLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_MON_OFF);
	lv_obj_set_style_local_radius(monLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_style_local_border_width(monLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_RING2);
	lv_obj_set_style_local_line_color(monLED3,LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_RING);			
	lv_obj_set_size(monLED3, LED_SIZE2, LED_SIZE2);
	lv_obj_align(monLED3, minLED3, LV_ALIGN_OUT_RIGHT_MID, 0, ((LED_SIZE2+LED_SPACE_V2)));	// 1/4 Kreis unter dayLED4	
	
/*	monLED2 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(monLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_MON_OFF);
	lv_obj_set_style_local_radius(monLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_style_local_border_width(monLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_RING2);
	lv_obj_set_style_local_line_color(monLED2,LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_RING);			
	lv_obj_set_size(monLED2, LED_SIZE2, LED_SIZE2);
	lv_obj_align(monLED2, monLED3, LV_ALIGN_OUT_RIGHT_MID, LED_SPACE_H2, 0);		
	
	monLED1 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(monLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_MON_OFF);
	lv_obj_set_style_local_radius(monLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_style_local_border_width(monLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_RING2);
	lv_obj_set_style_local_line_color(monLED1,LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_RING);			
	lv_obj_set_size(monLED1, LED_SIZE2, LED_SIZE2);
	lv_obj_align(monLED1, monLED2, LV_ALIGN_OUT_RIGHT_MID, LED_SPACE_H2, 0);		
	
	monLED0 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(monLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_MON_OFF);
	lv_obj_set_style_local_radius(monLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_style_local_border_width(monLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_RING2);
	lv_obj_set_style_local_line_color(monLED0,LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LED_COL_RING);			
	lv_obj_set_size(monLED0, LED_SIZE2, LED_SIZE2);
	lv_obj_align(monLED0, monLED1, LV_ALIGN_OUT_RIGHT_MID, LED_SPACE_H2, 0);	
*/		   
						   
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

		char secondsChar[3];
		sprintf(secondsChar, "%02d", static_cast<int>(second));

		char minutesChar[3];
		sprintf(minutesChar, "%02d", static_cast<int>(minute));

		char hoursChar[3];
		sprintf(hoursChar, "%02d", hour);


		// Binary Watch //
		//////////////////
/*	
		if (hoursChar[0] != displayedChar[0] || hoursChar[1] != displayedChar[1] || minutesChar[0] != displayedChar[2] || minutesChar[1] != displayedChar[3])
		{
			//bool binMinArray[5] = {false};
			//bool binHourArray[4] = {false};
			
			uint8_t binMinTmp = static_cast<int>(minute);
			uint8_t binHourTmp = static_cast<int>(hour);
			
			// Minuten //
			if (binMinTmp >= 32)
			{
				binMinTmp -= 32;
				lv_obj_set_style_local_bg_color(minLED5, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xDD0000));
				//binMinArray[5] = true;
			}
			else
			{
				lv_obj_set_style_local_bg_color(minLED5, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x220000));
			}
			
			if (binMinTmp >= 16)
			{
				binMinTmp -= 16;
				//binMinArray[4] = true;
				lv_obj_set_style_local_bg_color(minLED4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xDD0000));
			}
			else
			{
				lv_obj_set_style_local_bg_color(minLED4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x220000));
			}	
			
			if (binMinTmp >= 8)
			{
				binMinTmp -= 8;
				//binMinArray[3] = true;
				lv_obj_set_style_local_bg_color(minLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xDD0000));
			}
			else
			{
				lv_obj_set_style_local_bg_color(minLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x220000));
			}
			
			if (binMinTmp >= 4)
			{
				binMinTmp -= 4;
				//binMinArray[2] = true;
				lv_obj_set_style_local_bg_color(minLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xDD0000));
			}		
			else			
			{
				lv_obj_set_style_local_bg_color(minLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x220000));
			}
			
			if (binMinTmp >= 2)
			{
				binMinTmp -= 2;
				//binMinArray[1] = true;
				lv_obj_set_style_local_bg_color(minLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xDD0000));
			}
			else
			{
				lv_obj_set_style_local_bg_color(minLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x220000));
			}
			
			if (binMinTmp == 1)
			{
				//binMinArray[0] = true;
				lv_obj_set_style_local_bg_color(minLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xDD0000));
			}
			else
			{
				lv_obj_set_style_local_bg_color(minLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x220000));
			}			
			
			// Stunden //
			if (binHourTmp >= 16)
			{
				binHourTmp -= 16;
				//binHourArray[4] = true;
				lv_obj_set_style_local_bg_color(hourLED4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x00DD00));
			}
			else
			{
				lv_obj_set_style_local_bg_color(hourLED4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x002200));
			}	
			
			if (binHourTmp >= 8)
			{
				binHourTmp -= 8;
				//binHourArray[3] = true;
				lv_obj_set_style_local_bg_color(hourLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x00DD00));
			}
			else
			{
				lv_obj_set_style_local_bg_color(hourLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x002200));
			}
			
			if (binHourTmp >= 4)
			{
				binHourTmp -= 4;
				//binHourArray[2] = true;
				lv_obj_set_style_local_bg_color(hourLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x00DD00));
			}		
			else			
			{
				lv_obj_set_style_local_bg_color(hourLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x002200));
			}
			
			if (binHourTmp >= 2)
			{
				binHourTmp -= 2;
				//binHourArray[1] = true;
				lv_obj_set_style_local_bg_color(hourLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x00DD00));
			}
			else
			{
				lv_obj_set_style_local_bg_color(hourLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x002200));
			}
			
			if (binHourTmp == 1)
			{
				//binHourArray[0] = true;
				lv_obj_set_style_local_bg_color(hourLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x00DD00));
			}
			else
			{
				lv_obj_set_style_local_bg_color(hourLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x002200));
			}			
			
			
		}
		// Ende Binary Watch //
*/	
	
	
	
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


