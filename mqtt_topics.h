

class MQTTTopic
{
private:
  String _type;
  String _suffix;
public:
  MQTTTopic(String type, String suffix)
    : _type(type)
    , _suffix(suffix)
  {}

  String topic()
  {
    String n = "/textime-" + String(ESP.getChipId(), HEX) + "/";
    return _type + n + _suffix;
  }
};


MQTTTopic mqttTopicSubLedColor("cmnd", "led/color");
MQTTTopic mqttTopicSubLedMode("cmnd", "led/mode");
MQTTTopic mqttTopicSubLedAnim("cmnd", "led/animation");

MQTTTopic mqttTopicPubLedColor("stat", "led/color");
MQTTTopic mqttTopicPubLedMode("stat", "led/mode");
MQTTTopic mqttTopicPubLedAnim("stat", "led/animation");

MQTTTopic mqttTopicPubTemp("tele", "temperature");
MQTTTopic mqttTopicPubLight("tele", "ambientlight");
MQTTTopic mqttTopicPubRssi("tele", "rssi");