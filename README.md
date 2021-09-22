# ESP8266-to-AWS-IoT-Core

This Project using NodeMCU ESP8266 with Arduino IDE for programming the board. For Application using Android Studio with programming langguage Kotlin. Project using AWS IoT Core to be MQTT Broker for pass data from Board to Application.

[Application Source Code](https://github.com/rafeyosa/Android-to-AWS-IoT-Core)

## Library Used in NodeMCU Program :
* [Arduino ESP8266 Filesystem Uploader](https://github.com/esp8266/arduino-esp8266fs-plugin)
* [Arduino Client for MQTT](https://github.com/knolleary/pubsubclient)
* [NTPClient](https://github.com/arduino-libraries/NTPClient)

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