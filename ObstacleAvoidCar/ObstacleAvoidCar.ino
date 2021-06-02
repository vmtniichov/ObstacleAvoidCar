#include <NewPing.h>
#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 100 //Giới hạn khoảng cách đọc của sonar (cm)
//Khởi tạo sonar
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);


//L298N pin --> arduino
#define ENA_PIN  5
#define ENB_PIN  6

//Left
#define EN1_PIN  9
#define EN2_PIN  8

//Right
#define EN3_PIN  3
#define EN4_PIN  2

//Motor Engine
const int MotorLeft1   = EN1_PIN;
const int MotorLeft2   = EN2_PIN; 
const int MotorRight1    = EN3_PIN;
const int MotorRight2    = EN4_PIN;
const int MotorLeftPWM = ENA_PIN;
const int MotorRightPWM  = ENB_PIN;

//Khoảng cách để tiến hành xoay
const int limit_distance = 30;

//Dùng để tính thời gian nếu k nhận được tín hiệu từ sonar
unsigned long preTime = 0;

//Chạy thẳng
void forward(int curSpeed) 
{
  digitalWrite(MotorLeft1, LOW);
  digitalWrite(MotorLeft2, HIGH);
  digitalWrite(MotorRight1, LOW);
  digitalWrite(MotorRight2, HIGH);
  analogWrite(MotorRightPWM, curSpeed);
  analogWrite(MotorLeftPWM, curSpeed);
}

//Lùi
void backward(int curSpeed) 
{
  digitalWrite(MotorLeft1, HIGH);
  digitalWrite(MotorLeft2, LOW);
  digitalWrite(MotorRight1, HIGH);
  digitalWrite(MotorRight2, LOW);
  
  analogWrite(MotorRightPWM, curSpeed);
  analogWrite(MotorLeftPWM, curSpeed);
}

//xoay phải
void turnRight(int curSpeed) 
{
  digitalWrite(MotorLeft1, LOW);
  digitalWrite(MotorLeft2, HIGH);
  digitalWrite(MotorRight1, HIGH);
  digitalWrite(MotorRight2, LOW);
  
  analogWrite(MotorRightPWM, curSpeed);
  analogWrite(MotorLeftPWM, curSpeed);
}

//xoay trái
void turnLeft(int curSpeed) 
{
  digitalWrite(MotorLeft1, HIGH);
  digitalWrite(MotorLeft2, LOW);
  digitalWrite(MotorRight1, LOW);
  digitalWrite(MotorRight2, HIGH);
  
  analogWrite(MotorRightPWM, curSpeed);
  analogWrite(MotorLeftPWM, curSpeed);
}

//Lấy khoảng cách từ sonar
float getDistance(){
  float distance = sonar.ping_cm();
  
  Serial.print("Khoảng cách: ");
  Serial.print(distance); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
  Serial.println("cm");
  return distance;
}

void setup() {
  //serial
  Serial.begin(115200);
  
  pinMode(MotorLeft1, OUTPUT);
  pinMode(MotorLeft2, OUTPUT);
  pinMode(MotorLeft1,  OUTPUT);
  pinMode(MotorLeft2,  OUTPUT);
  pinMode(MotorRightPWM, OUTPUT); //enable for right side motor
  pinMode(MotorLeftPWM, OUTPUT); //enable for right side motor

  Serial.println("Set up Done!");
}
void loop(){

  //Chạy thẳng khi không có vật cản và tín hiệu nhận từ sonar khác 0
  while(getDistance() != 0 && getDistance() > limit_distance){
      forward(224);
  }
  
  //Xử lý khi khoảng cách nhận từ sonar dưới khoảng cách giới hạn có vật cản
  while(getDistance() != 0 && getDistance() <= limit_distance){
      
      while(getDistance() < 17 ){
        backward(192);
      }
      
      //xoay theo hướng ngẫu nhiên
      if(millis()%2==1) {
        //Quay liên tục tới khi vật cản ra khỏi khoảng cách giới hạn
        while(getDistance() <= limit_distance){
            turnLeft(168);
        }
      }
      
      else {
        //Quay liên tục tới khi vật cản ra khỏi khoảng cách giới hạn
        while(getDistance() <= limit_distance){
            turnRight(168);
        }
      }
  }
      //Khoảng cách quá gần sẽ lùi
      
  //Xử lý khi không nhận được tín hiệu từ sonar
  while(getDistance() == 0){
    
    forward(192);

    //Lấy thời gian hiện tại
    unsigned long curTime = millis();

    //Nếu không nhận được tín hiệu trong 0.5s thực thi khối lệnh bên dưới
    if(curTime - preTime >= 750){
      
        preTime = curTime;
        //xoay theo hướng ngẫu nhiên cho tới khi vật cản ra khỏi tầm
        if(millis()%2==1) {
          while(getDistance()==0){
              turnLeft(168);
          }
        }
      
        else {
          while(getDistance()==0){
              turnRight(168);
          }
        } 
        
    }//END IF TIME
    
  }//End while distance = 0
  
}//END loop()
