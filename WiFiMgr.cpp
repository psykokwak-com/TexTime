// 
// 
// 

#include "WiFiMgr.h"

WiFiMgrClass::WiFiMgrClass()
  : _STAssid("")
  , _STAkey("")
  , _devicename("")
  , _STAlastTry(0)
  , _STAtryTimeout(15 * 1000)
  , _APssid("")
  , _APkey("")
  , _APlastTry(0)
  , _APtryTimeout(10 * 60 * 1000)
  , _STAconnected(false)
  , _STAIPip(192,168,1,1)
  , _STAIPgw(192,168,1,1)
  , _STAIPmask(255,255,255,0)
  , _STAIPdns(192,168,1,1)
  , _STADHCP(true)
{
  _APssid = "ESP-" + String(ESP.getChipId(), HEX);
}

void WiFiMgrClass::setAPssid(String ssid, String key)
{
  _APssid = ssid;
  _APkey = key;
}

void WiFiMgrClass::setSTAIPdhcp(bool dhcp)
{
  _STADHCP = dhcp;
}

void WiFiMgrClass::setSTAIPip(IPAddress ip, IPAddress gw, IPAddress mask, IPAddress dns)
{
  _STAIPip = ip;
  _STAIPgw = gw;
  _STAIPmask = mask;
  _STAIPdns = dns;

  setSTAIPdhcp(false);
}

void WiFiMgrClass::tryToConnect(const String& ssid, const String& key, const String& devicename)
{
  _STAssid = ssid;
  _STAkey = key;
  _devicename = devicename;

  tryToReconnect();
}

void WiFiMgrClass::tryToReconnect()
{

  // If STA mode is not configured, directly switch in AP mode
  if (_STAssid.length() == 0) {
    setAPMode();
    return;
  }

  _STAconnected = false;

  WiFi.mode(WIFI_STA);

  wifi_station_set_hostname(_devicename.c_str()); //See more at : http ://www.esp8266.com/viewtopic.php?f=29&t=11124#sthash.458xtq2U.dpuf

  Serial.print("Trying to connect to ");
  Serial.println(_STAssid.c_str());

  if (!_STADHCP)
    WiFi.config(_STAIPip, _STAIPgw, _STAIPmask, _STAIPdns);

  WiFi.begin(_STAssid, _STAkey);

  WiFi.setSleepMode(WIFI_NONE_SLEEP);

  _STAlastTry = millis();
}

void WiFiMgrClass::setAPMode()
{
  IPAddress apIP(192, 168, 1, 1);
  IPAddress apNetMsk(255, 255, 255, 0);

  WiFi.setAutoReconnect(false);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, apNetMsk);

  Serial.println("Setting AP mode");

  if (_APkey.length() == 0)
    WiFi.softAP(_APssid);
  else
    WiFi.softAP(_APssid, _APkey);

  WiFi.setSleepMode(WIFI_NONE_SLEEP);

  _APlastTry = millis();

  _STAconnected = false;
}

bool WiFiMgrClass::handle()
{
  // STA mode and connected. Do Nothing
  if (WiFi.getMode() == WIFI_STA && WiFi.status() == WL_CONNECTED) {
    if (_STAconnected)
      return false;

    Serial.println("WiFi Connected");

    Serial.print("WiFi IP : ");
    Serial.println(WiFi.localIP());

    WiFi.setAutoReconnect(true);

    _STAconnected = true;
    return true;
  }

  // STA mode and not connected after timeout. Switch on AP mode
  if (WiFi.getMode() == WIFI_STA && millis() - _STAlastTry > _STAtryTimeout) {
    setAPMode();
  }

  // AP mode and timeout. Try to reconnect in STA mode
  if (WiFi.getMode() == WIFI_AP && millis() - _APlastTry > _APtryTimeout) {
    tryToReconnect();
  }

  return false;
}

WiFiMgrClass WiFiMgr;
