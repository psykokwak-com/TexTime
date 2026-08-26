/*
 * * ESP8266 template with phone config web page
 * based on BVB_WebConfig_OTA_V7 from Andreas Spiess https://github.com/SensorsIot/Internet-of-Things-with-ESP8266
 *
 */
#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Ticker.h>
#include <EEPROM.h>
#include <DNSServer.h>
#include <NeoPixelBus.h>
#include <NeoPixelBusLg.h>
#include <NeoPixelAnimator.h>
#include <Wire.h>
#include <RtcDS3231.h>
#include <ESP8266SSDP.h>
#include <ESP8266NetBIOS.h>
#include <ESP8266LLMNR.h>

#include "PubSubClient.h"

#include "WiFiMgr.h"
#include "global.h"
#include "mqtt_topics.h"
#include "list.h"
#include "RTC.h"
#include "NTP.h"
#include "LightSensor.h"
#include "LedStrip.h"
#include "mqtt.h"

// Include the HTML, STYLE and Script "Pages"

#include "Page_ico.h"
#include "Page_index.h"
#include "Page_ntp.h"
#include "Page_information.h"
#include "Page_general.h"
#include "Page_network.h"
#include "Page_mqtt.h"
#include "Page_script.js.h"
#include "Page_style.css.h"
#include "Page_scheduler.h"
#include "Page_tetris.h"
#include "Page_snake.h"



extern "C" {
#include "user_interface.h"
}

