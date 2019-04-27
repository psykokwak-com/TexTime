

//
//  HTML PAGE
//
const char PAGE_network[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<a href="/"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>Network Configuration</strong>
<hr>
Connect to Router with these settings:<br>
<form action="" method="get">
<table border="0"  cellspacing="0" cellpadding="3" style="width:410px" >
<tr><td align="right">SSID:</td><td><input type="text" id="ssid" name="ssid" value=""></td></tr>
<tr><td align="right">Password:</td><td><input type="text" id="password" name="password" value=""></td></tr>
<tr><td align="right">DHCP:</td><td><input type="checkbox" id="dhcp" name="dhcp" onclick="validate()"></td></tr>
<tr><td align="right">IP:</td><td><input type="text" id="ip_0" name="ip_0" size="3">.<input type="text" id="ip_1" name="ip_1" size="3">.<input type="text" id="ip_2" name="ip_2" size="3">.<input type="text" id="ip_3" name="ip_3" value="" size="3"></td></tr>
<tr><td align="right">Netmask:</td><td><input type="text" id="nm_0" name="nm_0" size="3">.<input type="text" id="nm_1" name="nm_1" size="3">.<input type="text" id="nm_2" name="nm_2" size="3">.<input type="text" id="nm_3" name="nm_3" size="3"></td></tr>
<tr><td align="right">Gateway:</td><td><input type="text" id="gw_0" name="gw_0" size="3">.<input type="text" id="gw_1" name="gw_1" size="3">.<input type="text" id="gw_2" name="gw_2" size="3">.<input type="text" id="gw_3" name="gw_3" size="3"></td></tr>
<tr><td align="right">DNS:</td><td><input type="text" id="dn_0" name="dn_0" size="3">.<input type="text" id="dn_1" name="dn_1" size="3">.<input type="text" id="dn_2" name="dn_2" size="3">.<input type="text" id="dn_3" name="dn_3" size="3"></td></tr>
<tr><td align="right">Device name :</td><td><input type="text" id="devicename" name="devicename" value=""></td></tr>
<tr><td colspan="2" align="center"><input type="submit" style="width:150px" class="btn btn--m btn--blue" value="Save"></td></tr>
</table>
</form>
<hr>
<strong>Connection State:</strong><div id="connectionstate">N/A</div>
<hr>
<strong>Networks:</strong><br>
<table border="0" cellspacing="3" style="width:410px" >
<tr><td><div id="networks">Scanning...</div></td></tr>
<tr><td align="center"><a href="javascript:GetState(); void 0" style="width:150px" class="btn btn--m btn--blue">Refresh</a></td></tr>
</table>

<script type="text/javascript">
    function validate() {
        document.getElementById("ip_0").disabled = document.getElementById('dhcp').checked; 
        document.getElementById("ip_1").disabled = document.getElementById('dhcp').checked; 
        document.getElementById("ip_2").disabled = document.getElementById('dhcp').checked; 
        document.getElementById("ip_3").disabled = document.getElementById('dhcp').checked; 
        document.getElementById("nm_0").disabled = document.getElementById('dhcp').checked; 
        document.getElementById("nm_1").disabled = document.getElementById('dhcp').checked; 
        document.getElementById("nm_2").disabled = document.getElementById('dhcp').checked; 
        document.getElementById("nm_3").disabled = document.getElementById('dhcp').checked;
        document.getElementById("gw_0").disabled = document.getElementById('dhcp').checked; 
        document.getElementById("gw_1").disabled = document.getElementById('dhcp').checked; 
        document.getElementById("gw_2").disabled = document.getElementById('dhcp').checked; 
        document.getElementById("gw_3").disabled = document.getElementById('dhcp').checked;
        document.getElementById("dn_0").disabled = document.getElementById('dhcp').checked; 
        document.getElementById("dn_1").disabled = document.getElementById('dhcp').checked; 
        document.getElementById("dn_2").disabled = document.getElementById('dhcp').checked; 
        document.getElementById("dn_3").disabled = document.getElementById('dhcp').checked;
}
</script>

<script>

function GetState()
{
	setValues("/admin/connectionstate");
}
function selssid(value)
{
	document.getElementById("ssid").value = value; 
}


window.onload = function ()
{
	load("style.css","css", function() 
	{
		load("microajax.js","js", function() 
		{
					setValues("/admin/networkvalues", function() {
            validate();
            GetState();
          });
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}

</script>

)=====";


const char PAGE_networkReload[] PROGMEM = R"=====(
<meta http-equiv="refresh" content="10; URL=network.html">
Please Wait....Configuring and Restarting.
)=====";

//
//  SEND HTML PAGE OR IF A FORM SUMBITTED VALUES, PROCESS THESE VALUES
// 

void send_network_configuration_html()
{
	
	if (_server.args() > 0 )  // Save Settings
	{
		//String temp = "";
		_config.dhcp = false;
		for ( uint8_t i = 0; i < _server.args(); i++ ) {
      //Serial.println(_server.argName(i) + " = " + _server.arg(i));

			if (_server.argName(i) == "ssid") _config.ssid =   /*urldecode*/(_server.arg(i));
			if (_server.argName(i) == "password") _config.password =    /*urldecode*/(_server.arg(i));
			if (_server.argName(i) == "ip_0") if (checkRange(_server.arg(i))) 	_config.IP[0] =  _server.arg(i).toInt();
			if (_server.argName(i) == "ip_1") if (checkRange(_server.arg(i))) 	_config.IP[1] =  _server.arg(i).toInt();
			if (_server.argName(i) == "ip_2") if (checkRange(_server.arg(i))) 	_config.IP[2] =  _server.arg(i).toInt();
			if (_server.argName(i) == "ip_3") if (checkRange(_server.arg(i))) 	_config.IP[3] =  _server.arg(i).toInt();
			if (_server.argName(i) == "nm_0") if (checkRange(_server.arg(i))) 	_config.Netmask[0] =  _server.arg(i).toInt();
			if (_server.argName(i) == "nm_1") if (checkRange(_server.arg(i))) 	_config.Netmask[1] =  _server.arg(i).toInt();
			if (_server.argName(i) == "nm_2") if (checkRange(_server.arg(i))) 	_config.Netmask[2] =  _server.arg(i).toInt();
			if (_server.argName(i) == "nm_3") if (checkRange(_server.arg(i))) 	_config.Netmask[3] =  _server.arg(i).toInt();
			if (_server.argName(i) == "gw_0") if (checkRange(_server.arg(i))) 	_config.Gateway[0] =  _server.arg(i).toInt();
			if (_server.argName(i) == "gw_1") if (checkRange(_server.arg(i))) 	_config.Gateway[1] =  _server.arg(i).toInt();
			if (_server.argName(i) == "gw_2") if (checkRange(_server.arg(i))) 	_config.Gateway[2] =  _server.arg(i).toInt();
			if (_server.argName(i) == "gw_3") if (checkRange(_server.arg(i))) 	_config.Gateway[3] =  _server.arg(i).toInt();
      if (_server.argName(i) == "dn_0") if (checkRange(_server.arg(i))) 	_config.DNS[0] = _server.arg(i).toInt();
      if (_server.argName(i) == "dn_1") if (checkRange(_server.arg(i))) 	_config.DNS[1] = _server.arg(i).toInt();
      if (_server.argName(i) == "dn_2") if (checkRange(_server.arg(i))) 	_config.DNS[2] = _server.arg(i).toInt();
      if (_server.argName(i) == "dn_3") if (checkRange(_server.arg(i))) 	_config.DNS[3] = _server.arg(i).toInt();
      if (_server.argName(i) == "dhcp") _config.dhcp = true;
      if (_server.argName(i) == "devicename") _config.DeviceName = /*urldecode*/(_server.arg(i));
		}
    _server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    _server.sendHeader("Pragma", "no-cache");
    _server.sendHeader("Expires", "-1");

    _server.send_P(200, "text/html", PAGE_networkReload);

    //printConfig();

		WriteConfig();
    ESP.restart();
	}
	else
	{
    _server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    _server.sendHeader("Pragma", "no-cache");
    _server.sendHeader("Expires", "-1");

    _server.send_P(200, "text/html", PAGE_network);
	}
	//Serial.println(__FUNCTION__); 
}



//
//   FILL THE PAGE WITH VALUES
//

void send_network_configuration_values_html()
{

	String values ="";

	values += "ssid|" + (String) _config.ssid + "|input\n";
	values += "password|" +  (String) _config.password + "|input\n";
	values += "ip_0|" +  (String) _config.IP[0] + "|input\n";
	values += "ip_1|" +  (String) _config.IP[1] + "|input\n";
	values += "ip_2|" +  (String) _config.IP[2] + "|input\n";
	values += "ip_3|" +  (String) _config.IP[3] + "|input\n";
	values += "nm_0|" +  (String) _config.Netmask[0] + "|input\n";
	values += "nm_1|" +  (String) _config.Netmask[1] + "|input\n";
	values += "nm_2|" +  (String) _config.Netmask[2] + "|input\n";
	values += "nm_3|" +  (String) _config.Netmask[3] + "|input\n";
	values += "gw_0|" +  (String) _config.Gateway[0] + "|input\n";
	values += "gw_1|" +  (String) _config.Gateway[1] + "|input\n";
	values += "gw_2|" +  (String) _config.Gateway[2] + "|input\n";
	values += "gw_3|" +  (String) _config.Gateway[3] + "|input\n";
  values += "dn_0|" + (String)_config.DNS[0] + "|input\n";
  values += "dn_1|" + (String)_config.DNS[1] + "|input\n";
  values += "dn_2|" + (String)_config.DNS[2] + "|input\n";
  values += "dn_3|" + (String)_config.DNS[3] + "|input\n";
	values += "dhcp|" +  (String) (_config.dhcp ? "checked" : "") + "|chk\n";
  values += "devicename|" + (String)_config.DeviceName + "|input\n";

  _server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  _server.sendHeader("Pragma", "no-cache");
  _server.sendHeader("Expires", "-1");

	_server.send ( 200, "text/plain", values);
	//Serial.println(__FUNCTION__); 
}


//
//   FILL THE PAGE WITH NETWORKSTATE & NETWORKS
//

void send_connection_state_values_html()
{

	String state = "N/A";
	String Networks = "";
	if (WiFi.status() == 0) state = "Idle";
	else if (WiFi.status() == 1) state = "NO SSID AVAILBLE";
	else if (WiFi.status() == 2) state = "SCAN COMPLETED";
	else if (WiFi.status() == 3) state = "CONNECTED";
	else if (WiFi.status() == 4) state = "CONNECT FAILED";
	else if (WiFi.status() == 5) state = "CONNECTION LOST";
	else if (WiFi.status() == 6) state = "DISCONNECTED";



	 int n = WiFi.scanNetworks();

	 if (n == 0)
	 {
		 Networks = "<font color='#FF0000'>No networks found!</font>";
	 }
	else
    {
	 
		
		Networks = "Found " +String(n) + " Networks<br>";
		Networks += "<table border='0' cellspacing='0' cellpadding='3'>";
		Networks += "<tr bgcolor='#DDDDDD' ><td><strong>Name</strong></td><td><strong>Quality</strong></td><td><strong>Enc</strong></td><tr>";
		for (int i = 0; i < n; ++i)
		{
      Networks += "<tr><td><a href='javascript:selssid(\"" + String(WiFi.SSID(i)) + "\"); void 0'>" + String(WiFi.SSID(i)) + "</a></td><td>" + String(GetRSSIinPercent(WiFi.RSSI(i))) + "%</td><td>" + String((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*") + "</td></tr>";
		}
		Networks += "</table>";
	}
   
	String values ="";
	values += "connectionstate|" +  state + "|div\n";
	values += "networks|" +  Networks + "|div\n";

  _server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  _server.sendHeader("Pragma", "no-cache");
  _server.sendHeader("Expires", "-1");

	_server.send ( 200, "text/plain", values);
	//Serial.println(__FUNCTION__); 
}


