//
// Led Strip configuration for TexTime
//

#include "fonts.h"
#include "textime.h"

#define NROW 10
#define NCOL 11
#define NEDGE 4

#define pVOID  Pixel()
#define pBLACK Pixel(RgbColor(0  ,   0,   0))
#define pRED   Pixel(RgbColor(255,   0,   0))
#define pGREEN Pixel(RgbColor(0  , 255,   0))
#define pBLUE  Pixel(RgbColor(0  , 0  , 255))
#define pWHITE Pixel(RgbColor(255, 255, 255))


class Pixel
{
public:
  RgbColor color;
  bool display;

  Pixel()
    : color(RgbColor(0, 0, 0))
    , display(false)
  {
  }

  Pixel(const RgbColor &c)
  {
    color = c;
    display = true;
  }
};

class PixelsArray {
private:
  Pixel _pixels[NROW][NCOL];

public:
  PixelsArray()
  {
    clear();
  }

  void fill(const Pixel &p)
  {
    for (int i = 0; i < NROW; i++)
    {
      for (int j = 0; j < NCOL; j++)
      {
        _pixels[i][j] = p;
      }
    }
  }

  void clear()
  {
    fill(pVOID);
  }

  void setPixel(const Pixel &p, int row, int col)
  {
    if (row < 0) return;
    if (col < 0) return;
    if (row > NROW - 1) return;
    if (col > NCOL - 1) return;

    _pixels[row][col] = p;
  }

  Pixel getPixel(int row, int col)
  {
    if (row < 0) return pVOID;
    if (col < 0) return pVOID;
    if (row > NROW - 1) return pVOID;
    if (col > NCOL - 1) return pVOID;

    return _pixels[row][col];
  }
};

struct PixelsContainer
{
  PixelsArray pixelsArray;
  Pixel pixelsEdge[NEDGE];
  bool hasChanged;
};



class LedConfiguration {
public:
  virtual int ledsByPixelForMatrix() = 0;
  virtual int ledsByPixelForEdges() = 0;
  virtual int ledsNumber() = 0;
  virtual String getName() = 0;
  virtual const uint8_t *getLedsMatrixId(int row, int col) = 0;
  virtual const uint8_t *getLedsEdgeId(int n) = 0;
  virtual ~LedConfiguration() {}
};

class LedConfiguration40x40 : public LedConfiguration {
private:
  const uint8_t _matchingPixelsMatrix[NROW][NCOL][1] = {
    { { 0  }, { 1  }, { 5  }, { 6  }, { 14 }, { 15 }, { 27 }, { 28  }, { 44  }, { 45  }, { 64  } },
    { { 2  }, { 4  }, { 7  }, { 13 }, { 16 }, { 26 }, { 29 }, { 43  }, { 46  }, { 63  }, { 65  } },
    { { 3  }, { 8  }, { 12 }, { 17 }, { 25 }, { 30 }, { 42 }, { 47  }, { 62  }, { 66  }, { 81  } },
    { { 9  }, { 11 }, { 18 }, { 24 }, { 31 }, { 41 }, { 48 }, { 61  }, { 67  }, { 80  }, { 82  } },
    { { 10 }, { 19 }, { 23 }, { 32 }, { 40 }, { 49 }, { 60 }, { 68  }, { 79  }, { 83  }, { 94  } },
    { { 20 }, { 22 }, { 33 }, { 39 }, { 50 }, { 59 }, { 69 }, { 78  }, { 84  }, { 93  }, { 95  } },
    { { 21 }, { 34 }, { 38 }, { 51 }, { 58 }, { 70 }, { 77 }, { 85  }, { 92  }, { 96  }, { 103 } },
    { { 35 }, { 37 }, { 52 }, { 57 }, { 71 }, { 76 }, { 86 }, { 91  }, { 97  }, { 102 }, { 104 } },
    { { 36 }, { 53 }, { 56 }, { 72 }, { 75 }, { 87 }, { 90 }, { 98  }, { 101 }, { 105 }, { 108 } },
    { { 54 }, { 55 }, { 73 }, { 74 }, { 88 }, { 89 }, { 99 }, { 100 }, { 106 }, { 107 }, { 109 } }
  };
  const uint8_t _matchingPixelsEdge[NEDGE][1] = { { 112 }, { 111 }, { 110 }, { 113 } };

public:
  virtual String getName()
  {
    return "40x40@1";
  }

  int ledsByPixelForMatrix()
  {
    return 1;
  }

  int ledsByPixelForEdges()
  {
    return 1;
  }

  int ledsNumber()
  {
    return (NROW * NCOL) + NEDGE;
  }

  const uint8_t *getLedsMatrixId(int row, int col)
  {
    if (row < 0) return NULL;
    if (col < 0) return NULL;
    if (row > NROW - 1) return NULL;
    if (col > NCOL - 1) return NULL;

    return _matchingPixelsMatrix[row][col];
  }

