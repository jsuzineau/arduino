/*
doc utilisées
https://wiki.seeedstudio.com/Grove-Finger-clip_Heart_Rate_Sensor_with_shell/
https://wiki.seeedstudio.com/Wio-Terminal-Displaying-Gyro/
*/
#include <Wire.h>
#include"seeed_line_chart.h" //include the line chart library

TFT_eSPI tft;

#define MAX_SIZE 200 //maximum size of data
doubles readings;
double readings_min=0;
TFT_eSprite spr = TFT_eSprite(&tft);  // Sprite

void setup() {
    tft.begin();
    tft.setRotation(3);
    
    spr.createSprite(TFT_HEIGHT,TFT_WIDTH);
    spr.setRotation(3);

    Serial.begin(115200);
    Serial.println("heart rate sensor:");

    Wire.begin();
}

void loop() {
    spr.fillSprite(TFT_WHITE);

    Wire.requestFrom(0xA0 >> 1, 1);    // request 1 bytes from slave device
    while(Wire.available()) 
      {          // slave may send less than requested
      unsigned char c = Wire.read();   // receive heart rate value (a byte)
      Serial.println(c, DEC);         // print heart rate value
      if ((0==readings_min)||(c<readings_min))
        readings_min= c;
      if (readings.size() == MAX_SIZE) 
        readings.pop(); //this is used to remove the first read variable
      readings.push(c); //read variables and store in data
      }

    //Settings for the line graph title
    auto header =  text(0, 0)
                .value("Valeurs de pouls")
                .align(center)
                .valign(vcenter)
                .width(spr.width())
                .thickness(2);

    header.height(header.font_height(&spr) * 2);
    header.draw(&spr); // Header height is the twice the height of the font

  //Settings for the line graph
    auto content = line_chart(20, header.height()); //(x,y) where the line graph begins
         content
                .height(spr.height() - header.height() * 1.5) //actual height of the line chart
                .width(spr.width() - content.x() * 2) //actual width of the line chart
                .based_on(readings_min) //Starting point of y-axis, must be a float
                .show_circle(false) //drawing a cirle at each point, default is on.
                .value({readings}) //passing through the data to line graph
                .max_size(MAX_SIZE)          
                .color(TFT_BLUE)
                .backgroud(TFT_WHITE)
                .draw(&spr);

    spr.pushSprite(0, 0);
    delay(50);
}