#include <sys/time.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <esp_sleep.h>
#define GPIO_DEEP_SLEEP_DURATION     1
RTC_DATA_ATTR static time_t last;
RTC_DATA_ATTR static uint32_t bootcount;

BLEAdvertising *pAdvertising;
struct timeval now; 
void setBeacon() {
  char beacon_data[22];
  uint16_t beconUUID = 0xFEAA;
  uint16_t volt = 3300; // 3300mV = 3.3V
  uint16_t temp = (uint16_t)((float)23.00);
  uint32_t tmil = now.tv_sec*10;
  uint8_t temp_farenheit;
  float temp_celsius;
  temp_farenheit= 25;
  temp_celsius = 20;
  temp = (uint16_t)(temp_celsius);
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  oAdvertisementData.setFlags(0x06); // GENERAL_DISC_MODE 0x02 | BR_EDR_NOT_SUPPORTED 0x04
  oAdvertisementData.setCompleteServices(BLEUUID(beconUUID));
  //0x1EFF5655188752B65F2B5E00FC3151503F940824CBF1FC5C8EA3F825856057 OFF LAB
    beacon_data[0] = 0x1E;  // Eddystone Frame Type (Eddystone-URL)
    beacon_data[1] = 0xFF;  // Beacons TX power at 0m
    beacon_data[2] = 0x56;  // URL Scheme ‘https://’
    beacon_data[3] = 0x55;  // URL add  1
    beacon_data[4] = 'v';  // URL add  2
    beacon_data[5] = 'o';  // URL add  3
    beacon_data[6] = 't';  // URL add  4
    beacon_data[7] = 'r';  // URL add  5
    beacon_data[8] = 'i';  // URL add  6
    beacon_data[9] = 'x';  // URL add  7
    beacon_data[10] = '.';  // URL add  8
    beacon_data[11] = 'c';  // URL add  9
    beacon_data[12] = 'o';  // URL add 10
    beacon_data[13] = 'm';  // URL add 11

  oAdvertisementData.setServiceData(BLEUUID(beconUUID), std::string(beacon_data, 14));
//14:- the size of url that you have put  in beacon_data[]array.
  pAdvertising->setScanResponseData(oAdvertisementData);
}

void setup() {
  Serial.begin(115200);
  gettimeofday(&now, NULL);
  Serial.printf("start ESP32 %dn",bootcount++);
  Serial.printf("deep sleep (%lds since last reset, %lds since last boot)n",now.tv_sec,now.tv_sec-last);
  last = now.tv_sec;
  // Create the BLE Device
  BLEDevice::init("ESP32");
  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pAdvertising = pServer->getAdvertising();
  setBeacon();
   // Start advertising
  Serial.printf("enter deep sleepn");
  delay(1000);
  Serial.printf("in deep sleepn");
}

void loop(){
  pAdvertising->start();
  Serial.println("Advertizing started…");
  delay(1000);
  pAdvertising->stop();
   Serial.println("Advertizing stopped…");
  delay(1000);
}
