#ifndef PAGE_INFOMATION_H
#define PAGE_INFOMATION_H


// HTML PAGE REMOVED - Now using dashboard in Page_index.h
// This page is obsolete and replaced by the dashboard interface


//
// FILL WITH INFOMATION
// 

void send_information_configuration_values_html ()
{
  String values ="";

  values += "x_ssid|" + (String)WiFi.SSID() + "|div\n";
  values += "x_rssi|" + (String)GetRSSIinPercent(WiFi.RSSI()) + "|div\n";
  values += "x_ip|" +  (String) WiFi.localIP()[0] + "." +  (String) WiFi.localIP()[1] + "." +  (String) WiFi.localIP()[2] + "." + (String) WiFi.localIP()[3] +  "|div\n";
  values += "x_gateway|" +  (String) WiFi.gatewayIP()[0] + "." +  (String) WiFi.gatewayIP()[1] + "." +  (String) WiFi.gatewayIP()[2] + "." + (String) WiFi.gatewayIP()[3] +  "|div\n";
  values += "x_netmask|" + (String)WiFi.subnetMask()[0] + "." + (String)WiFi.subnetMask()[1] + "." + (String)WiFi.subnetMask()[2] + "." + (String)WiFi.subnetMask()[3] + "|div\n";
  values += "x_dns|" + (String)WiFi.dnsIP()[0] + "." + (String)WiFi.dnsIP()[1] + "." + (String)WiFi.dnsIP()[2] + "." + (String)WiFi.dnsIP()[3] + "|div\n";
  values += "x_mac|" + GetMacAddress() + "|div\n";
  values += "x_version|" + printDateTime(RtcDateTime(__DATE__, __TIME__)) + "|div\n";
  values += "x_boot|" + printDateTime(convertDateTimeToUptime(convertUnixTimeStamp(millis64() / 1000))) + "|div\n";
  values += "x_date|" + printDateTime(_dateTime) + "|div\n";
  values += "x_als|" + String(getAvgLux()) + "|div\n";
  values += "x_temp|" + (RTC.GetIsRunning() ? String(RTC.GetTemperature().AsFloatDegC()) : String("N/A")) + "|div\n";
  values += "x_brightness|" + String((int)QTLed.getBrightness()) + "|div\n";

  _server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  _server.sendHeader("Pragma", "no-cache");
  _server.sendHeader("Expires", "-1");

  _server.send ( 200, "text/plain", values);

  //Serial.println(__FUNCTION__);
}


#endif
