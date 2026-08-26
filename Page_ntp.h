
// HTML PAGE REMOVED - Now using dashboard in Page_index.h
// This page is obsolete and replaced by the dashboard interface


void send_NTP_configuration_html()
{
  if (_server.args() > 0 )  // Save Settings
  {
    _config.isDayLightSaving = false;
    String temp = "";
    for ( uint8_t i = 0; i < _server.args(); i++ ) {
      if (_server.argName(i) == "ntpserver") _config.ntpServerName = _server.arg(i); 
      if (_server.argName(i) == "update") _config.Update_Time_Via_NTP_Every = constrain(_server.arg(i).toInt(), NTP_INTERVAL_MIN, NTP_INTERVAL_MAX);
      if (_server.argName(i) == "tz") _config.timeZone =  _server.arg(i).toInt(); 
      if (_server.argName(i) == "dst") _config.isDayLightSaving = true; 
    }
    WriteConfig();
    getNTPtime(); // Update NTP time
  }
  _server.sendHeader("Location", "/");
  _server.send(302); 
  //Serial.println(__FUNCTION__); 
  
}



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
