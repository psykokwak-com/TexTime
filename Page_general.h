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
      if (_server.argName(i) == "brightnesssensibility") _config.luxSensitivity = _server.arg(i).toInt();
    }

		WriteConfig();

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
  values += "color|#" + dec2hex2(_config.color[0]) + dec2hex2(_config.color[1]) + dec2hex2(_config.color[2]) + "|input\n";
  values += "colorText|#" + dec2hex2(_config.color[0]) + dec2hex2(_config.color[1]) + dec2hex2(_config.color[2]) + "|input\n";
  values += "lang|" + (String)_config.language + "|input\n";
  values += "mode|" + (String)_config.mode + "|input\n";
  values += "animation|" + (String)_config.animation + "|input\n";
  values += "colorrandom|" + (String)_config.colorRandom + "|input\n";
  values += "ledconfig|" + (String)_config.ledConfig + "|input\n";
  values += "brightnesssensibility|" + (String)_config.luxSensitivity + "|input\n";

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
        _config.brightnessAutoMinDay = _server.arg(i).toInt();

      if (_server.argName(i) == "brightnessnight")
        _config.brightnessAutoMinNight = _server.arg(i).toInt();

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
      {
        _config.luxSensitivity = _server.arg(i).toInt();
      }
    }
  }
  _server.send(200, "text/plain", "OK");
}