  const uint8_t *getLedsEdgeId(int n)
  {
    if (n < 0) return NULL;
    if (n > NEDGE - 1) return NULL;

    return _matchingPixelsEdge[n];
  }
};

class LedConfiguration100x100_1 : public LedConfiguration {
private:
  const uint8_t _matchingPixelsMatrix[NROW][NCOL][1] = {
    { { 21  }, { 19  }, { 17  }, { 15  }, { 13  }, { 11  }, { 9   }, { 7   }, { 5   }, { 3   }, { 1   } },
    { { 24  }, { 26  }, { 28  }, { 30  }, { 32  }, { 34  }, { 36  }, { 38  }, { 40  }, { 42  }, { 44  } },
    { { 67  }, { 65  }, { 63  }, { 61  }, { 59  }, { 57  }, { 55  }, { 53  }, { 51  }, { 49  }, { 47  } },
    { { 70  }, { 72  }, { 74  }, { 76  }, { 78  }, { 80  }, { 82  }, { 84  }, { 86  }, { 88  }, { 90  } },
    { { 113 }, { 111 }, { 109 }, { 107 }, { 105 }, { 103 }, { 101 }, { 99 }, { 97  }, { 95  }, { 93  } },
    { { 116 }, { 118 }, { 120 }, { 122 }, { 124 }, { 126 }, { 128 }, { 130 }, { 132 }, { 134 }, { 136 } },
    { { 159 }, { 157 }, { 155 }, { 153 }, { 151 }, { 149 }, { 147 }, { 145 }, { 143 }, { 141 }, { 139 } },
    { { 162 }, { 164 }, { 166 }, { 168 }, { 170 }, { 172 }, { 174 }, { 176 }, { 178 }, { 180 }, { 182 } },
    { { 205 }, { 203 }, { 201 }, { 199 }, { 197 }, { 195 }, { 193 }, { 191 }, { 189 }, { 187 }, { 185 } },
    { { 208 }, { 210 }, { 212 }, { 214 }, { 216 }, { 218 }, { 220 }, { 222 }, { 224 }, { 226 }, { 228 } }
  };
  const uint8_t _matchingPixelsEdge[NEDGE][1] = { { 232 }, { 231 }, { 230 }, { 233 } };

public:
  virtual String getName()
  {
    return "100x100@1";
  }

  int ledsByPixelForMatrix()
  {
    return 1;
  }

  int ledsByPixelForEdges()
  {
    return 1;
  }

  int ledsNumber()
  {
    return (NROW * 23) + NEDGE;
  }

  const uint8_t *getLedsMatrixId(int row, int col)
  {
    if (row < 0) return NULL;
    if (col < 0) return NULL;
    if (row > NROW - 1) return NULL;
    if (col > NCOL - 1) return NULL;

    return _matchingPixelsMatrix[row][col];
  }

  const uint8_t *getLedsEdgeId(int n)
  {
    if (n < 0) return NULL;
    if (n > NEDGE - 1) return NULL;

    return _matchingPixelsEdge[n];
  }
};

class LedConfiguration100x100_2 : public LedConfiguration {
private:
  const uint8_t _matchingPixelsMatrix[NROW][NCOL][2] = {
    { { 21 , 22  }, { 19 , 20  }, { 17 , 18  }, { 15 , 16  }, { 13 , 14  }, { 11 , 12  }, { 9  , 10  }, { 7  , 8   }, { 5  , 6   }, { 3  , 4   }, { 1  , 2   } },
    { { 25 , 26  }, { 27 , 28  }, { 29 , 30  }, { 31 , 32  }, { 33 , 34  }, { 35 , 36  }, { 37 , 38  }, { 39 , 40  }, { 41 , 42  }, { 43 , 44  }, { 45 , 46  } },
    { { 69 , 70  }, { 67 , 68  }, { 65 , 66  }, { 63 , 64  }, { 61 , 62  }, { 59 , 60  }, { 57 , 58  }, { 55 , 56  }, { 53 , 54  }, { 51 , 52  }, { 49 , 50  } },
    { { 73 , 74  }, { 75 , 76  }, { 77 , 78  }, { 79 , 80  }, { 81 , 82  }, { 83 , 84  }, { 85 , 86  }, { 87 , 88  }, { 89 , 90  }, { 91 , 92  }, { 93 , 94  } },
    { { 117, 118 }, { 115, 116 }, { 113, 114 }, { 111, 112 }, { 109, 110 }, { 107, 108 }, { 105, 106 }, { 103, 104 }, { 101, 102 }, { 99 , 100 }, { 97 , 98  } },
    { { 121, 122 }, { 123, 124 }, { 125, 126 }, { 127, 128 }, { 129, 130 }, { 131, 132 }, { 133, 134 }, { 135, 136 }, { 137, 138 }, { 139, 140 }, { 141, 142 } },
    { { 165, 166 }, { 163, 164 }, { 161, 162 }, { 159, 160 }, { 157, 158 }, { 155, 156 }, { 153, 154 }, { 151, 152 }, { 149, 150 }, { 147, 148 }, { 145, 146 } },
    { { 169, 170 }, { 171, 172 }, { 173, 174 }, { 175, 176 }, { 177, 178 }, { 179, 180 }, { 181, 182 }, { 183, 184 }, { 185, 186 }, { 187, 188 }, { 189, 190 } },
    { { 213, 214 }, { 211, 212 }, { 209, 210 }, { 207, 208 }, { 205, 206 }, { 203, 204 }, { 201, 202 }, { 199, 200 }, { 197, 198 }, { 195, 196 }, { 193, 194 } },
    { { 217, 218 }, { 219, 220 }, { 221, 222 }, { 223, 224 }, { 225, 226 }, { 227, 228 }, { 229, 230 }, { 231, 232 }, { 233, 234 }, { 235, 236 }, { 237, 238 } }
  };
  const uint8_t _matchingPixelsEdge[NEDGE][1] = { { 242 }, { 241 }, { 240 }, { 243 } };

public:
  virtual String getName()
  {
    return "100x100@2";
  }

