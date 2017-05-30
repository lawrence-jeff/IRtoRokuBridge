# IRtoRokuBridge
Converts IR codes to Roku API commands to allow the use of IR universal remotes w/ a Roku Stick

Allows you to use a universal IR based remote (ex Harmony) to control a Roku stick so you can use a single remote for everything.
(Roku Sticks use some sort of RF or bluetooth so can't be controlled by a classic IR only remote)
This also has direct channel activation which I use via customized buttons in the Watch Roku activity programmed in my Harmony 550
This gives you a button on the screen labeled Syfy (for example) which directly kicks off that app.
 
To customize:
 1) Replace the IR reciever pin with yours - I use a nodemcu device with IR reciever and LED attached to D5 (Use Vin/5v for power for better range)
 2) Replace the Roku IP with your value
 3) Update your wifi information
 4) Use the existing codes or customize. The built in codes are from something in the Harmony datbase called Rikki Roku. if you want to use custom codes find a device that uses NEC codes (you can use IRDump example to confirm encoding) and then via telnet or serial debug press each button to get the code - then update the function for the corresponding cmd.
5) Customize the quick launch apps. Obtain the appID by selecting the channel using the app/remote and then from a browser get the app id via http://RokuIP:8060/query/active-app (Replace RokuIP with the IP of your device). Use the ID provided in place of the ones in this code.

This uses pieces from a number of programs:
 IRremoteESP8266: https://github.com/markszabo/IRremoteESP8266/
 Telnet Debug https://github.com/JoaoLopesF/ESP8266-RemoteDebug-Telnet
