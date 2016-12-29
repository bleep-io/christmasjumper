#include <ESP8266WiFi.h>
#include "FS.h"
#include <Adafruit_NeoPixel.h>

const char* host = "ChristmasJumper";
IPAddress apIP(192, 168, 4, 1);
WiFiServer server(80);
WiFiClient client;

const char* ssid = "";
const char* password = "";

extern "C" 
{
#include "user_interface.h"
}
#define BUTTON_PIN   0 
#define PIXEL_PIN    5 
#define PIXEL_COUNT  9

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

uint32_t yellow = strip.Color(255, 100, 0);
bool oldState = HIGH;  // Switch
int showType = 0;
bool animate = false;  // Web Button

const char* host2 = "hooks.slack.com";
const char* SlackWebhookURL = "";

const char* fingerprint = "";

void setup() {
  
  Serial.begin(115200);  // Set up logging
  Serial.println("------ Starting Setup ------");
  treeMessage();

  // Start Wifi Init
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);
  //WiFi.mode(WIFI_AP);
  //uint8_t mac[WL_MAC_ADDR_LENGTH];
  //WiFi.softAPmacAddress(mac);
  //String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
  //               String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  //macID.toUpperCase();
  //WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  //WiFi.softAP("Xmas Jumper 192.168.4.1");    // Create WiFi network

  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin();
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wifi Connected");
  
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); 
  const String ipadd = WiFi.localIP().toString();

 WiFiClientSecure client;
 const int httpPort = 443;
 if (client.connect(host2, httpPort)) {
  
 }
 // verify the signature of the ssl certificate
 if (client.verify(fingerprint, host2)) {
 Serial.println("ssl cert matches");
 } else {
 Serial.println("ssl cert mismatch");
 }

  String channel = "#christmasjumper";
String username = "christmasjumper";
String message = "Current IP Address: " + ipadd;
String PostData="payload={\"channel\": \"" + channel + "\", \"username\": \"" + username + "\", \"text\": \"" + message + "\", \"icon_emoji\": \":christmas_tree:\"}";
 Serial.println(PostData);
 
 client.print("POST ");
 client.print(SlackWebhookURL);
 client.println(" HTTP/1.1");
 client.print("Host: ");
 client.println(host2);
 client.println("User-Agent: ArduinoIoT/1.0");
 client.println("Connection: close");
 client.println("Content-Type: application/x-www-form-urlencoded;");
 client.print("Content-Length: ");
 client.println(PostData.length());
 client.println();
 client.println(PostData);
 client.flush();
 
  
  // Start Server
  server.begin(); 
  Serial.println("Web Server Up!");

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Set up Neopixel Strip
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
 // strip.setBrightness(64); // Adjust brightness here!
  strip.setPixelColor(8, yellow);
  strip.show();
  Serial.println("Lights On!");
  
  Serial.println("------ Setup Complete ------");
  }


String MakeHTTPHeader(unsigned long ulLength)
{
  String sHeader;
  
  sHeader  = F("HTTP/1.1 200 OK");
  sHeader += F("\r\nContent-Type: text/html\r\n\r\n");
  sHeader += "</html>\n";
  return(sHeader);
}