  int ledsByPixelForMatrix()
  {
    return 2;
  }

  int ledsByPixelForEdges()
  {
    return 1;
  }

  int ledsNumber()
  {
    return (NROW * 24) + NEDGE;
  }

  const uint8_t *getLedsMatrixId(int row, int col)
  {
    if (row < 0) return NULL;
    if (col < 0) return NULL;
    if (row > NROW - 1) return NULL;
    if (col > NCOL - 1) return NULL;

    return _matchingPixelsMatrix[row][col];
  }

  const uint8_t *getLedsEdgeId(int n)
  {
    if (n < 0) return NULL;
    if (n > NEDGE - 1) return NULL;

    return _matchingPixelsEdge[n];
  }
};



//  0         x
// 0+----------
//  |
//  |
//  |
// y|  
//
void copyCharToMatrix(const uint8_t src[FONTROW][FONTCOL], PixelsArray &dst, int posx, int posy, const RgbColor &color)
{
  for (int r = 0; r < FONTROW; r++)
  {
    for (int c = 0; c < FONTCOL; c++)
    {
      if (src[r][c])
        dst.setPixel(Pixel(color), posx + r, posy + c);
    }
  }
}

// Display number [-9:99]
void copyNumberToMatrix(int n, PixelsArray &dst, const RgbColor &color)
{
  int x = 1;

  if (n < 0)
    ::copyCharToMatrix(_font_minus, dst, x, 0, color); // Display "-"
  else
  {
    if (n / 10 != 0)
      ::copyCharToMatrix(_font_number[abs(n / 10)], dst, x, 0, color); // Display "First number"
    else
      ::copyCharToMatrix(_font_number[0], dst, x, 0, color); // Display "First number"
  }
  ::copyCharToMatrix(_font_number[abs(n % 10)], dst, x, 0 + 6, color); // Display "Second number"
}


enum RandomColorMode
{
  ColorRandomNo = 0,
  ColorRandomAll,
  ColorRandomLetter,
  ColorRandomWord
};


class LedStripMode
{
protected:
  String _name;
  PixelsContainer *_pPixelContainer;
  RgbColor _color;
  RandomColorMode _colorRandomMode;

  void setPixelsColor(const Pixel &p)
  {
    _pPixelContainer->pixelsArray.fill(p);
    for (int i = 0; i < NEDGE; i++)
      _pPixelContainer->pixelsEdge[i] = p;
  }

  void clearPixelsColor()
  {
    // Clear pixels
    setPixelsColor(pVOID);
  }
  
public:
  LedStripMode(String name, PixelsContainer *pPixelContainer)
    : _name(name)
    , _pPixelContainer(pPixelContainer)
    , _color(RgbColor(255, 255, 255))
    , _colorRandomMode(ColorRandomNo)
  {
  }

  String getName()
  {
    return _name;
  }

  void setColor(RgbColor c)
  {
    _color = c;
  }

  RgbColor getColor()
  {
    return _color;
  }

  void setColorRandom(RandomColorMode c)
  {
    _colorRandomMode = c;
  }

  virtual void begin() = 0;
  virtual void handle() = 0;
};

class LedStripModeNothing : public LedStripMode
{
public:
  LedStripModeNothing(PixelsContainer *pPixelContainer)
    : LedStripMode("Nothing", pPixelContainer)
  {
  }

  void begin()
  {
    // Clear display
    clearPixelsColor();

    _pPixelContainer->hasChanged = true;
  }

  void handle()
  {
  }
};

class LedStripModeTime : public LedStripMode
{
private:
  TextTimeFr _textime;
  int _m;
  int _h;

public:
  LedStripModeTime(PixelsContainer *pPixelContainer)
    : LedStripMode("Time", pPixelContainer)
    , _m(-1)
    , _h(-1)
  {
  }

