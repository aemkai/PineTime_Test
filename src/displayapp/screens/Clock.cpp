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

	batteryIcon = lv_label_create(lv_scr_act(), nullptr);
	lv_label_set_text(batteryIcon, Symbols::batteryFull);
	lv_obj_align(batteryIcon, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, -5, 2);
	
	label_volt = lv_label_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_text_color(label_volt, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x00CDCD));
	lv_label_set_text(label_volt, "3.45V");	
	lv_obj_align(label_volt, batteryIcon, LV_ALIGN_OUT_LEFT_MID, -5, 0);

	batteryPlug = lv_label_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_text_color(batteryPlug, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFF0000));
	lv_label_set_text(batteryPlug, Symbols::plug);
	lv_obj_align(batteryPlug, label_volt, LV_ALIGN_OUT_LEFT_MID, -5, 0);

	bleIcon = lv_label_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_text_color(bleIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x0000FF));
	lv_label_set_text(bleIcon, Symbols::bluetooth);
	lv_obj_align(bleIcon, batteryPlug, LV_ALIGN_OUT_LEFT_MID, -5, 0);

	//notificationIcon = lv_label_create(lv_scr_act(), NULL);
	//lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(false));
	//lv_obj_align(notificationIcon, nullptr, LV_ALIGN_IN_TOP_LEFT, 10, 0);

	label_date = lv_label_create(lv_scr_act(), nullptr);
	lv_obj_align(label_date, lv_scr_act(), LV_ALIGN_CENTER, 0, 65);
	lv_obj_set_style_local_text_color(label_date, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x999999));
						   
						   					   
	label_time = lv_label_create(lv_scr_act(), nullptr);
	lv_obj_align(label_time, lv_scr_act(), LV_ALIGN_CENTER, 0, -45);
		// -> auch bei Anzeige nochmal gesetzt wg. 12 h -> Werte angleichen!
		// -> hier schon setzen, da Binär-Uhr daran ausgerichtet wird
	//lv_obj_set_style_local_text_font(label_time, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_extrabold_compressed);
	lv_obj_set_style_local_text_font(label_time, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_76);

	label_sec = lv_label_create(lv_scr_act(), nullptr);
	lv_obj_align(label_sec, label_time, LV_ALIGN_OUT_BOTTOM_MID, 85, 10);
		// -> auch bei Anzeige nochmal gesetzt wg. 12 h -> Werte angleichen!
		// -> hier schon setzen, da Binär-Uhr daran ausgerichtet wird
	//lv_obj_set_style_local_text_font(label_time, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_extrabold_compressed);
	lv_obj_set_style_local_text_font(label_sec, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_42);
	lv_obj_set_style_local_text_color(label_sec, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xCCCC00));

	label_time_ampm = lv_label_create(lv_scr_act(), nullptr);
	lv_label_set_text_static(label_time_ampm, "");
	lv_obj_align(label_time_ampm, lv_scr_act(), LV_ALIGN_IN_RIGHT_MID, -30, -55);

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

	/* Binary Watch */
	/****************/

	/* 2 Möglichkeiten, Anzeige zu beeinflussen:
		-> border width: leerer Ring (=1) vs. gefüllt (=size/2)
		-> set_value: grau (val=min) vs. farbig (val=max)
	*/	
	/********************/
	/* Generate Circles */
	/********************/
	
	// for minutes //
	/////////////////
