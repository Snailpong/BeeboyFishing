#define BUTTON 12
#define RELAY 13

int relay_state = 0;
int button_state = 0;

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(RELAY, OUTPUT);
  Serial.begin(9600);
  
}

void loop() {
  Serial.println(button_state);
  delay(10);
  if(digitalRead(BUTTON) == LOW) {
    if(button_state == 0) {
      button_state = 1;
      relay_state = ~relay_state;
      digitalWrite(RELAY, relay_state);
    }
  } else {
    button_state = 0;
  }
}
