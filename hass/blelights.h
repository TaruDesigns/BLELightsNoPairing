#include "esphome.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include "blecodes.h"

const float maxMired = 500;
const float minMired = 153;

float color_temperature_ = -1.0f; // Variable to store previous value. There should be a better way to do this
float bright_ = -1.0f;  // Variable to store previous value  
bool onoffstatus_ = false;

#define COLOR_COOL 0.35 // Values below this are mapped to "Cool white"
#define COLOR_WARM 0.70 // Value above this is mapped to "Warm white"

#define BRIGHTNESS_FULL 0.70 // Values (%) for what intervals map to brightness modes
#define BRIGHTNESS_HALF 0.35

#define DATALEN 29 // Length of array "beacon data". Note: The full data is actually 30 bytes but one of them is set by default
#define ADVERT_TIME 500 // Time the "advert" beacon will be on (milliseconds)
// BLE RELATED STUFF

char beacon_data[DATALEN];
BLEAdvertising *pAdvertising;

// This should be included in BLECodes.h inside every "major" function
// But I can't figure out how to make it so the compiler doesn't complain
void SendBLECommand(char beacon_data[]) {
    // Send the command
    beaconData_Common(beacon_data); // I update it every time to avoid weird memory bugs
    BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
    oAdvertisementData.setManufacturerData(std::string(beacon_data, DATALEN));
    pAdvertising->setAdvertisementData(oAdvertisementData);
    pAdvertising->setScanResponse(false);
    pAdvertising->start();
    delay(ADVERT_TIME); //Maybe there's a way to do this async so it doesn't block the entire component.
    pAdvertising->stop(); 
}


void ColorHandler(float color_temperature){
  // Generic Handler for colour modes
      if(color_temperature <= COLOR_COOL){
        //Send "cool"
        ESP_LOGD("custom", "COOL White");
        Cool_LabRoom(beacon_data);
      } else if (color_temperature >= COLOR_WARM){
        //Send "Warm white"
        ESP_LOGD("custom", "WARM White");
        Warm_LabRoom(beacon_data);
      }
        else{
        //Send "Neutral white"
        ESP_LOGD("custom", "NEUTRAL White");
        Neutral_LabRoom(beacon_data);
      }   
      SendBLECommand(beacon_data);
}

void BrightnessHandler(float bright){
  // Generic handler for "brightness" adjustment
      if(bright >= BRIGHTNESS_FULL){
        //Send "Full"
        ESP_LOGD("custom", "FULL Power");
        FullPower_LabRoom(beacon_data);
      } else if (bright >= BRIGHTNESS_HALF){
        //Send "Half power"
        ESP_LOGD("custom", "HALF Power");
        HalfPower_LabRoom(beacon_data);
      }
        else{
        //Send "NightMode" NOTE: Always send the color here as well, since "night mode" changes the color too
        // This doesn't work, need to find better BLE codes
        ESP_LOGD("custom", "Night Mode");
        ColorHandler(color_temperature_);
      }   
      SendBLECommand(beacon_data);
}

void ONOFFHandler(bool onoffstatus){
      if(onoffstatus){
        // Turn ON
        ESP_LOGD("custom", "TURN ON");
        PowerON_LabRoom(beacon_data);
      }
      else{
        // TURN OFF
        ESP_LOGD("custom", "TURN OFF");
        PowerOFF_LabRoom(beacon_data);
      }
      SendBLECommand(beacon_data);
}

// For testing I will implement only one light for now
class BLELights : public Component, public LightOutput {
 public:
  void setup() override {
    // This will be called by App.setup()
      BLEDevice::init("ESP32");
  // Create the BLE Server
      BLEServer *pServer = BLEDevice::createServer();
      pAdvertising = pServer->getAdvertising();
      //beaconData_Common(beacon_data);

  }
  LightTraits get_traits() override {
    // return the traits this light supports
    auto traits = LightTraits();
    traits.set_supported_color_modes({ColorMode::COLOR_TEMPERATURE, ColorMode::BRIGHTNESS});
    // DO NOT DELETE: This is actually necessary for the slider in HASS.
    traits.set_min_mireds(minMired); // home assistant minimum 153
    traits.set_max_mireds(maxMired); // home assistant maximum 500
    return traits;
  }



  void write_state(LightState *state) override {
    // This will be called by the light to get a new state to be written.
    float color_temperature, white_brightness;
    bool onoffstatus;
    // Get the state of these light color values. In range from 0.0 (off) to 1.0 (on)
    onoffstatus = state->current_values.is_on();
    // Turn it on/off if needed
    if(onoffstatus_!= onoffstatus){
      onoffstatus_ = onoffstatus;
      ONOFFHandler(onoffstatus);
    }

    // This will leave color temp and brightness as a float from 0 to 1
    state->current_values_as_ct(&color_temperature, &white_brightness); 
    ESP_LOGD("custom", "Outputs:  ColorTemp %f", color_temperature);    
    ESP_LOGD("custom", "Outputs:  Brightness %f", white_brightness);     

    //------ This returns the color temp in mireds----
    // float colorTemp;
    //colorTemp = state->current_values.get_color_temperature();
    //-------------------------------------------------

    //------ This returns the brightness mapped to 0-1 as the "slider" in HASS----
    float bright;
    bright = state->current_values.get_brightness();
    //-------------------------------------------------
    ESP_LOGD("custom", "Outputs:  BRIGHTNESS MAPPED %f", bright);  
    //--- Adjust Color Temperature ---
    if(color_temperature_ != color_temperature ){
      color_temperature_ = color_temperature;
      // New value for color temperature, send the command
      ColorHandler(color_temperature);
    }
    //--- Adjust Brightness -----
    if(bright_ != bright){
      bright_ = bright;
      // New value for color temperature, send the command
      BrightnessHandler(bright);
    }
  }
};