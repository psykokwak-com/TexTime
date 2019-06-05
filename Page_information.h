#ifndef PAGE_INFOMATION_H
#define PAGE_INFOMATION_H


//
//   The HTML PAGE
//
const char PAGE_information[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" href="style.css" type="text/css" />
<script src="microajax.js"></script> 
<a href="/"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>System Information</strong>
<hr>
<table border="0" cellspacing="0" cellpadding="3" style="width:410px" >

<tr><td align="right">Date :</td><td><span id="x_date"></span></td></tr>
<tr><td align="right">Uptime :</td><td><span id="x_boot"></span></td></tr>
<tr><td align="right">Build Date :</td><td><span id="x_version"></span></td></tr>

<tr><td colspan="2" align="center"><hr></td></tr>

<tr><td align="right">SSID :</td><td><span id="x_ssid"></span></td></tr>
<tr><td align="right">RSSI :</td><td><span id="x_rssi"></span>%</td></tr>
<tr><td align="right">IP :</td><td><span id="x_ip"></span></td></tr>
<tr><td align="right">Netmask :</td><td><span id="x_netmask"></span></td></tr>
<tr><td align="right">Gateway :</td><td><span id="x_gateway"></span></td></tr>
<tr><td align="right">DNS :</td><td><span id="x_dns"></span></td></tr>
<tr><td align="right">Mac :</td><td><span id="x_mac"></span></td></tr>

<tr><td colspan="2" align="center"><hr></td></tr>

<tr><td align="right">Ambient light :</td><td><span id="x_als"></span> lux</td></tr>
<tr><td align="right">Brightness :</td><td><span id="x_brightness"></span></td></tr>
<tr><td align="right">Temperature :</td><td><span id="x_temp"></span> C</td></tr>

<tr><td colspan="2" align="center"><hr></td></tr>

<!-- <tr><td colspan="2" align="center"><a href="javascript:GetState(); void 0" class="btn btn--m btn--blue">Refresh</a></td></tr> -->
</table>
<script>

function getState()
{
  setValues("/admin/infovalues");
}

window.onload = function ()
{
  load("style.css","css", function() 
  {
    load("microajax.js","js", function() 
    {
        setInterval(getState, 500);
    });
  });
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}



</script>
)=====" ;


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
