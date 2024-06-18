#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>
#include <WiFi.h>

#define SCREEN_WIDTH 128 //specifies the width of the OLED display in pixels
#define SCREEN_HIGHT 64  //specifies the hight of the OLED display in pixels
#define OLED_RESET -1    //pin number for the reset pin of the OLED
#define SCREEN_ADDRESS 0x3C  //specifies the I2C address of the OLED display

#define buzzer 5
#define  LED_1 15
#define hum_LED 2 // blue LED to indicate out of range humidity
#define temp_LED 0  //green LED to indicate out of range temperature
#define PB_CANCEL 34
#define PB_OK 32
#define PB_UP 33
#define PB_DOWN 35
#define DHTPIN 12
//constants related to time synchronization using NTP
#define NTP_SERVER     "pool.ntp.org"
int UTC_OFFSET= 0;//represents the offset, in seconds, between Coordinated Universal Time (UTC) and the local time zone
#define UTC_OFFSET_DST 0//// Offset during Daylight Saving Time

//defining required global variables
//variables to keep the track of current time
int days=0;
int hours=0;
int minutes=0;
int seconds=0;
//variables to handle different modes in the menu
int current_mode=0;
int max_modes=5;
String modes[]={"1 - Set Time","2 - Set Alarm 1","3 - Set Alarm 2","4 - Set Alarm 3","5 - Disable Alarms"};
//variables to handle the alarms
bool alarm_enabled=true;
int n_alarms=3;
int alarm_hours[]={0,1,2};
int alarm_minutes[]={1,10,20};
bool alarm_triggered[]={false, false, false};//to find whether alarm has triggered once

int notes[9] = {200, 300, 200, 300, 200, 300, 400, 400, 400};//notes to ring the buzzer

Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HIGHT,&Wire ,OLED_RESET); //initializing the object display
DHTesp dhtSensor; //initializing the sensor object

void setup() {
  Serial.begin(9600);//initializing the serial monitor
  //initializing the display
  if(! display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)){
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
//setting pinmodes
pinMode(buzzer, OUTPUT);  
pinMode(LED_1, OUTPUT);
pinMode(temp_LED, OUTPUT);
pinMode(hum_LED, OUTPUT);
pinMode(PB_CANCEL, INPUT);
pinMode(PB_OK, INPUT);
pinMode(PB_UP, INPUT);
pinMode(PB_DOWN, INPUT);
//initializing the DHT sensor
dhtSensor.setup(DHTPIN, DHTesp::DHT22);

display.display();
delay(2000);
//connecting to wifi// reason to fetch time over wifi
WiFi.begin("Wokwi-GUEST", "", 6);
while (WiFi.status() != WL_CONNECTED) {
  delay(250);
  display.clearDisplay();
  print_line("Connecting to WIFI",0,0,2);
}

display.clearDisplay();
print_line("Connected to WIFI",0,0,2);
delay(2000);

//Configure device time settings
//Fetch the time in the time zone from the NTP server over Wi-Fi.
configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);

display.clearDisplay();
print_line("Welcome to medi box!",0,0,2);
delay(2000);
display.clearDisplay(); 
}

void loop() {
  update_time_with_check_alarm();//update time and check alarm

  // goto menu when press ok button
  if (digitalRead(PB_OK)==LOW){
    delay(200);// to stop bouncing of push button
    go_to_menu();
  }

  check_temp();
}

void print_line(String text, int column, int row, int text_size){
 
 display.setTextSize(text_size);
 display.setTextColor(SSD1306_WHITE);
 display.setCursor(column,row);
 display.println(text);
 display.display();

}

//printing current time
void print_time_now(void){
  display.clearDisplay();
  print_line(String(days),0,0,2);
  print_line(":",20,0,2);
  print_line(String(hours),30,0,2);
  print_line(":",50,0,2);
  print_line(String(minutes),60,0,2);
  print_line(":",80,0,2);
  print_line(String(seconds),90,0,2);
  
 }