void loop() {
  
  // Get current button state.
  bool newState = digitalRead(BUTTON_PIN);

  // Check if state changed from high to low (button press).
  if ((newState == LOW && oldState == HIGH) || (animate == true)) {
    // Short delay to debounce button.
    delay(2);
    
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON_PIN);
    if ((newState == LOW) || (animate == true)) {
      if (newState == LOW){
      showType++;
      if (showType > 9){
        showType=0;
        }
      startShow(showType);     
      }
    if (animate == true){
      startShow(showType);
     }  
    }
   }
   // Set the last button state to the old state.
      oldState = newState;

  WiFiClient client = server.available();
  if (!client) 
  {
    return;
  }
  
  String sRequest = client.readStringUntil('\r');
  Serial.println(sRequest);
  client.flush();
  
  // get path; end of path is either space or ?
  // Syntax is e.g. GET /?show=1234 HTTP/1.1
  String sPath="",sParam="", sCmd="";
  String sGetstart="GET ";
  int iStart,iEndSpace,iEndQuest;
  iStart = sRequest.indexOf(sGetstart);
  if (iStart>=0)
  {
    iStart+=+sGetstart.length();
    iEndSpace = sRequest.indexOf(" ",iStart);
    iEndQuest = sRequest.indexOf("?",iStart);
    
    // are there parameters?
    if(iEndSpace>0)
    {
      if(iEndQuest>0)
      {
        // there are parameters
        sPath  = sRequest.substring(iStart,iEndQuest);
        sParam = sRequest.substring(iEndQuest,iEndSpace);
      }
      else
      {
        // NO parameters
        sPath  = sRequest.substring(iStart,iEndSpace);
      }
    }
  }
 
  
  String sResponse,sResponse2,sHeader;

   if (sPath.startsWith("/pixel_inputs&LED"))
   {
    // 
 if (sPath.startsWith("/pixel_inputs&LED3=0")) {
 showType=0;
    }
    else if (sPath.startsWith("/pixel_inputs&LED3=1")) {
 showType=1;
animate = true;
    }
    else if (sPath.startsWith("/pixel_inputs&LED3=2")) {
 showType=2;
animate = true;
    }
    else if (sPath.startsWith("/pixel_inputs&LED3=3")) {
 showType=3;
animate = true;
    }
    else if (sPath.startsWith("/pixel_inputs&LED3=4")) {
 showType=4;
animate = true;
    }
    else if (sPath.startsWith("/pixel_inputs&LED3=5")) {
 showType=5;
animate = true;
    }
    else if (sPath.startsWith("/pixel_inputs&LED3=6")) {
 showType=6;
animate = true;
    }
    else if (sPath.startsWith("/pixel_inputs&LED3=7")) {
 showType=7;
animate = true;
    }
    else if (sPath.startsWith("/pixel_inputs&LED3=9")) {
 showType=8;
animate = true;
    }
}


    
  /////////////////////////////////////////
  // format the html page for Pixel Input /
  /////////////////////////////////////////
 if(sPath=="/")
  {
    sResponse  = F("<html>\n<head>\n<title>Christmas Jumper Control</title>\n</head>\n");    
    sResponse += F("\n<script>\nfunction GetButton1()\n{\nnocache = \"&nocache=\" + Math.random() * 1000000;\nvar request = new XMLHttpRequest();\nrequest.open(\"GET\", \"pixel_inputs&LED3=0\" + nocache, true);\nrequest.send(null);\n}\nfunction GetButton2()\n{\nnocache = \"&nocache=\" + Math.random() * 1000000;\nvar request = new XMLHttpRequest();\nrequest.open(\"GET\", \"pixel_inputs&LED3=1\" + nocache, true);\nrequest.send(null);\n}\n");
    sResponse += F("\nfunction GetButton3()\n{\n\nnocache = \"&nocache=\" + Math.random() * 1000000;\nvar request = new XMLHttpRequest();\nrequest.open(\"GET\", \"pixel_inputs&LED3=2\" + nocache, true);\nrequest.send(null);\n}\nfunction GetButton4()\n{\nnocache = \"&nocache=\" + Math.random() * 1000000;\nvar request = new XMLHttpRequest();\nrequest.open(\"GET\", \"pixel_inputs&LED3=3\" + nocache, true);\nrequest.send(null);\n}\nfunction GetButton5()\n");
    sResponse += F("{\nnocache = \"&nocache=\" + Math.random() * 1000000;\nvar request = new XMLHttpRequest();\nrequest.open(\"GET\", \"pixel_inputs&LED3=4\" + nocache, true);\nrequest.send(null);\n}\nfunction GetButton6()\n{\nnocache = \"&nocache=\" + Math.random() * 1000000;\nvar request = new XMLHttpRequest();\nrequest.open(\"GET\", \"pixel_inputs&LED3=5\" + nocache, true);\nrequest.send(null);\n}\n");
    sResponse += F("\nfunction GetButton7()\n{\nnocache = \"&nocache=\" + Math.random() * 1000000;\nvar request = new XMLHttpRequest();\nrequest.open(\"GET\", \"pixel_inputs&LED3=6\" + nocache, true);\nrequest.send(null);\n}\nfunction GetButton8()\n{\nnocache = \"&nocache=\" + Math.random() * 1000000;\nvar request = new XMLHttpRequest();\nrequest.open(\"GET\", \"pixel_inputs&LED3=7\" + nocache, true);\nrequest.send(null);\n}\nfunction GetButton9()\n{\nnocache = \"&nocache=\" + Math.random() * 1000000;\nvar request = new XMLHttpRequest();\nrequest.open(\"GET\", \"pixel_inputs&LED3=9\" + nocache, true);\nrequest.send(null);\n}\n");
    sResponse += F("\nfunction GetButton10()\n{\nnocache = \"&nocache=\" + Math.random() * 1000000;\nvar request = new XMLHttpRequest();\nrequest.open(\"GET\", \"pixel_inputs&LED3=9\" + nocache, true);\nrequest.send(null);\n}\n</script>\n<body>\n<h1>Christmas Jumper Controller<BR>_________________</h1>\n<BR><BR>\n<body onload=\"GetESP8266IO()\">\n<h1><FONT SIZE=-1>Choose Animation of LED's</h1>\n<form id=\"txt_form\" name=\"frmText\">\n");


    sResponse += F("\n<button type=\"button\" id=\"LED3\" onclick=\"GetButton1()\">Turn Off</button><br /><br />\n<button type=\"button\" id=\"LED3\" onclick=\"GetButton2()\">All Red</button><br /><br />\n<button type=\"button\" id=\"LED3\" onclick=\"GetButton3()\">All Green</button><br /><br />\n<button type=\"button\" id=\"LED3\" onclick=\"GetButton4()\">All Blue</button><br /><br />\n<button type=\"button\" id=\"LED3\" onclick=\"GetButton5()\">Flashing White</button><br /><br />\n<button type=\"button\" id=\"LED3\" onclick=\"GetButton6()\">Flashing Red</button><br />");
    sResponse2 = F("<br />\n<button type=\"button\" id=\"LED3\" onclick=\"GetButton7()\">Flashing Blue</button><br /><br />\n<button type=\"button\" id=\"LED3\" onclick=\"GetButton9()\">Multi Coloured</button><br /><br />\n</form>\n<br />\n");    
 
    sResponse2 += F("\n<font color=\"#000000\"><body bgcolor=\"#a0dFfe\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=yes\"><BR><BR>\n</div>\n<div style=\"clear:both;\"></div><p>\n");

        // Send the response to the client 
    client.print(MakeHTTPHeader(sResponse.length()+sResponse2.length()).c_str());
    client.print(sResponse);
    client.print(sResponse2);
}
  

