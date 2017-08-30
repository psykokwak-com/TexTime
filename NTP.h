
/*
**
**  NTP
**
*/

#define NTP_PACKET_SIZE 48
#define LEAP_YEAR(Y) ( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) )

struct strDateTime
{
  byte hour;
  byte minute;
  byte second;
  int year;
  byte month;
  byte day;
  byte wday;
};

unsigned long _timestamp = 0;  	          // GLOBALTIME, will be refreshed every Second
strDateTime _dateTime;                    // Global DateTime structure, will be refreshed every Second
unsigned long _previousUpdate = 0;        // Used to save last automatic ntp update time


static const uint8_t _monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
byte _packetBuffer[NTP_PACKET_SIZE];
WiFiUDP _UDPNTPClient;



void getNTPtime()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    _UDPNTPClient.begin(2390);  // Port for NTP receive
    IPAddress timeServerIP;
    WiFi.hostByName(_config.ntpServerName.c_str(), timeServerIP);

    Serial.println("NTP:sending NTP packet...");
    memset(_packetBuffer, 0, NTP_PACKET_SIZE);
    _packetBuffer[0] = 0b11100011;   // LI, Version, Mode
    _packetBuffer[1] = 0;     // Stratum, or type of clock
    _packetBuffer[2] = 6;     // Polling Interval
    _packetBuffer[3] = 0xEC;  // Peer Clock Precision
    _packetBuffer[12] = 49;
    _packetBuffer[13] = 0x4E;
    _packetBuffer[14] = 49;
    _packetBuffer[15] = 52;
    _UDPNTPClient.beginPacket(timeServerIP, 123);
    _UDPNTPClient.write(_packetBuffer, NTP_PACKET_SIZE);
    _UDPNTPClient.endPacket();
  }
}

