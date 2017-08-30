//
// Led Strip configuration for MyQlockTwo
//

#include "fonts.h"
#include "qlocktwo.h"

#define NROW 10
#define NCOL 11
#define NEDGE 4

#define PIXELS ((NROW * NCOL) + NEDGE)

#define BLACK RgbColor(0, 0, 0)

const uint8_t matchingPixelsMatrix[NROW][NCOL] = {
  {  1,   2,   6,   7,  15,  16,  28,  29,  45,  46,  65},
  {  3,   5,   8,  14,  17,  27,  30,  44,  47,  64,  66},
  {  4,   9,  13,  18,  26,  31,  43,  48,  63,  67,  82},
  { 10,  12,  19,  25,  32,  42,  49,  62,  68,  81,  83},
  { 11,  20,  24,  33,  41,  50,  61,  69,  80,  84,  95},
  { 21,  23,  34,  40,  51,  60,  70,  79,  85,  94,  96},
  { 22,  35,  39,  52,  59,  71,  78,  86,  93,  97, 104},
  { 36,  38,  53,  58,  72,  77,  87,  92,  98, 103, 105},
  { 37,  54,  57,  73,  76,  88,  91,  99, 102, 106, 109},
  { 55,  56,  74,  75,  89,  90, 100, 101, 107, 108, 110}
};

const uint8_t matchingPixelsEdge[NEDGE] = { 113, 112, 111, 114 };

enum MyLedStripMode
{
  ModeInit = 0,
  ModeNothing = 1,
  ModeNormal = 2,
  ModeSec = 3,
  ModeDay = 4,
  ModeTemp = 5,
  ModeTest = 100,
  ModeSpeedTest = 101,
  ModeStressTest = 102
};

enum MyLedStripColorRandom
{
  ColorRandomNo = 0,
  ColorRandomAll,
  ColorRandomLetter,
  ColorRandomWord
};


class PixelsArray{
private:
  RgbColor _pixels[NROW][NCOL];

public:
  PixelsArray()
  {
    clear();
  }

  void fill(const RgbColor &c)
  {
    for (int i = 0; i < NROW; i++)
    {
      for (int j = 0; j < NCOL; j++)
      {
        _pixels[i][j] = c;
      }
    }
  }

  void clear()
  {
    fill(BLACK);
  }

  void setPixel(const RgbColor &c, int row, int col)
  {
    if (row < 0) return;
    if (col < 0) return;
    if (row > NROW - 1) return;
    if (col > NCOL - 1) return;

    _pixels[row][col] = c;
  }

  RgbColor getPixel(int row, int col)
  {
    if (row < 0) return BLACK;
    if (col < 0) return BLACK;
    if (row > NROW - 1) return BLACK;
    if (col > NCOL - 1) return BLACK;

    return _pixels[row][col];
  }
};

struct PixelsContainer
{
  PixelsArray pixelsArray;
  RgbColor pixelsEdge[NEDGE];
  bool hasChanged;
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
  for (int i = 0; i < FONTROW; i++)
  {
    for (int j = 0; j < FONTCOL; j++)
    {
      if (src[i][j])
        dst.setPixel(color, posx + i, posy + j);
    }
  }
}

// Display number [-9:99]
void copyNumberToMatrix(int n, PixelsArray &dst, const RgbColor &color)
{
  int x = 1;

  if (n < 0)
    ::copyCharToMatrix(_minus, dst, x, 0, color); // Display "-"
  else
    if (n / 10 != 0)
      ::copyCharToMatrix(_number[abs(n / 10)], dst, x, 0, color); // Display "First number"
    else
      ::copyCharToMatrix(_number[0], dst, x, 0, color); // Display "First number"

  ::copyCharToMatrix(_number[abs(n % 10)], dst, x, 0 + 6, color); // Display "Second number"
}

class MyNeoEsp8266UartWS2813
{
public:
  static const uint32_t ByteSendTimeUs = 10; // us it takes to send a single pixel element at 800khz speed
  static const uint32_t UartBaud = 3636363; // 3200000; // 800khz, 4 serial bytes per NeoByte
  static const uint32_t RESMinTimeUs = 300; // us it takes to wait before sending a new frame
};
typedef NeoEsp8266UartMethodBase<MyNeoEsp8266UartWS2813, NeoEsp8266AsyncUart> MyNeoEsp8266AsyncUart800KbpsMethod;
typedef NeoEsp8266UartMethodBase<MyNeoEsp8266UartWS2813, NeoEsp8266Uart> MyNeoEsp8266Uart800KbpsMethod;

