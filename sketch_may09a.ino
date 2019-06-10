#include <math.h>
#include <EEPROM.h>
#include <Servo.h>
#include <dht.h>


dht DHT;
Servo myservo;  
int val;

int potpin = 0 , contor = 0  ;
float temp,hum;
bool open = 0;

int tiltVector[10]={0,0,0,0,0,0,0,0,0,0};
int occupation_level ;
 
uint8_t adress = 150;
uint8_t adress2 = 160;

void setup() {
    
    Serial.begin(9600);
    myservo.attach(12); 
    contor = EEPROM.read(adress);
    
    
    if(contor%2==0){open = 0;}
    else{open = 1;}
    
    val = analogRead(potpin); 
  
    if(open==1){
       val = map(val, 0, 1023, 0, 250); //Loocks the door
       delay(3000); 
       Serial.println("Door locked!");
      myservo.write(val);                  
    
    }
    else{
      val = map(val, 0, 1023, 0, 35);//Unlocks the door
      delay(3000);
      Serial.println("Door unlocked!");
      myservo.write(val);
    }

    
    contor++;
   
    EEPROM.write(adress,contor);
}

void loop() {

  DHT.read11(A1);
 
  if(contor%2==0){open = 0;}
  else{open = 1;}
  
  occupation_level = contor;
  if(occupation_level>9){occupation_level = 9;}
  
  if(occupation_level==9){
    int i;
    for(i=0;i<9;i++){
      tiltVector[i]=tiltVector[i+1];
    }
     int tilt_Detector = digitalRead(7);
    if(tilt_Detector == LOW){tiltVector[9] = 0;}
  
    else{tiltVector[9] = 1;}
    
  }
  
  
  if(check_earthquake()==1){
      if(open==0){
      val = map(val, 0, 1023, 0, 35);//Unlocks the door in case of an Earthquake
      
      delay(1000); 
      myservo.write(val);                  
      
      contor++;
      EEPROM.write(adress,contor);
      
      }
      
  }
  
  
   int temp = temperature();
   if(open==0&&temp>37){  //In case of a Fire it unlocks the door
      val = map(val, 0, 1023, 0, 35);
      delay(1000); //Unlocks the door
      myservo.write(val);                  
      
      contor++;
      EEPROM.write(adress,contor);
    }
  
  delay(3000);
  print_tilt();
      
  
  if(open==1){Serial.println("Unlocked");}
  else{Serial.println("Locked");}
  Serial.println("\n");


}



int temperature(){
  hum = DHT.humidity;
  temp = DHT.temperature;
  Serial.println(temp);
  Serial.println(hum);
  return temp;
}



int check_earthquake(){

  int contor2 = 0;
  int i;
  for(i=0;i<10;i++){
    if(tiltVector[i]==1){
      contor2++;
    }
  }
  
  if(contor2>2){
    Serial.println("Earthquake!");
    return true;
    }
  else{return false;}
}



void print_tilt(){
  int i;
  for(i=0;i<occupation_level-1;i++){
    Serial.print(tiltVector[i]);
    Serial.print(",");
  }
  Serial.print(tiltVector[occupation_level]);
  Serial.println(" ");
}


   
