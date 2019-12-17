#ifndef GLOBAL_H
#define GLOBAL_H


ESP8266HTTPUpdateServer _httpUpdater;
ESP8266WebServer _server(80);
DNSServer _dnsServer;
WiFiClient _mqttWifiClient;
PubSubClient _mqtt(_mqttWifiClient);


struct strConfig {
  boolean dhcp;                         // 1 Byte - EEPROM 16 
  boolean isDayLightSaving;             // 1 Byte - EEPROM 17
  long Update_Time_Via_NTP_Every;       // 4 Byte - EEPROM 18
  long timeZone;                        // 4 Byte - EEPROM 22
  byte  IP[4];                          // 4 Byte - EEPROM 32
  byte  Netmask[4];                     // 4 Byte - EEPROM 36
  byte  Gateway[4];                     // 4 Byte - EEPROM 40
  byte  DNS[4];                         // 4 Byte - EEPROM 44
  String ssid;                          // up to 64 Byte - EEPROM 64
  String password;                      // up to 64 Byte - EEPROM 128
  String ntpServerName;                 // up to 64 Byte - EEPROM 192
  String DeviceName;                    // up to 64 Byte - EEPROM 256

  boolean brightnessAuto;               // 1 Byte - EEPROM 384
  byte brightness;                      // 1 Byte - EEPROM 385
  byte color[4];                        // 4 Byte - EEPROM 386
  byte mode;                            // 1 Byte - EEPROM 390
  byte animation;                       // 1 Byte - EEPROM 391
  byte colorRandom;                     // 1 Byte - EEPROM 392
  byte brightnessAutoMinDay;            // 1 Byte - EEPROM 393
  byte brightnessAutoMinNight;          // 1 Byte - EEPROM 394
  byte ledConfig;                       // 1 Byte - EEPROM 395
  byte luxSensitivity;                  // 1 Byte - EEPROM 396
  byte language;                        // 1 Byte - EEPROM 397

  String MQTTServer;                    // up to 64 Byte - EEPROM 512
  String MQTTLogin;                     // up to 64 Byte - EEPROM 576
  String MQTTPassword;                  // up to 64 Byte - EEPROM 640
  long MQTTPort;                        // 4 Byte - EEPROM 704
  long MQTTPubInterval;                 // 4 Byte - EEPROM 708

} _config;


//  Auxiliary function to handle EEPROM

void EEPROMWritelong(int address, long value){
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);

  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}

long EEPROMReadlong(long address){
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);

  //Return the recomposed long by using bit shift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

// Check the Values is between 0-255
boolean checkRange(String Value){
  if (Value.toInt() < 0 || Value.toInt() > 255)
  {
    return false;
  }
  else
  {
    return true;
  }
}

void WriteStringToEEPROM(int beginaddress, String string){
  char  charBuf[string.length() + 1];
  string.toCharArray(charBuf, string.length() + 1);
  for (unsigned int t =  0; t < sizeof(charBuf); t++)
  {
    EEPROM.write(beginaddress + t, charBuf[t]);
  }
}

String  ReadStringFromEEPROM(int beginaddress){
  volatile byte counter = 0;
  char rChar;
  String retString = "";

  if (EEPROM.read(beginaddress) == 255)
    return String();

  while (1)
  {
    rChar = EEPROM.read(beginaddress + counter);
    if (rChar == 0) break;
    if (counter > 63) break;
    counter++;
    retString.concat(rChar);

  }
  return retString;
}


/*
**
** CONFIGURATION HANDLING
**
*/

void ResetEEPROM()
{
  for (int i = 0; i < 511; i++)
    EEPROM.write(i, 0xFF);

  EEPROM.commit();
}