/*	
	minLED5 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(minLED5, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
	lv_obj_set_style_local_radius(minLED5, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_size(minLED5, LED_SIZE, LED_SIZE);
	lv_obj_align(minLED5, lv_scr_act(), LV_ALIGN_IN_LEFT_MID, 10, 5);		// absolut setzen (LV_ALIGN_IN_LEFT_MID, 5, 5) ist i.O., wenn Zeit noch 20 px höher -> LV_ALIGN_IN_RIGHT_MID, 0, -35);)
	//lv_obj_align(minLED5, label_time, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10); 	// relativ zu Uhrzeit setzen an linker unterer Kante, dann 10 px drunter
	
	minLED4 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(minLED4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
	lv_obj_set_style_local_radius(minLED4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_size(minLED4, LED_SIZE, LED_SIZE);
	lv_obj_align(minLED4, minLED5, LV_ALIGN_OUT_RIGHT_MID, (LED_SIZE/2), 0);

	minLED3 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(minLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
	lv_obj_set_style_local_radius(minLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_size(minLED3, LED_SIZE, LED_SIZE);
	lv_obj_align(minLED3, minLED4, LV_ALIGN_OUT_RIGHT_MID, (LED_SIZE/2), 0);		
*/	
	minLED2 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(minLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
	lv_obj_set_style_local_radius(minLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_size(minLED2, LED_SIZE, LED_SIZE);
//lv_obj_align(minLED2, minLED3, LV_ALIGN_OUT_RIGHT_MID, (LED_SIZE/2), 0);		
lv_obj_align(minLED2, lv_scr_act(), LV_ALIGN_IN_LEFT_MID, 10, 5);		// absolut setzen (LV_ALIGN_IN_LEFT_MID, 5, 5) ist i.O., wenn Zeit noch 20 px höher -> LV_ALIGN_IN_RIGHT_MID, 0, -35);)						   
	
	minLED1 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(minLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
	lv_obj_set_style_local_radius(minLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_size(minLED1, LED_SIZE, LED_SIZE);
	lv_obj_align(minLED1, minLED2, LV_ALIGN_OUT_RIGHT_MID, (LED_SIZE/2), 0);		
	
	minLED0 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(minLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
	lv_obj_set_style_local_radius(minLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_size(minLED0, LED_SIZE, LED_SIZE);
	lv_obj_align(minLED0, minLED1, LV_ALIGN_OUT_RIGHT_MID, (LED_SIZE/2), 0);		
	
	// for hours //
	///////////////
	
	hourLED4 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(hourLED4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
	lv_obj_set_style_local_radius(hourLED4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_size(hourLED4, LED_SIZE, LED_SIZE);
//lv_obj_align(hourLED4, minLED4, LV_ALIGN_OUT_BOTTOM_MID, 0, (LED_SIZE/4));	// 1/4 Kreis unter minLED4
lv_obj_align(hourLED4, minLED1, LV_ALIGN_OUT_BOTTOM_MID, 0, (LED_SIZE/4));	// 1/4 Kreis unter minLED4
						   
/*
	hourLED3 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(hourLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
	lv_obj_set_style_local_radius(hourLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_size(hourLED3, LED_SIZE, LED_SIZE);
	lv_obj_align(hourLED3, hourLED4, LV_ALIGN_OUT_RIGHT_MID, (LED_SIZE/2), 0);		
	
	hourLED2 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(hourLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
	lv_obj_set_style_local_radius(hourLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_size(hourLED2, LED_SIZE, LED_SIZE);
	lv_obj_align(hourLED2, hourLED3, LV_ALIGN_OUT_RIGHT_MID, (LED_SIZE/2), 0);		
	
	hourLED1 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(hourLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
	lv_obj_set_style_local_radius(hourLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_size(hourLED1, LED_SIZE, LED_SIZE);
	lv_obj_align(hourLED1, hourLED2, LV_ALIGN_OUT_RIGHT_MID, (LED_SIZE/2), 0);		
	
	hourLED0 = lv_obj_create(lv_scr_act(), nullptr);
	lv_obj_set_style_local_bg_color(hourLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
	lv_obj_set_style_local_radius(hourLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_size(hourLED0, LED_SIZE, LED_SIZE);
	lv_obj_align(hourLED0, hourLED1, LV_ALIGN_OUT_RIGHT_MID, (LED_SIZE/2), 0);		
*/
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

	char timeStr[7];
	sprintf(timeStr, "%c%c:%c%c", hoursChar[0],hoursChar[1],minutesChar[0], minutesChar[1]);

	char secondsStr[3];	  
	  
	  
	// Binary Watch //
	//////////////////
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
//				lv_obj_set_style_local_bg_color(minLED5, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFF0000));
				//binMinArray[5] = true;
			}
			else
			{
//				lv_obj_set_style_local_bg_color(minLED5, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x550000));
			}
			
			if (binMinTmp >= 16)
			{
				binMinTmp -= 16;
				//binMinArray[4] = true;
//				lv_obj_set_style_local_bg_color(minLED4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFF0000));
			}
			else
			{
//				lv_obj_set_style_local_bg_color(minLED4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x550000));
			}	
			
			if (binMinTmp >= 8)
			{
				binMinTmp -= 8;
				//binMinArray[3] = true;
//				lv_obj_set_style_local_bg_color(minLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFF0000));
			}
			else
			{
//				lv_obj_set_style_local_bg_color(minLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x550000));
			}
			
			if (binMinTmp >= 4)
			{
				binMinTmp -= 4;
				//binMinArray[2] = true;
				lv_obj_set_style_local_bg_color(minLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFF0000));
			}		
			else			
			{
				lv_obj_set_style_local_bg_color(minLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x550000));
			}
			
			if (binMinTmp >= 2)
			{
				binMinTmp -= 2;
				//binMinArray[1] = true;
				lv_obj_set_style_local_bg_color(minLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFF0000));
			}
			else
			{
				lv_obj_set_style_local_bg_color(minLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x550000));
			}
			
			if (binMinTmp == 1)
			{
				//binMinArray[0] = true;
				lv_obj_set_style_local_bg_color(minLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFF0000));
			}
			else
			{
				lv_obj_set_style_local_bg_color(minLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x550000));
			}			
/*			
			// Stunden //
			if (binHourTmp >= 16)
			{
				binHourTmp -= 16;
				//binHourArray[4] = true;
				lv_obj_set_style_local_bg_color(hourLED4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x00FF00));
			}
			else
			{
				lv_obj_set_style_local_bg_color(hourLED4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x005500));
			}	
			
			if (binHourTmp >= 8)
			{
				binHourTmp -= 8;
				//binHourArray[3] = true;
				lv_obj_set_style_local_bg_color(hourLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x00FF00));
			}
			else
			{
				lv_obj_set_style_local_bg_color(hourLED3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x005500));
			}
			
			if (binHourTmp >= 4)
			{
				binHourTmp -= 4;
				//binHourArray[2] = true;
				lv_obj_set_style_local_bg_color(hourLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x00FF00));
			}		
			else			
			{
				lv_obj_set_style_local_bg_color(hourLED2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x005500));
			}
			
			if (binHourTmp >= 2)
			{
				binHourTmp -= 2;
				//binHourArray[1] = true;
				lv_obj_set_style_local_bg_color(hourLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x00FF00));
			}
			else
			{
				lv_obj_set_style_local_bg_color(hourLED1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x005500));
			}
			
			if (binHourTmp == 1)
			{
				//binHourArray[0] = true;
				lv_obj_set_style_local_bg_color(hourLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x00FF00));
			}
			else
			{
				lv_obj_set_style_local_bg_color(hourLED0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x005500));
			}			
*/			
			
		}
		// Ende Binary Watch //
	  
	  
	  
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

	lv_obj_align(label_time, lv_scr_act(), LV_ALIGN_CENTER, 0, -45);
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


