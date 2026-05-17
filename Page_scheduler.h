
// Slot layout in EEPROM (8 bytes per slot):
//   [0] mode (0xFF = slot disabled)
//   [1] animation
//   [2] (colorRandom & 0x03) | ((animSpeed-1) << 2)
//   [3] animBrightnessMin
//   [4] animBrightnessMax
//   [5] R  [6] G  [7] B
//
// 336 slots total: 7 days × 48 half-hours (slot = hour*2 + (min>=30?1:0))

inline int schedAddr(byte day, byte slot) {  // slot 0..47
  return SCHEDULER_EEPROM_BASE + 1 + (int(day) * 48 + slot) * SCHEDULER_SLOT_SIZE;
}

void send_scheduler_config() {
  byte en = EEPROM.read(SCHEDULER_EEPROM_BASE);
  String out = "schedulerEnabled|" + String(en == 1 ? "checked" : "") + "|chk\n";
  _server.send(200, "text/plain", out);
}

void send_scheduler_data() {
  _server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  _server.send(200, "text/plain", "");
  char buf[17];
  for (int day = 0; day < 7; day++) {
    for (int slot = 0; slot < 48; slot++) {
      int addr = schedAddr(day, slot);
      for (int k = 0; k < SCHEDULER_SLOT_SIZE; k++)
        sprintf(buf + k * 2, "%02X", EEPROM.read(addr + k));
      buf[16] = 0;
      _server.sendContent(buf);
    }
  }
  _server.sendContent("");
}

void save_scheduler_slot() {
  int day = -1, hour = -1, minute = 0;
  byte mode = 0xFF, animation = 0, colorRandom = 0, animSpeed = 10;
  byte animBrightnessMin = 10, animBrightnessMax = 100;
  byte r = 255, g = 255, b = 255;
  bool slotEnabled = false;

  for (uint8_t i = 0; i < _server.args(); i++) {
    String n = _server.argName(i);
    String v = _server.arg(i);
    if (n == "day")            day = v.toInt();
    else if (n == "hour")      hour = v.toInt();
    else if (n == "minute")    minute = v.toInt();
    else if (n == "enabled")   slotEnabled = (v == "1");
    else if (n == "mode")      mode = constrain(v.toInt(), 0, 7);
    else if (n == "animation") animation = constrain(v.toInt(), 0, 12);
    else if (n == "colorrandom")   colorRandom = constrain(v.toInt(), 0, 3);
    else if (n == "animspeed")     animSpeed = constrain(v.toInt(), 1, 20);
    else if (n == "animbrightmin") animBrightnessMin = constrain(v.toInt(), 0, 100);
    else if (n == "animbrightmax") animBrightnessMax = constrain(v.toInt(), 0, 100);
    else if (n == "color") {
      String cs = v;
      if (cs.startsWith("#")) cs = cs.substring(1);
      int32_t l = strtol(cs.c_str(), 0, 16);
      r = (l >> 16) & 0xFF;
      g = (l >> 8) & 0xFF;
      b = l & 0xFF;
    }
  }

  if (day < 0 || day > 6 || hour < 0 || hour > 23) {
    _server.send(400, "text/plain", "ERROR");
    return;
  }

  int slot = hour * 2 + (minute >= 30 ? 1 : 0);
  int addr = schedAddr(day, slot);
  if (!slotEnabled) {
    EEPROM.write(addr, 0xFF);
  } else {
    EEPROM.write(addr,     mode);
    EEPROM.write(addr + 1, animation);
    EEPROM.write(addr + 2, (colorRandom & 0x03) | ((animSpeed - 1) << 2));
    EEPROM.write(addr + 3, animBrightnessMin);
    EEPROM.write(addr + 4, animBrightnessMax);
    EEPROM.write(addr + 5, r);
    EEPROM.write(addr + 6, g);
    EEPROM.write(addr + 7, b);
  }
  EEPROM.commit();
  _server.send(200, "text/plain", "OK");
}

void save_scheduler_enabled() {
  for (uint8_t i = 0; i < _server.args(); i++) {
    if (_server.argName(i) == "enabled") {
      EEPROM.write(SCHEDULER_EEPROM_BASE, _server.arg(i) == "1" ? 1 : 0);
      EEPROM.commit();
    }
  }
  _server.send(200, "text/plain", "OK");
}

void save_scheduler_raz() {
  for (int i = 0; i < 336; i++)
    EEPROM.write(SCHEDULER_EEPROM_BASE + 1 + i * SCHEDULER_SLOT_SIZE, 0xFF);
  EEPROM.commit();
  _server.send(200, "text/plain", "OK");
}

// Bulk save: body arg "data" = 336*16 hex chars representing all slots
void save_scheduler_bulk() {
  String data = _server.arg("data");
  if (data.length() != 336 * 16) {
    _server.send(400, "text/plain", "ERROR");
    return;
  }
  char buf[3]; buf[2] = 0;
  for (int i = 0; i < 336; i++) {
    int addr = SCHEDULER_EEPROM_BASE + 1 + i * SCHEDULER_SLOT_SIZE;
    for (int k = 0; k < SCHEDULER_SLOT_SIZE; k++) {
      buf[0] = data[i * 16 + k * 2];
      buf[1] = data[i * 16 + k * 2 + 1];
      EEPROM.write(addr + k, (byte)strtol(buf, 0, 16));
    }
  }
  EEPROM.commit();
  _server.send(200, "text/plain", "OK");
}

byte _schedLastSlot = 255;
byte _schedLastDay  = 255;

void apply_scheduler_now() {
  _schedLastSlot = 255;
  _server.send(200, "text/plain", "OK");
}

// Called from loop() every iteration — runs at most once per half-hour (or immediately after apply_scheduler_now)
void handleScheduler() {
  if (EEPROM.read(SCHEDULER_EEPROM_BASE) != 1) return;

  byte currentSlot = _dateTime.hour * 2 + (_dateTime.minute >= 30 ? 1 : 0);
  // _dateTime.wday: 1=Sunday..7=Saturday → convert to 0=Mon..6=Sun
  byte currentDay = (_dateTime.wday <= 1) ? 6 : (_dateTime.wday - 2);

  if (currentSlot == _schedLastSlot && currentDay == _schedLastDay) return;
  _schedLastSlot = currentSlot;
  _schedLastDay  = currentDay;

  int addr = schedAddr(currentDay, currentSlot);
  byte mode = EEPROM.read(addr);
  if (mode == 0xFF || mode > 7) return;

  byte animation      = constrain(EEPROM.read(addr + 1), 0, 12);
  byte packed         = EEPROM.read(addr + 2);
  byte colorRandom    = packed & 0x03;
  byte animSpeed      = constrain(((packed >> 2) & 0x1F) + 1, 1, 20);
  byte animBrightMin  = constrain(EEPROM.read(addr + 3), 0, 100);
  byte animBrightMax  = constrain(EEPROM.read(addr + 4), 0, 100);
  byte cr = EEPROM.read(addr + 5);
  byte cg = EEPROM.read(addr + 6);
  byte cb = EEPROM.read(addr + 7);

  _config.animBrightnessMin = animBrightMin;
  _config.animBrightnessMax = animBrightMax;

  QTLed.setMode(mode);
  QTLed.setAnimation(animation);
  QTLed.setColor(cr, cg, cb);
  QTLed.setColorRandom((RandomColorMode)colorRandom);
  QTLed.setAnimSpeed(animSpeed);
}
