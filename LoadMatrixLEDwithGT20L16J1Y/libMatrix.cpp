#include "libMatrix.h"
#include <SPI.h>

Cmatrix::Cmatrix(){
  bitSet(TIMSK2, TOIE2);
  
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setDataMode(SPI_MODE0);
  
  m_speed = 0;
}

void Cmatrix::init(int RCK, int kido, int ViewLength){
  setRCK(RCK);
  setBlight(kido);
  setViewLength(ViewLength);
}

void Cmatrix::setRCK(int RCK){
  m_RCK = RCK;
}

void Cmatrix::setBlight(int kido){
  m_kido = kido;
}

void Cmatrix::setScrollSpeed(int scrollSpeed){
  m_speed = scrollSpeed;
}

void Cmatrix::setViewLength(int ViewLength){
  m_ViewLength = ViewLength-1;
}

void Cmatrix::setChar(char *input){
  int i=0;
  memset(m_str, 0, __LEN * 2);
  for(; i<__LEN -1 && input[i]!=NULL; i++){
    m_str[i] = ascii[ input[i] - ' '+1];    //変な文字が来た時対策をしてない
  }
  m_str[i] = 0x8000;
}

void Cmatrix::showStop(){
  for(int i=0; i<m_ViewLength +1; i++){
    m_view[m_ViewLength - i] = m_str[i];  //文字が少ない時にバグるんじゃないかな
  }
  m_speed = 0;
}

void Cmatrix::scroll(){
  static int scroll;
  for(int i=0; i<m_ViewLength +1; i++){
    
    //スクロールするときは桁数分ずらして空白からスタートする｡
    if(i + scroll < m_ViewLength){
      //空白を入れる
      m_view[m_ViewLength - i] = 0x00;
    }else{
      //文字をスクロール用にセットする
      m_view[m_ViewLength - i] = m_str[i+scroll-m_ViewLength];  //文字が少ない時にバグるんじゃないかな

      //全部通り過ぎたところと 0x8000(表示用終端文字)を比較して一致したらスクロールをリセット
//      if(m_str[i+scroll-m_ViewLength-m_ViewLength] == 0x8000) scroll=-m_ViewLength-1;
      if(m_str[i+scroll-m_ViewLength-m_ViewLength] == 0x8000) scroll=-m_ViewLength;
    }
  }
  scroll++;
}

void Cmatrix::putnum(){
}

void Cmatrix::cls(){
}

uint16_t cycleX[8] = {0x1000, 0x2000, 0x0400, 0x0002, 0x0004, 0x0008, 0x0010, 0x0800 };
uint16_t cycleO[6] = {0x0200, 0x4000, 0x0001, 0x0020, 0x0080, 0x0100 };

//割り込みルーチン
void Cmatrix::update(){
  static uint8_t row;
  static int count;

    digitalWrite(m_RCK, LOW);

    SPI.transfer((0xFF & ~(1<<row)) );
//    SPI.transfer(m_kido << 4 | (0x0F & ~(1<<row)) );

    SPI.transfer(m_view[row] >> 8 & 0xFF );
    SPI.transfer(m_view[row] & 0xFF);

    digitalWrite(m_RCK, HIGH);
    
    if(m_speed){
      if(m_speed < count){
        scroll();
        count = 0;
      }
      count++;
    }
    
    row++;
//    row &=0x03;  //4桁
    row &=0x07;  //8桁
}

  
Cmatrix LEDmatrix;


//タイマオーバーフロー割り込みを設定
ISR(TIMER2_OVF_vect){
//  LEDmatrix.update();
}

