

//
//  HTML PAGE
//
const char PAGE_mqtt[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<a href="/"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>MQTT Configuration</strong>
<hr>
Connect to MQTT Broker with these settings:<br><br>
<form action="" method="get">
<table border="0"  cellspacing="0" cellpadding="3" style="width:410px" >
<tr><td align="right">Broker host:</td><td><input type="text" id="host" name="host" value=""></td></tr>
<tr><td align="right">Broker port:</td><td><input type="text" id="port" name="port" value=""></td></tr>
<tr><td align="right">Broker login:</td><td><input type="text" id="login" name="login" value=""></td></tr>
<tr><td align="right">Broker password:</td><td><input type="password" id="password" name="password" value=""></td></tr>
<tr><td align="right">Publish interval:</td><td><input type="text" id="interval" name="interval" value=""></td></tr>
<tr><td colspan="2" align="center"><input type="submit" style="width:150px" class="btn btn--m btn--blue" value="Save"></td></tr>
</table>
</form>

<hr>
<strong>Connection State:</strong><div id="connectionstate">N/A</div>
<hr>

<h3>Topic list (all payloads are ascii) :</h3>
<b>Subscriber topics : </b><br>
<div id="sublist" style="font-size: smaller;"></div>
<br>
<b>Publisher topics : </b><br>
<div id="publist" style="font-size: smaller;"></div>
<br>
<script>
getState = function ()
{
  setValues("/admin/mqttconnectionvalues");
}

window.onload = function ()
{
	load("style.css","css", function() 
	{
		load("microajax.js","js", function() 
		{
					setValues("/admin/mqttfieldsvalues", function() {
            setInterval(getState, 500);
          });
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}

</script>

)=====";


//
//  SEND HTML PAGE OR IF A FORM SUMBITTED VALUES, PROCESS THESE VALUES
// 

void send_mqtt_configuration_html()
{
	
	if (_server.args() > 0 )  // Save Settings
	{
		for ( uint8_t i = 0; i < _server.args(); i++ ) {
      //Serial.println(_server.argName(i) + " = " + _server.arg(i));

			if (_server.argName(i) == "host") _config.MQTTServer = (_server.arg(i));
			if (_server.argName(i) == "port") _config.MQTTPort = _server.arg(i).toInt();
			if (_server.argName(i) == "login") _config.MQTTLogin = _server.arg(i);
      if (_server.argName(i) == "password") _config.MQTTPassword = _server.arg(i);
      if (_server.argName(i) == "interval") _config.MQTTPubInterval = _server.arg(i).toInt();

		}
    _server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    _server.sendHeader("Pragma", "no-cache");
    _server.sendHeader("Expires", "-1");

    _server.send_P(200, "text/html", PAGE_mqtt);

    //printConfig();

		WriteConfig();

    _mqtt.disconnect();
    _mqtt.setServer(_config.MQTTServer.c_str() , _config.MQTTPort);
    // let the main loop calls mqttreconnect();
	}
	else
	{
    _server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    _server.sendHeader("Pragma", "no-cache");
    _server.sendHeader("Expires", "-1");

    _server.send_P(200, "text/html", PAGE_mqtt);
	}
	//Serial.println(__FUNCTION__); 
}



//
//   FILL THE PAGE WITH VALUES
//

void send_mqtt_configuration_values_html()
{

	String values ="";

	values += "host|" + (String) _config.MQTTServer + "|input\n";
	values += "port|" + String(_config.MQTTPort) + "|input\n";
	values += "login|" +  (String) _config.MQTTLogin + "|input\n";
	values += "password|" +  (String) _config.MQTTPassword + "|input\n";
  values += "interval|" + String(_config.MQTTPubInterval) + "|input\n";


  String sublist;
  sublist += "\"" + mqttTopicSubLedColor.topic() + "\" : set display color. Value in hex. eg : #00FF00<br>";
  sublist += "\"" + mqttTopicSubLedMode.topic() + "\" : set display mode. Value in dec. eg : 1<br>";
  sublist += "\"" + mqttTopicSubLedAnim.topic() + "\" : set display animation. Value in dec. eg : 3<br>";
  sublist += "<i>Empty payload returns current value. See publishing \"stat\" topics.</i><br>";

  String publist;
  publist += "\"" + mqttTopicPubLedColor.topic() + "\" : get display color. Value in hex. eg : #00FF00<br>";
  publist += "\"" + mqttTopicPubLedMode.topic() + "\" : get display mode. Value in dec. eg : 1<br>";
  publist += "\"" + mqttTopicPubLedAnim.topic() + "\" : get display animation. Value in dec. eg : 3<br>";
  publist += "<br>";
  publist += "\"" + mqttTopicPubTemp.topic() + "\" : get temperature. Value in degrees celius.<br>";
  publist += "\"" + mqttTopicPubLight.topic() + "\" : get ambient light. Value in lumens.<br>";
  publist += "\"" + mqttTopicPubRssi.topic() + "\" : get WiFi RSSI. Value in %.<br>";

  values += "sublist|" + sublist + "|div\n";
  values += "publist|" + publist + "|div\n";

  _server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  _server.sendHeader("Pragma", "no-cache");
  _server.sendHeader("Expires", "-1");

	_server.send ( 200, "text/plain", values);
	//Serial.println(__FUNCTION__); 
}

void send_mqtt_connection_values_html()
{
  String s = "";
  switch (_mqtt.state())
  {
  case MQTT_CONNECTION_TIMEOUT: s = "MQTT_CONNECTION_TIMEOUT"; break;
  case MQTT_CONNECTION_LOST: s = "MQTT_CONNECTION_LOST"; break;
  case MQTT_CONNECT_FAILED: s = "MQTT_CONNECT_FAILED"; break;
  case MQTT_DISCONNECTED: s = "MQTT_DISCONNECTED"; break;
  case MQTT_CONNECTED: s = "MQTT_CONNECTED"; break;
  case MQTT_CONNECT_BAD_PROTOCOL: s = "MQTT_CONNECT_BAD_PROTOCOL"; break;
  case MQTT_CONNECT_BAD_CLIENT_ID: s = "MQTT_CONNECT_BAD_CLIENT_ID"; break;
  case MQTT_CONNECT_UNAVAILABLE: s = "MQTT_CONNECT_UNAVAILABLE"; break;
  case MQTT_CONNECT_BAD_CREDENTIALS: s = "MQTT_CONNECT_BAD_CREDENTIALS"; break;
  case MQTT_CONNECT_UNAUTHORIZED: s = "MQTT_CONNECT_UNAUTHORIZED"; break;
  default: s = "MQTT_UNKNOW_ERROR"; break;
  }

  String values = "";
  values += "connectionstate|" + s + "|div\n";

  _server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  _server.sendHeader("Pragma", "no-cache");
  _server.sendHeader("Expires", "-1");

  _server.send(200, "text/plain", values);
  //Serial.println(__FUNCTION__); 
}