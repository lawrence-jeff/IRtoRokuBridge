![image](https://user-images.githubusercontent.com/5248605/26999629-1feb274c-4d64-11e7-8383-fce992335fc0.png)
# IRtoRokuBridge
Uses an ESP8266 to convert IR codes to Roku API commands to allow the use of IR universal remotes w/ a Roku Stick

Allows you to use a universal IR based remote (ex Harmony) to control a Roku stick so you can use a single remote for everything.
(Roku Sticks use some sort of RF or bluetooth so can't be controlled by a classic IR only remote)
This also has direct channel activation which I use via customized Remote buttons in the Watch Roku activity programmed in my Harmony 550
This gives you a button on the screen labeled (for example) Syfy which directly runs that app/channel.\

Hardware:<br>
See Fritzing folder for a downloadable project but it's pretty basic, you need an ESP8266 (I prefer the nodemcu boards which run about 4 dollars from ebay) and an IR reciever.

Connect them like this:<br>
<img src="https://github.com/lawrence-jeff/IRtoRokuBridge/blob/master/Fritzing/IRtoRokuBridge_bb.png" width="250">
To customize the code:
 1) Replace the IR reciever pin with yours - I use a nodemcu device with IR reciever and LED attached to D5 (Use Vin/5v for power for better range). THe circuit above is a simplified version that drops the status LED
 2) Replace the Roku IP with your value
 3) Update your wifi information
 4) Use the existing codes or customize. The built in codes are from something in the Harmony datbase called Rikki Roku. if you want to use custom codes find a device that uses NEC codes (you can use IRDump example to confirm encoding) and then via telnet or serial debug press each button to get the code - then update the function for the corresponding cmd.
5) Customize the quick launch apps. Obtain the appID by selecting the channel using the app/remote and then from a browser get the app id via http://RokuIP:8060/query/active-app (Replace RokuIP with the IP of your device). Use the ID provided in place of the ones in this code.
6) Upload via arduino IDE (uses arduino ESP8266 project)

This uses pieces from a number of programs:<br?
 IRremoteESP8266: https://github.com/markszabo/IRremoteESP8266/<br>
 Telnet Debug https://github.com/JoaoLopesF/ESP8266-RemoteDebug-Telnet<br>
