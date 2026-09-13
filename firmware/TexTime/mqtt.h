

uint64_t _mqttPollingPublisherTimer = 0;
void mqttPollingPublisher()
{
  // if not connected, exit
  if (!_mqtt.connected())
    return;

  // polling not to fast
  if (millis64() - _mqttPollingPublisherTimer < (uint64_t)_config.MQTTPubInterval * 1000 && _mqttPollingPublisherTimer != 0)
    return;

  if (RTC.GetIsRunning())
    _mqtt.publish(mqttTopicPubTemp.topic().c_str(), String(RTC.GetTemperature().AsFloatDegC(), 2).c_str(), true);

  _mqtt.publish(mqttTopicPubLight.topic().c_str(), String(getAvgLux()).c_str(), true);

  _mqtt.publish(mqttTopicPubRssi.topic().c_str(), String(GetRSSIinPercent(WiFi.RSSI())).c_str(), true);

  byte r, g, b;
  QTLed.getColor(r, g, b);

  String sr = ("0" + String((int)r, HEX)); sr = sr.substring(sr.length() - 2);
  String sg = ("0" + String((int)g, HEX)); sg = sg.substring(sg.length() - 2);
  String sb = ("0" + String((int)b, HEX)); sb = sb.substring(sb.length() - 2);
  String s = "#" + (sr + sg + sb);
  s.toUpperCase();
  _mqtt.publish(mqttTopicPubLedColor.topic().c_str(), s.c_str(), true);

  _mqtt.publish(mqttTopicPubLedMode.topic().c_str(), String(QTLed.getModeIndex()).c_str(), true);

  _mqtt.publish(mqttTopicPubLedAnim.topic().c_str(), String(QTLed.getAnimationIndex()).c_str(), true);

  _mqttPollingPublisherTimer = millis64();
}

void mqttCallback(char* topic, byte* payloadraw, unsigned int length) {
  String payload;

  for (unsigned int i = 0; i < length; i++)
    payload += (char)payloadraw[i];

  Serial.print("topic:");
  Serial.print(topic);
  Serial.print(" : ");
  Serial.print((char*)payload.c_str());
  Serial.print(" (");
  Serial.print(payload.length());
  Serial.println(")");

  // Set color
  if (!strncmp(topic, mqttTopicSubLedColor.topic().c_str(), mqttTopicSubLedColor.topic().length())) {

    if (payload.length() == 0) {
      byte r, g, b;
      QTLed.getColor(r, g, b);

      String sr = ("0" + String((int)r, HEX)); sr = sr.substring(sr.length() - 2);
      String sg = ("0" + String((int)g, HEX)); sg = sg.substring(sg.length() - 2);
      String sb = ("0" + String((int)b, HEX)); sb = sb.substring(sb.length() - 2);
      String s = "#" + (sr + sg + sb);
      s.toUpperCase();

      _mqtt.publish(mqttTopicPubLedColor.topic().c_str(), s.c_str(), true);

      return;
    }
    
    if (payload.length() != 7)
      return;

    int32_t l = strtol(payload.c_str() + 1, 0, HEX);
    byte r = (l >> 16) & 0xFF;
    byte g = (l >> 8) & 0xFF;
    byte b = (l >> 0) & 0xFF;
    QTLed.setColor(r, g, b);

    Serial.print("Set color from MQTT : #");
    Serial.println(l, HEX);
  }

  // Set mode
  if (!strncmp(topic, mqttTopicSubLedMode.topic().c_str(), mqttTopicSubLedMode.topic().length())) {

    // If there is no payload, send back the current value
    if (payload.length() == 0) {
      _mqtt.publish(mqttTopicPubLedMode.topic().c_str(), String(QTLed.getModeIndex()).c_str(), true);
      return;
    }

    if (!QTLed.setMode(payload.toInt()))
      return;

    Serial.print("Set mode from MQTT : ");
    Serial.println(payload);
  }

  // Set animation
  if (!strncmp(topic, mqttTopicSubLedAnim.topic().c_str(), mqttTopicSubLedAnim.topic().length())) {

    // If there is no payload, send back the current value
    if (payload.length() == 0) {
      _mqtt.publish(mqttTopicPubLedAnim.topic().c_str(), String(QTLed.getAnimationIndex()).c_str(), true);
      return;
    }

    if (!QTLed.setAnimation(payload.toInt()))
      return;

    Serial.print("Set animation from MQTT : ");
    Serial.println(payload);
  }
}

void mqttReconnect() {
  static uint64_t t = 0;

  // if connected, exit
  if (_mqtt.connected())
    return;

  // if there is no mqtt broker, exit
  if (_config.MQTTServer.length() == 0)
    return;

  // retry not to fast
  if (millis64() - t < 5000)
    return;

  // Connect
  int r = 0;
  Serial.println("MQTT connection...");
  if (_config.MQTTLogin.length() == 0 && _config.MQTTPassword.length() == 0)
    r = _mqtt.connect(String("TexTime-" + String(ESP.getChipId(), HEX)).c_str());
  else
    r = _mqtt.connect(String("TexTime-" + String(ESP.getChipId(), HEX)).c_str(), _config.MQTTLogin.c_str(), _config.MQTTPassword.c_str());

  if (!r) {
    // Unable to connect
    Serial.print("MQTT failed, rc=");
    Serial.print(_mqtt.state());
    Serial.println(" try again in 5 seconds");

    t = millis64();

    return;
  }

  _mqttPollingPublisherTimer = 0;

  // Connection OK
  Serial.println("MQTT connected :)");

  _mqtt.subscribe(mqttTopicSubLedColor.topic().c_str());
  _mqtt.subscribe(mqttTopicSubLedMode.topic().c_str());
  _mqtt.subscribe(mqttTopicSubLedAnim.topic().c_str());
}