//#define NeoEsp8266Method NeoEsp8266AsyncUart800KbpsMethod
//#define NeoEsp8266Method NeoEsp8266Uart800KbpsMethod

#define NeoEsp8266Method MyNeoEsp8266AsyncUart800KbpsMethod
//#define NeoEsp8266Method MyNeoEsp8266Uart800KbpsMethod

//#define NeoEsp8266Method NeoEsp8266BitBang800KbpsMethod

class MyLedStrip
{
protected:
  NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp8266Method> *_pStrip;
  PixelsContainer _pixels;
  QlockTwoFr _qlocktwo;
  RgbColor _color;

  bool myShow()
  {
    if (!_pStrip->CanShow())
      return false;

    //interrupts();
    _pStrip->Show();
    //noInterrupts();

    return true;
  }

  void mySetPixel(int index, Neo3Elements::ColorObject c)
  {
    _pStrip->SetPixelColor(index, c);
  }

private:
  int _pixelcount;
  int _automaticBrightness;
  MyLedStripMode _mode, _pmode;
  MyLedStripColorRandom _colorRandom;


  virtual void update() = 0;

  void fillColor(const RgbColor &c)
  {
    _pixels.pixelsArray.fill(c);
    for (int i = 0; i < NEDGE; i++)
      _pixels.pixelsEdge[i] = c;
    
    _pixels.hasChanged = true;
  }

  void handleModeNothing()
  {
    if (_pmode == _mode)
      return ;

    fillColor(BLACK);

    _pixels.hasChanged = true;
  }

  void handleModeNormal()
  {
    static int sm = 0;
    static int sh = 0;

    if (_pmode != _mode)
    {
      sm = -1;
      sh = -1;
    }
    int h = _dateTime.hour;
    int m = _dateTime.minute;

    if (sm == m && sh == h)
      return;

    sm = m;
    sh = h;


    //randomSeed(analogRead(PIN_ALS));

    QlockTwoBlobs b = _qlocktwo.getBlobsFromTime(h, m);

    if (!b.number)
      return; // TODO: Display something useful

    RgbColor c = _color;

    fillColor(BLACK);
    
    if (_colorRandom == ColorRandomAll)
      c = HslColor((float)random(256) / 255.0, 1.0, 0.5);

    for (int i = 0; i < b.number; i++)
    {
      if (_colorRandom == ColorRandomWord)
        c = HslColor((float)random(256) / 255.0, 1.0, 0.5);

      for (int j = 0; j < b.blobs[i]->number; j++)
      {
        if (_colorRandom == ColorRandomLetter)
          c = HslColor((float)random(256) / 255.0, 1.0, 0.5);

        _pixels.pixelsArray.setPixel(c, b.blobs[i]->pixels[j].row, b.blobs[i]->pixels[j].col);
      }
    }

    if (_colorRandom == ColorRandomWord)
      c = HslColor((float)random(256) / 255.0, 1.0, 0.5);

    for (int i = 0; i < m % 5; i++)
    {
      if (_colorRandom == ColorRandomLetter)
        c = HslColor((float)random(256) / 255.0, 1.0, 0.5);

      _pixels.pixelsEdge[i] = c;
    }

    _pixels.hasChanged = true;
  }

  void handleModeSecond()
  {
    static int s = 0;

    if (_pmode != _mode)
      s = -1;

    if (s == (int)_dateTime.second)
      return;

    s = _dateTime.second;

    fillColor(BLACK);
    ::copyNumberToMatrix(s, _pixels.pixelsArray, _color);

    _pixels.hasChanged = true;
  }

  void handleModeDay()
  {
    static int s = 0;

    if (_pmode != _mode)
      s = -1;

    if (s == (int)_dateTime.day)
      return;

    s = _dateTime.day;

    fillColor(BLACK);
    ::copyNumberToMatrix(s, _pixels.pixelsArray, _color);

    _pixels.hasChanged = true;
  }

