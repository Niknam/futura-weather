#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "http.h"
#include "util.h"
#include "weather_layer.h"
#include "time_layer.h"
#include "link_monitor.h"
#include "config.h"

#define MY_UUID { 0x91, 0x41, 0xB6, 0x28, 0xBC, 0x89, 0x49, 0x8E, 0xB1, 0x47, 0x04, 0x9F, 0x49, 0xC0, 0x99, 0xAD }

PBL_APP_INFO(MY_UUID,
             "Futura Weather", "Niknam Moslehi", // Modification of "Roboto Weather" by Martin Rosinski
             2, 0, // App version
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_WATCH_FACE);

#define TIME_FRAME      (GRect(0, 2, 144, 168-6))
#define DATE_FRAME      (GRect(1, 66, 144, 168-62))

// POST variables
#define WEATHER_KEY_LATITUDE 1
#define WEATHER_KEY_LONGITUDE 2
#define WEATHER_KEY_UNIT_SYSTEM 3
	
// Received variables
#define WEATHER_KEY_ICON 1
#define WEATHER_KEY_TEMPERATURE 2
	
#define WEATHER_HTTP_COOKIE 1949327671
#define TIME_HTTP_COOKIE 1131038282

Window window;          // Main window
TextLayer date_layer;   // Layer for the date
TimeLayer time_layer;   // Layer for the time
WeatherLayer weather_layer; // Layer for the weather information

GFont font_date;        // Font for date
GFont font_hour;        // Font for hour
GFont font_minute;      // Font for minute

static int initial_minute;
static bool has_failed = false;
static bool has_timed_out = false;
static bool phone_disconnected = false;

//Counters
static int fail_count = 0;
static int http_request_build_count = 0;
static int http_request_send_count = 0;
static int load_count = 0;

//Weather Stuff
static int our_latitude, our_longitude;
static int16_t last_valid_temperature;
static bool located = false;
static bool initial_request = true;
static bool has_temperature = false;
static bool is_stale = false;

void request_weather();

void failed(int32_t cookie, int http_status, void* context) {
	
	/* This is triggered by invalid responses from the bridge on the phone,
	 * due to e.g. HTTP errors or phone disconnections.
	 */
	if(HTTP_INVALID_BRIDGE_RESPONSE) {
		
		// Phone can still be reached (assume HTTP error)
		if (http_time_request() == HTTP_OK) {
			
			/* Display 'cloud error' icon if:
			 * - HTTP error is noticed upon inital load
			 * - watch has recovered from a phone disconnection, but has no internet connection
			 */
			if (!has_temperature || phone_disconnected) {
				weather_layer_set_icon(&weather_layer, WEATHER_ICON_CLOUD_ERROR);
				phone_disconnected = false;
			}
			
			/* Assume that weather information is stale after 1 hour without a successful
			 * weather request. Indicate this by removing the degree symbol.			
			 */
			else if (has_timed_out || has_temperature || 
					 http_request_build_count > 0 || http_request_send_count > 0) {
				if (fail_count >= 60) {
					is_stale = true;
					weather_layer_set_temperature(&weather_layer, last_valid_temperature, is_stale);
				}
			}
		}
		else {
			
			/* Redundant check to make sure that HTTP time-outs are not triggering the
			 * 'phone error' icon. If appearing to be timed out after 1 hour without a
			 * successful weather request, indicate stale weather information by removing
			 * the degree symbol.
			 */
			if (fail_count >= 60 && has_timed_out && !phone_disconnected) {
				is_stale = true;
				weather_layer_set_temperature(&weather_layer, last_valid_temperature, is_stale);
			}
			
			/* Display 'phone error' icon if:
			 * - watch appears to be disconnected from phone
			 * - watch appears to be disconnected from bridge app on phone
			 *
			 * In case this is triggered by a HTTP time-out, subsequently send a 
			 * weather request for a possible quick re-connection.
			 */
			else {
				if (http_status==1008) {
					phone_disconnected = true;
					weather_layer_set_icon(&weather_layer, WEATHER_ICON_PHONE_ERROR);
					link_monitor_handle_failure(http_status);
				
					http_location_request();
					request_weather();
				}
			}
		}
	}
	
	// Remove temperature text 30 minutes after a phone/bridge app disconnection
	if (fail_count >= 30 && phone_disconnected) {
		text_layer_set_text(&weather_layer.temp_layer, " ");
		has_temperature = false;
	}
	
	// Indicate failure and activate fail counter (see handle_tick() function)
	has_failed = true;
	
	// Re-request the location and subsequently weather on next minute tick
	located = false;
}

