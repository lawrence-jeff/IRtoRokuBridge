/*
 * IRtoRokuBridge
 * Allows you to use a universal IR based remote (ex Harmony) to control a Roku stick so you can use a single device
 * Roku Sticks use some sort of RF or bluetooth so can't be controlled by a classic IR only remote
 * This also has direct channel activation which I use via customized buttons in the Watch Roku activity programmed in my Harmony 550
 * This gives you a button on the screen labeled Syfy for instance which directly kicks off that app
 * To customize the apps just obtain the appID by selecting the channel and then from a browser get the app id via
 * http://RokuIP:8060/query/active-app (Replace RokuIP with the IP of your device)
 * This uses pieces from a number of programs
 * IRremoteESP8266: https://github.com/markszabo/IRremoteESP8266/
 * Telnet Debug https://github.com/JoaoLopesF/ESP8266-RemoteDebug-Telnet
* To customize
* 1) Replace the IR reciever pin with yours - II use a nodemcu device with IR reciever and LED attached to D5
* 2) Replace the Roku IP with your value
* 3) Update your wifi information
* 4) Use the existing codes - they are from something in the Harmony datbase called Rikki. if you want to use custom codes find a device that uses NEC codes (you can use IRDump example to confirm encoding)
* and then via telnet or serial debug press each button to get the code - then update the function for the corresponding cmd.
* */

#include <IRremoteESP8266.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoOTA.h>
#include "RemoteDebug.h" 
IRrecv irrecv(14); //14 is D5 on nodemcu

const char* ssid = "************";
const char* password = "**************";
const long NECRepeat=0xFFFFFFFF;
int RepeatCount=0;
String RokuCmd,RokuCmdLastValid;
long IRCode=0x0;

HTTPClient http;
decode_results results;
RemoteDebug Debug;


void setup()
{
  //Start Serial
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Booted");

 //Start Debug
  Debug.begin("IRtoRokuBridge");
  
  //Start OTA
  ArduinoOTA.begin();
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  
  //Start IR
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Waiting for IR");

  // Start Network- Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (irrecv.decode(&results)) {
    IRCode=results.value;
    //This has basic support for NEC repeat code - it can be improved I'm sure
    if (IRCode != NECRepeat){
      returnCmd(IRCode);
    }
    else{
      ++RepeatCount;
      //The filtering of the first two repeats is based on trial and error - may need to adjust based on your remote/click speed
      if (RepeatCount >2){
         RokuCmd=RokuCmdLastValid; }
      else{
         RokuCmd="Ignore";}
    }
    //Process Cmd
    if ( RokuCmd !="Ignore"){
      String url = "http://192.168.0.193:8060" + RokuCmd;
      http.begin(url);
      http.POST("");
      http.end();
    }
    irrecv.resume(); // Receive the next value
  } 
  ArduinoOTA.handle(); //This does impact IR reading a bit
  Debug.handle(); //This does impact IR reading a bit
  delay(20);
}

void returnCmd(long IRCode){
 switch (IRCode) {
    case 0xF9708F:
      RokuCmd= "/keypress/Right";
      break;
    case 0xF9609F:
       RokuCmd= "/keypress/Left";
       break;
    case 0xF9C23D:
       RokuCmd= "/keypress/Up";
       break;
    case 0xF950AF:
       RokuCmd= "/keypress/Down";
       break;
    case 0xF940BF:
       RokuCmd= "/keypress/Select";
       break;
    case 0xF9F20D:
       RokuCmd= "/keypress/Back";
       break;
    case 0xF95AA5:
       RokuCmd= "/keypress/Home";
       break;
    case 0xF90AF5:
       RokuCmd= "/keypress/Play";
       break;
    case 0xF9B24D:
       RokuCmd= "/keypress/Play";  //Play/Pause are the same on roku
       break;
    case 0xF9C03F:
       RokuCmd= "/keypress/Fwd";   
       break;
    case 0xF9E01F:
       RokuCmd= "/keypress/Rev"; 
       break;
    case 0xF9926D:
       RokuCmd= "/keypress/InstantReplay";
       break;
    case 0xF9A05F:
      RokuCmd= "/launch/86398"; //Syfy (see instructions to customize for your setup)
      break;
    case 0xF98877:
      RokuCmd= "/launch/35058"; //lifetime (see instructions to customize for your setup - wife watches this don't judge)
      break;
    case 0xF9906F:
      RokuCmd= "/launch/34278"; //disney JR (see instructions to customize for your setup)
      break;
    case 0xF9A857:
      RokuCmd="/launch/66595"; //nick jr (see instructions to customize for your setup)
      break;
    default: 
      Serial.printf("ERROR: No match for code %6X \n", IRCode);
      if (Debug.isActive(Debug.VERBOSE)) {    Debug.printf("IR CODE NOT RECOGNIZED: %8X \n", IRCode);  }
      RokuCmd= "Ignore";
      break;
   }
   //Not detected as a repeat so if it gets here reset repeat info
   RokuCmdLastValid=RokuCmd;
   RepeatCount=0;
  
}