void update_time(void){
  struct tm timeinfo;//tm=type of struct, struct is a data type in c++, time info is the name of the variable
  getLocalTime(&timeinfo);//retrieves the current local time and stores in timeinfo variable
  //extract speccific time components from timeinfo
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  hours= atoi(timeHour);//convert char into int

  char timeMinute[3];
  strftime(timeMinute,3, "%M", &timeinfo);
  minutes= atoi(timeMinute);

  char timeSecond[3];
  strftime(timeSecond,3, "%S", &timeinfo);
  seconds= atoi(timeSecond);

  char timeDay[3];
  strftime(timeDay, sizeof(timeDay), "%d", &timeinfo);
  days = atoi(timeDay);
 }

void update_time_with_check_alarm(){
  update_time();
  print_time_now();
  //check alarm
  if (alarm_enabled){
    for (int i=0;i<n_alarms;i++){
      if (alarm_triggered[i]==false && alarm_hours[i]==hours && alarm_minutes[i]==minutes){
        ring_alarm();
        alarm_triggered[i]=true;
      }
    }
  }
 }

void ring_alarm() {
  display.clearDisplay();
  print_line("MEDICINE TIME!", 0, 0, 2);

  digitalWrite(LED_1, HIGH); //ligh up the  LED

  bool break_happen = false;//to stop while statment
  //Ring the BUZZER
  while (break_happen == false && digitalRead(PB_CANCEL) == HIGH) { 
    for (int i = 0; i <=9; i++) {
      if (digitalRead(PB_CANCEL) == LOW) {
        delay(200);//prevent bouncing of the push button //while need to be stop >fast push kale nathotho aye aye ebenavane
        break_happen = true;
        break;//stop for statement
      }
      tone(buzzer, notes[i]);
      delay(500);
      noTone(buzzer);
      delay(2);
    }
  }

  digitalWrite(LED_1, LOW);
  display.clearDisplay();

}

void go_to_menu(){
  while(digitalRead(PB_CANCEL)==HIGH){
    display.clearDisplay();
    print_line(modes[current_mode],0,0,2);
    int pressed = wait_for_button_press();
    if (pressed==PB_UP){
      delay(200);
      current_mode+=1;
      current_mode=current_mode % max_modes;
    }
    else if (pressed==PB_DOWN){
      delay(200);
      current_mode-=1;
      if (current_mode<0){
        current_mode=max_modes-1;
      }
    }
    else if (pressed==PB_OK){
      delay(200);
      //Serial.println(current_mode);
      run_mode(current_mode);
    }
    else if (pressed==PB_CANCEL){
      delay(200);
      break;
    }
  }
 }
int wait_for_button_press(){
  while(true){
    if (digitalRead(PB_UP)==LOW){
      delay(200);
      return PB_UP;
    }
    if (digitalRead(PB_DOWN)==LOW){
      delay(200);
      return PB_DOWN;
    }
    if (digitalRead(PB_OK)==LOW){
      delay(200);
      return PB_OK;
    }
    if (digitalRead(PB_CANCEL)==LOW){
      delay(200);
      return PB_CANCEL;
    }
    update_time();
  }
}

void run_mode(int mode){
  if (mode==0){
    set_time();
  }
  else if (mode==1 || mode==2 || mode==3){
    set_alarm(mode-1);
  }
  else if (mode==4){
    alarm_enabled==false;
    display.clearDisplay();
    print_line("alarm disabled",0,0,2);
    delay(1500);
  }
}

