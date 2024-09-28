#include <TridentTD_LineNotify.h>

#define SSID        "YOUR_WIFI_SSID"
#define PASSWORD    "YOUR_WIFI_PASSWORD"
#define LINE_TOKEN  "YOUR_LINE_TOKEN"

#define THRESHOLD_DISTANCE 20
#define COUNT_BEFORE_WARN 10

int nBin1State = 0;
int nBin2State = 0;
int lBin1State = 0;
int lBin2State = 0;
int triggerCount1 = 0;
int triggerCount2 = 0;

#define LED_BIN1_PIN D1
#define LED_BIN2_PIN D2
#define ULT_SEND_BIN1 D4
#define ULT_RECV_BIN1 D3
#define ULT_SEND_BIN2 D6
#define ULT_RECV_BIN2 D5


float calculateDistance(int trigPin,int echoPin){
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin,LOW);
  unsigned long duration_bin = pulseIn(echoPin, HIGH, 20000);
  float distance = (duration_bin/2) / 29.1;
  return distance;
}

void setup() {
  Serial.begin(115200); 
  delay(100);
  Serial.println();
  Serial.println(LINE.getVersion());
  pinMode(LED_BIN1_PIN,OUTPUT);
  pinMode(LED_BIN2_PIN,OUTPUT);
  pinMode(ULT_SEND_BIN1,OUTPUT);
  pinMode(ULT_RECV_BIN1,INPUT);
  pinMode(ULT_SEND_BIN2,OUTPUT);
  pinMode(ULT_RECV_BIN2,INPUT);
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  while(WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(400); }
  WiFi.setAutoConnect(true);
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());  

  LINE.setToken(LINE_TOKEN);

}

void loop() {
  float distance1 = calculateDistance(ULT_SEND_BIN1, ULT_RECV_BIN1);
  float distance2 = calculateDistance(ULT_SEND_BIN2, ULT_RECV_BIN2);

  if (distance1 < THRESHOLD_DISTANCE){
    triggerCount1++;
  }
  else{
    triggerCount1 = 0;
    nBin1State = 0;
    digitalWrite(LED_BIN1_PIN,LOW);
  }
  if (triggerCount1 > COUNT_BEFORE_WARN){
    nBin1State = 1;
  }
  if (nBin1State != lBin1State){
    if (nBin1State){
      LINE.notify("Bin 1 is full");
      digitalWrite(LED_BIN1_PIN,HIGH);
    }
    lBin1State = nBin1State;
  }

  if (distance2 < THRESHOLD_DISTANCE){
    triggerCount2++;
  }
  else{
    triggerCount2 = 0;
    nBin2State = 0;
    digitalWrite(LED_BIN2_PIN,LOW);
  }
  if (triggerCount2 > COUNT_BEFORE_WARN){
    nBin2State = 1;
  }
  if (nBin2State != lBin2State){
    if (nBin2State){
      LINE.notify("Bin 2 is full");
      digitalWrite(LED_BIN2_PIN,HIGH);
    }
    lBin2State = nBin2State;
  }

  Serial.print("Distance : ");
  Serial.print(distance1);
  Serial.print(" , ");
  Serial.println(distance2);
  delay(1000);
}


