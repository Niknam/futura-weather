#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

/* Custom layer type for displaying time with different fonts for hour
* and minute.
*/
typedef struct _TimeLayer
{
    Layer layer;
    const char *hour_text;
    const char *minute_text;
    GFont hour_font;
    GFont minute_font;
    GTextLayoutCacheRef layout_cache;
    GColor text_color : 2;
    GColor background_color : 2;
    GTextOverflowMode overflow_mode : 2;
} TimeLayer;

void time_layer_update_proc(TimeLayer *tl, GContext* ctx);
void time_layer_set_text(TimeLayer *tl, char *hour_text, char *minute_text);
void time_layer_set_fonts(TimeLayer *tl, GFont hour_font, GFont minute_font);
void time_layer_set_text_color(TimeLayer *tl, GColor color);
void time_layer_set_background_color(TimeLayer *tl, GColor color);
void time_layer_init(TimeLayer *tl, GRect frame);