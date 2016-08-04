import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import controlP5.*; 
import netP5.*; 
import oscP5.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class loop_osc extends PApplet {





OscP5 oscP5; //\u53d7\u4fe1\u7528\u306e\u5909\u6570
NetAddress myRemoteLocation;  //\u9001\u4fe1\u7528\u306e\u30a2\u30c9\u30ec\u30b9\u3092\u4fdd\u6301\u3059\u308b\u5909\u6570

int[] steps = {1,2,3,4,5,6,7,8};
String[] synth = {"cycle","tri","phasor"};
int osculator = 0; // 0:none, 1:cycle, 2:tri, 3:phasor
int bpm = 120; // BPM
int count = 0; // Max\u304b\u3089\u53d7\u3051\u53d6\u308b\u30ab\u30a6\u30f3\u30bf\u5909\u6570
int volume = 0;




public void setup(){
    
    //\u53d7\u4fe1\u7528\u306e\u5909\u6570\u3002\u53f3\u306e\u6570\u5b57\u306f\u30dd\u30fc\u30c8\u756a\u53f7\u3002\u9001\u4fe1\u5074\u306e\u30dd\u30fc\u30c8\u756a\u53f7\u3068\u3042\u308f\u305b\u308b\u3002
    oscP5 = new OscP5(this,7401);
    //\u9001\u4fe1\u7528\u30aa\u30d6\u30b8\u30a7\u30af\u30c8\u3002\u5de6\u5074\u306e\u6570\u5b57\u304c\u76f8\u624b\u306eIP\u30a2\u30c9\u30ec\u30b9\u3001\u53f3\u5074\u304c\u76f8\u624b\u306e\u30dd\u30fc\u30c8\u756a\u53f7\u3002
    myRemoteLocation = new NetAddress("127.0.0.1",7400);

    //\u30c7\u30fc\u30bf\u3092\u9001\u308b\u5148\u306e\u95a2\u6570\u3092\u767b\u9332\u3059\u308b\u3002\u3053\u3053\u3067\u306f\u3001getData\u306f\u76f8\u624b\u5148\u306e\u95a2\u6570\u3002
    //\u300c/count\u300d\u306f\u9001\u4fe1\u5074\u3068\u53d7\u4fe1\u5074\u3067\u540c\u3058\u3067\u3042\u308b\u5fc5\u8981\u304c\u3042\u308b\u6697\u53f7\u306e\u3088\u3046\u306a\u3082\u306e
    oscP5.plug(this,"getCounter","/count");

    // GUI\u3092\u8a2d\u5b9a

}


public void draw() {
    background(0);
    ledDot(count);
    // println(bpm);
}


public void keyPressed(){
    OscMessage myMessage;

    switch(key){
        case('a'):
            myMessage = new OscMessage("/steps");
            int[] steps01 = {1,2,3,4,5,6,7,8};
            myMessage.add(steps01);
            oscP5.flush(myMessage,myRemoteLocation);
            println("a press");
            break;

        case('s'):
            myMessage = new OscMessage("/steps");
            int[] steps02 = {8,7,6,5,4,3,2,1};
            myMessage.add(steps02);
            oscP5.flush(myMessage,myRemoteLocation);
            println("s press");
            break;

        case('z'):
            myMessage = new OscMessage("/bpm");
            bpm = 120;
            myMessage.add(bpm);
            oscP5.flush(myMessage,myRemoteLocation);
            println("z press");
            break;

        case('x'):
            myMessage = new OscMessage("/bpm");
            bpm = 150;
            myMessage.add(bpm);
            oscP5.flush(myMessage,myRemoteLocation);
            println("x press");
            break;

        case('c'):
            myMessage = new OscMessage("/bpm");
            bpm = 80;
            myMessage.add(bpm);
            oscP5.flush(myMessage,myRemoteLocation);
            println("c press");
            break;

        case('q'):
            myMessage = new OscMessage("/osculator");
            osculator = 1;
            myMessage.add(osculator);
            oscP5.flush(myMessage,myRemoteLocation);
            println("q press");
            break;

        case('w'):
            myMessage = new OscMessage("/osculator");
            osculator = 2;
            myMessage.add(osculator);
            oscP5.flush(myMessage,myRemoteLocation);
            println("w press");
            break;

        case('e'):
            myMessage = new OscMessage("/osculator");
            osculator = 3;
            myMessage.add(osculator);
            oscP5.flush(myMessage,myRemoteLocation);
            println("e press");
            break;

        case('r'):
            myMessage = new OscMessage("/osculator");
            osculator = 0;
            myMessage.add(osculator);
            oscP5.flush(myMessage,myRemoteLocation);
            println("r press");
            break;
    }

    // \u30dc\u30ea\u30e5\u30fc\u30e0
    if(key == CODED){
        if (keyCode == UP) {
            myMessage = new OscMessage("/volume");

            volume += 10;

            if(volume > 100){
                volume = 100;
            }
            myMessage.add(volume);
            oscP5.flush(myMessage,myRemoteLocation);
            println("up:"+volume);

        } else if (keyCode == DOWN){
            myMessage = new OscMessage("/volume");
            volume -= 10;
            if(volume < 0){
                volume = 0;
            }
            myMessage.add(volume);
            oscP5.flush(myMessage,myRemoteLocation);
            println("down:"+volume);
        }

            // myMessage = new OscMessage("/osculator");
            // volume += 10;
            // myMessage.add(osculator);
            // oscP5.flush(myMessage,myRemoteLocation);
            // println("up:" + volume);

    }

}


public void getCounter(int aCount){
    // println(aCount);
    count = aCount;
}




public void ledDot(int aCount){
    fill(255);
    for(int cnt = 0; cnt < 8; cnt++){
        ellipse(cnt*40 + 40, 50, 20, 20);
    }
    fill(255,0,0);
    ellipse(aCount*40 + 40, 50, 20, 20);
}
  public void settings() {  size(500,500); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "loop_osc" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
