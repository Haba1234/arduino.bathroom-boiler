// frimware Optiboot v6.2 16MHz

// Enable debug prints to serial monitor
//#define MY_DEBUG 

#define MY_NODE_ID 2
// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

// Enable repeater functionality for this node
#define MY_REPEATER_FEATURE

#define MY_RF24_PA_LEVEL RF24_PA_MAX

#include <SPI.h>
#include <MySensors.h>

#define RELAY_OFF 1  // GPIO value to write to turn on attached relay
#define RELAY_ON 0 // GPIO value to write to turn off attached relay
#define PIN_RELAY  2  // Управление реле бойлера
#define PIN_ON_HEAT 3 // LED "Нагрев включен"
#define PIN_SEND_RECV 7 // LED "Прием/передача"

#define SENSOR1 1      // id для Бойлер

// 
MyMessage Sens1msg(SENSOR1, V_STATUS);

void before() { 
  pinMode(PIN_RELAY, OUTPUT);   
  pinMode(PIN_ON_HEAT, OUTPUT); 
  pinMode(PIN_SEND_RECV, OUTPUT);
  digitalWrite(PIN_RELAY, RELAY_OFF); 
  // Set relay to last known state (using eeprom storage) 
  //digitalWrite(PIN_RELAY, loadState(PIN_RELAY)?RELAY_ON:RELAY_OFF);
}

void setup() {
  
}

void presentation()  
{   
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Bathroom Boiler", "1.0");

  present(SENSOR1, S_BINARY, "Switch Boiler");
  wait(10);
  //send(Sens1msg.set(false), false);
  request(SENSOR1, V_STATUS);
  digitalWrite(PIN_SEND_RECV, true);
  wait(50);
  digitalWrite(PIN_SEND_RECV, false);
}


void loop() 
{
  
}

void receive(const MyMessage &message) {
  // We only expect one type of message from controller. But we better check anyway.
  if (message.type==V_STATUS) {
     // Change relay state
     digitalWrite(PIN_RELAY, message.getBool()?RELAY_ON:RELAY_OFF);
     // отправка подтверждения
     bool test = send(Sens1msg.set(message.getBool()), false);
     // меняем состояние LED нагрева
     digitalWrite(PIN_ON_HEAT, message.getBool()?true:false);
     digitalWrite(PIN_SEND_RECV, true);
     wait(50);
     digitalWrite(PIN_SEND_RECV, false);
     // Store state in eeprom
     //saveState(message.sensor, message.getBool());
     // Write some debug info
     #ifdef MY_DEBUG
        Serial.print("Incoming change for sensor:");
        Serial.print(message.sensor);
        Serial.print(", New status: ");
        Serial.println(message.getBool());
        Serial.print("Send ACK: ");
        Serial.println(test);
     #endif
   } 
}