  void begin()
  {
    _m = -1;
    _h = -1;
  }

  void handle()
  {
    int h = _dateTime.hour;
    int m = _dateTime.minute;

    if (_m == m && _h == h)
      return;

    _m = m;
    _h = h;

    TextTimeBlobs b = _textime.getBlobsFromTime(h, m);

    if (!b.number)
      return; // TODO: Display something useful

    // Clear display
    clearPixelsColor();

    RgbColor c = _color;

    if (_colorRandomMode == ColorRandomAll)
      c = HslColor((float)random(256) / 255.0, 1.0, 0.5);

    for (int i = 0; i < b.number; i++)
    {
      if (_colorRandomMode == ColorRandomWord)
        c = HslColor((float)random(256) / 255.0, 1.0, 0.5);

      for (int j = 0; j < b.blobs[i]->number; j++)
      {
        if (_colorRandomMode == ColorRandomLetter)
          c = HslColor((float)random(256) / 255.0, 1.0, 0.5);

        Pixel p;
        p.color = c;
        p.display = true;
        _pPixelContainer->pixelsArray.setPixel(p, b.blobs[i]->pixels[j].row, b.blobs[i]->pixels[j].col);
      }
    }

    if (_colorRandomMode == ColorRandomWord)
      c = HslColor((float)random(256) / 255.0, 1.0, 0.5);

    for (int i = 0; i < m % 5; i++)
    {
      if (_colorRandomMode == ColorRandomLetter)
        c = HslColor((float)random(256) / 255.0, 1.0, 0.5);

      Pixel p;
      p.color = c;
      p.display = true;
      _pPixelContainer->pixelsEdge[i] = p;
    }

    _pPixelContainer->hasChanged = true;
  }
};


class LedStripModeSeconds : public LedStripMode
{
private:
  int _s;

public:
  LedStripModeSeconds(PixelsContainer *pPixelContainer)
    : LedStripMode("Seconds", pPixelContainer)
    , _s(-1)
  {
  }

  void begin()
  {
    _s = -1;
  }

  void handle()
  {
    if (_s == (int)_dateTime.second)
      return;

    _s = _dateTime.second;

    // Clear display
    clearPixelsColor();

    ::copyNumberToMatrix(_s, _pPixelContainer->pixelsArray, _color);

    _pPixelContainer->hasChanged = true;
  }
};

class LedStripModeDay : public LedStripMode
{
private:
  int _s;

public:
  LedStripModeDay(PixelsContainer *pPixelContainer)
    : LedStripMode("Day", pPixelContainer)
    , _s(-1)
  {
  }

  void begin()
  {
    _s = -1;
  }

  void handle()
  {
    if (_s == (int)_dateTime.day)
      return;

    _s = _dateTime.day;

    // Clear display
    clearPixelsColor();

    ::copyNumberToMatrix(_s, _pPixelContainer->pixelsArray, _color);

    _pPixelContainer->hasChanged = true;
  }
};


class LedStripModeTemperature : public LedStripMode
{
private:
  int _s;

public:
  LedStripModeTemperature(PixelsContainer *pPixelContainer)
    : LedStripMode("Temperature", pPixelContainer)
    , _s(-1)
  {
  }

  void begin()
  {
    _s = -1;
  }

  void handle()
  {
    if (_s == (int)_dateTime.second)
      return;

    _s = _dateTime.second;

    // Clear display
    clearPixelsColor();

    // Display Temperature
    if (!RTC.GetIsRunning())
      return; // TODO: Display something useful

    int8_t t = RTC.GetTemperature().AsFloatDegC();

    ::copyNumberToMatrix(t, _pPixelContainer->pixelsArray, _color);

    _pPixelContainer->hasChanged = true;
  }
};

class LedStripModeTestColors : public LedStripMode
{
private:
  int _t;

public:
  LedStripModeTestColors(PixelsContainer *pPixelContainer)
    : LedStripMode("Test Colors", pPixelContainer)
    , _t(0)
  {
  }

  void begin()
  {
    _t = 0;
  }

  void handle()
  {
    switch (_t)
    {
    case 0:
      setPixelsColor(pRED);
      _pPixelContainer->hasChanged = true;
      break;
    case 100000:
      setPixelsColor(pGREEN);
      _pPixelContainer->hasChanged = true;
      break;
    case 200000:
      setPixelsColor(pBLUE);
      _pPixelContainer->hasChanged = true;
      break;
    case 300000:
      setPixelsColor(pWHITE);
      _pPixelContainer->hasChanged = true;
      break;
    case 400000:
      _t = -1;
      break;
    }
    _t++;
  }
};

class LedStripModeTestSpeed : public LedStripMode
{
private:
  int _r;
  int _c;

public:
  LedStripModeTestSpeed(PixelsContainer *pPixelContainer)
    : LedStripMode("Test Speed", pPixelContainer)
    , _r(0)
    , _c(0)
  {
  }