void WriteConfig(){

  Serial.println("Writing Config");
  EEPROM.write(0, 'C');
  EEPROM.write(1, 'F');
  EEPROM.write(2, 'G');
  EEPROM.write(3, '2');

  EEPROM.write(16, _config.dhcp);
  EEPROM.write(17, _config.isDayLightSaving);

  EEPROMWritelong(18, _config.Update_Time_Via_NTP_Every); // 4 Byte
  EEPROMWritelong(22, _config.timeZone); // 4 Byte

  EEPROM.write(32, _config.IP[0]);
  EEPROM.write(33, _config.IP[1]);
  EEPROM.write(34, _config.IP[2]);
  EEPROM.write(35, _config.IP[3]);

  EEPROM.write(36, _config.Netmask[0]);
  EEPROM.write(37, _config.Netmask[1]);
  EEPROM.write(38, _config.Netmask[2]);
  EEPROM.write(39, _config.Netmask[3]);

  EEPROM.write(40, _config.Gateway[0]);
  EEPROM.write(41, _config.Gateway[1]);
  EEPROM.write(42, _config.Gateway[2]);
  EEPROM.write(43, _config.Gateway[3]);

  EEPROM.write(44, _config.DNS[0]);
  EEPROM.write(45, _config.DNS[1]);
  EEPROM.write(46, _config.DNS[2]);
  EEPROM.write(47, _config.DNS[3]);

  WriteStringToEEPROM(64, _config.ssid);
  WriteStringToEEPROM(128, _config.password);
  WriteStringToEEPROM(192, _config.ntpServerName);
  WriteStringToEEPROM(256, _config.DeviceName);

  EEPROM.write(384, _config.brightnessAuto);
  EEPROM.write(385, _config.brightness);
  EEPROM.write(386, _config.color[0]); // R
  EEPROM.write(387, _config.color[1]); // G
  EEPROM.write(388, _config.color[2]); // B
  EEPROM.write(389, _config.color[3]); // W
  EEPROM.write(390, _config.mode);
  EEPROM.write(391, _config.animation);
  EEPROM.write(392, _config.colorRandom);
  EEPROM.write(393, _config.brightnessAutoMinDay);
  EEPROM.write(394, _config.brightnessAutoMinNight);
  EEPROM.write(395, _config.ledConfig);
  EEPROM.write(396, _config.luxSensitivity);
  EEPROM.write(397, _config.language);

  WriteStringToEEPROM(512, _config.MQTTServer);
  WriteStringToEEPROM(576, _config.MQTTLogin);
  WriteStringToEEPROM(640, _config.MQTTPassword);
  EEPROMWritelong(704, _config.MQTTPort);
  EEPROMWritelong(708, _config.MQTTPubInterval);

  EEPROM.commit();
}

boolean ReadConfig(){
  Serial.println("Reading Configuration");
  if (EEPROM.read(0) == 'C' && EEPROM.read(1) == 'F'  && EEPROM.read(2) == 'G' && EEPROM.read(3) == '2')
  {
    Serial.println("Configuration Found!");
    _config.dhcp = 	EEPROM.read(16);
    _config.isDayLightSaving = EEPROM.read(17);
    _config.Update_Time_Via_NTP_Every = EEPROMReadlong(18); // 4 Byte
    _config.timeZone = EEPROMReadlong(22); // 4 Byte
    _config.IP[0] = EEPROM.read(32);
    _config.IP[1] = EEPROM.read(33);
    _config.IP[2] = EEPROM.read(34);
    _config.IP[3] = EEPROM.read(35);
    _config.Netmask[0] = EEPROM.read(36);
    _config.Netmask[1] = EEPROM.read(37);
    _config.Netmask[2] = EEPROM.read(38);
    _config.Netmask[3] = EEPROM.read(39);
    _config.Gateway[0] = EEPROM.read(40);
    _config.Gateway[1] = EEPROM.read(41);
    _config.Gateway[2] = EEPROM.read(42);
    _config.Gateway[3] = EEPROM.read(43);
    _config.DNS[0] = EEPROM.read(44);
    _config.DNS[1] = EEPROM.read(45);
    _config.DNS[2] = EEPROM.read(46);
    _config.DNS[3] = EEPROM.read(47);
    _config.ssid = ReadStringFromEEPROM(64);
    _config.password = ReadStringFromEEPROM(128);
    _config.ntpServerName = ReadStringFromEEPROM(192);
    _config.DeviceName = ReadStringFromEEPROM(256);

    // Application parameters here ... from EEPROM 192 to 511
    _config.brightnessAuto = EEPROM.read(384);
    _config.brightness = EEPROM.read(385);
    _config.color[0] = EEPROM.read(386); // R
    _config.color[1] = EEPROM.read(387); // G
    _config.color[2] = EEPROM.read(388); // B
    _config.color[3] = EEPROM.read(389); // W
    _config.mode = EEPROM.read(390);
    _config.animation = EEPROM.read(391);
    _config.colorRandom = EEPROM.read(392);
    _config.brightnessAutoMinDay = EEPROM.read(393);
    _config.brightnessAutoMinNight = EEPROM.read(394);
    _config.ledConfig = EEPROM.read(395);
    _config.luxSensitivity = EEPROM.read(396);
    _config.language = EEPROM.read(397);

    _config.MQTTServer = ReadStringFromEEPROM(512);
    _config.MQTTLogin = ReadStringFromEEPROM(576);
    _config.MQTTPassword = ReadStringFromEEPROM(640);
    _config.MQTTPort = EEPROMReadlong(704);
    _config.MQTTPubInterval = EEPROMReadlong(708);

    return true;

  }
  else
  {
    Serial.println("Configuration NOT FOUND!!!!");
    ResetEEPROM();
    return false;
  }
}


