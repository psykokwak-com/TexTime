#define TEXTIMEMAXBLOBSIZE 16

struct TextTimePixel
{
  uint8_t row;
  uint8_t col;
};

struct TextTimeBlob
{
  uint8_t number;
  TextTimePixel pixels[TEXTIMEMAXBLOBSIZE];
};

struct TextTimeBlobs
{
  uint8_t number;
  TextTimeBlob *blobs[TEXTIMEMAXBLOBSIZE];
};

class TextTime
{
  protected:
    TextTimeBlob _it;
    TextTimeBlob _is;
    TextTimeBlob _past;
    TextTimeBlob _to;
    TextTimeBlob _am;
    TextTimeBlob _pm;
    TextTimeBlob _oclock;
    TextTimeBlob _hours[13];
    TextTimeBlob _minutes[13];

  public:
    virtual TextTimeBlobs getBlobsFromTime(int hour, int minute) = 0;
    virtual cl_Lst<TextTimePixel> getPixelsFromLetter(char c) = 0;
};

class TextTimeFr: public TextTime
{

  public:
    TextTimeFr()
    {
      _it.number = 2;
      _it.pixels[0].row = 0; _it.pixels[0].col = 0; // I
      _it.pixels[1].row = 0; _it.pixels[1].col = 1; // T

      _is.number = 2;
      _is.pixels[0].row = 0; _is.pixels[0].col = 3; // I
      _is.pixels[1].row = 0; _is.pixels[1].col = 4; // S

      _past.number = 4;
      _past.pixels[0].row = 4; _past.pixels[0].col = 0; // P
      _past.pixels[1].row = 4; _past.pixels[1].col = 1; // A
      _past.pixels[2].row = 4; _past.pixels[2].col = 2; // S
      _past.pixels[3].row = 4; _past.pixels[3].col = 3; // T

      _to.number = 2;
      _to.pixels[0].row = 3; _to.pixels[0].col = 9;  // T
      _to.pixels[1].row = 3; _to.pixels[1].col = 10; // O

      _am.number = 2;
      _am.pixels[0].row = 0; _am.pixels[0].col = 7;  // A
      _am.pixels[1].row = 0; _am.pixels[1].col = 8;  // M

      _pm.number = 2;
      _pm.pixels[0].row = 0; _pm.pixels[0].col = 9;  // P
      _pm.pixels[1].row = 0; _pm.pixels[1].col = 10; // M

      _oclock.number = 6;
      _oclock.pixels[0].row = 9; _oclock.pixels[0].col = 5;  // O'
      _oclock.pixels[1].row = 9; _oclock.pixels[1].col = 6;  // C
      _oclock.pixels[2].row = 9; _oclock.pixels[2].col = 7;  // L
      _oclock.pixels[3].row = 9; _oclock.pixels[3].col = 8;  // O
      _oclock.pixels[4].row = 9; _oclock.pixels[4].col = 9;  // C
      _oclock.pixels[5].row = 9; _oclock.pixels[5].col = 10; // K



      _hours[0].number = 6;
      _hours[0].pixels[0].row = 8; _hours[0].pixels[0].col = 5;   // T
      _hours[0].pixels[1].row = 8; _hours[0].pixels[1].col = 6;   // W
      _hours[0].pixels[2].row = 8; _hours[0].pixels[2].col = 7;   // E
      _hours[0].pixels[3].row = 8; _hours[0].pixels[3].col = 8;   // L
      _hours[0].pixels[4].row = 8; _hours[0].pixels[4].col = 9;   // V
      _hours[0].pixels[5].row = 8; _hours[0].pixels[5].col = 10;  // E

      _hours[1].number = 3;
      _hours[1].pixels[0].row = 5; _hours[1].pixels[0].col = 0;  // O
      _hours[1].pixels[1].row = 5; _hours[1].pixels[1].col = 1;  // N
      _hours[1].pixels[2].row = 5; _hours[1].pixels[2].col = 2;  // E

      _hours[2].number = 3;
      _hours[2].pixels[0].row = 6; _hours[2].pixels[0].col = 8;  // T
      _hours[2].pixels[1].row = 6; _hours[2].pixels[1].col = 9;  // W
      _hours[2].pixels[2].row = 6; _hours[2].pixels[2].col = 10; // O

      _hours[3].number = 5;
      _hours[3].pixels[0].row = 5; _hours[3].pixels[0].col = 6;  // T
      _hours[3].pixels[1].row = 5; _hours[3].pixels[1].col = 7;  // H
      _hours[3].pixels[2].row = 5; _hours[3].pixels[2].col = 8;  // R
      _hours[3].pixels[3].row = 5; _hours[3].pixels[3].col = 9;  // E
      _hours[3].pixels[4].row = 5; _hours[3].pixels[4].col = 10; // E

      _hours[4].number = 4;
      _hours[4].pixels[0].row = 6; _hours[4].pixels[0].col = 0;  // F
      _hours[4].pixels[1].row = 6; _hours[4].pixels[1].col = 1;  // O
      _hours[4].pixels[2].row = 6; _hours[4].pixels[2].col = 2;  // U
      _hours[4].pixels[3].row = 6; _hours[4].pixels[3].col = 3;  // R

      _hours[5].number = 4;
      _hours[5].pixels[0].row = 6; _hours[5].pixels[0].col = 4;  // F
      _hours[5].pixels[1].row = 6; _hours[5].pixels[1].col = 5;  // I
      _hours[5].pixels[2].row = 6; _hours[5].pixels[2].col = 6;  // V
      _hours[5].pixels[3].row = 6; _hours[5].pixels[3].col = 7;  // E

      _hours[6].number = 3;
      _hours[6].pixels[0].row = 5; _hours[6].pixels[0].col = 3;  // S
      _hours[6].pixels[1].row = 5; _hours[6].pixels[1].col = 4;  // I
      _hours[6].pixels[2].row = 5; _hours[6].pixels[2].col = 5;  // X

      _hours[7].number = 5;
      _hours[7].pixels[0].row = 8; _hours[7].pixels[0].col = 0;  // S
      _hours[7].pixels[1].row = 8; _hours[7].pixels[1].col = 1;  // E
      _hours[7].pixels[2].row = 8; _hours[7].pixels[2].col = 2;  // V
      _hours[7].pixels[3].row = 8; _hours[7].pixels[3].col = 3;  // E
      _hours[7].pixels[4].row = 8; _hours[7].pixels[4].col = 4;  // N

      _hours[8].number = 5;
      _hours[8].pixels[0].row = 7; _hours[8].pixels[0].col = 0;  // E
      _hours[8].pixels[1].row = 7; _hours[8].pixels[1].col = 1;  // I
      _hours[8].pixels[2].row = 7; _hours[8].pixels[2].col = 2;  // G
      _hours[8].pixels[3].row = 7; _hours[8].pixels[3].col = 3;  // H
      _hours[8].pixels[4].row = 7; _hours[8].pixels[4].col = 3;  // T

      _hours[9].number = 4;
      _hours[9].pixels[0].row = 4; _hours[9].pixels[0].col = 7;   // N
      _hours[9].pixels[1].row = 4; _hours[9].pixels[1].col = 8;   // I
      _hours[9].pixels[2].row = 4; _hours[9].pixels[2].col = 9;   // N
      _hours[9].pixels[3].row = 4; _hours[9].pixels[3].col = 10;  // E

      _hours[10].number = 3;
      _hours[10].pixels[0].row = 9; _hours[10].pixels[0].col = 0;  // T
      _hours[10].pixels[1].row = 9; _hours[10].pixels[1].col = 1;  // E
      _hours[10].pixels[2].row = 9; _hours[10].pixels[2].col = 2;  // N

      _hours[11].number = 6;
      _hours[11].pixels[0].row = 7; _hours[11].pixels[0].col = 5;   // E
      _hours[11].pixels[1].row = 7; _hours[11].pixels[1].col = 6;   // L
      _hours[11].pixels[2].row = 7; _hours[11].pixels[2].col = 7;   // E
      _hours[11].pixels[3].row = 7; _hours[11].pixels[3].col = 8;   // V
      _hours[11].pixels[4].row = 7; _hours[11].pixels[4].col = 9;   // E
      _hours[11].pixels[5].row = 7; _hours[11].pixels[5].col = 10;  // N

      _hours[12].number = 6;
      _hours[12].pixels[0].row = 8; _hours[12].pixels[0].col = 5;   // T
      _hours[12].pixels[1].row = 8; _hours[12].pixels[1].col = 6;   // W
      _hours[12].pixels[2].row = 8; _hours[12].pixels[2].col = 7;   // E
      _hours[12].pixels[3].row = 8; _hours[12].pixels[3].col = 8;   // L
      _hours[12].pixels[4].row = 8; _hours[12].pixels[4].col = 9;   // V
      _hours[12].pixels[5].row = 8; _hours[12].pixels[5].col = 10;  // E



      _minutes[0].number = 0; // between [0:4]

      _minutes[1].number = 8 - 4; // between [5:9]
      _minutes[1].pixels[0].row = 2; _minutes[1].pixels[0].col = 6;  // F
      _minutes[1].pixels[1].row = 2; _minutes[1].pixels[1].col = 7;  // I
      _minutes[1].pixels[2].row = 2; _minutes[1].pixels[2].col = 8;  // V
      _minutes[1].pixels[3].row = 2; _minutes[1].pixels[3].col = 9;  // E

      _minutes[2].number = 7 - 4; // between [10:14]
      _minutes[2].pixels[0].row = 3; _minutes[2].pixels[0].col = 5;  // T
      _minutes[2].pixels[1].row = 3; _minutes[2].pixels[1].col = 6;  // E
      _minutes[2].pixels[2].row = 3; _minutes[2].pixels[2].col = 7;  // N

      _minutes[3].number = 12 - 4; // between [15:19]
      _minutes[3].pixels[0].row = 1; _minutes[3].pixels[0].col = 0;    // A
      _minutes[3].pixels[1].row = 1; _minutes[3].pixels[1].col = 2;    // Q
      _minutes[3].pixels[2].row = 1; _minutes[3].pixels[2].col = 3;    // U
      _minutes[3].pixels[3].row = 1; _minutes[3].pixels[3].col = 4;    // A
      _minutes[3].pixels[4].row = 1; _minutes[3].pixels[4].col = 5;    // R
      _minutes[3].pixels[5].row = 1; _minutes[3].pixels[5].col = 6;    // T
      _minutes[3].pixels[6].row = 1; _minutes[3].pixels[6].col = 7;    // E
      _minutes[3].pixels[7].row = 1; _minutes[3].pixels[7].col = 8;    // R

      _minutes[4].number = 10 - 4; // between [20:24]
      _minutes[4].pixels[0].row = 2; _minutes[4].pixels[0].col = 0;  // T
      _minutes[4].pixels[1].row = 2; _minutes[4].pixels[1].col = 1;  // W
      _minutes[4].pixels[2].row = 2; _minutes[4].pixels[2].col = 2;  // E
      _minutes[4].pixels[3].row = 2; _minutes[4].pixels[3].col = 3;  // N
      _minutes[4].pixels[4].row = 2; _minutes[4].pixels[4].col = 4;  // T
      _minutes[4].pixels[5].row = 2; _minutes[4].pixels[5].col = 5;  // Y

      _minutes[5].number = 14 - 4; // between [25:29]
      _minutes[5].pixels[0].row = 2; _minutes[5].pixels[0].col = 0;    // T
      _minutes[5].pixels[1].row = 2; _minutes[5].pixels[1].col = 1;    // W
      _minutes[5].pixels[2].row = 2; _minutes[5].pixels[2].col = 2;    // E
      _minutes[5].pixels[3].row = 2; _minutes[5].pixels[3].col = 3;    // N
      _minutes[5].pixels[4].row = 2; _minutes[5].pixels[4].col = 4;    // T
      _minutes[5].pixels[5].row = 2; _minutes[5].pixels[5].col = 5;    // Y
      _minutes[5].pixels[6].row = 2; _minutes[5].pixels[6].col = 6;    // F
      _minutes[5].pixels[7].row = 2; _minutes[5].pixels[7].col = 7;    // I
      _minutes[5].pixels[8].row = 2; _minutes[5].pixels[8].col = 8;    // V
      _minutes[5].pixels[9].row = 2; _minutes[5].pixels[9].col = 9;    // E

      _minutes[6].number = 8 - 4; // between [30:34]
      _minutes[6].pixels[0].row = 3; _minutes[6].pixels[0].col = 0;   // H
      _minutes[6].pixels[1].row = 3; _minutes[6].pixels[1].col = 1;   // A
      _minutes[6].pixels[2].row = 3; _minutes[6].pixels[2].col = 2;   // L
      _minutes[6].pixels[3].row = 3; _minutes[6].pixels[3].col = 3;   // F

      _minutes[7].number = 12 - 2; // between [35:39]
      _minutes[7].pixels[0].row = 2; _minutes[7].pixels[0].col = 0;    // T
      _minutes[7].pixels[1].row = 2; _minutes[7].pixels[1].col = 1;    // W
      _minutes[7].pixels[2].row = 2; _minutes[7].pixels[2].col = 2;    // E
      _minutes[7].pixels[3].row = 2; _minutes[7].pixels[3].col = 3;    // N
      _minutes[7].pixels[4].row = 2; _minutes[7].pixels[4].col = 4;    // T
      _minutes[7].pixels[5].row = 2; _minutes[7].pixels[5].col = 5;    // Y
      _minutes[7].pixels[6].row = 2; _minutes[7].pixels[6].col = 6;    // F
      _minutes[7].pixels[7].row = 2; _minutes[7].pixels[7].col = 7;    // I
      _minutes[7].pixels[8].row = 2; _minutes[7].pixels[8].col = 8;    // V
      _minutes[7].pixels[9].row = 2; _minutes[7].pixels[9].col = 9;    // E

      _minutes[8].number = 8 - 2; // between [40:44]
      _minutes[8].pixels[0].row = 2; _minutes[8].pixels[0].col = 0;   // T
      _minutes[8].pixels[1].row = 2; _minutes[8].pixels[1].col = 1;   // W
      _minutes[8].pixels[2].row = 2; _minutes[8].pixels[2].col = 2;   // E
      _minutes[8].pixels[3].row = 2; _minutes[8].pixels[3].col = 3;   // N
      _minutes[8].pixels[4].row = 2; _minutes[8].pixels[4].col = 4;   // T
      _minutes[8].pixels[5].row = 2; _minutes[8].pixels[5].col = 5;   // Y

      _minutes[9].number = 10 - 2; // between [45:49]
      _minutes[9].pixels[0].row = 1; _minutes[9].pixels[0].col = 0;   // A
      _minutes[9].pixels[1].row = 1; _minutes[9].pixels[1].col = 2;   // Q
      _minutes[9].pixels[2].row = 1; _minutes[9].pixels[2].col = 3;   // U
      _minutes[9].pixels[3].row = 1; _minutes[9].pixels[3].col = 4;   // A
      _minutes[9].pixels[4].row = 1; _minutes[9].pixels[4].col = 5;   // R
      _minutes[9].pixels[5].row = 1; _minutes[9].pixels[5].col = 6;   // T
      _minutes[9].pixels[6].row = 1; _minutes[9].pixels[6].col = 7;   // E
      _minutes[9].pixels[7].row = 1; _minutes[9].pixels[7].col = 8;   // R

      _minutes[10].number = 5 - 2; // between [50:54]
      _minutes[10].pixels[0].row = 3; _minutes[10].pixels[0].col = 5;  // T
      _minutes[10].pixels[1].row = 3; _minutes[10].pixels[1].col = 6;  // E
      _minutes[10].pixels[2].row = 3; _minutes[10].pixels[2].col = 7;  // N

      _minutes[11].number = 6 - 2; // between [55:59]
      _minutes[11].pixels[0].row = 2; _minutes[11].pixels[0].col = 6;  // F
      _minutes[11].pixels[1].row = 2; _minutes[11].pixels[1].col = 7;  // I
      _minutes[11].pixels[2].row = 2; _minutes[11].pixels[2].col = 8;  // V
      _minutes[11].pixels[3].row = 2; _minutes[11].pixels[3].col = 9;  // E

    }

