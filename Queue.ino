
#include <queue.h>
//#include "Arduino_FreeRTOS.h" in case you are using an Arduino Board
//LEDs
#define GREEN 23 
#define YELLOW 22 
#define RED 21 

struct presence {
  int hall;
  int touch;
  };

QueueHandle_t structQueue;


void setup() {
Serial.begin(115200);
pinMode(GREEN, OUTPUT);
pinMode(YELLOW, OUTPUT);
pinMode(RED, OUTPUT);

structQueue = xQueueCreate(10, sizeof(struct presence));
if(structQueue != NULL){
  
  xTaskCreate(Hall,"ReadHall",128,NULL,2,NULL);
  xTaskCreate(Touch,"ReadTouch",128,NULL,1,NULL);
  xTaskCreate(Alarming,"Alarming",128,NULL,3,NULL);

  }
  vTaskStartScheduler();

}

void loop() {

}
// this task read the internal HAll sensor value
void Hall(void * pvParameters){
  
  (void) pvParameters;
  for(;;){
    struct presence presence1;
    presence1.hall = hallRead();
    xQueueSend(structQueue, &presence1,portMAX_DELAY);
    Serial.println("task ReadHall is rinnung on core : ");
   // Serial.print(xPortGetCoreID());
    taskYIELD();
    
    }
  }
// this task read the internal touch sensor
  void Touch(void * pvParameters){
  
  (void) pvParameters;
  for(;;){
    struct presence presence2;
    presence2.touch = touchRead(T0);
    xQueueSend(structQueue, &presence2,portMAX_DELAY);
    Serial.println("task ReadTouch is rinnung on core : ");
   // Serial.print(xPortGetCoreID());
    taskYIELD();
    
    }
  }
// task indicates the state of the alarm 
  void Alarming(void * pvParameters){
    (void) pvParameters;
    for(;;){
      struct presence alarme;
      if(xQueueReceive(structQueue,&alarme,portMAX_DELAY)== pdPASS){
        if(alarme.hall < 10){
          digitalWrite(YELLOW,HIGH);
          delay(1000);
          digitalWrite(YELLOW,LOW);
          delay(1000);
          }
          else if(alarme.touch < 20){
          digitalWrite(RED,HIGH);
          delay(1000);
          digitalWrite(RED,LOW);
          delay(1000);
          }
          else{          digitalWrite(GREEN,HIGH);}

          taskYIELD();
        }
      }
    
    
    }
