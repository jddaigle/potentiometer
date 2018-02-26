#include <Homie.h>
#include <Adafruit_NeoPixel.h>

const int PIN_DOOR = 5;
const int LED = 13;
const int NEO_PIXEL_PIN = 4;

#define NUMPIXELS 2
#define BRIGHTNESS 255

Bounce debouncer = Bounce(); // Bounce is built into Homie, so you can use it without including it first
boolean lastDoorValue = false;
boolean switchValue = false;
boolean statusChange = false;
boolean configMode = false;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);



HomieNode switchNode("switch", "switch");

void setLedColor(int ledAddress, int red, int green, int blue)
{
    strip.setPixelColor(ledAddress, strip.Color(red,green,blue));
    strip.show();
}

void changeSwitchValue(boolean value)
{
  switchValue = value;
  Homie.getLogger() << "Switch is now " << (switchValue ? "open" : "close") << switchValue << endl;
  switchNode.setProperty("open").send(switchValue ? "1" : "0");
  //digitalWrite(LED, switchValue);
}

void toggleSwitchStatus()
{
  boolean value = !switchValue;
  changeSwitchValue(value);
}

bool ledHandler(const HomieRange& range, const String& value) {
  //if (value != "true" && value != "false")
  //    return false;

  Serial << "Received value :" << value;
  changeSwitchValue(value.toInt());

  return true;
}




void loopHandler()
{
  int physicalSwitchValue = debouncer.read();

  if (!physicalSwitchValue && !statusChange)
  {
    statusChange = true;
    toggleSwitchStatus();
  }
  else if (physicalSwitchValue)
  {
    statusChange = false;
  }
}

void onHomieEvent(const HomieEvent& event) {
  configMode = false;
  switch(event.type) {
    case HomieEventType::STANDALONE_MODE:
      // Do whatever you want when standalone mode is started
      break;
    case HomieEventType::CONFIGURATION_MODE:
      configMode = true;
      break;
    case HomieEventType::NORMAL_MODE:
      // Do whatever you want when normal mode is started
      break;
    case HomieEventType::OTA_STARTED:
      // Do whatever you want when OTA is started
      break;
    case HomieEventType::OTA_FAILED:
      // Do whatever you want when OTA is failed
      break;
    case HomieEventType::OTA_SUCCESSFUL:
      // Do whatever you want when OTA is successful
      break;
    case HomieEventType::ABOUT_TO_RESET:
      // Do whatever you want when the device is about to reset
      break;
    case HomieEventType::WIFI_CONNECTED:
      // Do whatever you want when Wi-Fi is connected in normal mode

      // You can use event.ip, event.gateway, event.mask
      break;
    case HomieEventType::WIFI_DISCONNECTED:
      // Do whatever you want when Wi-Fi is disconnected in normal mode

      // You can use event.wifiReason
      break;
    case HomieEventType::MQTT_READY:
      // Do whatever you want when MQTT is connected in normal mode
      break;
    case HomieEventType::MQTT_DISCONNECTED:
      // Do whatever you want when MQTT is disconnected in normal mode

      // You can use event.mqttReason
      break;
    case HomieEventType::MQTT_PACKET_ACKNOWLEDGED:
      // Do whatever you want when an MQTT packet with QoS > 0 is acknowledged by the broker

      // You can use event.packetId
      break;
    case HomieEventType::READY_TO_SLEEP:
      // After you've called `prepareToSleep()`, the event is triggered when MQTT is disconnected
      break;
  }
}

void setup() {
  Serial.begin(115200);

  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'



  Homie.disableLedFeedback();
  Homie.disableResetTrigger();

  pinMode(PIN_DOOR, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  attachInterrupt(digitalPinToInterrupt(PIN_DOOR), loopHandler, CHANGE);
  debouncer.attach(PIN_DOOR);
  debouncer.interval(50);

  Serial << "My light-switch";

  Homie_setFirmware("My light-switch", "1.0.0");
  //Homie.setLoopFunction(loopHandler);
  switchNode.advertise("open").settable(ledHandler);

  Homie.onEvent(onHomieEvent); // before Homie.setup()

  Homie.setup();
}

void loop() {
  Homie.loop();
  debouncer.update();


if(configMode)
{
  setLedColor(0, 0, 0, 255);
}
else
{
  if(switchValue)
  {
    setLedColor(0, 255, 255, 255);
  }
  else
  {
    setLedColor(0, 0, 0, 0);
  }
}

}