void success(int32_t cookie, int http_status, DictionaryIterator* received, void* context) {
	
	is_stale = false;
	
	if(cookie != WEATHER_HTTP_COOKIE) return;
	Tuple* icon_tuple = dict_find(received, WEATHER_KEY_ICON);
	if(icon_tuple) {
		int icon = icon_tuple->value->int8;
		if(icon >= 0 && icon < 18) {
			if (!has_temperature && icon == 17) {
				weather_layer_set_icon(&weather_layer, icon);
			}
			else if (icon != 17) {
				weather_layer_set_icon(&weather_layer, icon);
			}
		}
	}
	Tuple* temperature_tuple = dict_find(received, WEATHER_KEY_TEMPERATURE);
	if(temperature_tuple) {
		last_valid_temperature = temperature_tuple->value->int16;
		weather_layer_set_temperature(&weather_layer, temperature_tuple->value->int16, is_stale);
		has_temperature = true;
	}
	
	link_monitor_handle_success();
	has_failed = false;
	phone_disconnected = false;
}

void location(float latitude, float longitude, float altitude, float accuracy, void* context) {
	// Fix the floats
	our_latitude = latitude * 1000000;
	our_longitude = longitude * 1000000;
	located = true;
	request_weather();
}

void reconnect(void* context) {
	located = false;
	request_weather();
}

void request_weather();

void handle_tick(AppContextRef ctx, PebbleTickEvent *t)
{
    // 'Animate' loading icon until the first successful weather request
	if (!has_temperature && !has_failed)
	{
		if (load_count == 4) {
			weather_layer_set_icon(&weather_layer, WEATHER_ICON_LOADING1);
		}
		if (load_count == 5) {
			weather_layer_set_icon(&weather_layer, WEATHER_ICON_LOADING2);
		}
		if (load_count == 6) {
			weather_layer_set_icon(&weather_layer, WEATHER_ICON_LOADING3);
			load_count = 3;
		}		
		load_count++;
	}
	
	// Subsequently update time and date once every minute
	if (t->units_changed & MINUTE_UNIT) 
	{
		// Need to be static because pointers to them are stored in the text layers
	    static char date_text[] = "XXX 00";
	    static char hour_text[] = "00";
	    static char minute_text[] = ":00";

	    (void)ctx;  // Prevent "unused parameter" warning
		
		if (t->units_changed & DAY_UNIT)
	    {		
		    string_format_time(date_text,
	                           sizeof(date_text),
	                           "%a %d",
	                           t->tick_time);

			// Triggered if day of month < 10
			if (date_text[4] == '0')
			{
			    // Hack to get rid of the leading zero of the day of month
	            memmove(&date_text[4], &date_text[5], sizeof(date_text) - 1);
			}
			
			/*** LOCALIZATION CODE BEGIN ***
			
			// Primitive hack to translate the day of week to another language
			// Needs to be exactly 3 characters, e.g. "Mon" or "Mo "
			// Supported characters: A-Z, a-z, 0-9
			
			if (date_text[0] == 'M')
			{
				memcpy(&date_text, "XXX", 3); // Monday
			}
			
			if (date_text[0] == 'T' && date_text[1] == 'u')
			{
				memcpy(&date_text, "XXX", 3); // Tuesday
			}
			
			if (date_text[0] == 'W')
			{
				memcpy(&date_text, "XXX", 3); // Wednesday
			}
			
			if (date_text[0] == 'T' && date_text[1] == 'h')
			{
				memcpy(&date_text, "XXX", 3); // Thursday
			}
			
			if (date_text[0] == 'F')
			{
				memcpy(&date_text, "XXX", 3); // Friday
			}
			
			if (date_text[0] == 'S' && date_text[1] == 'a')
			{
				memcpy(&date_text, "XXX", 3); // Saturday
			}
			
			if (date_text[0] == 'S' && date_text[1] == 'u')
			{
				memcpy(&date_text, "XXX", 3); // Sunday
			}
			
			// Uncomment the line below if your labels consist of 2 characters and 1 space, e.g. "Mo "
			//memmove(&date_text[3], &date_text[4], sizeof(date_text) - 1);
			
			*** LOCALIZATION CODE END ***/
			
	        text_layer_set_text(&date_layer, date_text);
	    }

	    if (clock_is_24h_style())
	    {
	        string_format_time(hour_text, sizeof(hour_text), "%H", t->tick_time);
			if (hour_text[0] == '0')
	        {
	            // Hack to get rid of the leading zero of the hour
	            memmove(&hour_text[0], &hour_text[1], sizeof(hour_text) - 1);
	        }
	    }
	    else
	    {
	        string_format_time(hour_text, sizeof(hour_text), "%I", t->tick_time);
	        if (hour_text[0] == '0')
	        {
	            // Hack to get rid of the leading zero of the hour
	            memmove(&hour_text[0], &hour_text[1], sizeof(hour_text) - 1);
	        }
	    }

	    string_format_time(minute_text, sizeof(minute_text), ":%M", t->tick_time);
	    time_layer_set_text(&time_layer, hour_text, minute_text);
		
		// Start a counter upon an error and increase by 1 each minute
		// Helps determine when to display error feedback and to stop trying to recover from errors
		if (has_failed)
		{
			fail_count++;
		}
		else
		{
			fail_count = 0;
		}
		
		// Request updated weather every 15 minutes
		// Stop requesting after 3 hours without any successful connection
		if(initial_request || !has_temperature || ((t->tick_time->tm_min % 15) == initial_minute && fail_count <= 180))
		{
			http_location_request();
			initial_request = false;
		}
		else 
		{
			// Ping the phone once every minute to validate the connection
			// Stop pinging after 3 hours without any successful connection
			if (fail_count <= 180)
			{
				// Upon an error, also make a weather request with each ping
				if (fail_count > 0)
				{
					http_location_request();
				}
				
				link_monitor_ping();
			}
			
			/****** The code snippet below is used for the "no vibration" version
			// Ping the phone every 5 minutes
			if (!(t->tick_time->tm_min % 5) && fail_count == 0)
			{
				link_monitor_ping();
			}
			// Upon an error, ping the phone every minute
			// Stop pinging after 3 hours without any successful connection
			else if (fail_count > 0 && fail_count <= 180)
			{
				http_location_request();
				link_monitor_ping();
			}
			******/
		}
	}
}


