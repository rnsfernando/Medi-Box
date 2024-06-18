#include<PubSubClient.h>
#include<WiFi.h>
#include <DHTesp.h>
#include <NTPClient.h>//to take time to our application(to initiate this we need wifi udp also since ntp data packet is a type of udp data packet)
#include <WiFiUdp.h>
#include <ESP32Servo.h>

#define Buzzer 5
const int DHTPIN= 12;
#define ldrLeftPin  32 // Pin for left LDR
#define ldrRightPin  33 // Pin for right LDR
#define servoM 18

//Parameters related to light intensity
float gama = 0.75;
const float RL10 = 50;
float offsetAngle ;
float maxIntensity=0;

int angle = 0;//variable to store angle of servo motor
float cf;//controlling factor of servo motor
float D;//change according to wich ldr gives the max intensity

// to initialize MQTT (pubSub library) need wifi client
WiFiClient espClient;
PubSubClient mqttClient(espClient);
DHTesp dhtSensor;
Servo servo;


WiFiUDP ntpUDP;//instance of wifiudp
NTPClient timeClient(ntpUDP);//to create time clent we need wifiudp that's why we created it before 

char tempAr[6];//creating temp character array since .publish only accespt character arrays 

bool isScheduledON=false;
unsigned long scheduledOnTime;

void setup() {
  Serial.begin(115200);
  setupWifi();
  setupMqtt();
  pinMode(Buzzer,OUTPUT);
  digitalWrite(Buzzer, LOW);
  pinMode(ldrLeftPin,INPUT);
  pinMode(ldrRightPin,INPUT);
  
  servo.attach(servoM, 500, 2400);// Attaches the servo motor to the pin and specifies the pulse width range for angle control.
  dhtSensor.setup(DHTPIN, DHTesp::DHT22);
  timeClient.begin();
  timeClient.setTimeOffset(5.5*3600);//setting 5.5hours offset

}

void loop() {
  //check whethere we are connected to mqtt
  if(!mqttClient.connected()){
    connectToBroker();
  }
//loop through mqtt to check whether there are any incoming messages and all the handeling happens 
  mqttClient.loop();//to handle the mqtt(for incomming messages)

  updateTemperature();
  // Serial.print("temp");
  // Serial.println(tempAr);
  mqttClient.publish("TEMP", tempAr);

  checkSchedule();
  checkIntensity();
  delay(1000);
}

void setupWifi(){
  WiFi.begin("Wokwi-GUEST", "");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  
}

void setupMqtt(){
  //setting mqtt server
  mqttClient.setServer("test.mosquitto.org",1883);//(server,port)
  //subscription is done by callback function
  mqttClient.setCallback(receiveCallback);//when ever message receive this recieve call back function will work(to catch the incomming messages)
}

void connectToBroker(){
  while (!mqttClient.connected()){
    Serial.print("Attempting MQTT connection...");
    if ( mqttClient.connect("ESP32-154698752025")){//inside brackets should put id for esp32(an unique id) if you use authentication. otherwise just put som erandom numbers
      Serial.println("connected");
      //subscribe to a certain data
      mqttClient.subscribe("main on and off");//subscrib the mqtt
      mqttClient.subscribe("schedule on");
      mqttClient.subscribe("Setted Minimum angle");
      mqttClient.subscribe("Setted Controlling factor");
    }else{
      Serial.println("faild");
      Serial.println(mqttClient.state());//returen the state if not connected with the reason
      delay(5000);// wait for 5s befor start to reconnect again
    }
  }
}

void updateTemperature(){
  TempAndHumidity data=dhtSensor.getTempAndHumidity();
  String(data.temperature,2).toCharArray(tempAr, 6/*decimal placees*/);//convert into a string with 2 decimal places and put into charArray since publish only accept charstrings 
}

