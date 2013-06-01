#include "link_monitor.h"
#include "config.h"

enum LinkStatus
{
	LinkStatusUnknown,
	LinkStatusFailed,
	LinkStatusOK,
};
	
enum LinkStatus __linkStatus = LinkStatusUnknown;
	
extern TextLayer date_layer;
	
void link_monitor_ping()
{
	//Sending ANY message to the phone would do
	http_time_request();
}

void link_monitor_handle_failure(int error)
{
	switch(error)
	{
		case 1008: //Watchapp not running
			//Considered a link failure
			break;
		
		case 1064: //APP_MSG_BUSY
			//These are more likely to specify a temporary error than a lost watch
			return;
		
		case HTTP_INVALID_BRIDGE_RESPONSE + 1000:
			//The phone may have no internet connection, but the link should be fine
			return;
	
#ifdef DEBUG
		default:
			//Unrecognised failure reason. Debug.
			text_layer_set_text(&date_layer, itoa(error));
#endif
	}
	
	if(__linkStatus == LinkStatusOK)
	{
		//The link has just failed, notify the user
		// Vibe pattern: ON, OFF, ON, ...
		static const uint32_t const segments[] = { 150, 100, 150, 100, 300 };
		VibePattern pat = {
			.durations = segments,
			.num_segments = ARRAY_LENGTH(segments),
		};
	
		vibes_enqueue_custom_pattern(pat);
	}
	
	__linkStatus = LinkStatusFailed;
}

void link_monitor_handle_success()
{
	if(__linkStatus == LinkStatusFailed)
	{
		//Notify the user of reconnection
		vibes_short_pulse();
	}
	
	__linkStatus = LinkStatusOK;
}