#include <sys/time.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <esp_sleep.h>

#define GPIO_DEEP_SLEEP_DURATION      1
#define DATALEN 29 // Length of array "beacon data". Note: The full data is actually 30 bytes but one of them is set by default

RTC_DATA_ATTR static time_t last;
RTC_DATA_ATTR static uint32_t bootcount;

BLEAdvertising *pAdvertising;
struct timeval now; 

//Common functions for beacon data stuff
char beacon_data[DATALEN];

void setBeacon(char beacon_data[]) {
    BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
    oAdvertisementData.setFlags(0x01);
    oAdvertisementData.setManufacturerData(std::string(beacon_data, DATALEN));
    pAdvertising->setAdvertisementData(oAdvertisementData);
    pAdvertising->setScanResponse(false);
}

void AdvertisePress(){
  //"Cycle" the advertisement beacon like a button press
  pAdvertising->start();
  Serial.println("ad on");  
  delay(2000);
  pAdvertising->stop();
  Serial.println("ad off");    
}

// --- Common Data, will only be used once
void beaconData_Common(char beacon_data[]) {
     //Sets the "Common" Beacon Data
    beacon_data[0] = 0x56; // Begin starting sequence
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
    beacon_data[12] = 0x51; // End of "starting sequence"
    beacon_data[27] = 0x60; // Trailing bytes
    beacon_data[28] = 0x57; // Trailing bytes

}

//---- Addresses for each remote ----
void beaconData_LabRoom(char beacon_data[]) {
    // TODO This is really dirty, need to change it to use an input argument instead
    beacon_data[14] = 0x3F; // Address 1
    beacon_data[15] = 0x94; // Address 2
}

void beaconData_BedRoom(char beacon_data[]) {
    beacon_data[14] = 0xF0; // Address 1
    beacon_data[15] = 0x97; // Address 2
}

void beaconData_LivRoom(char beacon_data[]) {
    beacon_data[14] = 0x72; // Address 1
    beacon_data[15] = 0x9c; // Address 2
}

// --- Specific Functions ---
void beaconData_PowerON(char beacon_data[]) {
     //Sets the "Common" Beacon Data
    beacon_data[13] = 0xD0; // Primary Function
    beacon_data[16] = 0x08; // Secondary Function 1
    beacon_data[17] = 0x24;// Secondary Function 2
    beacon_data[18] = 0xCB; // Secondary Function 3
    beacon_data[20] = 0xFC; // ThirdFunction
}

void beaconData_PowerOFF(char beacon_data[]) {
     //Sets the "Common" Beacon Data
    beacon_data[13] = 0x50; // Primary Function
    beacon_data[16] = 0x08; // Secondary Function 1
    beacon_data[17] = 0x24;// Secondary Function 2
    beacon_data[18] = 0xCB; // Secondary Function 3
    beacon_data[20] = 0xFC; // ThirdFunction
}
//------------Power ON --------------------


void PowerON_LabRoom(char beacon_data[]) {
    beaconData_PowerON(beacon_data);
    beaconData_LabRoom(beacon_data);

    beacon_data[19] = 0x2F; // Garbage 1

    beacon_data[21] = 0x0D;
    beacon_data[22] = 0xDF;
    beacon_data[23] = 0x0C;
    beacon_data[24] = 0xA9;
    beacon_data[25] = 0xF8;
    beacon_data[26] = 0xCC;

    setBeacon(beacon_data);
    AdvertisePress();
}



void PowerOFF_Test(char beacon_data[]) {
beacon_data[0] = 0x02; 
beacon_data[1] = 0x1b; 
beacon_data[2] = 0x16; 
beacon_data[3] = 0xf0; 
beacon_data[4] = 0x08; 
beacon_data[5] = 0x10; 
beacon_data[6] = 0x00; 
beacon_data[7] = 0x8a; 
beacon_data[8] = 0xd0; 
beacon_data[9] = 0xc5; 
beacon_data[10] = 0x0c;
beacon_data[11] = 0x25;
beacon_data[12] = 0xc4;
beacon_data[13] = 0x5f;
beacon_data[14] = 0x1f;
beacon_data[15] = 0x0b;
beacon_data[16] = 0x9a;
beacon_data[17] = 0x22;
beacon_data[18] = 0xcb;
beacon_data[19] = 0xef;
beacon_data[20] = 0xfe;
beacon_data[21] = 0x3d;
beacon_data[22] = 0xd9;
beacon_data[23] = 0x39;
beacon_data[24] = 0xe2;
beacon_data[25] = 0xfa;
beacon_data[26] = 0x9c;
beacon_data[27] = 0xd6;
beacon_data[28] = 0x0d;

    setBeacon(beacon_data);
    AdvertisePress();
}



//------------Power OFF --------------------



void PowerOFF_LabRoom(char beacon_data[]) {
    beaconData_PowerOFF(beacon_data);
    beaconData_LabRoom(beacon_data);

    beacon_data[19] = 0xEF; // Garbage 1

    beacon_data[21] = 0xB1;
    beacon_data[22] = 0x63;
    beacon_data[23] = 0xE5;
    beacon_data[24] = 0x15;
    beacon_data[25] = 0xE6;
    beacon_data[26] = 0x43;

    setBeacon(beacon_data);
    AdvertisePress();
}



//------------NightMode --------------------




//---------------ARDUINO-----------
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
  beaconData_Common(beacon_data);
}



void loop(){
  beaconData_Common(beacon_data);
  // Turn LAB OFF
  /*Serial.println("Turning LAB OFF...");
  PowerOFF_LabRoom(beacon_data);
  delay(4500);*/
  // Turn LAB ON
  Serial.println("Turning LAB ON..."); 
  PowerON_LabRoom(beacon_data);
  delay(4500);

  // Turn LAB OFF: ALTERNATE
  Serial.println("ALTERNATE: Turning LAB OFF..."); 
  PowerOFF_Test(beacon_data);
  delay(4500);  

}