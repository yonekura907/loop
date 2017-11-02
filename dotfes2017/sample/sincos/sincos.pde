float circle = 200;
float rot;
float col;
float freq = 0.000005;
float cont = 0;
float r;

void setup() {
  size(600,600);
}

void draw() {
  background(0);
  translate(300,300);
  rotate(radians(rot));

  //これ以下のellipseは半径で描きますよ
  ellipseMode(RADIUS);
  for (float i=0; i<500; i+=5) {
    
    //座標
    circle = 200 + 50*sin(millis()*freq*i);
    
    //色
    col = map(circle, 150, 250, 255, 60);
    
    //半径
    r = map(circle, 150, 250, 10, 2);
    
    //   赤　　緑　　青
    fill(col, 255, 255);
    noStroke();
    //stroke(col, 0, 74);
    ellipse(circle*cos(i), circle*sin(i),r,r);
    rot = rot+0.00005;
  }
   
}