  void handleModeTemperature()
  {
    static int s = 0;

    if (_pmode != _mode)
      s = 200000;

    if (s == 0)
      return;

    if (s == 200000)
    {
      // Display Temperature
      if (!RTC.GetIsRunning())
        return; // TODO: Display something useful
      
      int8_t t = RTC.GetTemperature().AsWholeDegrees();

      fillColor(BLACK);
      ::copyNumberToMatrix(t, _pixels.pixelsArray, _color);

      _pixels.hasChanged = true;
    }


    if (s == 000001)
    { // Refresh
      s = 200000;
      return ;
    }

    s--;
  }

  bool handleModeStressTest()
  {
    static int s = 0;

    if (_pmode != _mode)
      s = 400000;

    if (s == 0)
    {
      setMode(ModeNormal);
      return true;
    }

    if (s % 2)
      fillColor(RgbColor(255, 255, 255));
    else
      fillColor(RgbColor(0, 0, 0));

    _pixels.hasChanged = true;

    s--;

    return false;
  }


  bool handleModeSpeedTest()
  {
    static int s = 0;
    static int c = 0;
    static int r = 0;

    if (_pmode != _mode)
      s = 400000;

    if (s == 0)
    {
      setMode(ModeNormal);
      return true;
    }

    if (!_pStrip->CanShow())
      return false;

    fillColor(BLACK);

    _pixels.pixelsArray.setPixel(_color, r, c);

    _pixels.hasChanged = true;

    c++;

    if (c == NCOL)
    {
      r++;
      c = 0;
    }

    if (r == NROW)
    {
      r = 0;
    }

    s--;

    return false;
  }

  // Return finished flag
  bool handleModeTest()
  {
    static int s = 0;

    if (_pmode != _mode)
      s = 400000;

    if (s == 0)
    {
      setMode(ModeNormal);
      return true;
    }

    if (s == 400000) fillColor(RgbColor(255, 0, 0));
    if (s == 300000) fillColor(RgbColor(0, 255, 0));
    if (s == 200000) fillColor(RgbColor(0, 0, 255));
    if (s == 100000) fillColor(RgbColor(255, 255, 255));

    s--;

    return false;
  }

  // Update brightness every 50ms
  void handleAutomaticBrightness()
  {
    static uint64_t p = 0;

    uint64_t v = millis64() / 50;

    if (v != p)
    {
      int sd = _config.brightnessAutoMinDay;      // minimum brightness during the day
      int sn = _config.brightnessAutoMinNight;    // minimum brightness during the night
      int sm = 255;                               // maximum brightness
      int lmin = 0;                               // minimum lux sensitivity allowed
      int lmax = 400;                             // maximum lux sensitivity allowed

      int s = sn;

      if (_dateTime.hour > 21 && _dateTime.hour < 9) s = sn; // between 22h and 9h
      if (_dateTime.hour > 9 && _dateTime.hour < 21) s = sd; // between 10h and 21h

      if (_dateTime.hour == 21) s = map(_dateTime.minute, 0, 59, sd, sn); // during the 21th hour
      if (_dateTime.hour == 9) s = map(_dateTime.minute, 0, 59, sn, sd); // during the 9th hour

      int l = getAvgLux();
      if (l < lmin) l = lmin;
      if (l > lmax) l = lmax;
      setBrightness(map(l, lmin, lmax, s, sm)); // limit
    }

    p = v;
  }

public:
  MyLedStrip() :
    _pStrip(NULL),
    _pixelcount(PIXELS),
    _automaticBrightness(1),
    _mode(ModeInit),
    _pmode(ModeInit),
    _colorRandom(ColorRandomNo)
  {
    _color.R = 255;
    _color.G = 255;
    _color.B = 255;

  }

  ~MyLedStrip()
  {
    end();
  }

  void begin()
  {
    if (_pStrip)
      return;

    // Cannot use DMA because DMA GPIO is already used by serial/USB bridge :(
    _pStrip = new NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp8266Method>(_pixelcount, D4);
    _pStrip->Begin();

    fillColor(BLACK);
    update(); // Force update with "normal" animator mode
  }

  void end()
  {
    if (_pStrip)
      delete _pStrip;
    _pStrip = NULL;
  }

