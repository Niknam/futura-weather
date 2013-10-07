#ifndef WEATHER_LAYER_H
#define WEATHER_LAYER_H

typedef struct {
	Layer layer;
	BmpContainer icon_layer;
	TextLayer temp_layer;
	TextLayer temp_layer_background;
	bool has_weather_icon;
	char temp_str[5];
} WeatherLayer;

typedef enum {
	WEATHER_ICON_CLEAR_DAY = 0,
	WEATHER_ICON_CLEAR_NIGHT,
	WEATHER_ICON_RAIN,
	WEATHER_ICON_SNOW,
	WEATHER_ICON_SLEET,
	WEATHER_ICON_WIND,
	WEATHER_ICON_FOG,
	WEATHER_ICON_CLOUDY,
	WEATHER_ICON_PARTLY_CLOUDY_DAY,
	WEATHER_ICON_PARTLY_CLOUDY_NIGHT,
	WEATHER_ICON_THUNDER,
	WEATHER_ICON_RAIN_SNOW,
	WEATHER_ICON_RAIN_SLEET,
	WEATHER_ICON_SNOW_SLEET,
	WEATHER_ICON_COLD,
	WEATHER_ICON_HOT,
	WEATHER_ICON_DRIZZLE,
	WEATHER_ICON_NOT_AVAILABLE,
	WEATHER_ICON_PHONE_ERROR,
	WEATHER_ICON_CLOUD_ERROR,
	WEATHER_ICON_LOADING1,
	WEATHER_ICON_LOADING2,
	WEATHER_ICON_LOADING3,
	WEATHER_ICON_COUNT
} WeatherIcon;

void weather_layer_init(WeatherLayer* weather_layer, GPoint pos);
void weather_layer_deinit(WeatherLayer* weather_layer);
void weather_layer_set_icon(WeatherLayer* weather_layer, WeatherIcon icon);
void weather_layer_set_temperature(WeatherLayer* weather_layer, int16_t temperature, bool is_stale);

#endif
