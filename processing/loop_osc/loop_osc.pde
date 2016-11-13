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
//int gVolume = 0;
//int gWaveKind = 0;

void setup(){
  //size(640,480);
  
  //Serial Port Open
  //myPort = new Serial(this, serialPortName, 9600);
  //myPort = new Serial(this, serialPortName, 115200);
  
  /* 受信用の変数。右の数字はポート番号。送信側のポート番号とあわせる。 */
  oscP5 = new OscP5(this,7100);
  
  //送信用オブジェクト。左側の数字が相手のIPアドレス、右側が相手のポート番号。
  maxLocation = new NetAddress(toMaxIPAddr, 7400);
  
  //データを送る先の関数を登録する。ここでは、getDataは相手先の関数。
  //「/count」は送信側と受信側で同じである必要がある暗号のようなもの
  oscP5.plug(this, "syncStart","/start");
  oscP5.plug(this, "forceSync","/sync");
}

void draw(){
  //background(0);
  //fill(fillColor);
  //ellipse(width/2,height/2,diameter,diameter);
}

//OSC Receive Event (Counter)
public void syncStart(){
     println("start");
    //count = aCount;
    myPort.write(1);
}

public void forceSync(){
     println("sync");
    //count = aCount;
    myPort.write(2);
}

//Serial Receive Event
void serialEvent(Serial myPort) {
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
    //else if (gDataCnt == 9) {
    //  gBpm = resvValue;
    //}
    else if (gDataCnt == 9) {
      gOctave = resvValue;
      //gVolume = resvValue;
    }
    else if (gDataCnt == 10) {
      gVibrato = resvValue;
      //gWaveKind = resvValue;
    }
  //}
  //print(gDataCnt);
  
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
    //Send BPM
    //else if (gDataCnt == 9) {
    //  //println("bpm: " + gBpm);
    //  OscMessage bpmMsg = new OscMessage("/bpm");
    //  bpmMsg.add(gBpm);
    //  oscP5.send(bpmMsg, maxLocation); //送信
    //}
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
      //waveMsg.add(1);
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