  void setBrightness(uint8_t b)
  {
    if (b < 1) b = 1;
    if (b > 255) b = 255;

    _pStrip->SetBrightness(b);

    myShow();
  }

  uint8_t getBrightness()
  {
    return _pStrip->GetBrightness();
  }

  void setAutomaticBrightness(int b)
  {
    _automaticBrightness = b;
  }

  void setMode(MyLedStripMode m)
  {
    _mode = m;
    _pmode = ModeInit;

    Serial.printf("setMode(%d)\n", m);
  }

  void setColor(byte r, byte g, byte b)
  {
    _color = RgbColor(r, g, b);

    setMode(_mode); // Used to force repaint
  }

  void setColorRandom(MyLedStripColorRandom c)
  {
    _colorRandom = c;

    setMode(_pmode); // Used to force repaint
  }

  void handle()
  {
    if (_automaticBrightness)
      handleAutomaticBrightness();

    switch (_mode)
    {
    case ModeInit:
    case ModeNothing:
      handleModeNothing();
      break;
    case ModeTest:
      if (handleModeTest())
        return ;
      break;
    case ModeNormal:
      handleModeNormal();
      break;
    case ModeSec:
      handleModeSecond();
      break;
    case ModeDay:
      handleModeDay();
      break;
    case ModeTemp:
      handleModeTemperature();
      break;
    case ModeSpeedTest:
      if (handleModeSpeedTest())
        return;
      break;
    case ModeStressTest:
      if (handleModeStressTest())
        return;
      break;
    default:
      handleModeNormal();
      ;
    };

    _pmode = _mode;

    update();
  }
};



enum MyLedStripAnimatorMode
{
  AnimModeNormal,
  AnimModeBlinkRandom,
  AnimModeFire,
  AnimModeMatrix,
  AnimModePongAuto,
  AnimModePongManual,
  AnimModeAlphabet,
  AnimModeRainbowWithBg,
  AnimModeRainbowWithoutBg
};

enum MyLedStripAnimatorState
{
  AnimStateBegin,
  AnimStateExecute,
  AnimStateEnd
};

struct Pixel
{
  int row;
  int col;
  int edge;
  RgbColor color;
};

struct PongBall
{
  int row;
  int col;
  int drow;
  int dcol;
};

class MyLedStripAnimator : public MyLedStrip
{
private:
  MyLedStripAnimatorMode _animMode;
  MyLedStripAnimatorState _animState;
  Frame _animFrame;
  cl_Lst<Pixel> _animPixelsList;

  int _matrixModeColumns[NCOL];

  PongBall _pongModeBall0, _pongModeBall1, _pongModeBall2;
  int _pongModePlay1pos, _pongModePlay1posManual;

  int _aplhabetModeIndex = 0;

  double _rainbowModeIndex = 0.0;


  void updateSimple()
  {
    if (_animState == AnimStateEnd)
      return;

    _animState = AnimStateExecute;

    //Serial.printf("updateSimple() = AnimStateExecute\n");

    for (int i = 0; i < NROW; i++)
    {
      for (int j = 0; j < NCOL; j++)
      {
        mySetPixel(matchingPixelsMatrix[i][j] - 1, _pixels.pixelsArray.getPixel(i, j));
      }
    }

    for (int i = 0; i < NEDGE; i++)
      mySetPixel(matchingPixelsEdge[i] - 1, _pixels.pixelsEdge[i]);

    if (myShow())
    {
      _animState = AnimStateEnd;

      //Serial.printf("updateSimple() = AnimStateEnd\n");
    }
  }

