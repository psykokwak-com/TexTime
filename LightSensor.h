/**
* Ce programme permet de tester l'utilisation du module lux-mètre d'Adafruit.
* le module : http://www.adafruit.com/products/1384
* le datasheet du module : http://www.adafruit.com/datasheets/GA1A1S202WP_Spec.pdf
* Code source sous licence GNU GPL 2.0.
* Ce fichier fait partie du projet Milapli : https://github.com/sarinkhan/Milapli
*/

#define ALS_PIN A0
#define ALS_AVG_SIZE 200

int _avg[ALS_AVG_SIZE];
int _als = 0;

int getALSLux()
{
  /*
  double u = als;    // volts
  double i = 0.0;    // amp
  double l = 0.0;    // lux

  u = u / 1024.0;    // U in volts between 0 and 1 Volt
  i = u / 1960.0;    // i in amps with I = U / R, with R = 2000 ohm
  l = i * 2000000.0; // l in lux according the ALS data sheet

  return l;
  */
  return _als;
}

void updateAvgLux()
{
  static int i = 0;

  _avg[i++] = getALSLux();
  if (i == ALS_AVG_SIZE) i = 0;
}

int getAvgLux()
{

  //return getALSLux();

  //////////////////////////////////////////////////////////////////////////

  /*
  return random(-300, 800);

  //////////////////////////////////////////////////////////////////////////

  static int v = 0;
  static int way = 10;

  v = v + way;

  if (v >= 600) way = -10;
  if (v <= -200) way = 10;

  return v;
  
  //////////////////////////////////////////////////////////////////////////
  */

  int sum = 0;

  for (int i = 0; i < ALS_AVG_SIZE; i++)
    sum += _avg[i];

  if (sum < 0.0) sum = 0;

  return sum / ALS_AVG_SIZE;
}

void handleAmbiantLightSensor()
{
  static uint64_t pt = 0;

  uint64_t ct = millis64() / 10; // Read ADC every 10ms because of Wifi Bug (https://github.com/krzychb/EspScopeA0/tree/master/Bravo)

  if (ct == pt)
    return ;

  pt = ct;

  yield();
  _als = analogRead(ALS_PIN);
  yield();

  updateAvgLux();
}