void receiveCallback(char* topic, byte* payload, unsigned int length){//order of parameters can't change
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]");

  char payloadCharAr[length]; //to put the payload in a character arry to use it latter
  for (int i = 0; i<length;i++){
    Serial.print((char)payload[i]);//converted and will print in the same line,  print function only support characters not for bites
    payloadCharAr[i]=(char)payload[i];
  }

  Serial.println();

  //compare incoming message come from the topic we have subscribed to
  if(strcmp/*compare two things*/(topic, "main on and off" )==0){//if when compared, it gives 0 that means it has compared successfully and it's comming from the topic that we have subscribed to
    buzzerOn(payloadCharAr[0]=='1');
  }else if(strcmp(topic, "schedule on" )==0){
    if(payloadCharAr[0]=='N'){
      isScheduledON=false;
    }else{
      isScheduledON=true;
      scheduledOnTime=atol(payloadCharAr);//atol means array to long
    }
  }else if(strcmp(topic, "Setted Minimum angle" )==0){
      offsetAngle= atof(payloadCharAr);
      Serial.print(offsetAngle);
      Serial.println(offsetAngle);
  }else if(strcmp(topic, "Setted Controlling factor" )==0){
      cf= atof(payloadCharAr);
      Serial.print(offsetAngle);
      Serial.println(cf);
  }
}

void buzzerOn(bool on){
  if(on){
    tone(Buzzer,256);
  }else{
    noTone(Buzzer);
  }
}

unsigned long getTime(){//this time will present time in secods pass from javascrpt fixdate
  timeClient.update();
  return timeClient.getEpochTime();
}

void checkSchedule(){
  if(isScheduledON){
    unsigned long currentTime=getTime();
    if(currentTime>scheduledOnTime){
      buzzerOn(true);
      isScheduledON=false;
      mqttClient.publish("make main on off","1");
      mqttClient.publish("make schedule on off", "0");
      Serial.println("Scheduled ON");
    }
  }
}

void checkIntensity(){
  int ldrLeftValue = analogRead(ldrLeftPin);
  int ldrRightValue = analogRead(ldrRightPin);
  
  // Normalize LDR readings to a range of 0 to 1
  float intensityLeftValue = convertLDR(ldrLeftValue);
  float intensityRightValue = convertLDR(ldrRightValue);
  // Serial.print("left");
  // Serial.println(intensityLeftValue);
  // Serial.print("right");
  // Serial.println(intensityRightValue);
  
  maxIntensity = max(intensityLeftValue, intensityRightValue);
  Serial.print("max");
  Serial.println(maxIntensity);
  char maxIntensityCharArray[10]; // Buffer to hold the converted float value
  String(maxIntensity).toCharArray(maxIntensityCharArray,6);

  // Publish maximum intensity and which LDR it came from
  mqttClient.publish("Max_intensity", maxIntensityCharArray);
  if (maxIntensity == intensityLeftValue) {
    D=1.5;
    mqttClient.publish("Max_intensity_from", "Left LDR");
  } else {
    D=0.5;
    mqttClient.publish("Max_intensity_from", "Right LDR");
  }
  turnServo();
  delay(1000);
}
// Function to convert an analog reading from a LDR to a normalized lux value
float convertLDR(int value){
  float voltage = value / 1024. * 5;// Calculate voltage based on the input value (0 to 1023) assuming a 5V reference voltage
  float resistance = 2000 * voltage / (1 - voltage / 5);// Calculate the maximum lux 
  float maxlux = pow(RL10 * 1e3 * pow(10, gama) / 322.58, (1 / gama));//calculate lux value and normalize it by dividing it by the previously calculated maximum lux
  float lux = pow(RL10 * 1e3 * pow(10, gama) / resistance, (1 / gama))/maxlux;
  return lux;// Return the normalized lux value
}

void turnServo(){
  angle = min((int)(offsetAngle*D + (180-offsetAngle) * maxIntensity * cf), 180);//calculating angle of servo motor
  Serial.print("angle");
  Serial.println(angle);
  servo.write(angle);
}