  void updateBlinkRandom()
  {
    if (_animState == AnimStateEnd)
      return;

    if (_animState == AnimStateBegin)
    { // Anim Start
      //Serial.printf("updateBlinkRandom() = AnimStateBegin\n");

      _animState = AnimStateExecute;
      _animFrame.init(50);

      //randomSeed(analogRead(PIN_ALS));

      _animPixelsList.clear();

      for (int i = 0; i < NROW; i++)
      {
        for (int j = 0; j < NCOL; j++)
        {
          if (_pixels.pixelsArray.getPixel(i, j) == BLACK)
            continue;

          Pixel p;
          p.row = i;
          p.col = j;
          p.edge = -1;
          p.color = _pixels.pixelsArray.getPixel(i, j);

          _animPixelsList.push_back(p);
        }
      }

      for (int i = 0; i < NEDGE; i++)
      {
        if (_pixels.pixelsEdge[i] == BLACK)
          continue;

        Pixel p;
        p.row = -1;
        p.col = -1;
        p.edge = i;
        p.color = _pixels.pixelsEdge[i];

        _animPixelsList.push_back(p);
      }

      _pStrip->ClearTo(BLACK);
    }

    if (!_animFrame.next() || !_pStrip->CanShow())
      return;

    if (_animState == AnimStateExecute)
    { // Anim Exec

      if (_animPixelsList.size() > 0)
      {
        int idx = random(_animPixelsList.size());
        Pixel p = _animPixelsList[idx];

        if (p.edge == -1)
          mySetPixel(matchingPixelsMatrix[p.row][p.col] - 1, p.color);
        else
          mySetPixel(matchingPixelsEdge[p.edge] - 1, p.color);

        _animPixelsList.remove(idx);

        myShow();
      }

      if (_animPixelsList.size() == 0)
        _animState = AnimStateEnd;
    }

    if (_animState == AnimStateEnd)
    { // Anim End
      //Serial.printf("updateBlinkRandom() = AnimStateEnd\n");
    }
  }

  void updateFire()
  {
    if (_animState == AnimStateEnd)
      return;

    if (_animState == AnimStateBegin)
    { // Anim Start
      //Serial.printf("updateFire() = AnimStateBegin\n");

      _animFrame.init(8);
      _animState = AnimStateExecute;

      //randomSeed(analogRead(PIN_ALS));

      _animPixelsList.clear();

      for (int i = 0; i < NROW; i++)
      {
        for (int j = 0; j < NCOL; j++)
        {
          if (_pixels.pixelsArray.getPixel(i, j) == BLACK)
            continue;

          Pixel p;
          p.row = i;
          p.col = j;
          p.edge = -1;
          p.color = _pixels.pixelsArray.getPixel(i, j);

          _animPixelsList.push_back(p);
        }
      }

      //_pStrip->ClearTo(BLACK);
    }

    if (!_animFrame.next() || !_pStrip->CanShow())
      return;

    if (_animState == AnimStateExecute)
    { // Anim Exec
      //Serial.printf("updateFire() = AnimStateExecute Start\n");

      for (int i = 0; i < PIXELS; i++)
      {
        RgbColor c = RgbColor(58, 58, 6);
        c.Darken(random(15));
        c.R += random(15);

        mySetPixel(i, c);
      }

      for (int i = 0; i < _animPixelsList.size(); i++)
      {
        Pixel p = _animPixelsList[i];
        mySetPixel(matchingPixelsMatrix[p.row][p.col] - 1, p.color);
      }

      for (int i = 0; i < NEDGE; i++)
        mySetPixel(matchingPixelsEdge[i] - 1, _pixels.pixelsEdge[i]);

      myShow();

      //Serial.printf("updateFire() = AnimStateExecute End\n");
    }
  }

#define MATRIXLEN 9
  void updateMatrix()
  {
    if (_animState == AnimStateEnd)
      return;

    if (_animState == AnimStateBegin)
    { // Anim Start
      //Serial.printf("updateMatrix() = AnimStateBegin\n");

      _animFrame.init(8);
      _animState = AnimStateExecute;

      //randomSeed(analogRead(PIN_ALS));

      _animPixelsList.clear();

      for (int i = 0; i < NROW; i++)
      {
        for (int j = 0; j < NCOL; j++)
        {
          if (_pixels.pixelsArray.getPixel(i, j) == BLACK)
            continue;

          Pixel p;
          p.row = i;
          p.col = j;
          p.edge = -1;
          p.color = _pixels.pixelsArray.getPixel(i, j);

          _animPixelsList.push_back(p);
        }
      }

      //_pStrip->ClearTo(BLACK);
    }

    if (!_animFrame.next() || !_pStrip->CanShow())
      return;

    if (_animState == AnimStateExecute)
    { // Anim Exec
      //Serial.printf("updateMatrix() = AnimStateExecute Start\n");

      for (int i = 0; i < NCOL; i++)
      {
        if (_matrixModeColumns[i] == -1)
        {
          if (random(40) == 0)
            _matrixModeColumns[i] = 0;

          continue;
        }
        
        if (_matrixModeColumns[i] == NROW + MATRIXLEN)
        {
          _matrixModeColumns[i] = -1;
          continue;
        }

        _matrixModeColumns[i]++;
      }

      /*
      for (int i = 0; i < NCOL; i++)
        Serial.printf("%02d, ", _matrixModeColumns[i]);
      Serial.printf("\n");
      */

      _pStrip->ClearTo(BLACK);

      for (int i = 0; i < NCOL; i++)
      {
        RgbColor c = RgbColor(0, 255, 0);

        int line = _matrixModeColumns[i];
        if (line == -1) continue;

        for (int l = line; line - l < MATRIXLEN; l--)
        {
          if (l >= 0 && l < NROW)
            mySetPixel(matchingPixelsMatrix[l][i] - 1, c);

          c.Darken(30);
        }
      }

      for (int i = 0; i < _animPixelsList.size(); i++)
      {
        Pixel p = _animPixelsList[i];
        mySetPixel(matchingPixelsMatrix[p.row][p.col] - 1, p.color);
      }

      for (int i = 0; i < NEDGE; i++)
        _pStrip->SetPixelColor(matchingPixelsEdge[i] - 1, _pixels.pixelsEdge[i]);

      myShow();

      //Serial.printf("updateMatrix() = AnimStateExecute End\n");
    }
  }

