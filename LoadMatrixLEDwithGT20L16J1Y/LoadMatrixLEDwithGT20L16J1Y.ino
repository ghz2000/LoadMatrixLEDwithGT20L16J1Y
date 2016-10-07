#include <SPI.h>
#include "libMatrix.h"
#include "libGT20L16J1Y.h"
#include "SJIS.h"

#define KROM_SS 8
#define PANEL_EN 7
#define RCK 6
#define SCK 13
#define MOSI 11
#define MISO 12

CGT20L GT20L; //漢字ROM

void setup(){
  pinMode(PANEL_EN, OUTPUT);
  pinMode(RCK, OUTPUT);
  digitalWrite(PANEL_EN, HIGH);
  
/*  //漢字ROMのinitでやる
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV64);
  SPI.setDataMode(SPI_MODE0);
*/

  GT20L.init(SCK, MOSI, MISO, KROM_SS);
  setData();  //SJIS文字を読み込む

  Serial.begin(115200);
}


void view(uint16_t tmp[]){
     digitalWrite(RCK, LOW);

    for(int i=15; 0<=i; i--){
      SPI.transfer((tmp[i] >> 8 ));    //GU
      SPI.transfer((tmp[i] & 0xFF));    //GU
    }
    digitalWrite(RCK, HIGH);   
}

void loop(){
  uint16_t vBuf[32];
  uint16_t buf[16];
  uint8_t  tmp[32];
  int strPos = 1;
  int cSize=0;
  int vBufCapa = 0;

  
  uint8_t *sendData = message1;
  GT20L.setStrings(sendData);
  memset(vBuf, 0, sizeof(vBuf));


while(1){

SCAN:
  while(vBufCapa <= 32){
    cSize = GT20L.getMatrixData(tmp, strPos++);
    if(cSize == 0){ strPos=1; goto SCAN;}
    convertMatrix(buf, tmp, cSize);
    memcpy(&vBuf[vBufCapa/2], buf, cSize);
    vBufCapa += cSize;
  }

//  viewBuf(vBuf);  // Debug
  view(vBuf);
  scroll(vBuf, &vBufCapa);

  delay(100);
  }
}

// for Debug
void viewBuf(const uint16_t *buf){
  char tmp[17];
  Serial.println("---------- vBuf ------------");
  for(int i=0; i<32; i++){
    sprintBin(tmp, buf[i]);
    Serial.println(tmp);
    if(i==16)Serial.println("----------------");
  }
}

// for Debug
void sprintBin(char *res, uint16_t comp){
  for(int i=0; i<16; i++){
    if((comp >> i) & 0x0001){
      res[i] = '8';
    }else{
      res[i] = ' ';
    }
  }
  res[16] = NULL;
}


// スクロールする
void scroll(uint16_t *vBuf, int *vBufCapa){
  for(int i=0; i<31; i++){
    vBuf[i]   = vBuf[i+1];
  }
  *vBufCapa -= 2;
}

// データと表示を一致させる
void convertMatrix(uint16_t tmp1[],  uint8_t tmp[], int charSize){
  switch(charSize){
    case 16:
      for(int i=0; i<8; i++){
        tmp1[i] = (tmp[i+8] << 8) + tmp[i];
      }
      break;
    case 32:
      for(int i=0; i<16 ;i++){
        tmp1[i] = (tmp[i+16] << 8) + tmp[i];
      }
      break;
    default :
    break;
  }
}



//コピーする
void copyView(const unsigned int input[], unsigned int output[]){
  for(int i=0; i<16; i++){
    output[i] = input[i];
  }
}

//表示を正しい向きに回転させる(Red)
void lotateView(const unsigned int input[], unsigned int tmp[]){
  
  for(int x=0; x<16; x++){
    for(int y=0; y<16; y++){
      tmp[x] = (tmp[x]<<1) | ((input[y]>>(16-x ))  &0x01);
    }
  }
}

//色を反転させる
void revCont(unsigned int tmp[]){
  for(int i=0; i<16; i++){
    tmp[i] = tmp[i] ^ 0xFFFF;
  }
}

//右にずらす
void sift(int sift, unsigned int tmp[], const unsigned int a[], const unsigned int b[]){
  
  for(int i=0; i<16; i++){
    if(i+sift < 16) tmp[i] = a[i+sift];
    else tmp[i] = b[ i+sift -16];
  }
}