  void begin()
  {
    _r = 0;
    _c = 0;
  }

  void handle()
  {
    if (_pPixelContainer->hasChanged)
      return;

    if (_c == NCOL) {
      _c = 0;
      _r++;
    }

    if (_r == NROW) {
      _c = 0;
      _r = 0;
    }

    clearPixelsColor();
    _pPixelContainer->pixelsArray.setPixel(pWHITE, _r, _c++);

    _pPixelContainer->hasChanged = true;
  }
};



#define NeoEsp8266Method NeoEsp8266AsyncUart1Ws2813Method
class MyLedStrip
{
protected:
  NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp8266Method> *_pStrip;
  cl_Lst<LedConfiguration *> _ledConfiguration;
  int _ledConfigurationIndex;
  PixelsContainer _pixels;
  bool _automaticBrightness;
  cl_Lst<LedStripMode *> _modeList;
  int _modeIndex;

  bool refresh(PixelsContainer *pPixel)
  {
    if (!pPixel->hasChanged)
      return false;

    if (!_pStrip->CanShow())
      return false;

    // Reset led strip
    _pStrip->ClearTo(RgbColor(0, 0, 0));

    // Fill leds strip with matrix pixels
    for (int r = 0; r < NROW; r++) {
      for (int c = 0; c < NCOL; c++) {
        Pixel p = pPixel->pixelsArray.getPixel(r, c);
        const uint8_t *i = _ledConfiguration[_ledConfigurationIndex]->getLedsMatrixId(r, c);

        if (!p.display) continue;

        for (int l = 0; l < _ledConfiguration[_ledConfigurationIndex]->ledsByPixelForMatrix(); l++)
          _pStrip->SetPixelColor(i[l], p.color);
      }
    }

    // Fill leds strip with edge pixels
    for (int e = 0; e < NEDGE; e++) {
      Pixel p = pPixel->pixelsEdge[e];
      const uint8_t *i = _ledConfiguration[_ledConfigurationIndex]->getLedsEdgeId(e);

      if (!p.display) continue;

      for (int l = 0; l < _ledConfiguration[_ledConfigurationIndex]->ledsByPixelForEdges(); l++)
        _pStrip->SetPixelColor(i[l], p.color);
    }

    // Refresh display
    _pStrip->Show();

    pPixel->hasChanged = false;

    return true;
  }

  // Update brightness every 50ms
  void handleAutomaticBrightness()
  {
    static uint64_t p = 0;

    if (!_automaticBrightness)
      return;

    uint64_t v = millis64() / 50;

    if (v != p)
    {
      int sd = _config.brightnessAutoMinDay;      // minimum brightness during the day
      int sn = _config.brightnessAutoMinNight;    // minimum brightness during the night
      int sm = 255;                               // maximum brightness
      int lmin = 0;                               // minimum lux sensitivity allowed
      int lmax = _config.luxSensitivity * 10;     // maximum lux sensitivity allowed

      int s = sn;

      if (_dateTime.hour > 21 && _dateTime.hour < 9) s = sn; // between 22h and 9h
      if (_dateTime.hour > 9 && _dateTime.hour < 21) s = sd; // between 10h and 21h

      if (_dateTime.hour == 21) s = map(_dateTime.minute, 0, 59, sd, sn); // during the 21th hour
      if (_dateTime.hour == 9) s = map(_dateTime.minute, 0, 59, sn, sd); // during the 9th hour

      if (s == 0) s = 1;

      int l = getAvgLux();
      if (l < lmin) l = lmin;
      if (l > lmax) l = lmax;
      _pStrip->SetBrightness(map(l, lmin, lmax, s, sm)); // limit

      if (_pStrip->CanShow())
        _pStrip->Show();
    }

    p = v;
  }

  void handleMode()
  {
    if (_modeIndex < 0) return;
    if (_modeIndex > _modeList.size() - 1) return;

    _modeList[_modeIndex]->handle();
  }

public:
  MyLedStrip()
    : _pStrip(NULL)
    , _ledConfigurationIndex(0)
    , _automaticBrightness(false)
    , _modeIndex(0)
  {

    _ledConfiguration.push_back(new LedConfiguration40x40());
    _ledConfiguration.push_back(new LedConfiguration100x100_1());
    _ledConfiguration.push_back(new LedConfiguration100x100_2());

    _modeList.push_back(new LedStripModeNothing(&_pixels));
    _modeList.push_back(new LedStripModeTime(&_pixels));
    _modeList.push_back(new LedStripModeSeconds(&_pixels));
    _modeList.push_back(new LedStripModeDay(&_pixels));
    _modeList.push_back(new LedStripModeTemperature(&_pixels));
    _modeList.push_back(new LedStripModeTestColors(&_pixels));
    _modeList.push_back(new LedStripModeTestSpeed(&_pixels));
  }

