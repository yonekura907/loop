import netP5.*;
import oscP5.*;
import processing.serial.*;

Serial myPort;

final int STEP_NUM = 8;
float fillColor;
float diameter;
//int[] steps = {1,2,3,4,5,6,7,8};
//int[] steps = {8,7,6,5,4,3,2,1};
//int synths = 1; // 0:none, 1:cycle, 2:tri, 3:phasor
//int bpm = 150;

/* resept Data array */
//int[] resvData = new int[11];

OscP5 oscP5;
NetAddress myRemoteLocation;

final String serialPortName = "/dev/cu.usbmodem1421";

//final String toMaxIPAddr = "172.17.11.84";
final String toMaxIPAddr = "127.0.0.1";

int gDataCnt = 0;
int gStepCnt = 0;
int gSteps[] = new int[STEP_NUM];
int gBpm = 0;
int gVolume = 0;
int gWaveKind = 0;

void setup(){
  //size(640,480);
  
  //Serial Port Open
  myPort = new Serial(this, serialPortName, 9600);
  //myPort = new Serial(this, serialPortName, 115200);
  
  /* 受信用の変数。右の数字はポート番号。送信側のポート番号とあわせる。 */
  //oscP5 = new OscP5(this,7401);
  
  //送信用オブジェクト。左側の数字が相手のIPアドレス、右側が相手のポート番号。
  oscP5 = new OscP5(this,7400);
  myRemoteLocation = new NetAddress(toMaxIPAddr,7400);
  
  //データを送る先の関数を登録する。ここでは、getDataは相手先の関数。
  //「/count」は送信側と受信側で同じである必要がある暗号のようなもの
  //oscP5.plug(this, "getCounter","/count");
}

void draw(){
  //background(0);
  //fill(fillColor);
  //ellipse(width/2,height/2,diameter,diameter);
}

//OSC Receive Event (Counter)
//public void getCounter(int aCount){
//     println("getCounter " + aCount);
//    //count = aCount;
//    myPort.write(aCount);
//}

//Serial Receive Event
void serialEvent(Serial myPort){
  int resvValue = 0;
  
   
  //while(myPort.available() > 0) {
   
    resvValue = myPort.read();
    //resvData[dataCnt] = myPort.read();
    //println(resvValue);
    if (gDataCnt == 0) {
      gStepCnt = resvValue;
    }
    else if (1 <= gDataCnt && gDataCnt <= 8) {
      gSteps[gDataCnt - 1] = resvValue;
    }
    else if (gDataCnt == 9) {
      gBpm = resvValue;
    }
    else if (gDataCnt == 10) {
      gVolume = resvValue;
    }
    else if (gDataCnt == 11) {
      gWaveKind = resvValue;
    }
  //}
  //print(gDataCnt);
  
  try {
    //Send Steps
    if (gDataCnt == 0) {
      println("count: " + gStepCnt);
      OscMessage countMsg = new OscMessage("/counter");
      countMsg.add(gStepCnt);
      oscP5.send(countMsg, myRemoteLocation); //送信 
    }
    //Send Steps
    else if (gDataCnt == 8) {
      println(gSteps);
      OscMessage stepMsg = new OscMessage("/steps");
      stepMsg.add(gSteps);
      oscP5.send(stepMsg, myRemoteLocation); //送信
    }
    //Send BPM
    else if (gDataCnt == 9) {
      println("bpm: " + gBpm);
      OscMessage bpmMsg = new OscMessage("/bpm");
      bpmMsg.add(gBpm);
      oscP5.send(bpmMsg, myRemoteLocation); //送信
    }
    //Send Volume
    else if (gDataCnt == 10) {
      println("volume: " + gVolume);
      OscMessage volumeMsg = new OscMessage("/volume");
      volumeMsg.add(gVolume);
      oscP5.send(volumeMsg, myRemoteLocation); //送信
    }
    //Send Wave
    else if (gDataCnt == 11) {
      println("wave" + gWaveKind);
      OscMessage waveMsg = new OscMessage("/osculator");
      waveMsg.add(gWaveKind);
      //waveMsg.add(1);
      oscP5.send(waveMsg, myRemoteLocation); //送信
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