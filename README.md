# ESP8266-to-AWS-IoT-Core

This Project using NodeMCU ESP8266 with Arduino IDE for programming the board. For Application using Android Studio with programming langguage Kotlin. Project using AWS IoT Core to be MQTT Broker for pass data from Board to Application.

[Application Source Code](https://github.com/rafeyosa/Android-to-AWS-IoT-Core)

## Library Used in NodeMCU Program :
* [Arduino ESP8266 Filesystem Uploader](https://github.com/esp8266/arduino-esp8266fs-plugin)
* [Arduino Client for MQTT](https://github.com/knolleary/pubsubclient)
* [NTPClient](https://github.com/arduino-libraries/NTPClient)
* [Arduino ESP8266FS Plugin](https://github.com/esp8266/arduino-esp8266fs-plugin/releases/tag/0.5.0)

## Creating a Device on AWS IoT Core
1. Login to your AWS Console and search service name **AWS IoT**.
2. Create a Thing (Manage -> Things).
3. Click **Create things** to create new thing.
4. Then select the **Create a single thing** button and click **Next**.
5. Enter the **Thing Name**. In the example, a Thing by name `Thing_01` is created. Click Next to create certificate.
6. Click the option, **Auto-generate a new certificate**, to create a thing in AWS IoT. Click Next to add policy.
7. Click **Create policy** to  create new policy security *(new tab)*.
8. Enter the Name for your policy in the **Name** box as shown in the following screen.
9. Under Action, type: iot:*.
10. Under Resource ARN, type: topic name or just type: *
11. Click **Allow**.
12. Back to proceed create thing, attach policies to certificate and click **Create thing**

## How to Upload Program to ESP8266
1. Install board ESP8266.
> Then go to tools -> board -> board manager -> search ESP8266, install the one by ESP8266 Community, make sure to install version 2.7.4 and not the latest ones. I got the exact same issue on version 3.0.1.
> ['class BearSSL::WiFiClientSecure' has no member named 'loadCertificate'](https://stackoverflow.com/questions/67962746/class-bearsslwificlientsecure-has-no-member-named-loadcertificate)
2. Download or clone this project.
3. Install all required libraries.
4. Change `ssid` and `password` value, with your Wifi SSID and password.
5. (Update)