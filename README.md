# Arduino MQTT Button-Trigger
An MQTT Client that publishes a message upon push-button trigger

* Circuit:
   - Ethernet shield attached to pins 10, 11, 12, 13
   - Push button(s) connected to pins 2, 3, 4, 5
   - LED(s) connected to pins 17, 18
 
* Description:
   - every time a button is "pressed" (for longer than the defined debounce time) the controller publishes a defined MQTT message to the defined server. Feedback on successfull execution is provided via LEDs
 
* Utilised source code and libraries:
   - Button Debounce: https://docs.arduino.cc/built-in-examples/digital/Debounce
   - PubSubClient:
     - https://github.com/knolleary/pubsubclient
     - Full API documentation: https://pubsubclient.knolleary.net/api
     - MQTT_basic: https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_basic/mqtt_basic.ino
