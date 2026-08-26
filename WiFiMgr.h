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

  void tryToConnect(const String& ssid, const String& key, const String& devicename);
  bool handle();

protected:
private:
  String _STAssid;
  String _STAkey;
  String _devicename;
  unsigned long _STAlastTry;
  // Two grace periods. The short one applies before the clock has ever been on
  // the network, where the point is to offer the setup access point quickly.
  // The long one applies afterwards: a router reboot takes about a minute, and
  // dropping to AP mode for it costs ten minutes off the network.
  unsigned long _STAtryTimeout;
  unsigned long _STAretryTimeout;
  String _APssid;
  String _APkey;
  unsigned long _APlastTry;
  unsigned long _APtryTimeout;
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