  void updatePong(boolean automatic)
  {
    if (_animState == AnimStateEnd)
      return;

    if (_animState == AnimStateBegin)
    { // Anim Start
      //Serial.printf("updatePong() = AnimStateBegin\n");

      _animFrame.init(6);
      _animState = AnimStateExecute;

      _pongModePlay1posManual = (NCOL / 2) - 1;

      //randomSeed(analogRead(PIN_ALS));

      _animPixelsList.clear();

      for (int i = 0; i < NROW - 2; i++) // Do not display the last two line
      {
        for (int j = 0; j < NCOL; j++)
        {
          if (_pixels.pixelsArray.getPixel(i, j) == BLACK)
            continue;

          Pixel p;
          p.row = i;
          p.col = j;
          p.edge = -1;
          p.color = _pixels.pixelsArray.getPixel(i, j);

          _animPixelsList.push_back(p);
        }
      }

      //_pStrip->ClearTo(BLACK);
    }

    if (!_animFrame.next() || !_pStrip->CanShow())
      return;

    if (_animState == AnimStateExecute)
    { // Anim Exec
      //Serial.printf("updatePong() = AnimStateExecute Start\n");

      _pStrip->ClearTo(BLACK);

      RgbColor c = RgbColor(255, 0, 0);

      // Update ball position
      _pongModeBall0.row += (_pongModeBall0.drow * (random(30) ? 1 : 0));
      _pongModeBall0.col += (_pongModeBall0.dcol * (random(30) ? 1 : 0));

      // Update player position
      if (automatic)
        _pongModePlay1pos = _pongModeBall0.col - 1;
      else 
        _pongModePlay1pos = _pongModePlay1posManual;

      // Check walls
      if (_pongModeBall0.row == 0 && _pongModeBall0.drow < 0) _pongModeBall0.drow = 1;
      if (_pongModeBall0.row == (NROW - 1) && _pongModeBall0.drow > 0) _pongModeBall0.drow = -1;
      if (_pongModeBall0.col == 0 && _pongModeBall0.dcol < 0) _pongModeBall0.dcol = 1;
      if (_pongModeBall0.col == (NCOL - 1) && _pongModeBall0.dcol > 0) _pongModeBall0.dcol = -1;

      // Check player
      if (_pongModeBall0.row == (NROW - 2) && 
        (_pongModeBall0.col == _pongModePlay1pos + 0 || _pongModeBall0.col == _pongModePlay1pos + 1 || _pongModeBall0.col == _pongModePlay1pos + 2))
        _pongModeBall0.drow = -1;
      
      // Check time
      for (int i = 0; i < _animPixelsList.size(); i++)
      {
        Pixel p = _animPixelsList[i];
        if (p.row == _pongModeBall0.row && p.col == _pongModeBall0.col)
        {
          // time obstacle collision
          //_pongModeBall0.drow = (_pongModeBall0.drow > 0) ? -1 : 1;
          //_pongModeBall0.dcol = (_pongModeBall0.dcol > 0) ? -1 : 1;

          _animPixelsList.remove(i);
        }
      }

      // Display ball
      _pStrip->SetPixelColor(matchingPixelsMatrix[_pongModeBall0.row][_pongModeBall0.col] - 1, c);
      c.Darken(150);
      _pStrip->SetPixelColor(matchingPixelsMatrix[_pongModeBall1.row][_pongModeBall1.col] - 1, c);
      c.Darken(80);
      _pStrip->SetPixelColor(matchingPixelsMatrix[_pongModeBall2.row][_pongModeBall2.col] - 1, c);

      _pongModeBall2 = _pongModeBall1;
      _pongModeBall1 = _pongModeBall0;


      // Display player
      if (_pongModePlay1pos < 0) _pongModePlay1pos = 0;
      if (_pongModePlay1pos + 3 > NCOL - 1) _pongModePlay1pos = NCOL - 3;
      for (int i = _pongModePlay1pos; i < _pongModePlay1pos + 3; i++)
        _pStrip->SetPixelColor(matchingPixelsMatrix[NROW - 1][i] - 1, RgbColor(0, 255, 0));

      // Display Text
      for (int i = 0; i < _animPixelsList.size(); i++)
      {
        Pixel p = _animPixelsList[i];
        _pStrip->SetPixelColor(matchingPixelsMatrix[p.row][p.col] - 1, p.color);
      }

      for (int i = 0; i < NEDGE; i++)
        _pStrip->SetPixelColor(matchingPixelsEdge[i] - 1, _pixels.pixelsEdge[i]);

      myShow();

      //Serial.printf("updatePong() = AnimStateExecute End\n");
    }
  }

