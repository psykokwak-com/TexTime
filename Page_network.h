//
//  HTML PAGE
//
// HTML PAGES REMOVED - Now using dashboard in Page_index.h
// These pages are obsolete and replaced by the dashboard interface

// The names of neighbouring networks are arbitrary text chosen by other people,
// and the scan list is injected into the dashboard with innerHTML. Anything
// taken from a beacon has to arrive there as text, not as markup.
String htmlEscape(const String &s)
{
  String o;
  o.reserve(s.length() + 8);
  for (unsigned int i = 0; i < s.length(); i++)
  {
    char c = s[i];
    switch (c)
    {
      case '&':  o += F("&amp;");  break;
      case '<':  o += F("&lt;");   break;
      case '>':  o += F("&gt;");   break;
      case '"':  o += F("&quot;"); break;
      case '\'': o += F("&#39;");  break;
      default:   o += c;
    }
  }
  return o;
}

// For a name that lands inside a JavaScript string literal which is itself
// inside an HTML attribute. The browser undoes the HTML escaping first and the
// JavaScript escaping second, so they have to be applied the other way round.
String jsAttrEscape(const String &s)
{
  String o;
  o.reserve(s.length() + 8);
  for (unsigned int i = 0; i < s.length(); i++)
  {
    char c = s[i];
    if (c == '\\' || c == '\'') { o += '\\'; o += c; }
    else if (c == '\r' || c == '\n') { /* a newline would end the statement */ }
    else o += c;
  }
  return htmlEscape(o);
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

  // A failed scan returns a negative value (WIFI_SCAN_FAILED is -2). Testing
  // only for zero let that reach "String seenSSIDs[n]" below, where a negative
  // length becomes an enormous unsigned one.
  if (n <= 0)
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
        networks += "' onclick=\"selssid('" + jsAttrEscape(currentSSID) + "')\">";

        networks += "<div class='network-info'>";
        networks += "<div class='network-name'>" + htmlEscape(currentSSID) + "</div>";
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
          networks += "<button class='connect-btn' onclick=\"event.stopPropagation(); selssid('" + jsAttrEscape(currentSSID) + "')\">Connect</button>";
        }
        networks += "</div>";

        networks += "</div>";
      }
    }
  }

  _server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  _server.sendHeader("Pragma", "no-cache");
  _server.sendHeader("Expires", "-1");

  // Streamed in pieces rather than concatenated into one more String. The
  // network list is the largest thing this server builds, and copying it again
  // meant holding it twice at the moment memory is tightest -- where
  // String::concat fails silently, so the symptom was a truncated list rather
  // than an error.
  _server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  _server.send(200, "text/plain", "");
  _server.sendContent("connectionstate|" + state + "|div\n");
  _server.sendContent("networks|");
  _server.sendContent(networks);
  _server.sendContent("|div\n");
  _server.sendContent("");
  //Serial.println(__FUNCTION__);
}
