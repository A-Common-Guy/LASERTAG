// no delay, attach/interrupt
#include <TimerOne.h>

void(* Riavvia)(void) = 0;

#include <IRremote.h>

#define ar A0
#define code1 0xa60
#define code2 0xa70
#define code3 0xa80
#define code4 0xa90
int lifea = 9;
int th = 600; //min light val
int pmv = 10;
int GOL = 4;
bool nothit = true;
int fr = 200;
int frm = (fr * 1000);
int RECV_PIN = 4;
IRrecv irrecv(RECV_PIN);
IRsend irsend;
decode_results results;

////// quando ti scocci di scrivere una libreria esterna e quindi la crei nel file stesso (almeno va)!!

typedef struct basetimer{
  int trigtime;
  void (*trig)();
  uint32_t toptimer;
  uint32_t basetimer;
  bool enabled=false;
  
  void control(){
    if (millis()>toptimer and enabled){
      basetimer=millis();
      toptimer=millis()+trigtime;
      trig();
    }
  }
  void enable(){
    enabled=true;
    basetimer=millis();
    toptimer=basetimer+trigtime;
  }
  void disable(){
    enabled=false;
  }
}NiceTimer;

//// qui finisce la libreria

NiceTimer timer;

void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT_PULLUP);
  pinMode(GOL, OUTPUT);
  irrecv.enableIRIn();
  attachInterrupt(digitalPinToInterrupt(2), trigger, FALLING);
  Serial.begin(9600);
  timer.trigtime=2000;

  timer.trig=&semiauto;
    
  
}

void loop() {
  timer.control();
  

  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
  if (enemyfire(results.value)) {
    lifea--;
    nothit = false;
    if (lifea = 0) {
      digitalWrite(GOL, HIGH);
      delay(60000);
      Riavvia();
    }
    delay(10000);
    nothit = true;
  }
}


bool enemyfire(uint16_t results_value) {

  if (results_value == code1 or results_value == code2 or results_value == code3) {

    return true;
  } else {
    return false;
  }
}

void trigger() {
  
  for (int i = 0; i < 3; i++) {
    interrupts();
    irsend.sendSony(0xa90, 12);

    



  }
  irrecv.enableIRIn();
  detachInterrupt(digitalPinToInterrupt(2));
  timer.enable();
}

void semiauto() {
  attachInterrupt(digitalPinToInterrupt(2), trigger, FALLING);
  timer.disable();
}