void handleNTPRequest()
{
  unsigned long unixTime = 0;

  if (WiFi.status() != WL_CONNECTED)
    return ;

  int cb = _UDPNTPClient.parsePacket();

  if (cb != 0)
  {
    Serial.print("NTP:NTP packet received, length=");
    Serial.println(cb);

    _UDPNTPClient.read(_packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    unsigned long highWord = word(_packetBuffer[40], _packetBuffer[41]);
    unsigned long lowWord = word(_packetBuffer[42], _packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;

    unixTime = secsSince1900 - seventyYears;
  }

  if (unixTime > 0)
  {
    Serial.println("NTP Sync Dt : " + String(unixTime - _timestamp) + "s");
    _timestamp = unixTime; // store universally available time stamp
  }

  if (unixTime > 0)
  {
    // Update RTC
    RtcDateTime dt;
    dt.InitWithEpoch32Time(_timestamp);
    RTC.SetDateTime(dt);
  }

  // Resync with NTP
  if (_previousUpdate == 0) _previousUpdate = _timestamp;
  if (_config.Update_Time_Via_NTP_Every && ((long)(_timestamp - _previousUpdate) >= _config.Update_Time_Via_NTP_Every))
  {
    _previousUpdate = _timestamp;
    getNTPtime();
  }
}


strDateTime convertDateTimeToUptime(const strDateTime &d)
{
  strDateTime dt = d;

  dt.year = dt.year - 1970;
  dt.month = dt.month - 1;
  dt.day = dt.day - 1;

  return dt;
}

strDateTime convertUnixTimeStamp(unsigned long tempTimeStamp)
{
  strDateTime _tempDateTime;
  uint8_t year;
  uint8_t month, monthLength;
  uint32_t time;
  unsigned long days;

  time = (uint32_t)tempTimeStamp;
  _tempDateTime.second = time % 60;
  time /= 60; // now it is minutes
  _tempDateTime.minute = time % 60;
  time /= 60; // now it is hours
  _tempDateTime.hour = time % 24;
  time /= 24; // now it is days
  _tempDateTime.wday = ((time + 4) % 7) + 1;  // Sunday is day 1

  year = 0;
  days = 0;
  while ((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
    year++;
  }
  _tempDateTime.year = year; // year is offset from 1970

  days -= LEAP_YEAR(year) ? 366 : 365;
  time  -= days; // now it is days in this year, starting at 0

  days = 0;
  month = 0;
  monthLength = 0;
  for (month = 0; month < 12; month++) {
    if (month == 1) { // february
      if (LEAP_YEAR(year)) {
        monthLength = 29;
      } else {
        monthLength = 28;
      }
    } else {
      monthLength = _monthDays[month];
    }

    if (time >= monthLength) {
      time -= monthLength;
    } else {
      break;
    }
  }
  _tempDateTime.month = month + 1;  // jan is month 1
  _tempDateTime.day = time + 1;     // day of month
  _tempDateTime.year += 1970;

  return _tempDateTime;
}


//
// Summertime calculates the daylight saving time for middle Europe. Input: Unixtime in UTC
//
boolean summerTime(unsigned long timeStamp) {
  strDateTime  _tempDateTime = convertUnixTimeStamp(timeStamp);
  // printTime("Innerhalb ", _tempDateTime);

  if ((_tempDateTime.month < 3) || (_tempDateTime.month > 10)) return false; // keine Sommerzeit in Jan, Feb, Nov, Dez
  if ((_tempDateTime.month > 3) && (_tempDateTime.month < 10)) return true; // Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep
  if (((_tempDateTime.month == 3) && ((_tempDateTime.hour + 24 * _tempDateTime.day) >= (3 +  24 * (31 - (5 * _tempDateTime.year / 4 + 4) % 7)))) || ((_tempDateTime.month == 10) && ((_tempDateTime.hour + 24 * _tempDateTime.day) < (3 +  24 * (31 - (5 * _tempDateTime.year / 4 + 1) % 7)))))
    return true;
  else
    return false;
}

unsigned long adjustTimeZone(unsigned long timeStamp, int timeZone, bool isDayLightSavingSaving) {
  timeStamp += timeZone * 360; // adjust timezone
  if (isDayLightSavingSaving && summerTime(timeStamp)) timeStamp += 3600; // Sommerzeit beachten
  return timeStamp;
}

String printDateTime(const strDateTime &d)
{
  //return String(d.day) + String("/") + String(d.month) + String("/") + String(d.year) + String(" ") + String(d.hour) + String(":") + String(d.minute) + String(":") + String(d.second);

  char datestring[20];

  snprintf_P(datestring,
    countof(datestring),
    PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
    d.day,
    d.month,
    d.year,
    d.hour,
    d.minute,
    d.second);

  return datestring;
}

String printDateTime(const RtcDateTime& dt)
{
  strDateTime d;

  d.day = dt.Day();
  d.month = dt.Month();
  d.year = dt.Year();
  d.hour = dt.Hour();
  d.minute = dt.Minute();
  d.second = dt.Second();

  return printDateTime(d);
}


void handleTimeFromRTC()
{
  static uint64_t p = 1;

  uint64_t v = millis64() / (1000 * 60 * 60); // Resync time from RTC every 1 hour

  if (v != p)
  {
    if (RTC.GetIsRunning())
    {
      unsigned long t = (unsigned long)RTC.GetDateTime().Epoch32Time();
      Serial.println("RTC Sync Dt : " + String(t - _timestamp) + "s");
      _timestamp = t;
    }

    p = v;
  }
}

void updateTime()
{

  unsigned long t = adjustTimeZone(_timestamp, _config.timeZone, _config.isDayLightSaving);
  _dateTime = convertUnixTimeStamp(t);  //  convert to DateTime format

  //Serial.print("TS:" + String(_timestamp));
  //Serial.println(" Date:" + printDate());
}

void handleISRsecondTick()
{
  static uint64_t s = millis64() / 1000;
  static uint64_t pe = 0;

  uint64_t c = millis64() / 1000;

  while (pe < c - s)
  {
    _timestamp++;
    updateTime();
    pe++;
  }
}