////////////////////////////////////////////////////////////////////////2
}


void startShow(int i) {
  switch(i){
    case 0: colorWipe(strip.Color(0, 0, 0), 50);    // Black/off
            animate = false;
            break;
    case 1: colorWipe(strip.Color(255, 0, 0), 100);  // Red
            break;
    case 2: colorWipe(strip.Color(0, 255, 0), 100);  // Green
            break;
    case 3: colorWipe(strip.Color(0, 0, 255), 100);  // Blue
            break;
    case 4: theaterChase(strip.Color(127, 127, 127), 100); // White
            break;
    case 5: theaterChase(strip.Color(127,   0,   0), 100); // Red
            break;
    case 6: theaterChase(strip.Color(  0,   0, 127), 100); // Blue
            break;
    case 7: rainbow(20);
            break;
    case 8: rainbowCycle(20);
            break;
    case 9: theaterChaseRainbow(50);
            break;
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void treeMessage() {
 Serial.println("            \\ /        ");
 Serial.println("             *       ");
 Serial.println("            /_\\         ");
 Serial.println("           /_\\_\\         ");
 Serial.println("          /_/_/_\\      ");
 Serial.println("          /_\\_\\_\\       ");
 Serial.println("         /_/_/_/_\\      ");
 Serial.println("         /_\\_\\_\\_\\     ");
 Serial.println("        /_/_/_/_/_\\    ");
 Serial.println("        /_\\_\\_\\_\\_\\    ");
 Serial.println("       /_/_/_/_/_/_\\   ");
 Serial.println("       /_\\_\\_\\_\\_\\_\\   ");
 Serial.println("      /_/_/_/_/_/_/_\\  ");
 Serial.println("           [___]       ");
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
