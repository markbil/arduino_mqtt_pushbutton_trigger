/*
 * Arduino MQTT Button-Trigger: MQTT Client publishing a message upon push-button trigger
 * Circuit:
   - Ethernet shield attached to pins 10, 11, 12, 13
   - Push button(s) connected to pins 2, 3, 4, 5
   - LED(s) connected to pins 17, 18
 * Description:  
   - every time a button is "pressed" (for longer than the defined debounce time) the controller publishes a defined MQTT message to the defined server. Feedback on successfull execution is provided via LEDs
 * Utilised source code and libraries:
   - Ethernet Shield: 
   - Button Debounce: https://docs.arduino.cc/built-in-examples/digital/Debounce
   - PubSubClient:
     - https://github.com/knolleary/pubsubclient
     - Full API documentation: https://pubsubclient.knolleary.net/api
     - MQTT_basic: https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_basic/mqtt_basic.ino
 */

#include <Ethernet.h>
#include <PubSubClient.h>

// Botton Config
const int buttonPin1 = 2;
const int buttonPin2 = 3;
const int buttonPin3 = 4;
const int buttonPin4 = 5;  
const int ledPin_green = 14; //analog 0 = pin 14
const int ledPin_red = 19; //analog 1 = pin 15
const int ledPin_ext1 = 17; //analog 3 = pin 17   
const int ledPin_ext2 = 18; //analog 4 = pin 18

int buttonState1 = 0;
int buttonState1_previous = 0;
int buttonState2 = 0;
int buttonState2_previous = 0;
int buttonState3 = 0;
int buttonState3_previous = 0;
int buttonState4 = 0;
int buttonState4_previous = 0;

unsigned long lastDebounceTime_1 = 0;  // the last time the output pin was toggled
unsigned long lastDebounceTime_2 = 0;
unsigned long lastDebounceTime_3 = 0;
unsigned long lastDebounceTime_4 = 0;
unsigned long debounceDelay = 50;    // the debounce time im ms; increase if the output flickers

// Network Config
byte mac[] = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192, 168, 5, 202);
IPAddress mqtt_server(192, 168, 5, 201);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // provide feedback via LED that command has been executed
  // TODO: edit to provide feedback on specific topic/payload received by client that controls door opener
  if(strcmp(topic, "command/door2") == 0){
    blink(ledPin_ext2, 1, 5000);
  }
}

EthernetClient ethClient;
PubSubClient client(ethClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      led_connected();
      
      // Once connected...
      // ... and resubscribe
      client.subscribe("broadcast");
			client.subscribe("command/door1");
      client.subscribe("command/door2");
			//... publish an announcement
      client.publish("broadcast","hello world - arduino MQTT client");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      
      led_disconnected();
      // Wait 3 seconds before retrying
      delay(5000);
    }
  }
}

void blink(int ledID, int repeat, int inertia) {
  if (repeat == 1){
    digitalWrite(ledID, HIGH);
    delay(inertia);
    digitalWrite(ledID, LOW);
  }
  else{
    for (int i=0; i<repeat; i++){
      digitalWrite(ledID, HIGH);
      delay(inertia);
      digitalWrite(ledID, LOW);
      delay(inertia);
    }    
  }

}

void led_disconnected() {
	blink(ledPin_red, 3, 1000);
} 
void led_connected() {
	blink(ledPin_green, 3, 1000);
}

void setup()
{
  Serial.begin(57600);

	// Buttons
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
	
	//LEDs
  pinMode(ledPin_green, OUTPUT);
  pinMode(ledPin_red, OUTPUT);
  pinMode(ledPin_ext1, OUTPUT);
  pinMode(ledPin_ext2, OUTPUT);
  
  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);

  //blink(ledPin_ext2, 3, 1000);
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();


	// -------------MANAGE BUTTON 1 ---------------//	
	// local variable to store current buttonState1
  int buttonState1_current = digitalRead(buttonPin1);

  // If the switch changed, due to noise or pressing:
	if (buttonState1_current != buttonState1_previous) {
		// reset the debouncing timer
		lastDebounceTime_1 = millis();
	}

	if ((millis() - lastDebounceTime_1) > debounceDelay) {
		// whatever the reading is at, it's been there for longer than the debounce delay, so take it as the actual (confirmed) current state:

		//to avoid repeated action, check if the new (confirmed) state differs from the previous (confirmed) state 
		if (buttonState1 != buttonState1_previous) {
			buttonState1 = buttonState1_current;

			if (buttonState1 == HIGH) {
				// => the following is executed only if button is "pressed" and pressed for longer than the defined debounce time
				client.publish("command/door1","open");
			  client.publish("command/door2","open");
			}
		}
	}
   
  buttonState1_previous = buttonState1_current;


	// -------------MANAGE BUTTON 2 ---------------//	
	// local variable to store current buttonState1
  int buttonState2_current = digitalRead(buttonPin2);

  // If the switch changed, due to noise or pressing:
	if (buttonState2_current != buttonState2_previous) {
		// reset the debouncing timer
		lastDebounceTime_2 = millis();
	}

	if ((millis() - lastDebounceTime_2) > debounceDelay) {
		// whatever the reading is at, it's been there for longer than the debounce delay, so take it as the actual (confirmed) current state:

		//to avoid repeated action, check if the new (confirmed) state differs from the previous (confirmed) state 
		if (buttonState2 != buttonState2_previous) {
			buttonState2 = buttonState2_current;

			if (buttonState2 == HIGH) {
				// => the following is executed only if button is "pressed" and pressed for longer than the defined debounce time
				client.publish("command/door2","test_Button2");
			}
		}
	}
   
  buttonState2_previous = buttonState2_current;


	// -------------MANAGE BUTTON 3 ---------------//	
	// local variable to store current buttonState1
  int buttonState3_current = digitalRead(buttonPin3);

  // If the switch changed, due to noise or pressing:
	if (buttonState3_current != buttonState3_previous) {
		// reset the debouncing timer
		lastDebounceTime_3 = millis();
	}

	if ((millis() - lastDebounceTime_3) > debounceDelay) {
		// whatever the reading is at, it's been there for longer than the debounce delay, so take it as the actual (confirmed) current state:

		//to avoid repeated action, check if the new (confirmed) state differs from the previous (confirmed) state 
		if (buttonState3 != buttonState3_previous) {
			buttonState3 = buttonState3_current;

			if (buttonState3 == HIGH) {
				// => the following is executed only if button is "pressed" and pressed for longer than the defined debounce time
				client.publish("command/door2","test_Button3");
			}
		}
	}
   
  buttonState3_previous = buttonState3_current;


	// -------------MANAGE BUTTON 4 ---------------//	
	// local variable to store current buttonState1
  int buttonState4_current = digitalRead(buttonPin4);

  // If the switch changed, due to noise or pressing:
	if (buttonState4_current != buttonState4_previous) {
		// reset the debouncing timer
		lastDebounceTime_4 = millis();
	}

	if ((millis() - lastDebounceTime_4) > debounceDelay) {
		// whatever the reading is at, it's been there for longer than the debounce delay, so take it as the actual (confirmed) current state:

		//to avoid repeated action, check if the new (confirmed) state differs from the previous (confirmed) state 
		if (buttonState4 != buttonState4_previous) {
			buttonState4 = buttonState4_current;

			if (buttonState4 == HIGH) {
				// => the following is executed only if button is "pressed" and pressed for longer than the defined debounce time
				client.publish("command/door1","open");
			  client.publish("command/door2","open");
			}
		}
	}
   
  buttonState4_previous = buttonState4_current;
	
}
