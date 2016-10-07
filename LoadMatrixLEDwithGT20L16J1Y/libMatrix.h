/***********
***********/

#ifndef _libMatrix_
#define _libMatrix_

#include <Arduino.h>

//#define sign_  0x
// 0x8000 表示用終端文字


#define __LEN 100

class Cmatrix{
private:
public:
  uint16_t m_view[8];
  uint16_t m_str[__LEN];
  static const uint16_t ascii[];
  int m_RCK;
  int m_kido;
  int m_ViewLength;
  int m_speed;
  

public:
  Cmatrix();
  void init(int RCK, int kido, int ViewLength);
  void update();
  void setChar(char *);
  void putnum();
  void cls();
  void showStop();
  void scroll();
  void setRCK(int RCK);
  void setBlight(int kido);
  void setViewLength(int ViewLength);
  void setScrollSpeed(int scrollSpeed);
  void showCycleX();
  void showCycleO();

};

extern Cmatrix LEDmatrix;

#endif
