#include "esphome.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include "blecodes.h"

const float maxMired = 500;
const float minMired = 153;


#define COLOR_COOL 0.35 // Values below this are mapped to "Cool white"
#define COLOR_WARM 0.70 // Value above this is mapped to "Warm white"

#define BRIGHTNESS_FULL 0.70 // Values (%) for what intervals map to brightness modes
#define BRIGHTNESS_HALF 0.35

#define DATALEN 29 // Length of array "beacon data". Note: The full data is actually 30 bytes but one of them is set by default
#define ADVERT_TIME 250 // Time the "advert" beacon will be on (milliseconds)
#define WAIT_TIME 50 // Time to wait after "stop" advertising -> Useful to handle queued commands

// LIGHT NAMES
// LABROOM
// LIVROOM
// BEDROOM


// This should be included in BLECodes.h inside every "major" function
// But I can't figure out how to make it so the compiler doesn't complain






// For testing I will implement only one light for now
class BLELights : public Component, public LightOutput, public EntityBase {
 public:
      char beacon_data[DATALEN];
      std::string lightname;
      float color_temperature_ = -1.0f; // Variable to store previous value. There should be a better way to do this
      float bright_ = -1.0f;  // Variable to store previous value  
      bool onoffstatus_ = false;
      // BLE STUff
      BLEAdvertising *pAdvertising;
      BLEServer *pServer;
      BLELights(std::string nametype){
        lightname = nametype;
      }
  void setup() override {
    // This will be called by App.setup()
    BLEDevice::init("ESP32");   
    this->pServer = BLEDevice::createServer();
    this->pAdvertising = pServer->getAdvertising();
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

    //CHECK NAME
    //ESP_LOGD("custom", "Light Name: %s", this->lightname)
    // This will be called by the light to get a new state to be written.
    float color_temperature, white_brightness; // Local variables to check whether or not we update the class values
    bool onoffstatus;
    // Get the state of these light color values. In range from 0.0 (off) to 1.0 (on)
    onoffstatus = state->current_values.is_on();
    // Turn it on/off if needed
    if(this->onoffstatus_!= onoffstatus){
      this->onoffstatus_ = onoffstatus;
      ONOFFHandler(onoffstatus, this->lightname, this->beacon_data);
    }

    // This will leave color temp and brightness as a float from 0 to 1
    state->current_values_as_ct(&color_temperature, &white_brightness); 
    //ESP_LOGD("custom", "Outputs:  ColorTemp %f", color_temperature);    
    //ESP_LOGD("custom", "Outputs:  Brightness %f", white_brightness);     

    //------ This returns the brightness mapped to 0-1 as the "slider" in HASS----
    float bright;
    bright = state->current_values.get_brightness();
    //-------------------------------------------------
    //ESP_LOGD("custom", "Outputs:  BRIGHTNESS MAPPED %f", bright);  
    //--- Adjust Color Temperature ---
    if(this->color_temperature_ != color_temperature ){
      this->color_temperature_ = color_temperature;
      // New value for color temperature, send the command
      ColorHandler(color_temperature, this->lightname, this->beacon_data);
    }
    //--- Adjust Brightness -----
    if(this->bright_ != bright){
      this->bright_ = bright;
      // New value for color temperature, send the command
      BrightnessHandler(bright,this->lightname, this->beacon_data);
    }
  }
void SendBLECommand(char beacon_data[]) {
    // Start the BLE server thing
    // Send the command
    beaconData_Common(beacon_data); // I update it every time to avoid weird memory bugs
    BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
    oAdvertisementData.setManufacturerData(std::string(beacon_data, DATALEN));
    this->pAdvertising->setAdvertisementData(oAdvertisementData);
    this->pAdvertising->setScanResponse(false);
    this->pAdvertising->start();
    delay(ADVERT_TIME); //Maybe there's a way to do this async so it doesn't block the entire component.
    this->pAdvertising->stop(); 
    delay(WAIT_TIME);
    //BLEDevice::getAdvertising()->stop(); //Hardcode a "stop advertising" just in case    
    //BLEDevice::deinit(true);
}
//--------Handler Methods---------------
void ColorHandler(float color_temperature, std::string lightname, char beacon_data[]){
  // Generic Handler for colour modes
      if(color_temperature <= COLOR_COOL){
        //Send "cool"
        ESP_LOGD("custom", "COOL White");
        if(lightname == "LABROOM")
            Cool_LabRoom(beacon_data);
        else if(lightname == "LIVROOM")
            Cool_LivRoom(beacon_data);
        else if(lightname=="BEDROOM")
            Cool_BedRoom(beacon_data);
        }
      else if (color_temperature >= COLOR_WARM){
        //Send "Warm white"
        ESP_LOGD("custom", "WARM White");
          if(lightname == "LABROOM")
            Warm_LabRoom(beacon_data);
          else if(lightname == "LIVROOM")
            Warm_LivRoom(beacon_data);
          else if(lightname == "BEDROOM")
            Warm_BedRoom(beacon_data);
      }
      else {
        //Send "Neutral white"
        ESP_LOGD("custom", "NEUTRAL White");
          if(lightname == "LABROOM")
            Neutral_LabRoom(beacon_data);
          else if(lightname == "LIVROOM")
            Neutral_LivRoom(beacon_data);
          else if(lightname == "BEDROOM")
            Neutral_BedRoom(beacon_data);
      }   
      SendBLECommand(beacon_data);
}
void BrightnessHandler(float bright, std::string lightname, char beacon_data[]){
  // Generic handler for "brightness" adjustment
      if(bright >= BRIGHTNESS_FULL){
        //Send "Full"
        ESP_LOGD("custom", "FULL Power");
          if(lightname == "LABROOM")
            FullPower_LabRoom(beacon_data);
          else  if(lightname == "LIVROOM")
            FullPower_LivRoom(beacon_data);
          else  if(lightname == "BEDROOM")
            FullPower_BedRoom(beacon_data);
      } else if (bright >= BRIGHTNESS_HALF){
        //Send "Half power"
        ESP_LOGD("custom", "HALF Power");
          if(lightname == "LABROOM")
            HalfPower_LabRoom(beacon_data);
          else  if(lightname == "LIVROOM")
            HalfPower_LivRoom(beacon_data);
          else  if(lightname == "BEDROOM")
            HalfPower_BedRoom(beacon_data);
      }
        else{
        //Send "NightMode" NOTE: Night Mode also changes the color...
        ESP_LOGD("custom", "Night Mode");
          if(lightname == "LABROOM")
            NightMode_LabRoom(beacon_data);
          else  if(lightname == "LIVROOM")
            NightMode_LivRoom(beacon_data);
          else  if(lightname == "BEDROOM")
            NightMode_BedRoom(beacon_data);
      }   
      SendBLECommand(beacon_data);
}

void ONOFFHandler(bool onoffstatus, std::string lightname, char beacon_data[]){
      if(onoffstatus){
        // Turn ON
        ESP_LOGD("custom", "TURN ON");
          if(lightname == "LABROOM")
            PowerON_LabRoom(beacon_data);
          else  if(lightname == "LIVROOM")
            PowerON_LivRoom(beacon_data);
          else  if(lightname == "BEDROOM")
            PowerON_BedRoom(beacon_data);
      }
      else{
        // TURN OFF
        ESP_LOGD("custom", "TURN OFF");
          if(lightname == "LABROOM")
            PowerOFF_LabRoom(beacon_data);
          else  if(lightname == "LIVROOM")
            PowerOFF_LivRoom(beacon_data);
          else  if(lightname == "BEDROOM")
            PowerOFF_BedRoom(beacon_data);
      }
      SendBLECommand(beacon_data);
}
};