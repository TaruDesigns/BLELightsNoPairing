#include <sys/time.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <esp_sleep.h>
#define GPIO_DEEP_SLEEP_DURATION     1
#define ADV_LEN 31
RTC_DATA_ATTR static time_t last;
RTC_DATA_ATTR static uint32_t bootcount;

BLEAdvertising *pAdvertising;
struct timeval now; 

void setBeacon() {
  char beacon_data[29];
  uint16_t beconUUID = 0x1EFF;
  uint32_t tmil = now.tv_sec*10;
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  //oAdvertisementData.setFlags(0x06); // GENERAL_DISC_MODE 0x02 | BR_EDR_NOT_SUPPORTED 0x04
  //0x1EFF5655188752B65F2B5E00FC3151503F940824CBF1FC5C8EA3F825856057 OFF LAB
beacon_data[0] = 0x56;
beacon_data[1] = 0x55;
beacon_data[2] = 0x18;
beacon_data[3] = 0x87;
beacon_data[4] = 0x52;
beacon_data[5] = 0xB6;
beacon_data[6] = 0x5F;
beacon_data[7] = 0x2B;
beacon_data[8] = 0x5E;
beacon_data[9] = 0x00;
beacon_data[10] = 0xFC;
beacon_data[11] = 0x31;
beacon_data[12] = 0x51;
beacon_data[13] = 0x50;
beacon_data[14] = 0x3F;
beacon_data[15] = 0x94;
beacon_data[16] = 0x08;
beacon_data[17] = 0x24;
beacon_data[18] = 0xCB;
beacon_data[19] = 0xF1;
beacon_data[20] = 0xFC;
beacon_data[21] = 0x5C;
beacon_data[22] = 0x8E;
beacon_data[23] = 0xA3;
beacon_data[24] = 0xF8;
beacon_data[25] = 0x25;
beacon_data[26] = 0x85;
beacon_data[27] = 0x60;
beacon_data[28] = 0x57;

  oAdvertisementData.setManufacturerData(std::string(beacon_data, 29));
  pAdvertising->setScanResponseData(oAdvertisementData);
  //pAdvertising->setAdvertisementType(ADV_TYPE_NONCONN_IND);
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