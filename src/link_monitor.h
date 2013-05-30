//Monitors the connection to the phone
//Can alert the user if it's broken

#include "pebble_os.h"
#include "http.h"
#include "util.h"

void link_monitor_ping();
void link_monitor_handle_failure(int error);
void link_monitor_handle_success();