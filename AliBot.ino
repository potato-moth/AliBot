#include <VarSpeedServo.h>
#include "IRremote.h"
#include "Ultrasonic.h"

IRrecv irrecv(11); // указываем вывод, к которому подключен приемник
decode_results results;
VarSpeedServo myservo;
Ultrasonic ultrasonic(7, 8);

int randomNum;

int pinA1 = 3;
int pinA2 = 2;
int pinB1 = 5;
int pinB2 = 4;
int enableB = 6;
int enableA = 1;

void setup() {
  myservo.attach(9);
  irrecv.enableIRIn(); // запускаем прием
  pinMode (enableA, OUTPUT);
  pinMode (enableB, OUTPUT);
  pinMode(pinA1, OUTPUT);
  pinMode(pinA2, OUTPUT);
  pinMode(pinB1, OUTPUT);
  pinMode(pinB2, OUTPUT);
  

}

void loop(){
  
  if ( irrecv.decode( &results )) { // если данные пришли
    switch ( results.value ) {
    case 0xFF629D:
        forward();
        break;
    case 0xFFA857:        
        backward();
        break;
    case 0xFFC23D:       
        rightturn(100);
        break;
    case 0xFF22DD:       
        leftturn(100);
        break;
    case 0xFF02FD:
        breakRobot();
        break;
    case 0xFF42BD:
        motorAon();
        motorBon();
        break;
    case 0xFF52AD:
        motorAoff();
        motorBoff();
        break;
    case 0xFF4AB5:
        autoCar();
        break;
    }    
    irrecv.resume(); // принимаем следующую команду
  }
}
// Движение вперед
void forward(){
  digitalWrite (pinA1, HIGH);
  digitalWrite (pinA2, LOW);
  digitalWrite (pinB1, LOW);
  digitalWrite (pinB2, HIGH);
}
// Движение назад
void backward(){
  digitalWrite (pinA1, LOW);
  digitalWrite (pinA2, HIGH);
  digitalWrite (pinB1, HIGH);
  digitalWrite (pinB2, LOW);
}
// Движение назад в автоматическом режиме
void autoBack(int duration){
  digitalWrite (pinA1, LOW);
  digitalWrite (pinA2, HIGH);
  digitalWrite (pinB1, HIGH);
  digitalWrite (pinB2, LOW);
  delay(duration);
}
// Поворот направо
void rightturn(int duration){
  digitalWrite (pinA1, LOW);
  digitalWrite (pinA2, HIGH);
  digitalWrite (pinB1, LOW);
  digitalWrite (pinB2, HIGH);
  delay (duration);
  digitalWrite (pinA1, HIGH);
  digitalWrite (pinA2, HIGH);
  digitalWrite (pinB1, HIGH);
  digitalWrite (pinB2, HIGH);
}
// Поворот налево
void leftturn(int duration){
  digitalWrite (pinA1, HIGH);
  digitalWrite (pinA2, LOW);
  digitalWrite (pinB1, HIGH);
  digitalWrite (pinB2, LOW);
  delay (duration);
  digitalWrite (pinA1, HIGH);
  digitalWrite (pinA2, HIGH);
  digitalWrite (pinB1, HIGH);
  digitalWrite (pinB2, HIGH);
}
// Стоп
void breakRobot () {

motorAstop();

motorBstop();


}
void motorAstop() {

digitalWrite (pinA1, HIGH);

digitalWrite (pinA2, HIGH);

}

void motorBstop() {

digitalWrite (pinB1, HIGH);

digitalWrite (pinB2, HIGH);

}
void motorAon() {

digitalWrite (enableA, HIGH);

}

void motorBon() {

digitalWrite (enableB, HIGH);

}
void motorAoff() {

digitalWrite (enableA, LOW);

}

void motorBoff() {

digitalWrite (enableB, LOW);

}

void sonicScan(){
  myservo.write(60,100,true);
  myservo.write(120,100,true);
  myservo.write(180,100,true);
  myservo.write(120,100,true);
}

void autoCar(){
  motorAon();
  motorBon();
  sonicScan();
  while(1){
    float dist_cm = ultrasonic.distanceRead(CM);
    if(dist_cm>=15.0){
      forward();
      delay(100);
    }else{
      breakRobot();// Останавливаем робота
      delay(1000);
      randomNum = random(0,2);
      //autoBack(200);// Откатываем немного назад
        if(randomNum == 1){         //Условие начала поиска пути с правой стороны
          myservo.write(60,100,true); // Поворачиваем серву направо
          float dist_cm1 = ultrasonic.distanceRead(CM);
            if(dist_cm1>=15.0){             // Если справа нет препятствий  
              rightturn(200);               // поворачиваем платформу в направлении сервы
              myservo.write(120,100,true);  // Ставим серву под прямым углом к платформе
              forward();                    // и двигаемся вперед.
              delay(100);                   
            }else {                          // В противном случае поворачиваем серву налево
              myservo.write(180,100,true);
              float dist_cm2 = ultrasonic.distanceRead(CM);
                if(dist_cm2>=15.0){         // Если слева нет препятствий то выполняем аналогичный алгоритм действий.
                  leftturn(200);
                  myservo.write(120,100,true);
                  forward();
                  delay(100);
                }else{                    // Если и с левой стороны есть препятствие, то вращаем платформу вокруг своей оси на рандомное количество оборотов
                  rightturn(random(0,2000));
                  autoCar();
            }
        }
    
      }else{ // Условие начала поиска пути с левой стороны
        myservo.write(180,100,true); // Поворачиваем серву налево
          float dist_cm1 = ultrasonic.distanceRead(CM);
            if(dist_cm1>=15.0){             // Если слева нет препятствий  
              leftturn(200);               // поворачиваем платформу в направлении сервы
              myservo.write(120,100,true);  // Ставим серву под прямым углом к платформе
              forward();                    // и двигаемся вперед.
              delay(100);                   
            }else {                          // В противном случае поворачиваем серву направо
              myservo.write(60,100,true);
              float dist_cm2 = ultrasonic.distanceRead(CM);
                if(dist_cm2>=15.0){         // Если справа нет препятствий то выполняем аналогичный алгоритм действий.
                  rightturn(200);
                  myservo.write(120,100,true);
                  forward();
                  delay(100);
                }else{                    // Если и с левой стороны есть препятствие, то вращаем платформу вокруг своей оси на рандомное количество оборотов
                  leftturn(random(0,2000));
                  autoCar();
            }
        }
  }
  }
  
  }
}
  