  ~MyLedStrip()
  {
    end();

    _modeList.clear();
  }

  cl_Lst<LedStripMode *> *getModesList()
  {
    return &_modeList;
  }

  cl_Lst<LedConfiguration *> *getLedConfigurationList()
  {
    return &_ledConfiguration;
  }

  void begin()
  {
    end();

    if (!_pStrip)
    { 
      _ledConfigurationIndex = _config.ledConfig;
      
      // Cannot use DMA because DMA GPIO is already used by serial/USB bridge :(
      _pStrip = new NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp8266Method>(_ledConfiguration[_ledConfigurationIndex]->ledsNumber(), D4);
      _pStrip->Begin();
    }

    _pStrip->ClearTo(RgbColor(0, 0, 0));
    _pStrip->Show();
  }

  void end()
  {
    if (_pStrip)
      delete _pStrip;

    _pStrip = NULL;
  }

  void setAutomaticBrightness(int b)
  {
    _automaticBrightness = b;
  }

  void setBrightness(uint8_t b)
  {
    if (b < 1) b = 1;
    if (b > 255) b = 255;

    if (_automaticBrightness)
      return;

    _pStrip->SetBrightness(b);

    if (_pStrip->CanShow())
      _pStrip->Show();

    //refresh();
  }

  uint8_t getBrightness()
  {
    return _pStrip->GetBrightness();
  }

  void setColor(byte r, byte g, byte b)
  {
    for (int i = 0; i < _modeList.size(); i++)
      _modeList[i]->setColor(RgbColor(r, g, b));

    // Force redrawing to update the color now
    setMode(_modeIndex);

    // Update MQTT clients
    String sr = ("0" + String((int)r, HEX)); sr = sr.substring(sr.length() - 2);
    String sg = ("0" + String((int)g, HEX)); sg = sg.substring(sg.length() - 2);
    String sb = ("0" + String((int)b, HEX)); sb = sb.substring(sb.length() - 2);
    String s = "#" + (sr + sg + sb);
    s.toUpperCase();

    _mqtt.publish(mqttTopicPubLedColor.topic().c_str(), s.c_str());
  }

  void getColor(byte &r, byte &g, byte &b)
  {
    RgbColor c = _modeList[_modeIndex]->getColor();
    r = c.R;
    g = c.G;
    b = c.B;
  }

  void setColorRandom(RandomColorMode c)
  {
    for (int i = 0; i < _modeList.size(); i++)
      _modeList[i]->setColorRandom(c);

    // Force redrawing to update the color now
    setMode(_modeIndex);
  }

  bool setMode(int mode)
  {
    if (mode < 0) return false;
    if (mode > _modeList.size() - 1) return false;

    _modeIndex = mode;

    _modeList[_modeIndex]->begin();

    _mqtt.publish(mqttTopicPubLedMode.topic().c_str(), String(mode).c_str());

    return true;
  }

  int getModeIndex()
  {
    return _modeIndex;
  }

  void handle()
  {
    handleAutomaticBrightness();
    handleMode();
    refresh(&_pixels);
  }
};




class LedStripAnimation
{
protected:
  struct PixelPos
  {
    int r;
    int c;
    int e;
    Pixel p;
  };

protected:
  String _name;
  PixelsContainer *_pPixelContainerInput;
  PixelsContainer *_pPixelContainerOutput;

  void setPixelsColor(const Pixel &p)
  {
    _pPixelContainerOutput->pixelsArray.fill(p);
    for (int i = 0; i < NEDGE; i++)
      _pPixelContainerOutput->pixelsEdge[i] = p;
  }

  void clearPixelsColor()
  {
    // Clear pixels
    setPixelsColor(pVOID);
  }

public:
  LedStripAnimation(String name, PixelsContainer *pPixelContainerInput, PixelsContainer *pPixelContainerOutput)
    : _name(name)
    , _pPixelContainerInput(pPixelContainerInput)
    , _pPixelContainerOutput(pPixelContainerOutput)
  {
  }

  String getName()
  {
    return _name;
  }

  virtual void begin() = 0;
  virtual void handle() = 0;
};

class LedStripAnimationNormal : public LedStripAnimation
{
public:
  LedStripAnimationNormal(PixelsContainer *pPixelContainerInput, PixelsContainer *pPixelContainerOutput)
    : LedStripAnimation("Normal", pPixelContainerInput, pPixelContainerOutput)
  {
  }

  void begin()
  {
    // Force refresh of input pixels because
    // we copy them to the output buffer only
    // When they change. (To limit CPU usage)
    _pPixelContainerInput->hasChanged = true;
  }

  void handle()
  { // Just copy input pixels to output pixels if they change
    if (_pPixelContainerInput->hasChanged)
      *_pPixelContainerOutput = *_pPixelContainerInput;
  }
};

