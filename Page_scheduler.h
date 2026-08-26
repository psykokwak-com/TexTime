
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

// Put the display back under the user's own settings, undoing whatever slot
// override was last applied. Used both when the scheduler is switched off and
// when a slot boundary lands on an unpainted half-hour.
void restoreUserSettings() {
  syncAnimParamsFromConfig();
  QTLed.setMode(_config.mode);
  QTLed.setAnimation(_config.animation);
  QTLed.setColor(_config.color[0], _config.color[1], _config.color[2]);
  QTLed.setColorRandom((RandomColorMode)_config.colorRandom);
}

void save_scheduler_enabled() {
  for (uint8_t i = 0; i < _server.args(); i++) {
    if (_server.argName(i) == "enabled") {
      bool enabled = (_server.arg(i) == "1");
      EEPROM.write(SCHEDULER_EEPROM_BASE, enabled ? 1 : 0);
      EEPROM.commit();

      // Turning the scheduler off must hand the display back to the user
      // settings straight away, instead of leaving the last slot override in
      // place until the next reboot.
      if (!enabled)
        restoreUserSettings();
    }
  }
  _server.send(200, "text/plain", "OK");
}

// Bulk save. The payload is deduplicated to keep the POST body small:
//   "rules" = each distinct slot definition once, 16 hex chars each (255 max)
//   "map"   = one 2-hex-char rule index per half-hour slot, FF = empty slot
// A flat dump of the 336 slots would be a 5.4 KB body, which is a lot of heap
// to ask for on an ESP8266 that already holds a 4 KB EEPROM buffer. A typical
// schedule uses a handful of distinct rules and fits in well under 1 KB.
void save_scheduler_bulk() {
  String rules = _server.arg("rules");
  String map   = _server.arg("map");

  if (map.length() != 336 * 2 || (rules.length() % 16) != 0) {
    _server.send(400, "text/plain", "ERROR");
    return;
  }

  int ruleCount = rules.length() / 16;
  if (ruleCount > 255) {
    _server.send(400, "text/plain", "ERROR");
    return;
  }

  char buf[3]; buf[2] = 0;
  for (int i = 0; i < 336; i++) {
    int addr = SCHEDULER_EEPROM_BASE + 1 + i * SCHEDULER_SLOT_SIZE;

    buf[0] = map[i * 2];
    buf[1] = map[i * 2 + 1];
    int ruleIndex = (int)strtol(buf, 0, 16);

    // FF, or anything pointing past the rule table, means "no rule here".
    if (ruleIndex < 0 || ruleIndex >= ruleCount) {
      EEPROM.write(addr, 0xFF);
      continue;
    }

    for (int k = 0; k < SCHEDULER_SLOT_SIZE; k++) {
      buf[0] = rules[ruleIndex * 16 + k * 2];
      buf[1] = rules[ruleIndex * 16 + k * 2 + 1];
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

  // A game owns the display while it runs: applying a slot would change the
  // mode under the player's hands. Keep the slot marked as not yet applied so
  // it takes effect as soon as the game stops.
  if (QTLed.isTetrisActive() || QTLed.isSnakeActive()) {
    _schedLastSlot = 255;
    return;
  }

  byte currentSlot = _dateTime.hour * 2 + (_dateTime.minute >= 30 ? 1 : 0);
  // _dateTime.wday: 1=Sunday..7=Saturday → convert to 0=Mon..6=Sun
  byte currentDay = (_dateTime.wday <= 1) ? 6 : (_dateTime.wday - 2);

  if (currentSlot == _schedLastSlot && currentDay == _schedLastDay) return;
  _schedLastSlot = currentSlot;
  _schedLastDay  = currentDay;

  int addr = schedAddr(currentDay, currentSlot);
  byte mode = EEPROM.read(addr);

  // An unpainted half-hour means "nothing programmed here, go back to my
  // normal settings" -- not "keep whatever the last programmed slot left
  // behind". Without this, painting a single hour on Monday held its override
  // for the rest of the week.
  if (mode == 0xFF || mode > 7) {
    restoreUserSettings();
    return;
  }

  byte animation      = constrain(EEPROM.read(addr + 1), 0, 14);
  byte packed         = EEPROM.read(addr + 2);
  byte colorRandom    = packed & 0x03;
  byte animSpeed      = constrain(((packed >> 2) & 0x1F) + 1, 1, 20);
  byte animBrightMin  = constrain(EEPROM.read(addr + 3), 0, 100);
  byte animBrightMax  = constrain(EEPROM.read(addr + 4), 0, 100);
  byte cr = EEPROM.read(addr + 5);
  byte cg = EEPROM.read(addr + 6);
  byte cb = EEPROM.read(addr + 7);

  // Same rule as validateAnimBrightness(): min must stay below max, otherwise
  // the pulsing animations end up with a zero amplitude. Applied on the live
  // parameters only -- a slot override must never reach _config/EEPROM.
  if (animBrightMax < 1) animBrightMax = 1;
  if (animBrightMin >= animBrightMax) animBrightMin = animBrightMax - 1;

  _animParams.brightnessMin = animBrightMin;
  _animParams.brightnessMax = animBrightMax;

  QTLed.setMode(mode);
  QTLed.setAnimation(animation);
  QTLed.setColor(cr, cg, cb);
  QTLed.setColorRandom((RandomColorMode)colorRandom);
  QTLed.setAnimSpeed(animSpeed);
}
