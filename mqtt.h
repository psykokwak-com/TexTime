class MQTTTopic
{
private:
  String _prefix;
  String _suffix;
public:
  MQTTTopic(String suffix) 
    : _prefix("psykokwak/")
    , _suffix(suffix)
  {}

  String topic() 
  {
    String n = "textime-" + String(ESP.getChipId(), HEX);
    return _prefix + n + _suffix;
  }
};


MQTTTopic mqttTopicSubLedColor("/led/color");
MQTTTopic mqttTopicSubLedMode("/led/mode");
MQTTTopic mqttTopicSubLedAnim("/led/animation");

MQTTTopic mqttTopicPubTemp("/temperature");
MQTTTopic mqttTopicPubLight("/ambientlight");
MQTTTopic mqttTopicPubRssi("/rssi");


uint64_t _mqttPollingPublisherTimer = 0;
void mqttPollingPublisher()
{
  // if not connected, exit
  if (!_mqtt.connected())
    return;

  // polling not to fast
  if (millis64() - _mqttPollingPublisherTimer < _config.MQTTPubInterval * 1000 && _mqttPollingPublisherTimer != 0)
    return;

  if (RTC.GetIsRunning())
    _mqtt.publish(mqttTopicPubTemp.topic().c_str(), String(RTC.GetTemperature().AsFloatDegC(), 2).c_str());

  _mqtt.publish(mqttTopicPubLight.topic().c_str(), String(getAvgLux()).c_str());

  _mqtt.publish(mqttTopicPubRssi.topic().c_str(), String(GetRSSIinPercent(WiFi.RSSI())).c_str());

  _mqttPollingPublisherTimer = millis64();
}

void mqttCallback(char* topic, byte* payloadraw, unsigned int length) {
  String payload;

  for (int i = 0; i < length; i++)
    payload += (char)payloadraw[i];

  Serial.print("topic:");
  Serial.print(topic);
  Serial.print(" : ");
  Serial.print((char*)payload.c_str());
  Serial.println("");

  // Set color
  if (!strncmp(topic, mqttTopicSubLedColor.topic().c_str(), mqttTopicSubLedColor.topic().length())) {
    int32_t l = strtol(payload.c_str(), 0, HEX);
    byte r = (l >> 16) & 0xFF;
    byte g = (l >> 8) & 0xFF;
    byte b = (l >> 0) & 0xFF;
    QTLed.setColor(r, g, b);

    Serial.print("Set color from MQTT : #");
    Serial.println(l, HEX);
  }

  // Set mode
  if (!strncmp(topic, mqttTopicSubLedMode.topic().c_str(), mqttTopicSubLedMode.topic().length())) {
    QTLed.setMode(payload.toInt());

    Serial.print("Set mode from MQTT : ");
    Serial.println(payload);
  }

  // Set animation
  if (!strncmp(topic, mqttTopicSubLedAnim.topic().c_str(), mqttTopicSubLedAnim.topic().length())) {
    QTLed.setAnimation(payload.toInt());

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