  void updateAlphabet()
  {
    if (_animState == AnimStateEnd)
      return;

    if (_animState == AnimStateBegin)
    { // Anim Start
      //Serial.printf("updateAlphabet() = AnimStateBegin\n");

      _animFrame.init(0.25);
      _animState = AnimStateExecute;
    }

    if (!_animFrame.next() || !_pStrip->CanShow())
      return;

    if (_animState == AnimStateExecute)
    { // Anim Exec
      _pStrip->ClearTo(BLACK);

      for (int i = 0; i < NROW; i++)
      {
        for (int j = 0; j < NCOL; j++)
        {
          RgbColor p = _pixels.pixelsArray.getPixel(i, j);

          if (p == BLACK) continue;

          _pStrip->SetPixelColor(matchingPixelsMatrix[i][j] - 1, p);
        }
      }

      for (int i = 0; i < NEDGE; i++)
        _pStrip->SetPixelColor(matchingPixelsEdge[i] - 1, _pixels.pixelsEdge[i]);

      cl_Lst<QlockTwoPixel> l = _qlocktwo.getPixelsFromLetter(_aplhabetModeIndex + 'a');
      for (int i = 0; i < l.size(); i++)
      {
        if (_pixels.pixelsArray.getPixel(l[i].row, l[i].col) != BLACK)
          continue ;

        _pStrip->SetPixelColor(matchingPixelsMatrix[l[i].row][l[i].col] - 1, _color);
        break ;
      }

      _aplhabetModeIndex++;
      if (_aplhabetModeIndex == 26)
        _aplhabetModeIndex = 0;

      myShow();

      //Serial.printf("updateAlphabet() = AnimStateExecute End\n");
    }
  }

