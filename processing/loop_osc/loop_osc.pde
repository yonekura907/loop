import controlP5.*;
import netP5.*;
import oscP5.*;

OscP5 oscP5; //受信用の変数
NetAddress myRemoteLocation;  //送信用のアドレスを保持する変数

int[] steps = {1,2,3,4,5,6,7,8};
String[] synth = {"cycle","tri","phasor"};
int osculator = 0; // 0:none, 1:cycle, 2:tri, 3:phasor
int bpm = 120; // BPM
int count = 0; // Maxから受け取るカウンタ変数
int volume = 0;




void setup(){
    size(500,500);
    //受信用の変数。右の数字はポート番号。送信側のポート番号とあわせる。
    oscP5 = new OscP5(this,7401);
    //送信用オブジェクト。左側の数字が相手のIPアドレス、右側が相手のポート番号。
    myRemoteLocation = new NetAddress("127.0.0.1",7400);

    //データを送る先の関数を登録する。ここでは、getDataは相手先の関数。
    //「/count」は送信側と受信側で同じである必要がある暗号のようなもの
    oscP5.plug(this,"getCounter","/count");

    // GUIを設定

}


void draw() {
    background(0);
    ledDot(count);
    // println(bpm);
}


void keyPressed(){
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

    // ボリューム
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
