/* 音数 */
#define STEPNUM (16)

/* BPM計算の基準音符 */
/* 1:全音符, 2:二分音符, 4:四分音符, 8:八分音符, 16:十六分音符 */
#define BASESIGN (8)

/*
 * アナログ入力PIN定義
 * ----------------------------
 * A0~A5:アナログ入力PIN
 * A6:デジタルピン4
 * A7:デジタルピン6
 * A8:デジタルピン8
 * A9:デジタルピン9
 * A10:デジタルピン10
 * A11:デジタルピン12
 */
/* タイミングInput用PIN */
const int STEPPIN[] = {0, 1, 2, 3, 4, 5, 6, 7, 8 ,9 ,10 ,11 ,12, 13, 14, 15};


/* オシレーターInput用PIN */
// const int OSCILLATORPIN = 8;

/* 音階Input用PIN */
const int OCTAVEPIN = 9;

/* DelayInput用PIN */
const int DELAYPIN = 10;

/* BPMInput用PIN */
const int BPMPIN = 11;

/*
 * デジタル入出力PIN定義
 * ----------------------------
 * 4, 6, 8, 9, 10, 12はアナログ入力として
 * 使用予定なので、使用しない
 */
/* LED Output用PIN番号 */
const int  LEDPIN[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

/* ループカウント */
int gLoopCnt = 0;

/* MaxからOSCで送られてくるカウンター変数 */
int maxLoopCount = 0;

/* BPMカウント */
int gBpm = 60;
bool isStart = true;



/*
 * データ保存
 */

/* BPMベースの待ち時間 */
int gWaitValue = 1000;


/*
 * setup
 */
void setup( ){
  int sensorCnt = 0;
  int input = 0;

  /* LED表示用PINの設定 */
  for (sensorCnt = 0; sensorCnt < STEPNUM; sensorCnt++) {
    pinMode(LEDPIN[sensorCnt], OUTPUT);
  }

  /* カウンターリセット */
  gLoopCnt = 0;

  /* BPMリセット */
  // gBpm = 60;

  /* シリアルポートオープン */
  Serial.begin(115200);


  // while(true) {
  //   input = Serial.read();
  //   if (1 == input) {
  //     isStart = true;
  //     break;
  //   }
  // }
}


/*
 * loop
 */
void loop(){
  int sensorCnt = 0;
  int steps[STEPNUM];
  int bpm = 0;
  int octave = 0;
  int waitTime = 0;
  int delayNum = 0;
  int ocsillator = 0;
  int input = 0;
  bool isSound = true;

  int cnt;

  /* 初期化 */
  memset(steps, 0, STEPNUM * sizeof(byte));

  /* LED消灯 */
  /* LEDを点灯させた状態だと可変抵抗の値がうまく取得できなかったので、
   * 各センサー値取得前に消灯しさせておく
   */
  turnOffLED();

  /* 同期のメッセージを受けたら強制的にcount0からスタート */
  if (Serial.available() > 0) {

    /* Processingからシリアル通信で送られるカウントを保存 */
    maxLoopCount = Serial.read();


    // if (1 == gLoopCnt) {
    //   //Start
    //   gLoopCnt = 0;
    //   isStart = true;
    // }
    // else if (3 == gLoopCnt) {
    //   //Stopq
    //   isStart = false;
    // }
  }
  

//  if (false == isStart) {
//    return;
//  }

  /* 音階の取得 */
  getStepNum(steps, STEPNUM);

  /* BPM取得 */
  // bpm = getBPM();

  /* オクターブ取得 */
  // octave = getOctave();

  /* ディレイ取得 */
  // delayNum = getDelay();

  /* オシレーター取得 */
  // ocsillator = getOscillator();

  /* BPM -> msec変換 */
  // waitTime = bpmToMSec(gBpm);
  
  /* BPM最速の時の待機時間より早い場合は補正する */
  // if (75 > waitTime) {
  //   waitTime = 75;
  // }


  /* データをProcessingに送信 */
  sendData(steps, STEPNUM, octave, delayNum, bpm, ocsillator);

  /* LED表示 */
  // showLED(gLoopCnt % STEPNUM);
  showLED(maxLoopCount);


  /* 4周したらBPM変更 */
  // gLoopCnt++;
//  if ((STEPNUM * 4) <= gLoopCnt) {
//
//    gLoopCnt = 0;
//  }

  /* BPMに合わせて待機 */
  // delay(waitTime);
}


/*
 * Name : turnOffLED
 * description: LED点灯
 * --------------------------------------------
 * argument
 *  none
 * retturn
 *  none
 * --------------------------------------------
 */
void turnOffLED() {
  int sensorCnt = 0;
  int ledPin = -1;

  /* 全てのLEDをOFF */
  for (sensorCnt = 0; sensorCnt < STEPNUM; sensorCnt++) {
    digitalWrite(LEDPIN[sensorCnt], LOW);
  }
}


/*
 * Name : showLED
 * description: LED点灯
 * --------------------------------------------
 * argument
 *  [in]  int   aCount  点灯させるLED番号
 * retturn
 *  none
 * --------------------------------------------
 */
void showLED(int aCount) {
  int sensorCnt = 0;
  // int ledPin = 0;

  /* 必要なものだけ点灯 */
  // ledPin = LEDPIN[aCount];
  digitalWrite(LEDPIN[aCount], HIGH);
}


/*
 * Name : getStepNum
 * description: センサー値から各ステップの音階を取得
 * --------------------------------------------
 * argument
 *  [out] int*  pStepBuff センサ値保存領域のアドレス
 *  [in]  int   buffSize  センサ値保存領域のサイズ
 * retturn
 *  none
 * --------------------------------------------
 */
void getStepNum(int* pStepBuff, int buffSize) {
  int sensorCnt = 0;
  int sensorValue = 0;
  int roundValue = 0;

  /* パラメータチェック */
  if (NULL == pStepBuff) {
    return;
  }

  /* 音階の値を取得 */
  for (sensorCnt = 0; sensorCnt < buffSize; sensorCnt++) {
    sensorValue = analogRead(STEPPIN[sensorCnt]);
    /* センサー値を0~8の値に丸めて保存 */
    *(pStepBuff + sensorCnt) = roundAnalogToDigital(sensorValue);
  }

  return;
}


/*
 * Name : getTempo
 * description: センサー値からBPMを取得
 * --------------------------------------------
 * argument
 *  none
 * retturn
 *  bpm         BPM値
 * --------------------------------------------
 */
//int getTempo() {
//  int value = 0;
//  int tempo = 0;
//
//  /* センサー値取得を取得して60、120、240に変換 */
//  value = analogRead(TEMPOPIN);
//  tempo = roundAnalogToDigital(value);
////  bpm = int(float(value) * (180 - 40) / 1023 + 40);
//
//  return tempo;
//}


/*
 * Name : getBPM
 * description: センサー値からBPMを取得
 * --------------------------------------------
 * argument
 *  none
 * retturn
 *  bpm         BPM値
 * --------------------------------------------
 */
int getBPM() {
  int value = 0;
  int bpm = 0;

  /* センサー値取得を取得して60、120、240に変換 */
  value = analogRead(BPMPIN);
//  Serial.println(value);
//  bpm = map(value, 0, 1023, 60, 240);
  
  bpm = int(float(value) * (240 - 60) / 1023 + 60);
//  Serial.println(bpm);

  return bpm;
}


/*
 * Name : getWaitTimeByBPM
 * description: BPMからmsecに変換
 * --------------------------------------------
 * argument
 *  [in]  int   bpm
 * retturn
 *  msec        待機時間(msec)
 * --------------------------------------------
 */
int bpmToMSec(int bpm) {
  float msec = 0;

  /* BPM -> msec変換 */
  msec = 1000 * (60 / float(bpm)) * (4.0 / BASESIGN);

  return int(msec);
}


/*
 * Name : getOctave
 * description: センサー値から音の高さを取得
 * --------------------------------------------
 * argument
 *  none
 * retturn
 *  volume         音量
 * --------------------------------------------
 */
int getOctave() {
  int octave = 0;
  int value = 0;

  /* センサー値取得 */
  value = analogRead(OCTAVEPIN);
  octave = roundAnalogToDigital(value);

  return octave;
}


/*
 * Name : getDelay
 * description: センサー値からdelayを取得
 * --------------------------------------------
 * argument
 *  none
 * retturn
 *  volume         音量
 * --------------------------------------------
 */
int getDelay() {
  int vibrato = 0;
  int value = 0;

  /* センサー値取得 */
  value = analogRead(DELAYPIN);
  vibrato = roundAnalogToDigital(value);

  return vibrato;
}

/*
 * Name : getOscillator
 * description: センサー値からdelayを取得
 * --------------------------------------------
 * argument
 *  none
 * retturn
 *  volume         音量
 * --------------------------------------------
 */
int getOscillator() {
  // int oscillo = 0;
  // int value = 0;

  // /* センサー値取得 */
  // value = analogRead(OSCILLATORPIN);
  // oscillo = roundAnalogToDigital(value);

  // return oscillo;
}

/*
 * Name : roundAnalogToDigital
 * description: センサー値丸め
 * --------------------------------------------
 * argument
 *  [in]  int   value  センサ値
 * retturn
 *  none
 * --------------------------------------------
 */
int roundAnalogToDigital(int value) {
  int roundValue = 0;

  /* 7.5KΩ */
  if ((579 < value) && (value <= 599)) {
   roundValue = 1;
  }
  /* 5.1KΩ */
  else if ((672 < value) && (value <= 692)) {
   roundValue = 2;
  }
  /* 3KΩ */
  else if ((781 < value) && (value <= 801)) {
   roundValue = 3;
  }
  /* 1.2KΩ */
  else if ((906 < value) && (value <= 926)) {
    roundValue = 4;
  }
  /* 750Ω */
  else if ((943 < value) && (value <= 963)) {
    roundValue = 5;
  }
  /* 510Ω */
  else if ((964 < value) && (value <= 974)) {
    roundValue = 6;
  }
  /* 390Ω */
  else if ((975 < value) && (value <= 995)) {
    roundValue = 7;
  }
  /* 100Ω */
  else if ((1001 < value) && (value <= 1021)) {
    roundValue = 8;
  }
  else {
    roundValue = 0;
  }

  return roundValue;
}


/*
 * Name : sendData
 * description: Processing側にデータ送信
 * --------------------------------------------
 * argument
 *  [in] int* p_sensorValue  : センサー値保持領域のアドレス
 *  [in] int  sensorValueSize : センサー値保持領域のサイズ
 *  [in] int  octave         : 音の高さ
 *  [in] int  vibrato       : delay
 * retturn
 *  none
 * --------------------------------------------
 */
void sendData (int* pSensorValue, int sensorValueSize, int octave, int delayNUm, int bpm, int ocsillator) {
    /* 送信データ:0:カウンター、1~8:音階、9:オクターブ、10:ビブラート, 11:テンポ */
    int sendData[21];
    int sendSize = 0;
    int dataCnt = 0;
    int index = 0;

    /* パラメータチェック */
    if (NULL == pSensorValue) {
      return;
    }

    /* 送信データ初期化 */
    memset(sendData, 0, 21 * sizeof(int));

    /* 送信データにカウンターを設定 */
    sendData[0] = gLoopCnt % STEPNUM;
    sendSize++;
    index = sendSize;

    /* 送信データに音階の値を設定 */
    memcpy(&sendData[index], pSensorValue, sensorValueSize * sizeof(pSensorValue[0]));
    sendSize += sensorValueSize;
    index = sendSize;

    /* 送信データにBPM設定 */
    sendData[index] = bpm;
    sendSize++;
    index = sendSize;

    /* 送信データに音階設定 */
    sendData[index] = octave;
    sendSize++;
    index = sendSize;

    /* 送信データにビブラートを設定 */
    sendData[index] = delayNUm;
    sendSize++;
    index = sendSize;

    /* 送信データにオシレータを設定 */
    sendData[index] = ocsillator;
    sendSize++;
    index = sendSize;

//    for(dataCnt= 0; dataCnt < sendSize; dataCnt++) {
//      Serial.print(sendData[dataCnt]);
//      Serial.print(" ");
//    }
//    Serial.println("");

   /* SerialPort経由でProcessing側にデータを送信 */
   for (dataCnt = 0; dataCnt < 21; dataCnt++) {
     Serial.write(sendData[dataCnt]);
   }
   
   /* OF側できれいに受け取るために遅らせる */
   delay(10);

   return;
}
