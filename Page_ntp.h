


void send_ntp_configuration_values_html()
{
    
  String values ="";
  values += "ntpserver|" + (String) _config.ntpServerName + "|input\n";
  values += "update|" +  (String) _config.Update_Time_Via_NTP_Every + "|input\n";
  values += "tz|" +  (String) _config.timeZone + "|input\n";
  values += "dst|" +  (String) (_config.isDayLightSaving ? "checked" : "") + "|chk\n";

  _server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  _server.sendHeader("Pragma", "no-cache");
  _server.sendHeader("Expires", "-1");
  _server.send ( 200, "text/plain", values);

  //Serial.println(__FUNCTION__); 
}
