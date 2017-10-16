/* 音数 */
#define STEPNUM (8)

/* タイミングInput用PIN */
const int STEPPIN[] = {0, 1, 2, 3, 4, 5, 6, 7};

/* ループカウント */
int gLoopCnt = 0;

/* MaxからOSCで送られてくるカウンター変数 */
int maxLoopCount = 0;

const int sensorPin = 7; // 入力ピン

void setup() {
    /* シリアルポートオープン */
    Serial.begin(115200);

}

void loop() {
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

    /* 音階の取得 */
    getStepNum(steps, STEPNUM);


    /* データをProcessingに送信 */
    sendData(steps, STEPNUM, octave, delayNum, bpm, ocsillator);


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

        Serial.println(sensorValue);
    }

    return;
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
    int sendData[12];
    int sendSize = 0;
    int dataCnt = 0;
    int index = 0;

    /* パラメータチェック */
    if (NULL == pSensorValue) {
      return;
    }

    /* 送信データ初期化 */
    memset(sendData, 0, 12 * sizeof(int));

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
   for (dataCnt = 0; dataCnt < 12; dataCnt++) {
     Serial.write(sendData[dataCnt]);
   }

    return;
}
