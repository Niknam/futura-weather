Futura Weather 2
==============

![](https://previews.dropbox.com/p/thumb/AAaJIdtC_KpCmlNpDy_8C9HYAcVlNbNF8cGdQkOyMFdTBiKfLmm8CAPs9NZgKDOhi6SnNHQZ6CuvTLig_32LH0wHIxIRUCkycR7xW7uOjTDJ1Yx3j2NcJuL7lj_kg5mtau0roPH6DYU7H8DHOh_ObCNnfKXtZyOjjBpLoL-My3DvYnO3rbF0xMR1CVxLUCZgI2NC-ui1Meb7hVqu1HwPztlwSDSvbv6f25kXn65YBnugpknfS_CJzYjMgWictaFfX0thqD_SmeNUJNrvanq13N8bzJZM3YHtAy4UXoWIEU36vcpZBDHGZE1Sbk3-wdYkSFci3UuiEwMvrqPGR00dFZuB/p.png?size_mode=5)

Modification of "Roboto Weather" by Martin Rosinski. http://www.mypebblefaces.com/apps/3601/3408/

**Download here:**

[Celsius](http://www.mypebblefaces.com/download_app?cID=2905&fID=3735&version=2&uID=3263) [[QR](http://www.mypebblefaces.com/QR/temp/pebble711ab886558fe228fc3ccdf035abfdd7.png)]

[Celsius (no vibration alert)](http://www.mypebblefaces.com/download_app?cID=3822&fID=3777&version=2&uID=3263) [[QR](http://www.mypebblefaces.com/QR/temp/pebble7bc9d76276085491cae3f671c95a582d.png)] *

[Celsius (no phone link monitor)](http://www.mypebblefaces.com/download_app?cID=5759&fID=6608&version=2&uID=3263) [[QR](http://www.mypebblefaces.com/QR/temp/pebble696384727ee254ddeea08b609335bbde.png)] **

[Fahrenheit](http://www.mypebblefaces.com/download_app?cID=5760&fID=6609&version=2&uID=3263) [[QR](http://www.mypebblefaces.com/QR/temp/pebbled5b6df371fc71ce5cae91741716deb23.png)]

[Fahrenheit (no vibration alert)](http://www.mypebblefaces.com/download_app?cID=5761&fID=6610&version=2&uID=3263) [[QR](http://www.mypebblefaces.com/QR/temp/pebble4933fe7a41ca893b6572ea70f8230450.png)] *

[Fahrenheit (no phone link monitor)](http://www.mypebblefaces.com/download_app?cID=5762&fID=6611&version=2&uID=3263) [[QR](http://www.mypebblefaces.com/QR/temp/pebble7a6129f628e47686b60b80149f3f0610.png)] **

_* Does not vibrate upon phone disconnections, and pings the phone less frequently._

_** Does not monitor the link between watch and phone (i.e. does not ping the phone at all)._

Features
--------

- Location-based weather information from Yahoo! Weather (updates every 15 minutes)
- Phone disconnection alert
- Stale weather information indicator
- 12h/24h support

Installation Notes
------------------

For the weather notifications to work you need to have an **httpebble** app installed on your phone.

#### Android users
Download and install **Pebble Connect** from Google Play:
https://play.google.com/store/apps/details?id=com.lukekorth.httpebble

#### iOS users
Download and install **httpebble** from App Store:
https://itunes.apple.com/app/httpebble/id650174711?l=en&mt=8

Error Feedback
--------------

### Stale Weather Information
![](https://dl.dropboxusercontent.com/u/572608/futura-weather-v2/futura-weather-stale-indication.png)

The degree symbol will disappear if no successful weather request has been made during the previous 60 minutes, to indicate that the weather information is stale. Most often, this would also indicate that your Internet connection is offline.

The watch will try to reconnect during the subsequent 3 hours following the initial error, and thereafter become passive. At this point you will have to reload the watch face manually in order for it to send weather requests again.

### Phone/httpebble App Disconnections
![](https://dl.dropboxusercontent.com/u/572608/futura-weather-v2/futura-weather-phone-disconnection.png)

The watch regularly pings the phone to check whether it is still connected or not. Upon noticing that the phone is disconnected from the watch, the phone disconnection icon above will be displayed. 30 minutes after the disconnection, the temperature field will be blanked out. The same feedback will be displayed if the httpebble app is disconnected from the watch, or if loading the watch face while being disconnected from the phone or the httpebble app.

As when noticing HTTP errors, the watch will try to reconnect during the subsequent 3 hours following phone disconnections. Likewise, it will become passive thereafter and the watch face has to be manually reloaded in order to be able to check for connected phones again.

### HTTP Error
![](https://dl.dropboxusercontent.com/u/572608/futura-weather-v2/futura-weather-2-clouderror.png)

If loading the watch face while your phone's Internet connection is offline, or if the watch has problems connecting to the weather information provider, the above icon will be displayed. The same icon will be displayed if recovering from a phone disconnection while having no Internet connection.

### Loading Animation
![](https://dl.dropboxusercontent.com/u/572608/futura-weather-v2/futura-weather-loading.png)

Upon loading the watch face, the watch will instantly try to fetch updated weather. However, if experiencing delays of some sort, the watch will wait until the next consecutive minute to try again. To indicate that the watch face is still loading, a loading animation will appear 4 seconds after loading the watch face, if no successful weather request has been made until then.

Weather Condition Icons
-----------------------

All icons (except "Hot" and "Cold") courtesy of **Onlyoly** (http://drbl.in/hozN). Used with permission.

"Hot" and "Cold" icons courtesy of **Dmitry Baranovskiy** (http://thenounproject.com/DmitryBaranovskiy) and used under a Creative Commons Attribution license (CC BY 3.0).

![](https://dl.dropboxusercontent.com/u/572608/futura-weather-v2/weather-condition-icons.png)

Localization Notes
------------------

Follow these steps to translate the displayed weekdays into another language.

_**Nerd summary:** Compile your own version of this GitHub project. Localization code is found in main.c, within the_ *handle_tick() function. Uncomment and edit your own labels. Note that only ISO Basic Latin characters are supported.*

1. Log in to http://cloudpebble.net

2. Click "Import Project"

3. Click the tab "Import from GitHub"

4. Enter the project name of your choice and paste the following URL in the "GitHub Project" field: https://github.com/Niknam/futura-weather

5. Click "Import"

6. When the import is done, you should see a menu on the left-hand side. Click on "main.c" under "Source Files".

7. Scroll down until you see the text "LOCALIZATION CODE BEGIN". Remove this line. Also remove the line further down where you can see the text "LOCALIZATION CODE END". Once you have done this the localization code is activated and you can start editing it.

8. Replace **XXX** for each weekday with the label of your choice. Make sure to leave the quotation marks and to make the label no more and no less than 3 characters long. Supported characters are A-Z, a-z and 0-9.

9. If your labels consist of 2 characters and 1 space, e.g. "Mo ", make sure to _uncomment_ this line of code (i.e. remove the 2 initial slash symbols), just below the code for your 'Sunday' label: **memmove(&date_text[3], &date_text[4], sizeof(date_text) - 1);**

10. When you are done editing, click "Save" in the lower right corner

11. Click "Compilation" in the left-hand side menu, and then "Run build"

12. CloudPebble will thereafter generate a QR code. Either scan this with a code scanner on your mobile device, or click the "pbw" link to download the watch face file to your computer. Move this file to your Dropbox folder, or similar, to be able to access it from your mobile device and thereafter install it on your Pebble watch.


Version History
---------------

#### 2.0.2 (November 27, 2013)
- Fixes the issue of the temperature text not being displayed at temperatures of -10° and below
- Size of weather icon and temperature text slightly decreased to make weather layer less crowded

#### 2.0.1 (November 2, 2013)
- Small fix to make temperature more readable at subzero temperatures (adds a space between minus sign and temperature value)

#### 2.0 (October 8, 2013)
- New weather icons, courtesy of Onlyoly (http://drbl.in/hozN)
- New weather information provider: Yahoo! Weather (thanks to Luke Korth and orviwan for the great assistance!)
- More accurate error handling and feedback: Now differentiates between HTTP errors and phone disconnections, and also indicates stale weather by removing degree symbol (see "[Error Feedback](https://github.com/Niknam/futura-weather#error-feedback)")
- Loading animation will be displayed if no weather information is displayed within 4 seconds of loading the watch face
- Update interval changed back to 15 minutes
- Added possibility to localize weekdays into another language (Manual compilation needed. See "[Localization Notes](https://github.com/Niknam/futura-weather#localization-notes)" for further instructions.)

#### 1.73 (August 14, 2013)
- Update interval changed from 15 to 30 minutes to lower the monthly cost of weather requests
- Fix to reduce the amount of simultaneous weather requests

#### 1.72 (June 18, 2013)
- 3-character temperature support (100° and above, and -10° and below)

#### 1.71 (June 17, 2013)
- New endpoint location, provided by Luke Korth

#### 1.7 (June 1, 2013)
- Initial '0' removed from day of month
- Internet connection issues not considered as phone connection loss, i.e. no vibration alert (fix by ZoneMR)
