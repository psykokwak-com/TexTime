// WiFiMgr.h

#ifndef _WIFIMGR_h
#define _WIFIMGR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <ESP8266WiFi.h>

class WiFiMgrClass
{
public:
  WiFiMgrClass();

  void setSTAIPdhcp(bool dhcp);
  void setSTAIPip(IPAddress ip, IPAddress gw, IPAddress mask, IPAddress dns);

  void setAPssid(String ssid, String key = "");

  void tryToConnect(String ssid, String key, String devicename);
  bool handle();

protected:
private:
  String _STAssid;
  String _STAkey;
  String _devicename;
  unsigned int _STAlastTry;
  unsigned int _STAtryTimeout;
  String _APssid;
  String _APkey;
  unsigned int _APlastTry;
  unsigned int _APtryTimeout;
  bool _STAconnected;
  IPAddress _STAIPip;
  IPAddress _STAIPgw;
  IPAddress _STAIPmask;
  IPAddress _STAIPdns;
  bool _STADHCP;

  void tryToReconnect();
  void setAPMode();
};

extern WiFiMgrClass WiFiMgr;


#endif

