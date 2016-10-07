#include <Arduino.h>

uint8_t* message[5];
uint8_t  message1[150];
uint8_t  message2[50];
//uint8_t  message3[50];
//uint8_t  message4[50];
void setData(){
 message[0] = message1;
 message[1] = message2;
// message[2] = message3;
 //message[3] = message4;
 sprintf((char*)message1, "NT名古屋 当ビル4Fで開催中!! NT名古屋 この道を進んでエレベーターで4F! ニコニコ技術部 年に一度のイベント開催中!! ");
 //sprintf((char*)message3, "Arduino");
 //sprintf((char*)message4, "萌え萌えキュン!!");
  message2[0] = 0x82;
  message2[1] = 0xA0;
  message2[2] = 0x82;
  message2[3] = 0xA2;
  message2[4] = 0x82;
  message2[5] = 0xA4;
  message2[6] = 0x00;
}