class LedStripAnimationBlink : public LedStripAnimation
{
private:
  Frame _frame;
  cl_Lst<PixelPos> _pixelPosition;

  void initPixelsList()
  {
    // Clear the pixel list
    _pixelPosition.clear();

    // Fill on pixels from matrix to the list
    for (int c = 0; c < NCOL; c++) {
      for (int r = 0; r < NROW; r++) {
        Pixel p = _pPixelContainerInput->pixelsArray.getPixel(r, c);
        if (p.display) {
          PixelPos pp;
          pp.e = -1;
          pp.c = c;
          pp.r = r;
          pp.p = p;
          _pixelPosition.push_back(pp);
        }
      }
    }

    // Fill on pixels from edge to the list
    for (int e = 0; e < NEDGE; e++) {
      Pixel p = _pPixelContainerInput->pixelsEdge[e];
      if (p.display) {
        PixelPos pp;
        pp.e = e;
        pp.c = -1;
        pp.r = -1;
        pp.p = p;
        _pixelPosition.push_back(pp);
      }
    }
  }

public:
  LedStripAnimationBlink(PixelsContainer *pPixelContainerInput, PixelsContainer *pPixelContainerOutput)
    : LedStripAnimation("Blink", pPixelContainerInput, pPixelContainerOutput)
  {
  }

  void begin()
  {
    _frame.init(50);

    initPixelsList();
    clearPixelsColor();
  }

  void handle()
  {
    // If display has changed, reset the animation
    if (_pPixelContainerInput->hasChanged)
      begin();

    if (!_frame.next())
      return;

    if (!_pixelPosition.size())
      return;

    int idx = random(_pixelPosition.size());
    PixelPos pp = _pixelPosition[idx];
    _pixelPosition.remove(idx);

    if (pp.e == -1)
      _pPixelContainerOutput->pixelsArray.setPixel(pp.p, pp.r, pp.c);
    else
      _pPixelContainerOutput->pixelsEdge[pp.e] = pp.p;

    _pPixelContainerOutput->hasChanged = true;
  }
};

class LedStripAnimationFire : public LedStripAnimation
{
private:
  Frame _frame;

  RgbColor generateFireColor()
  {
    RgbColor c = RgbColor(58, 58, 6);;
    c.Darken(random(15));
    c.R += random(15);
    return c;
  }

public:
  LedStripAnimationFire(PixelsContainer *pPixelContainerInput, PixelsContainer *pPixelContainerOutput)
    : LedStripAnimation("Fire", pPixelContainerInput, pPixelContainerOutput)
  {
  }

  void begin()
  {
    _frame.init(8);
  }

  void handle()
  {
    if (!_frame.next())
      return;

    for (int c = 0; c < NCOL; c++) {
      for (int r = 0; r < NROW; r++) {
        Pixel pb;
        pb.color = generateFireColor();
        pb.display = true;

        Pixel pf = _pPixelContainerInput->pixelsArray.getPixel(r, c);

        _pPixelContainerOutput->pixelsArray.setPixel(pf.display ? pf : pb, r, c);
      }
    }

    for (int e = 0; e < NEDGE; e++) {
      Pixel pb;
      pb.color = generateFireColor();
      pb.display = true;

      Pixel pf = _pPixelContainerInput->pixelsEdge[e];

      _pPixelContainerOutput->pixelsEdge[e] = pf.display ? pf : pb;
    }

    _pPixelContainerOutput->hasChanged = true;
  }
};

class LedStripAnimationMatrix : public LedStripAnimation
{
private:
  Frame _frame;
  int _matrixColumn[NCOL];
  int _matrixColumnSize;

public:
  LedStripAnimationMatrix(PixelsContainer *pPixelContainerInput, PixelsContainer *pPixelContainerOutput)
    : LedStripAnimation("Matrix", pPixelContainerInput, pPixelContainerOutput)
    , _matrixColumnSize(9)
  {
  }

  void begin()
  {
    _frame.init(8);

    for (int i = 0; i < NCOL; i++)
      _matrixColumn[i] = -1;
  }

  void handle()
  {
    if (!_frame.next())
      return;

    clearPixelsColor();

    // Copy background matrix pixels

    // Create a new column if possible (= -1)
    for (int c = 0; c < NCOL; c++) {
      if (_matrixColumn[c] == -1) {
        if (random(30) == 0) {
          _matrixColumn[c] = 0;
        }
      }
    }

    // Update display columns
    for (int c = 0; c < NCOL; c++) {
      if (_matrixColumn[c] == -1)
        continue;

      Pixel green = pGREEN;
      for (int r = _matrixColumn[c]; r > _matrixColumn[c] - _matrixColumnSize; r--) {
        _pPixelContainerOutput->pixelsArray.setPixel(green, r, c);
        green.color.Darken(30);
      }

      _matrixColumn[c]++;

      if (_matrixColumn[c] > NROW + _matrixColumnSize)
        _matrixColumn[c] = -1;
    }

    // Copy foreground matrix pixels
    for (int c = 0; c < NCOL; c++) {
      for (int r = 0; r < NROW; r++) {
        Pixel pf = _pPixelContainerInput->pixelsArray.getPixel(r, c);
        if (pf.display)
          _pPixelContainerOutput->pixelsArray.setPixel(pf, r, c);
      }
    }

    // Copy foreground edge pixels
    for (int e = 0; e < NEDGE; e++) {
      Pixel pf = _pPixelContainerInput->pixelsEdge[e];
      if (pf.display)
        _pPixelContainerOutput->pixelsEdge[e] = pf;
    }

    _pPixelContainerOutput->hasChanged = true;
  }
};

