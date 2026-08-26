//
//  HTML PAGE
//

// HTML PAGE REMOVED - Now using dashboard in Page_index.h
// This page is obsolete and replaced by the dashboard interface
void send_general_html()
{
	
	if (_server.args() > 0 )  // Save Settings
	{
    _config.brightnessAuto = false;
		for ( uint8_t i = 0; i < _server.args(); i++ ) {
      if (_server.argName(i) == "brightnessauto") _config.brightnessAuto = true;
      if (_server.argName(i) == "brightness") _config.brightness = _server.arg(i).toInt();
      if (_server.argName(i) == "brightnessday") _config.brightnessAutoMinDay = _server.arg(i).toInt();
      if (_server.argName(i) == "brightnessnight") _config.brightnessAutoMinNight = _server.arg(i).toInt();
      if (_server.argName(i) == "brightnessmax") _config.brightnessMax = constrain(_server.arg(i).toInt(), 1, 255);
      if (_server.argName(i) == "color")
      {
        String colorStr = _server.arg(i);
        // Remove # if present
        if (colorStr.startsWith("#")) {
          colorStr = colorStr.substring(1);
        }
        int32_t l = strtol(colorStr.c_str(), 0, 16);
        _config.color[0] = (l >> 16) & 0xFF;
        _config.color[1] = (l >> 8) & 0xFF;
        _config.color[2] = (l >> 0) & 0xFF;
      }
      if (_server.argName(i) == "lang") _config.language = _server.arg(i).toInt();
      if (_server.argName(i) == "mode") _config.mode = _server.arg(i).toInt();
      if (_server.argName(i) == "animation") _config.animation = _server.arg(i).toInt();
      if (_server.argName(i) == "colorrandom") _config.colorRandom = _server.arg(i).toInt();
      if (_server.argName(i) == "ledconfig") _config.ledConfig = _server.arg(i).toInt();
      if (_server.argName(i) == "brightnesssensibility") _config.luxSensitivity = constrain(_server.arg(i).toInt(), 1, 255);
    }

		WriteConfig();

    // The saved settings are now the live ones.
    syncLiveFromConfig();

    QTLed.begin();

    QTLed.setAutomaticBrightness(_config.brightnessAuto);
    if (!_config.brightnessAuto)
      QTLed.setBrightness(_config.brightness);

    QTLed.setColor(_config.color[0], _config.color[1], _config.color[2]);
    QTLed.setColorRandom((RandomColorMode)_config.colorRandom);

    QTLed.setLanguage(_config.language);
    QTLed.setMode(_config.mode);
    QTLed.setAnimation(_config.animation);

    //ESP.restart();
	}
  _server.sendHeader("Location", "/");
  _server.send(302);
	//Serial.println(__FUNCTION__); 	
}


void send_general_configuration_values_html()
{
	String values = "";
  values += "brightnessauto|" + (String)(_config.brightnessAuto ? "checked" : "") + "|chk\n";
  values += "brightness|" + (String)_config.brightness + "|input\n";
  values += "brightnessday|" + (String)_config.brightnessAutoMinDay + "|input\n";
  values += "brightnessnight|" + (String)_config.brightnessAutoMinNight + "|input\n";
  values += "brightnessmax|" + (String)_config.brightnessMax + "|input\n";
  values += "color|#" + dec2hex2(_config.color[0]) + dec2hex2(_config.color[1]) + dec2hex2(_config.color[2]) + "|input\n";
  values += "colorText|#" + dec2hex2(_config.color[0]) + dec2hex2(_config.color[1]) + dec2hex2(_config.color[2]) + "|input\n";
  values += "lang|" + (String)_config.language + "|input\n";
  values += "mode|" + (String)_config.mode + "|input\n";
  values += "animation|" + (String)_config.animation + "|input\n";
  values += "colorrandom|" + (String)_config.colorRandom + "|input\n";
  values += "ledconfig|" + (String)_config.ledConfig + "|input\n";
  values += "brightnesssensibility|" + (String)_config.luxSensitivity + "|input\n";
  values += "animspeed|" + (String)_config.animSpeed + "|input\n";
  values += "animbrightmin|" + (String)_config.animBrightnessMin + "|input\n";
  values += "animbrightmax|" + (String)_config.animBrightnessMax + "|input\n";

	_server.send(200, "text/plain", values);
	//Serial.println(__FUNCTION__); 
}