//*** Normal code definition here ...
void setup() {
  bool CFG_saved = false;

  // No need to set this until option is set in arduino/visualmicro
  //system_update_cpu_freq(SYS_CPU_160MHZ);

  randomSeed(analogRead(ALS_PIN));

  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

  Serial.begin(115200);
  Serial.println("Booting");

  // Config load 
  EEPROM.begin(EEPROM_SIZE); // 1024 for config + 2689 for scheduler (1 enable byte + 336 slots x 8 bytes)
  CFG_saved = ReadConfig();
  if (!CFG_saved)
  {
    // DEFAULT CONFIG
    Serial.println("Set Default config");

    _config.ssid = "";       // SSID of access point
    _config.password = "";   // password of access point
    _config.dhcp = true;
    _config.IP[0] = 192; _config.IP[1] = 168; _config.IP[2] = 1; _config.IP[3] = 100;
    _config.Netmask[0] = 255; _config.Netmask[1] = 255; _config.Netmask[2] = 255; _config.Netmask[3] = 0;
    _config.Gateway[0] = 192; _config.Gateway[1] = 168; _config.Gateway[2] = 1; _config.Gateway[3] = 1;
    _config.DNS[0] = 192; _config.DNS[1] = 168; _config.DNS[2] = 1; _config.DNS[3] = 1;
    _config.DeviceName = "TexTime";

    _config.ntpServerName = "0.ch.pool.ntp.org"; // to be adjusted to PT ntp.ist.utl.pt
    _config.Update_Time_Via_NTP_Every = 86400;
    _config.timeZone = 10;
    _config.isDayLightSaving = true;

    _config.brightnessAuto = true;
    _config.brightness = 128; // [0:255]
    _config.brightnessAutoMinDay = 30; // [0:255]
    _config.brightnessAutoMinNight = 0; // [0:255]
    _config.brightnessMax = 255; // [0:255]
    _config.color[0] = 255; // R
    _config.color[1] = 255; // G
    _config.color[2] = 255; // B
    _config.color[3] = 255; // W
    _config.mode = 1;
    _config.animation = 0;
    _config.animSpeed = 10;
    _config.animBrightnessMin = 10;
    _config.animBrightnessMax = 100;
    _config.ledConfig = 0;
    _config.luxSensitivity = 40;
    _config.language = 0;

    _config.MQTTServer = "";
    _config.MQTTLogin = "";
    _config.MQTTPassword = "";
    _config.MQTTPort = 1883;
    _config.MQTTPubInterval = 120; // in sec

    WriteConfig();

    // ReadConfig() does this on the normal path; the defaults path needs it too,
    // otherwise the animation parameters stay at zero and every animation stalls.
    syncLiveFromConfig();
  }

  // Start led strip
  QTLed.begin(); // Must be called after Serial.begin() and EEPROM configuration

  // Start RTC
  RTC.Begin();
  RTC.Enable32kHzPin(false);
  RTC.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
  handleTimeFromRTC();
  updateTime();

  //  Connect to WiFi access point or start as Access point
  // Connect the ESP8266 to local WIFI network in Station mode
  //printConfig();

  // Configure WiFi
  WiFiMgr.setAPssid("TexTime-" + String(ESP.getChipId(), HEX));

  if (!_config.dhcp)
  {
    WiFiMgr.setSTAIPip(IPAddress(_config.IP[0], _config.IP[1], _config.IP[2], _config.IP[3]),
      IPAddress(_config.Gateway[0], _config.Gateway[1], _config.Gateway[2], _config.Gateway[3]),
      IPAddress(_config.Netmask[0], _config.Netmask[1], _config.Netmask[2], _config.Netmask[3]),
      IPAddress(_config.DNS[0], _config.DNS[1], _config.DNS[2], _config.DNS[3]));
  }

  // Start WiFi
  WiFiMgr.tryToConnect(_config.ssid, _config.password, _config.DeviceName);

  // Start HTTP Server for configuration
  // Serves the page only; configuration is written through /admin/save/*.
  _server.on("/", []() {
    _server.send_P(200, "text/html", PAGE_index);
  });

  _server.on("/index.html", []() {
    //Serial.println("index.html");
    _server.send_P(200, "text/html", PAGE_index);
  });

  _server.on("/favicon.ico", []() {
    //Serial.println("favicon.ico");
    _server.send_P(200, "image/png", PAGE_ico, PAGE_ico_size);
  });

  _server.on("/apple-touch-icon.png", []() {
    //Serial.println("favicon.ico");
    _server.send_P(200, "image/png", PAGE_ico, PAGE_ico_size);
  });

  _server.on("/style.css", []() {
    //Serial.println("style.css");
    _server.sendHeader("Cache-Control", "max-age=3600");
    _server.send_P(200, "text/css", PAGE_style_css);
  });

  _server.on("/microajax.js", []() {
    //Serial.println("microajax.js");
    _server.sendHeader("Cache-Control", "max-age=3600");
    _server.send_P(200, "application/javascript", PAGE_microajax_js);
  });

  _server.on("/description.xml", HTTP_GET, [](){
    SSDP.schema(_server.client());
  });

  _server.on("/admin/networkfieldsvalues", send_network_configuration_values_html);
  _server.on("/admin/networkconnectionvalues", send_network_connection_values_html);
  _server.on("/admin/mqttfieldsvalues", send_mqtt_configuration_values_html);
  _server.on("/admin/mqttconnectionvalues", send_mqtt_connection_values_html);
  _server.on("/admin/infovalues", send_information_configuration_values_html);
  _server.on("/admin/ntpfieldsvalues", send_ntp_configuration_values_html);
  _server.on("/admin/generalfieldsvalues", send_general_configuration_values_html);
  _server.on("/admin/generallangsvalues", send_general_langs_values_html);
  _server.on("/admin/generalmodesvalues", send_general_modes_values_html);
  _server.on("/admin/generalanimationsvalues", send_general_animations_values_html);
  _server.on("/admin/generalledconfigvalues", send_general_ledconfig_values_html);

  _server.on("/admin/langvalue", send_lang_value_html);
  _server.on("/admin/led", send_general_led);

  _server.on("/tetris.html", send_tetris_html);
  _server.on("/admin/tetris", send_tetris_action);
  _server.on("/admin/tetrisstate", send_tetris_state);

  _server.on("/snake.html", send_snake_html);
  _server.on("/admin/snake", send_snake_action);
  _server.on("/admin/snakestate", send_snake_state);

  // Async save endpoints
  _server.on("/admin/save/general", HTTP_POST, []() {
    if (_server.args() > 0) {
      // Process general settings save
      //
      // "brightnessauto" is a checkbox, and an unticked checkbox is simply
      // absent from the request -- indistinguishable from a form that was
      // submitted before its values finished loading. Only trust the absence
      // when the page states it really did send the form.
      if (_server.hasArg("formready"))
        _config.brightnessAuto = false;

      for (uint8_t i = 0; i < _server.args(); i++) {
        if (_server.argName(i) == "brightnessauto") _config.brightnessAuto = true;
        if (_server.argName(i) == "brightness") _config.brightness = _server.arg(i).toInt();
        if (_server.argName(i) == "brightnessday") _config.brightnessAutoMinDay = _server.arg(i).toInt();
        if (_server.argName(i) == "brightnessnight") _config.brightnessAutoMinNight = _server.arg(i).toInt();
        if (_server.argName(i) == "brightnessmax") _config.brightnessMax = constrain(_server.arg(i).toInt(), 1, 255);
        if (_server.argName(i) == "color") {
          String colorStr = _server.arg(i);
          // Remove # if present
          if (colorStr.startsWith("#")) {
            colorStr = colorStr.substring(1);
          }
          // An empty field parses as 0, which is black. That is never what the
          // user meant; it means the form had nothing to send yet.
          if (colorStr.length() > 0) {
            int32_t l = strtol(colorStr.c_str(), 0, 16);
            _config.color[0] = (l >> 16) & 0xFF;
            _config.color[1] = (l >> 8) & 0xFF;
            _config.color[2] = (l >> 0) & 0xFF;
          }
        }
        if (_server.argName(i) == "lang") _config.language = _server.arg(i).toInt();
        if (_server.argName(i) == "mode") _config.mode = _server.arg(i).toInt();
        if (_server.argName(i) == "animation") _config.animation = _server.arg(i).toInt();
        if (_server.argName(i) == "colorrandom") _config.colorRandom = _server.arg(i).toInt();
        if (_server.argName(i) == "ledconfig") _config.ledConfig = _server.arg(i).toInt();
        if (_server.argName(i) == "brightnesssensibility") _config.luxSensitivity = constrain(_server.arg(i).toInt(), 1, 255);
        if (_server.argName(i) == "animspeed") _config.animSpeed = constrain(_server.arg(i).toInt(), 1, 20);
        if (_server.argName(i) == "animbrightmin") _config.animBrightnessMin = constrain(_server.arg(i).toInt(), 0, 100);
        if (_server.argName(i) == "animbrightmax") _config.animBrightnessMax = constrain(_server.arg(i).toInt(), 0, 100);
      }

      // Same range rule as on the EEPROM read path, so what is stored can never
      // differ from what a reboot would accept.
      validateAnimBrightness();

      WriteConfig();

      QTLed.begin();

      // The user settings now win over any scheduler slot override still active.
      QTLed.applyUserSettings();
      
      _server.send(200, "text/plain", "OK");
    } else {
      _server.send(400, "text/plain", "ERROR");
    }
  });
  
  _server.on("/admin/save/network", HTTP_POST, []() {
    if (_server.args() > 0) {
      // Validate the static addresses before touching anything. This handler
      // reboots the device on its way out, so writing a half-parsed address
      // means coming back on one that cannot connect, with the setup access
      // point as the only way in.
      if (!_server.hasArg("dhcp")) {
        static const char *addrFields[] = { "ipaddress", "netmask", "gateway", "dnsserver" };
        for (uint8_t f = 0; f < 4; f++) {
          if (!_server.hasArg(addrFields[f])) continue;
          if (!isValidIPv4(_server.arg(addrFields[f]))) {
            _server.send(400, "text/plain", "BAD_ADDRESS");
            return;
          }
        }
      }

      bool openWifi = false;

      _config.dhcp = false;
      for (uint8_t i = 0; i < _server.args(); i++) {
        if (_server.argName(i) == "ssid") _config.ssid = _server.arg(i);
        if (_server.argName(i) == "password") { String p = _server.arg(i); if (p.length() > 0) _config.password = p; }
        if (_server.argName(i) == "openwifi") openWifi = true;
        if (_server.argName(i) == "ipaddress") {
          // Parse IP address string like "192.168.1.100"
          String ip = _server.arg(i);
          int parts[4];
          int partIndex = 0;
          int start = 0;
          int len_ip = ip.length();
          for (int j = 0; j <= len_ip && partIndex < 4; j++) {
            if (j == len_ip || ip.charAt(j) == '.') {
              String part = ip.substring(start, j);
              parts[partIndex] = part.toInt();
              if (checkRange(String(parts[partIndex]))) {
                _config.IP[partIndex] = parts[partIndex];
              }
              partIndex++;
              start = j + 1;
            }
          }
        }
        if (_server.argName(i) == "netmask") {
          // Parse netmask string
          String nm = _server.arg(i);
          int parts[4];
          int partIndex = 0;
          int start = 0;
          int len_nm = nm.length();
          for (int j = 0; j <= len_nm && partIndex < 4; j++) {
            if (j == len_nm || nm.charAt(j) == '.') {
              String part = nm.substring(start, j);
              parts[partIndex] = part.toInt();
              if (checkRange(String(parts[partIndex]))) {
                _config.Netmask[partIndex] = parts[partIndex];
              }
              partIndex++;
              start = j + 1;
            }
          }
        }
        if (_server.argName(i) == "gateway") {
          // Parse gateway string
          String gw = _server.arg(i);
          int parts[4];
          int partIndex = 0;
          int start = 0;
          int len_gw = gw.length();
          for (int j = 0; j <= len_gw && partIndex < 4; j++) {
            if (j == len_gw || gw.charAt(j) == '.') {
              String part = gw.substring(start, j);
              parts[partIndex] = part.toInt();
              if (checkRange(String(parts[partIndex]))) {
                _config.Gateway[partIndex] = parts[partIndex];
              }
              partIndex++;
              start = j + 1;
            }
          }
        }
        if (_server.argName(i) == "dnsserver") {
          // Parse DNS string
          String dns = _server.arg(i);
          int parts[4];
          int partIndex = 0;
          int start = 0;
          int len_dns = dns.length();
          for (int j = 0; j <= len_dns && partIndex < 4; j++) {
            if (j == len_dns || dns.charAt(j) == '.') {
              String part = dns.substring(start, j);
              parts[partIndex] = part.toInt();
              if (checkRange(String(parts[partIndex]))) {
                _config.DNS[partIndex] = parts[partIndex];
              }
              partIndex++;
              start = j + 1;
            }
          }
        }
        if (_server.argName(i) == "dhcp") _config.dhcp = true;
        if (_server.argName(i) == "devicename") _config.DeviceName = _server.arg(i);
      }

      // An empty password field means "unchanged", so joining an open network
      // needs an explicit opt-in. It wins over whatever the field contains.
      if (openWifi) _config.password = "";

      WriteConfig();
      _server.send(200, "text/plain", "OK");
      ESP.restart();
    } else {
      _server.send(400, "text/plain", "ERROR");
    }
  });
  
  _server.on("/admin/save/ntp", HTTP_POST, []() {
    if (_server.args() > 0) {
      _config.isDayLightSaving = false;
      for (uint8_t i = 0; i < _server.args(); i++) {
        if (_server.argName(i) == "ntpserver") _config.ntpServerName = _server.arg(i);
        if (_server.argName(i) == "update") _config.Update_Time_Via_NTP_Every = constrain(_server.arg(i).toInt(), NTP_INTERVAL_MIN, NTP_INTERVAL_MAX);
        if (_server.argName(i) == "tz") _config.timeZone = _server.arg(i).toInt();
        if (_server.argName(i) == "dst") _config.isDayLightSaving = true;
      }
      
      WriteConfig();
      getNTPtime();
      _server.send(200, "text/plain", "OK");
    } else {
      _server.send(400, "text/plain", "ERROR");
    }
  });
  
  _server.on("/admin/save/mqtt", HTTP_POST, []() {
    if (_server.args() > 0) {
      for (uint8_t i = 0; i < _server.args(); i++) {
        if (_server.argName(i) == "host") _config.MQTTServer = _server.arg(i);
        if (_server.argName(i) == "port") _config.MQTTPort = _server.arg(i).toInt();
        if (_server.argName(i) == "login") _config.MQTTLogin = _server.arg(i);
        if (_server.argName(i) == "mqttpassword") _config.MQTTPassword = _server.arg(i);
        if (_server.argName(i) == "interval") _config.MQTTPubInterval = _server.arg(i).toInt();
      }
      
      WriteConfig();
      _mqtt.disconnect();
      _mqtt.setServer(_config.MQTTServer.c_str(), _config.MQTTPort);
      _server.send(200, "text/plain", "OK");
    } else {
      _server.send(400, "text/plain", "ERROR");
    }
  });


  _server.onNotFound([]() {
    Serial.println("Page Not Found");
    _server.send(400, "text/html", "Page not Found");
  });

  _server.on("/admin/schedulerconfig", send_scheduler_config);
  _server.on("/admin/schedulerdata", send_scheduler_data);
  _server.on("/admin/save/schedulerbulk", HTTP_POST, save_scheduler_bulk);
  _server.on("/admin/scheduler/apply", HTTP_POST, apply_scheduler_now);
  _server.on("/admin/save/schedulerenabled", HTTP_POST, save_scheduler_enabled);

  _httpUpdater.setup(&_server);
  _server.begin();
  Serial.println("HTTP server started");

  // ***********  OTA SETUP
  //ArduinoOTA.setHostname(host);
  ArduinoOTA.onStart([]() { // what to do before OTA download insert code here
    Serial.println("Start");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
    ESP.restart();
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
    ESP.restart();
  });

  /* setup the OTA server */
  ArduinoOTA.begin();

  MDNS.begin(_config.DeviceName.c_str());
  MDNS.addService("http", "tcp", 80);

  NBNS.begin(_config.DeviceName.c_str());

  LLMNR.begin(_config.DeviceName.c_str());

  SSDP.setSchemaURL("description.xml");
  SSDP.setHTTPPort(80);
  SSDP.setName(_config.DeviceName);
  SSDP.setSerialNumber(String(ESP.getFlashChipId()));
  SSDP.setURL("index.html");
  SSDP.setModelName("TexTime");
  SSDP.setModelNumber("Build : " + printDateTime(RtcDateTime(__DATE__, __TIME__)));
  SSDP.setModelURL("http://www.psykokwak.com/blog/index.php/2017/04/04/64");
  SSDP.setManufacturer("Psykokwak");
  SSDP.setManufacturerURL("http://www.psykokwak.com");
  SSDP.setDeviceType("upnp:rootdevice");
  SSDP.begin();

  // MQTT configuration
  if (_config.MQTTPubInterval < 1) _config.MQTTPubInterval = 1;
  _mqttWifiClient.setNoDelay(true);
  _mqttWifiClient.setTimeout((unsigned long)MQTT_SOCKET_TIMEOUT * 1000UL);
  _mqtt.setServer(_config.MQTTServer.c_str(), _config.MQTTPort);
  _mqtt.setCallback(mqttCallback);

  QTLed.applyUserSettings();

  Serial.println("Ready");


  //**** Normal Sketch code here...


}


// the loop function runs over and over again forever
void loop() {

  // Handle WiFi AP/STA
  if (WiFiMgr.handle()) {
    getNTPtime();
  }

  // Update time from RTC
  handleTimeFromRTC();

  // Handle NTP receive packets
  handleNTPRequest();

  // Update time
  handleISRsecondTick();

  // Read current light value
  handleAmbientLightSensor();

  // OTA request handling
  ArduinoOTA.handle();

  // WebServer requests handling
  _server.handleClient();

  // DNS requests handling 
  //_dnsServer.processNextRequest();

  // MQTT
  mqttReconnect();
  _mqtt.loop();
  mqttPollingPublisher();

  // Hand the display back if a controller page went away without saying so
  QTLed.handleGameTimeout();

  // Handle led display
  handleScheduler();
  QTLed.handle();

  // For debug purpose only
  toggleLed(_timestamp);
}