// Initialize the application
void handle_init(AppContextRef ctx)
{
    PblTm tm;
    PebbleTickEvent t;
    ResHandle res_d;
    ResHandle res_h;

    window_init(&window, "Futura");
    window_stack_push(&window, true /* Animated */);
    window_set_background_color(&window, GColorBlack);

    resource_init_current_app(&APP_RESOURCES);

    res_d = resource_get_handle(RESOURCE_ID_FUTURA_18); // Date font
    res_h = resource_get_handle(RESOURCE_ID_FUTURA_CONDENSED_53); // Time font

    font_date = fonts_load_custom_font(res_d);
    font_hour = fonts_load_custom_font(res_h);
    font_minute = fonts_load_custom_font(res_h);

    time_layer_init(&time_layer, window.layer.frame);
    time_layer_set_text_color(&time_layer, GColorWhite);
    time_layer_set_background_color(&time_layer, GColorClear);
    time_layer_set_fonts(&time_layer, font_hour, font_minute);
    layer_set_frame(&time_layer.layer, TIME_FRAME);
    layer_add_child(&window.layer, &time_layer.layer);

    text_layer_init(&date_layer, window.layer.frame);
    text_layer_set_text_color(&date_layer, GColorWhite);
    text_layer_set_background_color(&date_layer, GColorClear);
    text_layer_set_font(&date_layer, font_date);
    text_layer_set_text_alignment(&date_layer, GTextAlignmentCenter);
    layer_set_frame(&date_layer.layer, DATE_FRAME);
    layer_add_child(&window.layer, &date_layer.layer);

	// Add weather layer
	weather_layer_init(&weather_layer, GPoint(0, 90));
	layer_add_child(&window.layer, &weather_layer.layer);
	
	http_register_callbacks((HTTPCallbacks){.failure=failed,.success=success,.reconnect=reconnect,.location=location}, (void*)ctx);
	
	// Refresh time
	get_time(&tm);
    t.tick_time = &tm;
    t.units_changed = SECOND_UNIT | MINUTE_UNIT | HOUR_UNIT | DAY_UNIT;
	
	initial_minute = (tm.tm_min % 15);
	
	handle_tick(ctx, &t);
}

// Shut down the application
void handle_deinit(AppContextRef ctx)
{
    fonts_unload_custom_font(font_date);
    fonts_unload_custom_font(font_hour);
    fonts_unload_custom_font(font_minute);
	
	weather_layer_deinit(&weather_layer);
}


/********************* Main Program *******************/

void pbl_main(void *params)
{
    PebbleAppHandlers handlers =
    {
       	.init_handler = &handle_init,
       	.deinit_handler = &handle_deinit,
       	.tick_info =
        {
      		.tick_handler = &handle_tick,
       		.tick_units = SECOND_UNIT
        },
		.messaging_info = {
			.buffer_sizes = {
				.inbound = 124,
				.outbound = 256,
		    }
		}
    };
	
	app_event_loop(params, &handlers);
}

void request_weather() {
	if(!located) {
		http_location_request();
		return;
	}
	// Build the HTTP request
	DictionaryIterator *body;
	HTTPResult result = http_out_get("https://ofkorth.net/pebble/weather2", WEATHER_HTTP_COOKIE, &body);
	
	if(result != HTTP_OK) {		
		
		if (HTTP_SEND_TIMEOUT && !phone_disconnected) {
			has_timed_out = true;
		}
		
		http_request_build_count++;
		
	} 
	else {
		has_timed_out = false;
		http_request_build_count = 0;
	}
	
	dict_write_int32(body, WEATHER_KEY_LATITUDE, our_latitude);
	dict_write_int32(body, WEATHER_KEY_LONGITUDE, our_longitude);
	dict_write_cstring(body, WEATHER_KEY_UNIT_SYSTEM, UNIT_SYSTEM);
	
	// Send it
	if(http_out_send() != HTTP_OK) {
		
		if (HTTP_SEND_TIMEOUT && !phone_disconnected) {
			has_timed_out = true;
		}
		
		http_request_send_count++;
		
	} 
	else {
		has_timed_out = false;
		http_request_send_count = 0;
	}
}