void send_general_ledconfig_values_html()
{
  cl_Lst<LedConfiguration *> *pl = QTLed.getLedConfigurationList();

  String values = "";
  for (int i = 0; i < pl->size(); i++)
    values += "ledconfig|" + (*pl)[i]->getName() + "|select\n";

  _server.send(200, "text/plain", values);
}

void send_general_langs_values_html()
{
  cl_Lst<TextTime *> *pl = QTLed.getLanguagesList();

  String values = "";
  for (int i = 0; i < pl->size(); i++)
    values += "lang|" + (*pl)[i]->getLanguage() + "|select\n";

  _server.send(200, "text/plain", values);
}

void send_general_modes_values_html()
{
  cl_Lst<LedStripMode *> *pl = QTLed.getModesList();

  String values = "";
  for (int i = 0; i < pl->size(); i++)
    values += "mode|" + (*pl)[i]->getName() + "|select\n";

  _server.send(200, "text/plain", values);
}

void send_general_animations_values_html()
{
  cl_Lst<LedStripAnimation *> *pl = QTLed.getAnimationsList();

  String values = "";
  for (int i = 0; i < pl->size(); i++)
    values += "animation|" + (*pl)[i]->getName() + "|select\n";

  _server.send(200, "text/plain", values);
}

void send_lang_value_html()
{
  _server.send(200, "text/plain", String(_config.language));
}

// Live preview. Everything here writes _live, never _config: dragging a slider
// is not a decision, and writing _config meant the next unrelated save quietly
// persisted whatever the slider happened to be on. /admin/save/general is what
// commits a choice.
void send_general_led()
{
  if (_server.args() > 0)
  {
    for (uint8_t i = 0; i < _server.args(); i++) {
      if (_server.argName(i) == "brightness")
      {
        int b = _server.arg(i).toInt();
        if (b < 0)
          QTLed.setAutomaticBrightness(true);
        else
        {
          QTLed.setAutomaticBrightness(false);
          QTLed.setBrightness(_server.arg(i).toInt());
        }
      }

      if (_server.argName(i) == "brightnessday")
        _live.brightnessAutoMinDay = _server.arg(i).toInt();

      if (_server.argName(i) == "brightnessnight")
        _live.brightnessAutoMinNight = _server.arg(i).toInt();

      if (_server.argName(i) == "brightnessmax")
        _live.brightnessMax = constrain(_server.arg(i).toInt(), 1, 255);

      if (_server.argName(i) == "lang")
      {
        QTLed.setLanguage(_server.arg(i).toInt());
      }

      if (_server.argName(i) == "mode")
      {
        QTLed.setMode(_server.arg(i).toInt());
      }

      if (_server.argName(i) == "color")
      {
        String colorStr = _server.arg(i);
        // Remove # if present
        if (colorStr.startsWith("#")) {
          colorStr = colorStr.substring(1);
        }
        int32_t l = strtol(colorStr.c_str(), 0, 16);
        byte r = (l >> 16) & 0xFF;
        byte g = (l >> 8) & 0xFF;
        byte b = (l >> 0) & 0xFF;

        QTLed.setColor(r, g, b);
      }

      if (_server.argName(i) == "animation")
      {
        QTLed.setAnimation(_server.arg(i).toInt());
      }

      if (_server.argName(i) == "colorrandom")
      {
        QTLed.setColorRandom((RandomColorMode)_server.arg(i).toInt());
      }
      if (_server.argName(i) == "brightnesssensibility")
        _live.luxSensitivity = constrain(_server.arg(i).toInt(), 1, 255);

      if (_server.argName(i) == "animspeed")
        QTLed.setAnimSpeed(constrain(_server.arg(i).toInt(), 1, 20));

      if (_server.argName(i) == "animbrightmin")
        _live.animBrightnessMin = constrain(_server.arg(i).toInt(), 0, 100);

      if (_server.argName(i) == "animbrightmax")
        _live.animBrightnessMax = constrain(_server.arg(i).toInt(), 0, 100);
    }

    if (_live.animBrightnessMax < 1) _live.animBrightnessMax = 1;
    if (_live.animBrightnessMin >= _live.animBrightnessMax)
      _live.animBrightnessMin = _live.animBrightnessMax - 1;
  }
  _server.send(200, "text/plain", "OK");
}
