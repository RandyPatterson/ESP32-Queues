/*
    demo for  LILYGO TTGO T-Display ESP32 1.14 Inch LCD Development Board For Arduino
    Correct display driver installed
    https://github.com/Xinyuan-LilyGO/TTGO-T-Display
 */
#include <Arduino.h>
#include "TFTDriver.h"
#include <SPI.h>

#define TFT_MOSI            19
#define TFT_SCLK            18
#define TFT_CS              5
#define TFT_DC              16
#define TFT_RST             23
#define TFT_BL          4  // Display backlight control pin


int counter = 0;
QueueHandle_t queue;
TFTDriver tft = TFTDriver(135, 240); // Invoke custom library

void display( void * pvParameters )
{
    int element;
    while(true){
        //remove item from queue,  will block until an item is available
        xQueueReceive(queue, &element, portMAX_DELAY);
        tft.fillRect(0,0,tft.width(),30,TFT_BLACK);
        tft.drawString(String(element),0,0);
        
        delay(1000);
 
    }
}



void setup()
{
    Serial.begin(115200);
    //create a queue that will hold up to 10 integers
    queue = xQueueCreate( 10, sizeof( int ) );


    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(0, 0);
    tft.setTextDatum(TL_DATUM);
    //turn on backlight
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);


    

xTaskCreatePinnedToCore(
                    display,   /* Function to implement the task */
                    "display", /* Name of the task */
                    1000,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    1,          /* Priority of the task */
                    NULL,       /* Task handle. */
                    0);  /* Core where the task should run, loop is on core 1 */

}




void loop()
{
    while(true) {
        counter++;
        //add data to queue.  When full will block until a slot is available
        xQueueSend(queue, &counter, portMAX_DELAY);
        Serial.println(counter);
        delay(200);
    }
    
}