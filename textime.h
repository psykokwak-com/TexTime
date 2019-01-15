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
  TextTimeBlob _hours[13];
  TextTimeBlob _hour1;
  TextTimeBlob _hour2;
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
    _it.pixels[1].row = 0; _it.pixels[1].col = 1; // L

    _is.number = 3;
    _is.pixels[0].row = 0; _is.pixels[0].col = 3; // E
    _is.pixels[1].row = 0; _is.pixels[1].col = 4; // S
    _is.pixels[2].row = 0; _is.pixels[2].col = 5; // T

    _hours[0].number = 6;
    _hours[0].pixels[0].row = 4; _hours[0].pixels[0].col = 5;  // M
    _hours[0].pixels[1].row = 4; _hours[0].pixels[1].col = 6;  // I
    _hours[0].pixels[2].row = 4; _hours[0].pixels[2].col = 7;  // N
    _hours[0].pixels[3].row = 4; _hours[0].pixels[3].col = 8;  // U
    _hours[0].pixels[4].row = 4; _hours[0].pixels[4].col = 9;  // I
    _hours[0].pixels[5].row = 4; _hours[0].pixels[5].col = 10; // T

    _hours[1].number = 3;
    _hours[1].pixels[0].row = 2; _hours[1].pixels[0].col = 4;  // U
    _hours[1].pixels[1].row = 2; _hours[1].pixels[1].col = 5;  // N
    _hours[1].pixels[2].row = 2; _hours[1].pixels[2].col = 6;  // E

    _hours[2].number = 4;
    _hours[2].pixels[0].row = 0; _hours[2].pixels[0].col = 7;  // D
    _hours[2].pixels[1].row = 0; _hours[2].pixels[1].col = 8;  // E
    _hours[2].pixels[2].row = 0; _hours[2].pixels[2].col = 9;  // U
    _hours[2].pixels[3].row = 0; _hours[2].pixels[3].col = 10; // X

    _hours[3].number = 5;
    _hours[3].pixels[0].row = 1; _hours[3].pixels[0].col = 6;  // T
    _hours[3].pixels[1].row = 1; _hours[3].pixels[1].col = 7;  // R
    _hours[3].pixels[2].row = 1; _hours[3].pixels[2].col = 8;  // O
    _hours[3].pixels[3].row = 1; _hours[3].pixels[3].col = 9;  // I
    _hours[3].pixels[4].row = 1; _hours[3].pixels[4].col = 10; // S

    _hours[4].number = 6;
    _hours[4].pixels[0].row = 1; _hours[4].pixels[0].col = 0;  // Q
    _hours[4].pixels[1].row = 1; _hours[4].pixels[1].col = 1;  // U
    _hours[4].pixels[2].row = 1; _hours[4].pixels[2].col = 2;  // A
    _hours[4].pixels[3].row = 1; _hours[4].pixels[3].col = 3;  // T
    _hours[4].pixels[4].row = 1; _hours[4].pixels[4].col = 4;  // R
    _hours[4].pixels[5].row = 1; _hours[4].pixels[5].col = 5;  // E

    _hours[5].number = 4;
    _hours[5].pixels[0].row = 3; _hours[5].pixels[0].col = 7;  // C
    _hours[5].pixels[1].row = 3; _hours[5].pixels[1].col = 8;  // I
    _hours[5].pixels[2].row = 3; _hours[5].pixels[2].col = 9;  // N
    _hours[5].pixels[3].row = 3; _hours[5].pixels[3].col = 10; // Q

    _hours[6].number = 3;
    _hours[6].pixels[0].row = 3; _hours[6].pixels[0].col = 4;  // S
    _hours[6].pixels[1].row = 3; _hours[6].pixels[1].col = 5;  // I
    _hours[6].pixels[2].row = 3; _hours[6].pixels[2].col = 6;  // X

    _hours[7].number = 4;
    _hours[7].pixels[0].row = 2; _hours[7].pixels[0].col = 7;  // S
    _hours[7].pixels[1].row = 2; _hours[7].pixels[1].col = 8;  // E
    _hours[7].pixels[2].row = 2; _hours[7].pixels[2].col = 9;  // P
    _hours[7].pixels[3].row = 2; _hours[7].pixels[3].col = 10; // T

    _hours[8].number = 4;
    _hours[8].pixels[0].row = 3; _hours[8].pixels[0].col = 0;  // H
    _hours[8].pixels[1].row = 3; _hours[8].pixels[1].col = 1;  // U
    _hours[8].pixels[2].row = 3; _hours[8].pixels[2].col = 2;  // I
    _hours[8].pixels[3].row = 3; _hours[8].pixels[3].col = 3;  // T

    _hours[9].number = 4;
    _hours[9].pixels[0].row = 2; _hours[9].pixels[0].col = 0;  // N
    _hours[9].pixels[1].row = 2; _hours[9].pixels[1].col = 1;  // E
    _hours[9].pixels[2].row = 2; _hours[9].pixels[2].col = 2;  // U
    _hours[9].pixels[3].row = 2; _hours[9].pixels[3].col = 3;  // F

    _hours[10].number = 3;
    _hours[10].pixels[0].row = 4; _hours[10].pixels[0].col = 2;  // D
    _hours[10].pixels[1].row = 4; _hours[10].pixels[1].col = 3;  // I
    _hours[10].pixels[2].row = 4; _hours[10].pixels[2].col = 4;  // X

    _hours[11].number = 4;
    _hours[11].pixels[0].row = 5; _hours[11].pixels[0].col = 0;  // O
    _hours[11].pixels[1].row = 5; _hours[11].pixels[1].col = 1;  // N
    _hours[11].pixels[2].row = 5; _hours[11].pixels[2].col = 2;  // Z
    _hours[11].pixels[3].row = 5; _hours[11].pixels[3].col = 3;  // E

    _hours[12].number = 4;
    _hours[12].pixels[0].row = 4; _hours[12].pixels[0].col = 0;  // M
    _hours[12].pixels[1].row = 4; _hours[12].pixels[1].col = 1;  // I
    _hours[12].pixels[2].row = 4; _hours[12].pixels[2].col = 2;  // D
    _hours[12].pixels[3].row = 4; _hours[12].pixels[3].col = 3;  // I

    _hour1.number = 5;
    _hour1.pixels[0].row = 5; _hour1.pixels[0].col = 5;  // H
    _hour1.pixels[1].row = 5; _hour1.pixels[1].col = 6;  // E
    _hour1.pixels[2].row = 5; _hour1.pixels[2].col = 7;  // U
    _hour1.pixels[3].row = 5; _hour1.pixels[3].col = 8;  // R
    _hour1.pixels[4].row = 5; _hour1.pixels[4].col = 9;  // E
 
    _hour2.number = 6;
    _hour2.pixels[0].row = 5; _hour2.pixels[0].col = 5;  // H
    _hour2.pixels[1].row = 5; _hour2.pixels[1].col = 6;  // E
    _hour2.pixels[2].row = 5; _hour2.pixels[2].col = 7;  // U
    _hour2.pixels[3].row = 5; _hour2.pixels[3].col = 8;  // R
    _hour2.pixels[4].row = 5; _hour2.pixels[4].col = 9;  // E
    _hour2.pixels[5].row = 5; _hour2.pixels[5].col = 10; // S

    _minutes[0].number = 0; // between [0:4]

    _minutes[1].number = 4; // between [5:9]
    _minutes[1].pixels[0].row = 8; _minutes[1].pixels[0].col = 6;  // C
    _minutes[1].pixels[1].row = 8; _minutes[1].pixels[1].col = 7;  // I
    _minutes[1].pixels[2].row = 8; _minutes[1].pixels[2].col = 8;  // N
    _minutes[1].pixels[3].row = 8; _minutes[1].pixels[3].col = 9;  // Q

    _minutes[2].number = 3; // between [10:14]
    _minutes[2].pixels[0].row = 6; _minutes[2].pixels[0].col = 8;  // D
    _minutes[2].pixels[1].row = 6; _minutes[2].pixels[1].col = 9;  // I
    _minutes[2].pixels[2].row = 6; _minutes[2].pixels[2].col = 10; // X

    _minutes[3].number = 7; // between [15:19]
    _minutes[3].pixels[0].row = 7; _minutes[3].pixels[0].col = 0;  // E
    _minutes[3].pixels[1].row = 7; _minutes[3].pixels[1].col = 1;  // T
    _minutes[3].pixels[2].row = 7; _minutes[3].pixels[2].col = 3;  // Q
    _minutes[3].pixels[3].row = 7; _minutes[3].pixels[3].col = 4;  // U
    _minutes[3].pixels[4].row = 7; _minutes[3].pixels[4].col = 5;  // A
    _minutes[3].pixels[5].row = 7; _minutes[3].pixels[5].col = 6;  // R
    _minutes[3].pixels[6].row = 7; _minutes[3].pixels[6].col = 7;  // T

    _minutes[4].number = 5; // between [20:24]
    _minutes[4].pixels[0].row = 8; _minutes[4].pixels[0].col = 0;  // V
    _minutes[4].pixels[1].row = 8; _minutes[4].pixels[1].col = 1;  // I
    _minutes[4].pixels[2].row = 8; _minutes[4].pixels[2].col = 2;  // N
    _minutes[4].pixels[3].row = 8; _minutes[4].pixels[3].col = 3;  // G
    _minutes[4].pixels[4].row = 8; _minutes[4].pixels[4].col = 4;  // T

    _minutes[5].number = 10;// between [25:29]
    _minutes[5].pixels[0].row = 8; _minutes[5].pixels[0].col = 0;  // V
    _minutes[5].pixels[1].row = 8; _minutes[5].pixels[1].col = 1;  // I
    _minutes[5].pixels[2].row = 8; _minutes[5].pixels[2].col = 2;  // N
    _minutes[5].pixels[3].row = 8; _minutes[5].pixels[3].col = 3;  // G
    _minutes[5].pixels[4].row = 8; _minutes[5].pixels[4].col = 4;  // T
    _minutes[5].pixels[5].row = 8; _minutes[5].pixels[5].col = 5;  // -
    _minutes[5].pixels[6].row = 8; _minutes[5].pixels[6].col = 6;  // C
    _minutes[5].pixels[7].row = 8; _minutes[5].pixels[7].col = 7;  // I
    _minutes[5].pixels[8].row = 8; _minutes[5].pixels[8].col = 8;  // N
    _minutes[5].pixels[9].row = 8; _minutes[5].pixels[9].col = 9;  // Q

    _minutes[6].number = 7; // between [30:34]
    _minutes[6].pixels[0].row = 9; _minutes[6].pixels[0].col = 0;  // E
    _minutes[6].pixels[1].row = 9; _minutes[6].pixels[1].col = 1;  // T
    _minutes[6].pixels[2].row = 9; _minutes[6].pixels[2].col = 3;  // D
    _minutes[6].pixels[3].row = 9; _minutes[6].pixels[3].col = 4;  // E
    _minutes[6].pixels[4].row = 9; _minutes[6].pixels[4].col = 5;  // M
    _minutes[6].pixels[5].row = 9; _minutes[6].pixels[5].col = 6;  // I
    _minutes[6].pixels[6].row = 9; _minutes[6].pixels[6].col = 7;  // E

    _minutes[7].number = 15; // between [35:39]
    _minutes[7].pixels[0].row = 6; _minutes[7].pixels[0].col = 0;  // M
    _minutes[7].pixels[1].row = 6; _minutes[7].pixels[1].col = 1;  // O
    _minutes[7].pixels[2].row = 6; _minutes[7].pixels[2].col = 2;  // I
    _minutes[7].pixels[3].row = 6; _minutes[7].pixels[3].col = 3;  // N
    _minutes[7].pixels[4].row = 6; _minutes[7].pixels[4].col = 4;  // S
    _minutes[7].pixels[5].row = 8; _minutes[7].pixels[5].col = 0;  // V
    _minutes[7].pixels[6].row = 8; _minutes[7].pixels[6].col = 1;  // I
    _minutes[7].pixels[7].row = 8; _minutes[7].pixels[7].col = 2;  // N
    _minutes[7].pixels[8].row = 8; _minutes[7].pixels[8].col = 3;  // G
    _minutes[7].pixels[9].row = 8; _minutes[7].pixels[9].col = 4;  // T
    _minutes[7].pixels[10].row = 8; _minutes[7].pixels[10].col = 5;// -
    _minutes[7].pixels[11].row = 8; _minutes[7].pixels[11].col = 6;// C
    _minutes[7].pixels[12].row = 8; _minutes[7].pixels[12].col = 7;// I
    _minutes[7].pixels[13].row = 8; _minutes[7].pixels[13].col = 8;// N
    _minutes[7].pixels[14].row = 8; _minutes[7].pixels[14].col = 9;// Q

    _minutes[8].number = 10; // between [40:44]
    _minutes[8].pixels[0].row = 6; _minutes[8].pixels[0].col = 0;  // M
    _minutes[8].pixels[1].row = 6; _minutes[8].pixels[1].col = 1;  // O
    _minutes[8].pixels[2].row = 6; _minutes[8].pixels[2].col = 2;  // I
    _minutes[8].pixels[3].row = 6; _minutes[8].pixels[3].col = 3;  // N
    _minutes[8].pixels[4].row = 6; _minutes[8].pixels[4].col = 4;  // S
    _minutes[8].pixels[5].row = 8; _minutes[8].pixels[5].col = 0;  // V
    _minutes[8].pixels[6].row = 8; _minutes[8].pixels[6].col = 1;  // I
    _minutes[8].pixels[7].row = 8; _minutes[8].pixels[7].col = 2;  // N
    _minutes[8].pixels[8].row = 8; _minutes[8].pixels[8].col = 3;  // G
    _minutes[8].pixels[9].row = 8; _minutes[8].pixels[9].col = 4;  // T

    _minutes[9].number = 12; // between [45:49]
    _minutes[9].pixels[0].row = 6; _minutes[9].pixels[0].col = 0;  // M
    _minutes[9].pixels[1].row = 6; _minutes[9].pixels[1].col = 1;  // O
    _minutes[9].pixels[2].row = 6; _minutes[9].pixels[2].col = 2;  // I
    _minutes[9].pixels[3].row = 6; _minutes[9].pixels[3].col = 3;  // N
    _minutes[9].pixels[4].row = 6; _minutes[9].pixels[4].col = 4;  // S
    _minutes[9].pixels[5].row = 6; _minutes[9].pixels[5].col = 6;  // L
    _minutes[9].pixels[6].row = 6; _minutes[9].pixels[6].col = 7;  // E
    _minutes[9].pixels[7].row = 7; _minutes[9].pixels[7].col = 3;  // Q
    _minutes[9].pixels[8].row = 7; _minutes[9].pixels[8].col = 4;  // U
    _minutes[9].pixels[9].row = 7; _minutes[9].pixels[9].col = 5;  // A
    _minutes[9].pixels[10].row = 7; _minutes[9].pixels[10].col = 6;// R
    _minutes[9].pixels[11].row = 7; _minutes[9].pixels[11].col = 7;// T

    _minutes[10].number = 8; // between [50:54]
    _minutes[10].pixels[0].row = 6; _minutes[10].pixels[0].col = 0;  // M
    _minutes[10].pixels[1].row = 6; _minutes[10].pixels[1].col = 1;  // O
    _minutes[10].pixels[2].row = 6; _minutes[10].pixels[2].col = 2;  // I
    _minutes[10].pixels[3].row = 6; _minutes[10].pixels[3].col = 3;  // N
    _minutes[10].pixels[4].row = 6; _minutes[10].pixels[4].col = 4;  // S
    _minutes[10].pixels[5].row = 6; _minutes[10].pixels[5].col = 8;  // D
    _minutes[10].pixels[6].row = 6; _minutes[10].pixels[6].col = 9;  // I
    _minutes[10].pixels[7].row = 6; _minutes[10].pixels[7].col = 10; // X

    _minutes[11].number = 9; // between [55:59]
    _minutes[11].pixels[0].row = 6; _minutes[11].pixels[0].col = 0;  // M
    _minutes[11].pixels[1].row = 6; _minutes[11].pixels[1].col = 1;  // O
    _minutes[11].pixels[2].row = 6; _minutes[11].pixels[2].col = 2;  // I
    _minutes[11].pixels[3].row = 6; _minutes[11].pixels[3].col = 3;  // N
    _minutes[11].pixels[4].row = 6; _minutes[11].pixels[4].col = 4;  // S
    _minutes[11].pixels[5].row = 8; _minutes[11].pixels[5].col = 6;  // C
    _minutes[11].pixels[6].row = 8; _minutes[11].pixels[6].col = 7;  // I
    _minutes[11].pixels[7].row = 8; _minutes[11].pixels[7].col = 8;  // N
    _minutes[11].pixels[8].row = 8; _minutes[11].pixels[8].col = 9;  // Q

    _minutes[12].number = 6; // between [30:34] (special for midday and midnight)
    _minutes[12].pixels[0].row = 9; _minutes[12].pixels[0].col = 0;  // E
    _minutes[12].pixels[1].row = 9; _minutes[12].pixels[1].col = 1;  // T
    _minutes[12].pixels[2].row = 9; _minutes[12].pixels[2].col = 3;  // D
    _minutes[12].pixels[3].row = 9; _minutes[12].pixels[3].col = 4;  // E
    _minutes[12].pixels[4].row = 9; _minutes[12].pixels[4].col = 5;  // M
    _minutes[12].pixels[5].row = 9; _minutes[12].pixels[5].col = 6;  // I
  }

  virtual TextTimeBlobs getBlobsFromTime(int hour, int minute)
  {
    TextTimeBlobs b;

    b.number = 0;

    if (hour > 23) return b;
    if (minute > 59) return b;

    b.blobs[b.number++] = &_it;
    b.blobs[b.number++] = &_is;

    if (minute >= 35)
      hour = hour + 1;
    if (hour > 23) hour = 0;

    b.blobs[b.number++] = (hour == 12) ? &(_hours[12]) : &(_hours[hour % 12]);

    if (hour != 0 && hour != 12)
      b.blobs[b.number++] = (hour == 1 || hour == 13) ? &_hour1 : &_hour2;

    b.blobs[b.number++] = ((hour == 0 || hour == 12) && (minute / 5 == 6)) ? &(_minutes[12]) : &(_minutes[minute / 5]);

    return b;
  }

  virtual cl_Lst<TextTimePixel> getPixelsFromLetter(char c)
  {
    cl_Lst<TextTimePixel> l;
    TextTimePixel p;

    if (c == 'a')
    {
      p.row = 1; p.col = 2; l.push_back(p);
      p.row = 7; p.col = 5; l.push_back(p);
      p.row = 9; p.col = 9; l.push_back(p);
    }

    if (c == 'b')
    {
      p.row = 0; p.col = 2; l.push_back(p);
    }

    if (c == 'c')
    {
      p.row = 3; p.col = 7; l.push_back(p);
      p.row = 8; p.col = 6; l.push_back(p);
    }

    if (c == 'd')
    {
      p.row = 0; p.col = 7; l.push_back(p);
      p.row = 4; p.col = 2; l.push_back(p);
      p.row = 6; p.col = 8; l.push_back(p);
      p.row = 9; p.col = 3; l.push_back(p);
    }

    if (c == 'e')
    {
      p.row = 0; p.col = 3; l.push_back(p);
      p.row = 0; p.col = 8; l.push_back(p);
      p.row = 1; p.col = 5; l.push_back(p);
      p.row = 2; p.col = 1; l.push_back(p);
      p.row = 2; p.col = 6; l.push_back(p);
      p.row = 2; p.col = 8; l.push_back(p);
      p.row = 5; p.col = 3; l.push_back(p);
      p.row = 5; p.col = 6; l.push_back(p);
      p.row = 5; p.col = 9; l.push_back(p);
      p.row = 6; p.col = 7; l.push_back(p);
      p.row = 7; p.col = 0; l.push_back(p);
      p.row = 9; p.col = 0; l.push_back(p);
      p.row = 9; p.col = 4; l.push_back(p);
      p.row = 9; p.col = 7; l.push_back(p);
    }

    if (c == 'f')
    {
      p.row = 2; p.col = 3; l.push_back(p);
    }

    if (c == 'g')
    {
      p.row = 8; p.col = 3; l.push_back(p);
    }

    if (c == 'h')
    {
      p.row = 3; p.col = 0; l.push_back(p);
      p.row = 5; p.col = 5; l.push_back(p);
    }

    if (c == 'i')
    {
      p.row = 0; p.col = 0; l.push_back(p);
      p.row = 1; p.col = 9; l.push_back(p);
      p.row = 3; p.col = 2; l.push_back(p);
      p.row = 3; p.col = 5; l.push_back(p);
      p.row = 3; p.col = 8; l.push_back(p);
      p.row = 4; p.col = 1; l.push_back(p);
      p.row = 4; p.col = 3; l.push_back(p);
      p.row = 4; p.col = 6; l.push_back(p);
      p.row = 4; p.col = 9; l.push_back(p);
      p.row = 6; p.col = 2; l.push_back(p);
      p.row = 6; p.col = 9; l.push_back(p);
      p.row = 8; p.col = 1; l.push_back(p);
      p.row = 8; p.col = 7; l.push_back(p);
      p.row = 9; p.col = 6; l.push_back(p);
    }

    if (c == 'j')
    {
      p.row = 0; p.col = 6; l.push_back(p);
    }

    if (c == 'k')
    {
      p.row = 5; p.col = 4; l.push_back(p);
    }

    if (c == 'l')
    {
      p.row = 0; p.col = 1; l.push_back(p);
      p.row = 6; p.col = 6; l.push_back(p);
    }

    if (c == 'm')
    {
      p.row = 4; p.col = 0; l.push_back(p);
      p.row = 4; p.col = 5; l.push_back(p);
      p.row = 6; p.col = 0; l.push_back(p);
      p.row = 7; p.col = 9; l.push_back(p);
      p.row = 9; p.col = 5; l.push_back(p);
      p.row = 9; p.col = 10; l.push_back(p);
    }

    if (c == 'n')
    {
      p.row = 2; p.col = 0; l.push_back(p);
      p.row = 2; p.col = 5; l.push_back(p);
      p.row = 3; p.col = 9; l.push_back(p);
      p.row = 4; p.col = 7; l.push_back(p);
      p.row = 5; p.col = 1; l.push_back(p);
      p.row = 6; p.col = 3; l.push_back(p);
      p.row = 8; p.col = 2; l.push_back(p);
      p.row = 8; p.col = 8; l.push_back(p);
    }

    if (c == 'o')
    {
      p.row = 1; p.col = 8; l.push_back(p);
      p.row = 5; p.col = 0; l.push_back(p);
      p.row = 6; p.col = 1; l.push_back(p);
    }

    if (c == 'p')
    {
      p.row = 2; p.col = 9; l.push_back(p);
      p.row = 7; p.col = 8; l.push_back(p);
      p.row = 9; p.col = 8; l.push_back(p);
    }

    if (c == 'q')
    {
      p.row = 1; p.col = 0; l.push_back(p);
      p.row = 3; p.col = 10; l.push_back(p);
      p.row = 7; p.col = 3; l.push_back(p);
      p.row = 8; p.col = 9; l.push_back(p);
    }

    if (c == 'r')
    {
      p.row = 1; p.col = 4; l.push_back(p);
      p.row = 1; p.col = 7; l.push_back(p);
      p.row = 5; p.col = 8; l.push_back(p);
      p.row = 7; p.col = 2; l.push_back(p);
      p.row = 7; p.col = 6; l.push_back(p);
    }

    if (c == 's')
    {
      p.row = 0; p.col = 4; l.push_back(p);
      p.row = 1; p.col = 10; l.push_back(p);
      p.row = 2; p.col = 7; l.push_back(p);
      p.row = 3; p.col = 4; l.push_back(p);
      p.row = 5; p.col = 10; l.push_back(p);
      p.row = 6; p.col = 4; l.push_back(p);
      p.row = 9; p.col = 2; l.push_back(p);
    }

    if (c == 't')
    {
      p.row = 0; p.col = 5; l.push_back(p);
      p.row = 1; p.col = 3; l.push_back(p);
      p.row = 1; p.col = 6; l.push_back(p);
      p.row = 2; p.col = 10; l.push_back(p);
      p.row = 3; p.col = 3; l.push_back(p);
      p.row = 4; p.col = 10; l.push_back(p);
      p.row = 7; p.col = 1; l.push_back(p);
      p.row = 7; p.col = 7; l.push_back(p);
      p.row = 8; p.col = 4; l.push_back(p);
      p.row = 9; p.col = 1; l.push_back(p);
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
      p.row = 8; p.col = 0; l.push_back(p);
    }

    if (c == 'w')
    {
      p.row = 6; p.col = 5; l.push_back(p);
    }

    if (c == 'x')
    {
      p.row = 0; p.col = 10; l.push_back(p);
      p.row = 3; p.col = 6; l.push_back(p);
      p.row = 4; p.col = 4; l.push_back(p);
      p.row = 6; p.col = 10; l.push_back(p);
    }

    if (c == 'y')
    {
      p.row = 7; p.col = 10; l.push_back(p);
    }

    if (c == 'z')
    {
      p.row = 5; p.col = 2; l.push_back(p);
    }

    return l;
  }
};