    virtual TextTimeBlobs getBlobsFromTime(int hour, int minute)
    {
      TextTimeBlobs b;

      b.number = 0;

      if (hour > 23) return b;
      if (minute > 59) return b;

      b.blobs[b.number++] = &_it;
      b.blobs[b.number++] = &_is;

      if (minute < 5)                   // O'CLOCK
        b.blobs[b.number++] = &_oclock;
      else if (minute >= 35) {
        hour = hour + 1;
        b.blobs[b.number++] = &_to;
      }
      else
        b.blobs[b.number++] = &_past;

      if (hour > 23) hour = 0;

      b.blobs[b.number++] = &(_hours[hour % 12]);    // DISPLAY HOURS
      b.blobs[b.number++] = &(_minutes[minute / 5]); // DISPALY MINUTES

      if (hour < 13)                    // AM
        b.blobs[b.number++] = &_am;
      else                              // PM
        b.blobs[b.number++] = &_pm;

      return b;
    }
    virtual cl_Lst<TextTimePixel> getPixelsFromLetter(char c)
    {
      cl_Lst<TextTimePixel> l;
      TextTimePixel p;

      if (c == 'a')
      {
        p.row = 0; p.col = 5; l.push_back(p);
        p.row = 0; p.col = 7; l.push_back(p);
        p.row = 1; p.col = 0; l.push_back(p);
        p.row = 1; p.col = 4; l.push_back(p);
        p.row = 3; p.col = 1; l.push_back(p);
        p.row = 4; p.col = 1; l.push_back(p);
        p.row = 1; p.col = 0; l.push_back(p);
      }

      if (c == 'b')
      {
        //     p.row = 0; p.col = 2; l.push_back(p);
      }

      if (c == 'c')
      {
        p.row = 1; p.col =  1; l.push_back(p);
        p.row = 1; p.col = 10; l.push_back(p);
        p.row = 9; p.col = 6; l.push_back(p);
      }

      if (c == 'd')
      {
        p.row = 1; p.col = 9; l.push_back(p);
      }

      if (c == 'e')
      {
        p.row = 1; p.col = 7; l.push_back(p);
        p.row = 2; p.col = 2; l.push_back(p);
        p.row = 2; p.col = 9; l.push_back(p);
        p.row = 3; p.col = 6; l.push_back(p);
        p.row = 4; p.col = 4; l.push_back(p);
        p.row = 5; p.col = 2; l.push_back(p);
        p.row = 5; p.col = 9; l.push_back(p);
        p.row = 5; p.col = 10; l.push_back(p);
        p.row = 6; p.col = 7; l.push_back(p);
        p.row = 7; p.col = 0; l.push_back(p);
        p.row = 7; p.col = 5; l.push_back(p);
        p.row = 7; p.col = 7; l.push_back(p);
        p.row = 7; p.col = 9; l.push_back(p);
        p.row = 8; p.col = 1; l.push_back(p);
        p.row = 8; p.col = 3; l.push_back(p);
        p.row = 8; p.col = 7; l.push_back(p);
        p.row = 8; p.col = 10; l.push_back(p);
        p.row = 9; p.col = 1; l.push_back(p);
        p.row = 9; p.col = 4; l.push_back(p);
      }

      if (c == 'f')
      {
        p.row = 2; p.col = 6; l.push_back(p);
        p.row = 3; p.col = 3; l.push_back(p);
        p.row = 3; p.col = 8; l.push_back(p);
        p.row = 6; p.col = 0; l.push_back(p);
        p.row = 6; p.col = 4; l.push_back(p);
      }

      if (c == 'g')
      {
        p.row = 7; p.col = 2; l.push_back(p);
      }

      if (c == 'h')
      {
        p.row = 3; p.col = 0; l.push_back(p);
        p.row = 5; p.col = 7; l.push_back(p);
      }

      if (c == 'i')
      {
        p.row = 0; p.col = 0; l.push_back(p);
        p.row = 0; p.col = 3; l.push_back(p);
        p.row = 2; p.col = 7; l.push_back(p);
        p.row = 4; p.col = 8; l.push_back(p);
        p.row = 5; p.col = 4; l.push_back(p);
        p.row = 6; p.col = 5; l.push_back(p);
        p.row = 7; p.col = 2; l.push_back(p);
      }

      if (c == 'j')
      {
        //      p.row = 0; p.col = 6; l.push_back(p);
      }

      if (c == 'k')
      {
        p.row = 9; p.col = 10; l.push_back(p);
      }

      if (c == 'l')
      {
        p.row = 0; p.col = 2; l.push_back(p);
        p.row = 3; p.col = 2; l.push_back(p);
        p.row = 7; p.col = 6; l.push_back(p);
        p.row = 9; p.col = 7; l.push_back(p);
      }

      if (c == 'm')
      {
        p.row = 0; p.col = 10; l.push_back(p);
        p.row = 0; p.col = 8; l.push_back(p);
      }

      if (c == 'n')
      {
        p.row = 2; p.col = 3; l.push_back(p);
        p.row = 3; p.col = 7; l.push_back(p);
        p.row = 4; p.col = 7; l.push_back(p);
        p.row = 4; p.col = 9; l.push_back(p);
        p.row = 5; p.col = 1; l.push_back(p);
        p.row = 7; p.col = 10; l.push_back(p);
        p.row = 8; p.col = 4; l.push_back(p);
        p.row = 9; p.col = 2; l.push_back(p);
      }

      if (c == 'o')
      {
        p.row = 3; p.col = 10; l.push_back(p);
        p.row = 5; p.col = 0; l.push_back(p);
        p.row = 6; p.col = 1; l.push_back(p);
        p.row = 6; p.col = 10; l.push_back(p);
        p.row = 9; p.col = 5; l.push_back(p);
        p.row = 9; p.col = 8; l.push_back(p);
      }

      if (c == 'p')
      {
        p.row = 0; p.col = 9; l.push_back(p);
        p.row = 4; p.col = 0; l.push_back(p);
      }

      if (c == 'q')
      {
        p.row = 1; p.col = 2; l.push_back(p);
      }

      if (c == 'r')
      {
        p.row = 1; p.col = 5; l.push_back(p);
        p.row = 4; p.col = 5; l.push_back(p);
        p.row = 5; p.col = 8; l.push_back(p);
        p.row = 6; p.col = 3; l.push_back(p);
      }

      if (c == 's')
      {
        p.row = 0; p.col = 4; l.push_back(p);
        p.row = 0; p.col = 6; l.push_back(p);
        p.row = 3; p.col = 4; l.push_back(p);
        p.row = 4; p.col = 2; l.push_back(p);
        p.row = 8; p.col = 0; l.push_back(p);
      }

      if (c == 't')
      {
        p.row = 0; p.col = 1; l.push_back(p);
        p.row = 1; p.col = 6; l.push_back(p);
        p.row = 2; p.col = 0; l.push_back(p);
        p.row = 2; p.col = 4; l.push_back(p);
        p.row = 3; p.col = 5; l.push_back(p);
        p.row = 3; p.col = 9; l.push_back(p);
        p.row = 4; p.col = 3; l.push_back(p);
        p.row = 5; p.col = 6; l.push_back(p);
        p.row = 6; p.col = 8; l.push_back(p);
        p.row = 7; p.col = 4; l.push_back(p);
        p.row = 8; p.col = 0; l.push_back(p);
      }

      if (c == 'u')
      {
        p.row = 0; p.col = 9; l.push_back(p);
        p.row = 1; p.col = 1; l.push_back(p);
        p.row = 2; p.col = 2; l.push_back(p);
        p.row = 2; p.col = 4; l.push_back(p);
        p.row = 3; p.col = 1; l.push_back(p);
        p.row = 4; p.col = 8; l.push_back(p);
        p.row = 5; p.col = 7; l.push_back(p);
        p.row = 7; p.col = 4; l.push_back(p);
        p.row = 8; p.col = 10; l.push_back(p);
      }

      if (c == 'v')
      {
        p.row = 2; p.col = 8; l.push_back(p);
        p.row = 6; p.col = 6; l.push_back(p);
        p.row = 7; p.col = 8; l.push_back(p);
        p.row = 8; p.col = 9; l.push_back(p);
      }

      if (c == 'w')
      {
        p.row = 2; p.col = 1; l.push_back(p);
        p.row = 6; p.col = 9; l.push_back(p);
        p.row = 8; p.col = 6; l.push_back(p);
      }

      if (c == 'x')
      {
        p.row = 2; p.col = 10; l.push_back(p);
        p.row = 5; p.col = 5; l.push_back(p);
      }

      if (c == 'y')
      {
        p.row = 2; p.col = 5; l.push_back(p);
      }

      if (c == 'z')
      {
        //      p.row = 5; p.col = 2; l.push_back(p);
      }

      return l;
    }
};
