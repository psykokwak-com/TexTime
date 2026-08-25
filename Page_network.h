//
//  HTML PAGE
//
// HTML PAGES REMOVED - Now using dashboard in Page_index.h
// These pages are obsolete and replaced by the dashboard interface

const char PAGE_networkReload[] PROGMEM = R"=====(
<meta http-equiv="refresh" content="10; URL=/">
Please Wait....Configuring and Restarting.
)=====";

//
//  SEND HTML PAGE OR IF A FORM SUMBITTED VALUES, PROCESS THESE VALUES
//

void send_network_configuration_html()
{
  if (_server.args() > 0)  // Save Settings
  {
    //String temp = "";
    _config.dhcp = false;
    for (uint8_t i = 0; i < _server.args(); i++) {
      //Serial.println(_server.argName(i) + " = " + _server.arg(i));

      if (_server.argName(i) == "ssid") _config.ssid =   /*urldecode*/(_server.arg(i));
      if (_server.argName(i) == "password") _config.password =    /*urldecode*/(_server.arg(i));
      if (_server.argName(i) == "ip_0") if (checkRange(_server.arg(i))) 	_config.IP[0] = _server.arg(i).toInt();
      if (_server.argName(i) == "ip_1") if (checkRange(_server.arg(i))) 	_config.IP[1] = _server.arg(i).toInt();
      if (_server.argName(i) == "ip_2") if (checkRange(_server.arg(i))) 	_config.IP[2] = _server.arg(i).toInt();
      if (_server.argName(i) == "ip_3") if (checkRange(_server.arg(i))) 	_config.IP[3] = _server.arg(i).toInt();
      if (_server.argName(i) == "nm_0") if (checkRange(_server.arg(i))) 	_config.Netmask[0] = _server.arg(i).toInt();
      if (_server.argName(i) == "nm_1") if (checkRange(_server.arg(i))) 	_config.Netmask[1] = _server.arg(i).toInt();
      if (_server.argName(i) == "nm_2") if (checkRange(_server.arg(i))) 	_config.Netmask[2] = _server.arg(i).toInt();
      if (_server.argName(i) == "nm_3") if (checkRange(_server.arg(i))) 	_config.Netmask[3] = _server.arg(i).toInt();
      if (_server.argName(i) == "gw_0") if (checkRange(_server.arg(i))) 	_config.Gateway[0] = _server.arg(i).toInt();
      if (_server.argName(i) == "gw_1") if (checkRange(_server.arg(i))) 	_config.Gateway[1] = _server.arg(i).toInt();
      if (_server.argName(i) == "gw_2") if (checkRange(_server.arg(i))) 	_config.Gateway[2] = _server.arg(i).toInt();
      if (_server.argName(i) == "gw_3") if (checkRange(_server.arg(i))) 	_config.Gateway[3] = _server.arg(i).toInt();
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

    _server.sendHeader("Location", "/");
    _server.send(302);
  }
  //Serial.println(__FUNCTION__);
}

//
//   FILL THE PAGE WITH VALUES
//

void send_network_configuration_values_html()
{
  String values = "";

  values += "ssid|" + (String)_config.ssid + "|input\n";
  values += "password||input\n";
  values += "ipaddress|" + (String)_config.IP[0] + "." + (String)_config.IP[1] + "." + (String)_config.IP[2] + "." + (String)_config.IP[3] + "|input\n";
  values += "netmask|" + (String)_config.Netmask[0] + "." + (String)_config.Netmask[1] + "." + (String)_config.Netmask[2] + "." + (String)_config.Netmask[3] + "|input\n";
  values += "gateway|" + (String)_config.Gateway[0] + "." + (String)_config.Gateway[1] + "." + (String)_config.Gateway[2] + "." + (String)_config.Gateway[3] + "|input\n";
  values += "dnsserver|" + (String)_config.DNS[0] + "." + (String)_config.DNS[1] + "." + (String)_config.DNS[2] + "." + (String)_config.DNS[3] + "|input\n";
  values += "dhcp|" + (String)(_config.dhcp ? "checked" : "") + "|chk\n";
  values += "devicename|" + (String)_config.DeviceName + "|input\n";

  _server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  _server.sendHeader("Pragma", "no-cache");
  _server.sendHeader("Expires", "-1");

  _server.send(200, "text/plain", values);
  //Serial.println(__FUNCTION__);
}

//
//   FILL THE PAGE WITH NETWORKSTATE & NETWORKS
//

void send_network_connection_values_html()
{
  String state = "N/A";
  String networks = "";
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
    networks = "<div class='loading'>No networks found</div>";
  }
  else
  {
    // Remove duplicates - track seen SSIDs
    String seenSSIDs[n];
    int uniqueCount = 0;

    networks = "";
    for (int i = 0; i < n; ++i)
    {
      String currentSSID = WiFi.SSID(i);
      bool isDuplicate = false;

      // Check if SSID already seen
      for (int j = 0; j < uniqueCount; j++) {
        if (seenSSIDs[j] == currentSSID) {
          isDuplicate = true;
          break;
        }
      }

      if (!isDuplicate && currentSSID.length() > 0) {
        seenSSIDs[uniqueCount] = currentSSID;
        uniqueCount++;

        int rssiPercent = GetRSSIinPercent(WiFi.RSSI(i));
        bool isSecured = (WiFi.encryptionType(i) != ENC_TYPE_NONE);
        bool isConnected = (currentSSID == WiFi.SSID());

        // Generate signal bars
        int bars = (rssiPercent + 19) / 20; // Convert to 1-5 scale
        String signalBars = "<div class='signal-bars'>";
        for (int b = 1; b <= 5; b++) {
          signalBars += "<div class='signal-bar";
          if (b <= bars) signalBars += " active";
          signalBars += "'></div>";
        }
        signalBars += "</div>";

        networks += "<div class='network-item";
        if (isConnected) networks += " connected";
        networks += "' onclick=\"selssid('" + currentSSID + "')\">";

        networks += "<div class='network-info'>";
        networks += "<div class='network-name'>" + currentSSID + "</div>";
        networks += "<div class='network-details'>";
        networks += "<div class='network-signal'>";
        networks += signalBars;
        networks += "<span>" + String(rssiPercent) + "%</span>";
        networks += "</div>";
        networks += "<div class='network-security'>";
        networks += "<span class='security-icon'>" + String(isSecured ? "🔒" : "🔓") + "</span>";
        networks += "<span>" + String(isSecured ? "Secured" : "Open") + "</span>";
        networks += "</div>";
        networks += "</div>";
        networks += "</div>";

        networks += "<div class='network-actions'>";
        if (isConnected) {
          networks += "<span class='connected-badge'>Connected</span>";
        }
        else {
          networks += "<button class='connect-btn' onclick=\"event.stopPropagation(); selssid('" + currentSSID + "')\">Connect</button>";
        }
        networks += "</div>";

        networks += "</div>";
      }
    }
  }

  String values = "";
  values += "connectionstate|" + state + "|div\n";
  values += "networks|" + networks + "|div\n";

  _server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  _server.sendHeader("Pragma", "no-cache");
  _server.sendHeader("Expires", "-1");

  _server.send(200, "text/plain", values);
  //Serial.println(__FUNCTION__);
}
