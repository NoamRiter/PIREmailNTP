#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <EEPROM.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

//define your default values here, if there are different values in config.json, they are overwritten.
//length should be max size + 1
char mqtt_server[40];
char emailTobeSend[1];

//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}


String webSite, javaScript, XML;



String passw = "z";
bool rightPassword = false;
String year = "2017";
String refreshPage = "45";
String mainHeading   = "Sias lamp control";
String subHeading    = "Let it be light!";
String menuName = "Sias rum";
String title     = "Esp8266 by Noam";
String copyrights = "Sias Art";
String webpage       = ""; //init webpage
String Email = "";
int port = 80; // Change the port to the one you open
String PublicIpAddress = "0"; //change it to your public IP. Open a rout/port on your ESP in router (TCP/UDP), Change the port to the one you open
String IpAddress = "0";
int openWifiManager;

// const int pin1 = D3;
// bool pin1Status = true;
// const int pin2 = D5;
// bool pin2Status = true;
// const int pin3 = D6;
// bool pin3Status = true;
// const int pin4 = D7;
// bool pin4Status = true;

bool isEmail = false;

ESP8266WebServer server(port);

void setup() {
  // put your setup code here, to run once:

  // pinMode(pin1, OUTPUT);
  // pinMode(pin2, OUTPUT);
  // pinMode(pin3, OUTPUT);
  // pinMode(pin4, OUTPUT);

  // digitalWrite(pin1, HIGH);
  // digitalWrite(pin2, HIGH);
  // digitalWrite(pin3, HIGH);
  // digitalWrite(pin4, HIGH);


  Serial.begin(115200);
  Serial.println();

  //clean FS, for testing
  //SPIFFS.format();

  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          strcpy(mqtt_server, json["mqtt_server"]);
          //in first use uncomment the line below
          strcpy(emailTobeSend, json["emailTobeSend"]);



        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read
  Serial.print("mqtt_server: ");
  Serial.println(mqtt_server);
  Serial.print("emailTobeSend: ");
  Serial.println(emailTobeSend);


  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);


  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);



  //add all your parameters here
  wifiManager.addParameter(&custom_mqtt_server);


  //reset settings - for testing
  //wifiManager.resetSettings();

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  wifiManager.setMinimumSignalQuality();

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration

  EEPROM.begin(500);
  openWifiManager = EEPROM.read(35);
  isEmail = EEPROM.read(35);
  Serial.println(openWifiManager);

  if (openWifiManager == 1)
  {
    wifiManager.resetSettings();
    EEPROM.write(35, 0);
    EEPROM.commit();
  }

  if (!wifiManager.autoConnect("Noams ESP", "password")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected......");

  //read updated parameters
  strcpy(mqtt_server, custom_mqtt_server.getValue());


  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["mqtt_server"] = mqtt_server;
    json["emailTobeSend"] = emailTobeSend;


    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.prettyPrintTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
  }

  IpAddress = WiFi.localIP().toString();

  for (int index = 0; index < sizeof(mqtt_server) - 1; index++)
  {
    if (mqtt_server[index] != '\0' && mqtt_server[index] != ' ')
    {
      Email += mqtt_server[index];
    }
  }
  Email.trim();

  if (isEmail)
  {
    yield();
    sendEmail(WiFi.localIP().toString());
    yield();
    isEmail = false;
    EEPROM.write(40, 0);
    EEPROM.commit();
  }

  Serial.print("");
  Serial.print("local ip: ");
  Serial.println(IpAddress);
  server.begin();

  switch (emailTobeSend[0]) {
    case 'A':
      // do nothing
      break;

    case 'B':
      yield();
      sendEmail("Alarm have been triggered");
      yield();
      emailTobeSend[0] = 'A';
      saveJson();
      yield();
      break;

    default:
      //do nothing!
      break;
  }





  server.on("/",         Product); // IP/
  server.on("/homepage", homepage);   // IP/homepage
  server.on("/Product", Product);      // IP/Product
  server.on("/Portfolio", Portfolio);      // IP/Portfolio
  server.on("/Services", Services);      // IP/Services
  server.on("/Aboutus", Aboutus);      // IP/Aboutus
  server.on("/Contact", Contact);      // IP/Contact
  server.on("/RestartWifiManager", RestartWifiManager);     //IP/RestartWifiManager
  server.on("/PinValue", PinValue);  //IP/PinValue
  server.on("/Password", Password);  //IP/PasswordValue
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();


  // Serial.print(mqtt_server);
}
