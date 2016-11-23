import netP5.*;
import oscP5.*;
import processing.serial.*;

Serial myPort;

final int BPM = 60;

final int STEP_NUM = 8;
float fillColor;
float diameter;

OscP5 oscP5;
NetAddress maxLocation;

final String serialPortName = "/dev/cu.usbmodem1421";

//final String toMaxIPAddr = "192.168.43.19";
final String toMaxIPAddr = "127.0.0.1";

int gDataCnt = 0;
int gStepCnt = 0;
int gSteps[] = new int[STEP_NUM];
int gBpm = 0;
int gOctave = 0;
int gVibrato = 0;

// boolean isStart = false;

void setup(){
  //size(640,480);

  //Serial Port Open
  myPort = new Serial(this, serialPortName, 115200);

  /* 受信用の変数。右の数字はポート番号。送信側のポート番号とあわせる。 */
  oscP5 = new OscP5(this,7100);

  //送信用オブジェクト。左側の数字が相手のIPアドレス、右側が相手のポート番号。
  maxLocation = new NetAddress(toMaxIPAddr, 7400);

  //データを送る先の関数を登録する。ここでは、getDataは相手先の関数。
  //「/count」は送信側と受信側で同じである必要がある暗号のようなもの
  oscP5.plug(this, "syncStart","/start");
  oscP5.plug(this, "forceSync","/sync");
  oscP5.plug(this, "syncStop", "/stop");
}

void draw(){
  //background(0);
  //fill(fillColor);
  //ellipse(width/2,height/2,diameter,diameter);

  //testCode();
}

//OSC Receive Event (Counter)
public void syncStart(){
     println("start");
    // isStart = true;
    myPort.write(1);
}

public void forceSync(){
     println("sync");
    myPort.write(2);
}

public void syncStop(){
     println("stop");
    myPort.write(3);
}

//Serial Receive Event
void serialEvent(Serial myPort) {
  int resvValue = 0;

  resvValue = myPort.read();

  if (gDataCnt == 0) {
    gStepCnt = resvValue;
  }
  else if (1 <= gDataCnt && gDataCnt <= 8) {
    gSteps[gDataCnt - 1] = resvValue;
  }
  else if (gDataCnt == 9) {
    //0/1でMAXに渡す
    gOctave = resvValue - 1;
  }
  else if (gDataCnt == 10) {
    //0/1でMaxに渡す
    gVibrato = resvValue - 1;
  }

  try {
    //Send Steps
    if (gDataCnt == 0) {
      //println("count: " + gStepCnt);
      OscMessage countMsg = new OscMessage("/counter");
      countMsg.add(gStepCnt);
      oscP5.send(countMsg, maxLocation); //送信
    }
    //Send Steps
    else if (gDataCnt == 8) {
      //println(gSteps);
      OscMessage stepMsg = new OscMessage("/steps");
      stepMsg.add(gSteps);
      oscP5.send(stepMsg, maxLocation); //送信
    }
    //Send Octave
    else if (gDataCnt == 9) {
      //println("volume: " + gVolume);
      OscMessage octaveMsg = new OscMessage("/octave");
      octaveMsg.add(gOctave);
      oscP5.send(octaveMsg, maxLocation); //送信
    }
    //Send Delay
    else if (gDataCnt == 10) {
      //println("wave" + gWaveKind);
      OscMessage delayMsg = new OscMessage("/delay");
      delayMsg.add(gVibrato);
      oscP5.send(delayMsg, maxLocation); //送信
    }
  }
  catch (RuntimeException e) {
    println("Error:OSC Message Send..");
  }

  gDataCnt++;
  if (12 == gDataCnt) {
    gDataCnt = 0;
  }
}

//void testCode() {

//  if (true != isStart) {
//    return;
//  }
//  OscMessage countMsg = new OscMessage("/counter");
//  countMsg.add(gStepCnt);
//  oscP5.send(countMsg, maxLocation); //送信

//  delay((60 / BPM) * (4 / 4) * 1000);
//}