void set_time(){//set the offset, in seconds, between Coordinated Universal Time (UTC) and the local time zone
  int temp_hour=0;//to get user input hour value
  while(true){
    display.clearDisplay();
    print_line("Enter hour: "+String(temp_hour),0,0,2);

    int pressed = wait_for_button_press();
    if (pressed==PB_UP){
      delay(200);
      temp_hour+=1;
      if (temp_hour>14){
        temp_hour=-12;//not to let temp_hour to be greater than 14
      }
    }
    else if (pressed==PB_DOWN){
      delay(200);
      temp_hour-=1;
      if (temp_hour<-12){//not to let temp hours to be lesser than -12
        temp_hour=14;
      }
    }
    else if (pressed==PB_OK){
      delay(200);
      hours=temp_hour;//set hours
      break;
    }
    else if (pressed==PB_CANCEL){
      delay(200);
      break;
    }
  }

  int temp_minute=0;//to get user niput minutes
  while(true){
    display.clearDisplay();
    print_line("Enter minute: "+String(temp_minute),0,0,2);

    int pressed = wait_for_button_press();
    if (pressed==PB_UP){
      delay(200);
      temp_minute+=1;
      if(temp_minute>45){
        temp_minute=0;//not to let temp minutes to be greater than 45
      }
    }
    else if (pressed==PB_DOWN){
      delay(200);
      temp_minute-=1;
      if (temp_minute<0){//not to let temp minutes to be lower than 0
        temp_minute=45;
      }
    }
    else if (pressed==PB_OK){
      delay(200);
      minutes=temp_minute;//set minute 
      break;
    }
    else if (pressed==PB_CANCEL){
      delay(200);
      break;
    }
  }
  UTC_OFFSET = temp_hour*3600 + temp_minute*60;
  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);//Fetch the time in the time zone from the NTP server over Wi-Fi.
  display.clearDisplay();
  print_line("Time is set",0,0,2);
  delay(1000);
}

void set_alarm(int alarm){
  //set alarm hour
  int temp_hour=alarm_hours[alarm];
  while(true){
    display.clearDisplay();
    print_line("Enter hour: "+String(temp_hour),0,0,2);

    int pressed = wait_for_button_press();
    if (pressed==PB_UP){
      delay(200);
      temp_hour+=1;
      temp_hour=temp_hour%24;
    }
    else if (pressed==PB_DOWN){
      delay(200);
      temp_hour-=1;
      if (temp_hour<0){
        temp_hour=23;
      }
    }
    else if (pressed==PB_OK){
      delay(200);
      alarm_hours[alarm]=temp_hour;
      break;
    }
    else if (pressed==PB_CANCEL){
      delay(200);
      break;
    }
  }
  //set alarm minute
  int temp_minute=alarm_minutes[alarm];
  while(true){
    display.clearDisplay();
    print_line("Enter minute: "+String(temp_minute),0,0,2);

    int pressed = wait_for_button_press();
    if (pressed==PB_UP){
      delay(200);
      temp_minute+=1;
      temp_minute=temp_minute%60;
    }
    else if (pressed==PB_DOWN){
      delay(200);
      temp_minute-=1;
      if (temp_minute<0){
        temp_minute=59;
      }
    }
    else if (pressed==PB_OK){
      delay(200);
      alarm_minutes[alarm]=temp_minute;
      break;
    }
    else if (pressed==PB_CANCEL){
      delay(200);
      break;
    }
  }
  display.clearDisplay();
  print_line("Alarm is set",0,0,2);
  delay(1000);
}

void check_temp(){
  TempAndHumidity data=dhtSensor.getTempAndHumidity();//this is a data type in dht sensor library
  //display.clearDisplay();
  if (data.temperature>32){
    
    print_line("Temp HIGH",3,40,1);
    
  }
  else if (data.temperature<26){
    
    print_line("Temp LOW",3,40,1);
    
  }
  if (data.temperature<26 || data.temperature>32){
    digitalWrite(temp_LED, HIGH);
  }
  else{
    digitalWrite(temp_LED, LOW);
  }
  if (data.humidity>80){
    
    print_line("Humidity HIGH",3,50,1);
    
  }
  else if (data.humidity<60){
    
    print_line("Humidity LOW",3,50,1);
    
  }
  if (data.humidity<60 || data.humidity>80){
    digitalWrite(hum_LED, HIGH);
  }
  else{
    digitalWrite(hum_LED, LOW);
  }
  delay(500);
}