void printConfig(){

  Serial.println("Printing Config");

  Serial.printf("DHCP:%d\n", _config.dhcp);
  Serial.printf("DayLight:%d\n", _config.isDayLightSaving);

  Serial.printf("NTP update every %ld sec\n", _config.Update_Time_Via_NTP_Every); // 4 Byte
  Serial.printf("Timezone %ld\n", _config.timeZone); // 4 Byte

  Serial.printf("IP:%d.%d.%d.%d\n", _config.IP[0],_config.IP[1],_config.IP[2],_config.IP[3]);
  Serial.printf("Mask:%d.%d.%d.%d\n", _config.Netmask[0],_config.Netmask[1],_config.Netmask[2],_config.Netmask[3]);
  Serial.printf("Gateway:%d.%d.%d.%d\n", _config.Gateway[0], _config.Gateway[1], _config.Gateway[2], _config.Gateway[3]);
  Serial.printf("DNS:%d.%d.%d.%d\n", _config.DNS[0], _config.DNS[1], _config.DNS[2], _config.DNS[3]);

 
  Serial.printf("SSID:%s\n", _config.ssid.c_str());
  Serial.printf("PWD:%s\n", _config.password.c_str());
  Serial.printf("NTP ServerName:%s\n", _config.ntpServerName.c_str());
  Serial.printf("Device Name:%s\n", _config.DeviceName.c_str());

  // Application Settings here... from EEPROM 192 up to 511 (0 - 511)
  Serial.printf("Brightness auto:%d\n", _config.brightnessAuto);
  Serial.printf("Brightness:%d\n", _config.brightness);
  Serial.printf("Color:%02X%02X%02X%02X\n", _config.color[0], _config.color[1], _config.color[2], _config.color[3]);
  Serial.printf("Mode:%d\n", _config.mode);
  Serial.printf("Animation:%d\n", _config.animation);
  Serial.printf("Color Random:%d\n", _config.colorRandom);
  Serial.printf("Minimum brightness auto during the day:%d\n", _config.brightnessAutoMinDay);
  Serial.printf("Minimum brightness auto during the night:%d\n", _config.brightnessAutoMinNight);
  Serial.printf("Led Configuration:%d\n", _config.ledConfig);
}


int32_t GetRSSIinPercent(int32_t rssi)
{
  int quality = 0;
  if (rssi <= -100)
  {
    quality = 0;
  }
  else if (rssi >= -50)
  {
    quality = 100;
  }
  else
  {
    quality = 2 * (rssi + 100);
  }
  return quality;
}

String GetMacAddress(){
  uint8_t mac[6];
  char macStr[18] = {0};
  WiFi.macAddress(mac);
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0],  mac[1], mac[2], mac[3], mac[4], mac[5]);
  return  String(macStr);
}

String GetAPMacAddress(){
  uint8_t mac[6];
  char macStr[18] = {0};
  WiFi.softAPmacAddress(mac);
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0],  mac[1], mac[2], mac[3], mac[4], mac[5]);
  return  String(macStr);
}

// convert a single hex digit character to its integer value (from https://code.google.com/p/avr-netino/)
unsigned char h2int(char c){
  if (c >= '0' && c <= '9') {
    return ((unsigned char)c - '0');
  }
  if (c >= 'a' && c <= 'f') {
    return ((unsigned char)c - 'a' + 10);
  }
  if (c >= 'A' && c <= 'F') {
    return ((unsigned char)c - 'A' + 10);
  }
  return (0);
}

String urldecode(String input) // (based on https://code.google.com/p/avr-netino/)
{
  char c;
  String ret = "";

  for (byte t = 0; t < input.length(); t++)
  {
    c = input[t];
    if (c == '+') c = ' ';
    if (c == '%') {


      t++;
      c = input[t];
      t++;
      c = (h2int(c) << 4) | h2int(input[t]);
    }

    ret.concat(c);
  }
  return ret;

}


uint64_t millis64() {
  static uint32_t low32, high32;
  uint32_t new_low32 = millis();
  if (new_low32 < low32) high32++;
  low32 = new_low32;
  return (uint64_t)high32 << 32 | low32;
}

void toggleLed(unsigned long counter)
{
  static int p = 0;

  int v = (counter % 2);

  if (v != p)
    digitalWrite(LED_BUILTIN, v);

  p = v;
}

String dec2hex2(byte v)
{
  String s = String((int)(v), HEX);

  if (v < 0x10)
    s = "0" + s;

  s.toUpperCase();

  return s;
}


class Frame
{
private:
  double _fps;
  uint64_t _framePrevious;

public:
  Frame() :
    _fps(10),
    _framePrevious(0)
  {
  }

  void init(double fps)
  {
    _fps = fps;
    _framePrevious = 0;
  }

  bool next()
  {
    uint64_t frame = millis64();

    if (frame - _framePrevious < (1000.0 / _fps))
      return false;

    _framePrevious = frame;
    return true;
  }
};

#endif