  void updateRainbow(bool withbackground)
  {
    if (_animState == AnimStateEnd)
      return;

    if (_animState == AnimStateBegin)
    { // Anim Start
      //Serial.printf("updateRainbow() = AnimStateBegin\n");

      _animFrame.init(8);
      _animState = AnimStateExecute;

      //randomSeed(analogRead(PIN_ALS));

      _animPixelsList.clear();

      for (int i = 0; i < NROW; i++)
      {
        for (int j = 0; j < NCOL; j++)
        {
          if (_pixels.pixelsArray.getPixel(i, j) == BLACK)
            continue;

          Pixel p;
          p.row = i;
          p.col = j;
          p.edge = -1;
          p.color = _pixels.pixelsArray.getPixel(i, j);

          _animPixelsList.push_back(p);
        }
      }

      //_pStrip->ClearTo(BLACK);
    }

    if (!_animFrame.next() || !_pStrip->CanShow())
      return;

    if (_animState == AnimStateExecute)
    { // Anim Exec
      //Serial.printf("updateRainbow() = AnimStateExecute Start\n");

      _rainbowModeIndex += 0.005;

      if (_rainbowModeIndex > 1.0)
        _rainbowModeIndex = 0.0;

      for (int i = 0; i < PIXELS; i++)
      {
        double p = ((double)i / (double)PIXELS) * (120.0 / 360.0);

        p = p + _rainbowModeIndex;

        if (p > 1.0) p = p - 1.0;

        HslColor c = HslColor(p, 1.0, 0.5);

        mySetPixel(i, c);
      }

      
      for (int i = 0; i < _animPixelsList.size(); i++)
      {
        Pixel p = _animPixelsList[i];
        HslColor c = _pStrip->GetPixelColor(matchingPixelsMatrix[p.row][p.col] - 1);

        c.H += 0.5;
        if (c.H > 1.0) c.H = c.H - 1.0;

        _animPixelsList[i].color = c;
      }

      if (!withbackground)
        _pStrip->ClearTo(BLACK);

      for (int i = 0; i < _animPixelsList.size(); i++)
      {
        Pixel p = _animPixelsList[i];
        mySetPixel(matchingPixelsMatrix[p.row][p.col] - 1, p.color);
      }
      

      for (int i = 0; i < NEDGE; i++)
      {
        if (_pixels.pixelsEdge[i] == BLACK)
          mySetPixel(matchingPixelsEdge[i] - 1, _pixels.pixelsEdge[i]);
        else
          mySetPixel(matchingPixelsEdge[i] - 1, _pStrip->GetPixelColor(matchingPixelsMatrix[0][0] - 1));
      }

      myShow();

      //Serial.printf("updateRainbow() = AnimStateExecute End\n");
    }
  }



public:
  MyLedStripAnimator() :
    MyLedStrip()
  {
    setAnimMode(AnimModeNormal);
  }

  void setAnimMode(MyLedStripAnimatorMode m)
  {
    _animMode = m;
    _animState = AnimStateBegin;
    _pixels.hasChanged = true;

    // Specific init for "matrix" effect
    for (int i = 0; i < NCOL; i++)
      _matrixModeColumns[i] = -1;

    // Specific init for "pong" effect
    _pongModeBall0.row = random(3, NROW - 2);
    _pongModeBall0.col = random(3, NCOL - 2);
    _pongModeBall0.drow = random(2) ? -1 : 1;
    _pongModeBall0.dcol = random(2) ? -1 : 1;
    _pongModeBall1 = _pongModeBall0;
    _pongModeBall2 = _pongModeBall0;
    _pongModePlay1pos = (NCOL / 2) - 1;
    _pongModePlay1posManual = (NCOL / 2) - 1;

    _aplhabetModeIndex = 0;

    Serial.printf("setAnimMode(%d)\n", m);
  }

  void setPongModePlay1posManual(int d)
  {
    int p = _pongModePlay1posManual + d;
    if (p < 0) p = 0;
    if (p > NROW - 2) p = NROW - 2;

    _pongModePlay1posManual = p;
  }

  // Update internal buffer and display pixels
  virtual void update()
  {
    if (_pixels.hasChanged)
    {
      _animState = AnimStateBegin;
      _pixels.hasChanged = false;
    }

    switch (_animMode)
    {
    case AnimModeNormal:
      updateSimple();
      break;
    case AnimModeBlinkRandom:
      updateBlinkRandom();
      break;
    case AnimModeFire:
      updateFire();
      break;
    case AnimModeMatrix:
      updateMatrix();
      break;
    case AnimModePongAuto:
      updatePong(true);
      break;
    case AnimModePongManual:
      updatePong(false);
      break;
    case AnimModeAlphabet:
      updateAlphabet();
      break;
    case AnimModeRainbowWithBg:
      updateRainbow(true);
      break;
    case AnimModeRainbowWithoutBg:
      updateRainbow(false);
      break;
    default:
      updateSimple();
    }
  }
};

MyLedStripAnimator QTLed;