class LedStripAnimationRainbow : public LedStripAnimation
{
private:
  Frame _frame;
  double _rainbowIndex;

public:
  LedStripAnimationRainbow(PixelsContainer *pPixelContainerInput, PixelsContainer *pPixelContainerOutput)
    : LedStripAnimation("Rainbow", pPixelContainerInput, pPixelContainerOutput)
    , _rainbowIndex(0)
  {
  }

  void begin()
  {
    _frame.init(10);
  }

  void handle()
  {
    if (!_frame.next())
      return;

    clearPixelsColor();

    _rainbowIndex += 0.005;

    if (_rainbowIndex > 1.0)
      _rainbowIndex = 0.0;

    // Copy foreground matrix pixels
    for (int c = 0; c < NCOL; c++) {
      for (int r = 0; r < NROW; r++) {

        double hsl = ((double)((r * NCOL) + c) / (double)(NROW * NCOL)) * (120.0 / 360.0);
        hsl += _rainbowIndex;
        if (hsl > 1.0) hsl -= 1.0;

        Pixel pf = _pPixelContainerInput->pixelsArray.getPixel(r, c);
        pf.color = HslColor(hsl, 1.0, 0.5);

        if (pf.display)
          _pPixelContainerOutput->pixelsArray.setPixel(pf, r, c);

        if (c == 0 && r == 0 && _pPixelContainerInput->pixelsEdge[0].display)
          _pPixelContainerOutput->pixelsEdge[0] = Pixel(pf.color);

        if (c == NCOL - 1 && r == 0 && _pPixelContainerInput->pixelsEdge[1].display)
          _pPixelContainerOutput->pixelsEdge[1] = Pixel(pf.color);;

        if (c == NCOL - 1 && r == NROW - 1 && _pPixelContainerInput->pixelsEdge[2].display)
          _pPixelContainerOutput->pixelsEdge[2] = Pixel(pf.color);;

        if (c == 0 && r == NROW - 1 && _pPixelContainerInput->pixelsEdge[3].display)
          _pPixelContainerOutput->pixelsEdge[3] = Pixel(pf.color);;
      }
    }

    _pPixelContainerOutput->hasChanged = true;
  }
};


class MyLedStripAnimator : public MyLedStrip
{
protected:
  PixelsContainer _animatedPixels;
  cl_Lst<LedStripAnimation *> _animationList;
  int _animationIndex;

  void handleAnimation()
  {
    if (_animationIndex < 0) return;
    if (_animationIndex > _animationList.size() - 1) return;

    // check if the animated pixel buffer is displayed
    // if not, do not update the animated pixel buffer
    if (_animatedPixels.hasChanged) return;

    // update the animated pixel buffer
    _animationList[_animationIndex]->handle();

    // if output pixels buffer is updated
    // then simulate update of the input buffer
    if (_animatedPixels.hasChanged)
      _pixels.hasChanged = false;
  }

public:
  MyLedStripAnimator()
    : MyLedStrip()
    , _animationIndex(0)
  {
    _animationList.push_back(new LedStripAnimationNormal(&_pixels, &_animatedPixels));
    _animationList.push_back(new LedStripAnimationBlink(&_pixels, &_animatedPixels));
    _animationList.push_back(new LedStripAnimationFire(&_pixels, &_animatedPixels));
    _animationList.push_back(new LedStripAnimationMatrix(&_pixels, &_animatedPixels));
    _animationList.push_back(new LedStripAnimationRainbow(&_pixels, &_animatedPixels));
  }

  bool setAnimation(int mode)
  {
    if (mode < 0) return false;
    if (mode > _animationList.size() - 1) return false;

    _animationIndex = mode;
    _animationList[_animationIndex]->begin();

    _mqtt.publish(mqttTopicPubLedAnim.topic().c_str(), String(mode).c_str());

    return true;
  }

  int getAnimationIndex()
  {
    return _animationIndex;
  }

  cl_Lst<LedStripAnimation *> *getAnimationsList()
  {
    return &_animationList;
  }

  void handle()
  {
    handleAutomaticBrightness();
    handleMode();
    handleAnimation();
    refresh(&_animatedPixels);
  }
};

MyLedStripAnimator QTLed;
