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

void beaconData_NightMode(char beacon_data[]) {
     //Sets the "Common" Beacon Data
    beacon_data[13] = 0x1C; // Primary Function
    beacon_data[16] = 0x08; // Secondary Function 1
    beacon_data[17] = 0x24;// Secondary Function 2
    beacon_data[18] = 0xCB; // Secondary Function 3
    beacon_data[20] = 0xFC; // ThirdFunction

}

void beaconData_Mode1(char beacon_data[]) {
     //Sets the "Common" Beacon Data
    beacon_data[13] = 0x5C; // Primary Function
    beacon_data[16] = 0x08; // Secondary Function 1
    beacon_data[17] = 0xDB;// Secondary Function 2
    beacon_data[18] = 0xCB; // Secondary Function 3
    beacon_data[20] = 0xFE; // ThirdFunction

}

void beaconData_Mode2(char beacon_data[]) {
     //Sets the "Common" Beacon Data
    beacon_data[13] = 0x5C; // Primary Function
    beacon_data[16] = 0xF7; // Secondary Function 1
    beacon_data[17] = 0x24;// Secondary Function 2
    beacon_data[18] = 0xCB; // Secondary Function 3
    beacon_data[20] = 0xFE; // ThirdFunction

}

void beaconData_Mode3(char beacon_data[]) {
     //Sets the "Common" Beacon Data
    beacon_data[13] = 0x5C; // Primary Function
    beacon_data[16] = 0xF7; // Secondary Function 1
    beacon_data[17] = 0xDB;// Secondary Function 2
    beacon_data[18] = 0xCB; // Secondary Function 3
    beacon_data[20] = 0xFE; // ThirdFunction

}

void beaconData_HalfPower(char beacon_data[]) {
     //Sets the "Common" Beacon Data
    beacon_data[13] = 0x5C; // Primary Function
    beacon_data[16] = 0xF6; // Secondary Function 1
    beacon_data[17] = 0xDA;// Secondary Function 2
    beacon_data[18] = 0xCB; // Secondary Function 3
    beacon_data[20] = 0x7C; // ThirdFunction

}


void beaconData_FullPower(char beacon_data[]) {
     //Sets the "Common" Beacon Data
    beacon_data[13] = 0x5C; // Primary Function
    beacon_data[16] = 0xF7; // Secondary Function 1
    beacon_data[17] = 0xDB;// Secondary Function 2
    beacon_data[18] = 0xCB; // Secondary Function 3
    beacon_data[20] = 0xBC; // ThirdFunction

}

//------------Power ON --------------------
void PowerON_BedRoom(char beacon_data[]) {
    beaconData_PowerON(beacon_data);
    beaconData_BedRoom(beacon_data);

    beacon_data[19] = 0xC1; // Garbage 1
    
    beacon_data[21] = 0xB1;
    beacon_data[22] = 0x63;
    beacon_data[23] = 0x54;
    beacon_data[24] = 0x15;
    beacon_data[25] = 0x94;
    beacon_data[26] = 0x50;

    setBeacon(beacon_data);
    AdvertisePress();
}


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

void PowerON_LivRoom(char beacon_data[]) {
    beaconData_PowerON(beacon_data);
    beaconData_LivRoom(beacon_data);
 
    beacon_data[19] = 0x0D; // Garbage 1

    beacon_data[21] = 0x40;
    beacon_data[22] = 0x92;
    beacon_data[23] = 0x7B;
    beacon_data[24] = 0xE4;
    beacon_data[25] = 0x6B;
    beacon_data[26] = 0x83;

    setBeacon(beacon_data);
    AdvertisePress();
}

//------------Power OFF --------------------
void PowerOFF_BedRoom(char beacon_data[]) {
    beaconData_PowerOFF(beacon_data);
    beaconData_BedRoom(beacon_data);

    beacon_data[19] = 0x1D; // Garbage 1
    
    beacon_data[21] = 0x4F;
    beacon_data[22] = 0x9D;
    beacon_data[23] = 0xC8;
    beacon_data[24] = 0xEB;
    beacon_data[25] = 0xA7;
    beacon_data[26] = 0x50;

    setBeacon(beacon_data);
    AdvertisePress();
}


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

void PowerOFF_LivRoom(char beacon_data[]) {
    beaconData_PowerOFF(beacon_data);
    beaconData_LivRoom(beacon_data);
 
    beacon_data[19] = 0xCD; // Garbage 1

    beacon_data[21] = 0xAB;
    beacon_data[22] = 0x79;
    beacon_data[23] = 0x5F;
    beacon_data[24] = 0x0F;
    beacon_data[25] = 0x85;
    beacon_data[26] = 0xB2;

    setBeacon(beacon_data);
    AdvertisePress();
}

//------------NightMode --------------------
void NightMode_BedRoom(char beacon_data[]) {
    beaconData_NightMode(beacon_data);
    beaconData_BedRoom(beacon_data);

    beacon_data[19] = 0x8D; // Garbage 1
    
    beacon_data[21] = 0x9E;
    beacon_data[22] = 0x4C;
    beacon_data[23] = 0xC9;
    beacon_data[24] = 0x3A;
    beacon_data[25] = 0x1C;
    beacon_data[26] = 0x5E;

    setBeacon(beacon_data);
    AdvertisePress();
}


void NightMode_LabRoom(char beacon_data[]) {
    beaconData_NightMode(beacon_data);
    beaconData_LabRoom(beacon_data);

    beacon_data[19] = 0x17; // Garbage 1

    beacon_data[21] = 0x18;
    beacon_data[22] = 0xCA;
    beacon_data[23] = 0x23;
    beacon_data[24] = 0xBC;
    beacon_data[25] = 0x06;
    beacon_data[26] = 0xF3;

    setBeacon(beacon_data);
    AdvertisePress();
}

void NightMode_LivRoom(char beacon_data[]) {
    beaconData_NightMode(beacon_data);
    beaconData_LivRoom(beacon_data);
 
    beacon_data[19] = 0xA5; // Garbage 1

    beacon_data[21] = 0xE2;
    beacon_data[22] = 0x30;
    beacon_data[23] = 0x0D;
    beacon_data[24] = 0x46;
    beacon_data[25] = 0x95;
    beacon_data[26] = 0x62;

    setBeacon(beacon_data);
    AdvertisePress();
}


//------------Mode1 --------------------
void Mode1_BedRoom(char beacon_data[]) {
    beaconData_Mode1(beacon_data);
    beaconData_BedRoom(beacon_data);

    beacon_data[19] = 0x75; // Garbage 1
    
    beacon_data[21] = 0x6C;
    beacon_data[22] = 0xBE;
    beacon_data[23] = 0x73;
    beacon_data[24] = 0xC8;
    beacon_data[25] = 0x78;
    beacon_data[26] = 0x97;

    setBeacon(beacon_data);
    AdvertisePress();
}


void Mode1_LabRoom(char beacon_data[]) {
    beaconData_Mode1(beacon_data);
    beaconData_LabRoom(beacon_data);

    beacon_data[19] = 0x27; // Garbage 1

    beacon_data[21] = 0x1B;
    beacon_data[22] = 0xC9;
    beacon_data[23] = 0x1A;
    beacon_data[24] = 0xBF;
    beacon_data[25] = 0xFD;
    beacon_data[26] = 0x51;

    setBeacon(beacon_data);
    AdvertisePress();
}

void Mode1_LivRoom(char beacon_data[]) {
    beaconData_Mode1(beacon_data);
    beaconData_LivRoom(beacon_data);
 
    beacon_data[19] = 0x85; // Garbage 1

    beacon_data[21] = 0x7B;
    beacon_data[22] = 0xA9;
    beacon_data[23] = 0xBF;
    beacon_data[24] = 0xDF;
    beacon_data[25] = 0x1F;
    beacon_data[26] = 0xCE;

    setBeacon(beacon_data);
    AdvertisePress();
}

//------------Mode2 --------------------
void Mode2_BedRoom(char beacon_data[]) {
    beaconData_Mode2(beacon_data);
    beaconData_BedRoom(beacon_data);

    beacon_data[19] = 0x35; // Garbage 1
    
    beacon_data[21] = 0x92;
    beacon_data[22] = 0x40;
    beacon_data[23] = 0xD6;
    beacon_data[24] = 0x36;
    beacon_data[25] = 0x1E;
    beacon_data[26] = 0xD1;

    setBeacon(beacon_data);
    AdvertisePress();
}


void Mode2_LabRoom(char beacon_data[]) {
    beaconData_Mode2(beacon_data);
    beaconData_LabRoom(beacon_data);

    beacon_data[19] = 0xA7; // Garbage 1

    beacon_data[21] = 0xC5;
    beacon_data[22] = 0x17;
    beacon_data[23] = 0x7B;
    beacon_data[24] = 0x61;
    beacon_data[25] = 0x14;
    beacon_data[26] = 0x0E;

    setBeacon(beacon_data);
    AdvertisePress();
}

void Mode2_LivRoom(char beacon_data[]) {
    beaconData_Mode2(beacon_data);
    beaconData_LivRoom(beacon_data);
 
    beacon_data[19] = 0x45; // Garbage 1

    beacon_data[21] = 0x92;
    beacon_data[22] = 0x40;
    beacon_data[23] = 0xB8;
    beacon_data[24] = 0x36;
    beacon_data[25] = 0xD2;
    beacon_data[26] = 0xBE;

    setBeacon(beacon_data);
    AdvertisePress();
}


//------------Mode3 --------------------
void Mode3_BedRoom(char beacon_data[]) {
    beaconData_Mode3(beacon_data);
    beaconData_BedRoom(beacon_data);

    beacon_data[19] = 0xB5; // Garbage 1
    
    beacon_data[21] = 0xCA;
    beacon_data[22] = 0x18;
    beacon_data[23] = 0xFB;
    beacon_data[24] = 0x6E;
    beacon_data[25] = 0x69;
    beacon_data[26] = 0xBF;

    setBeacon(beacon_data);
    AdvertisePress();
}


void Mode3_LabRoom(char beacon_data[]) {
    beaconData_Mode3(beacon_data);
    beaconData_LabRoom(beacon_data);

    beacon_data[19] = 0xD7; // Garbage 1

    beacon_data[21] = 0xC3;
    beacon_data[22] = 0x11;
    beacon_data[23] = 0xA4;
    beacon_data[24] = 0x67;
    beacon_data[25] = 0xC1;
    beacon_data[26] = 0x8C;

    setBeacon(beacon_data);
    AdvertisePress();
}

void Mode3_LivRoom(char beacon_data[]) {
    beaconData_Mode3(beacon_data);
    beaconData_LivRoom(beacon_data);
 
    beacon_data[19] = 0x05; // Garbage 1

    beacon_data[21] = 0x78;
    beacon_data[22] = 0xAA;
    beacon_data[23] = 0x08;
    beacon_data[24] = 0xDC;
    beacon_data[25] = 0x5B;
    beacon_data[26] = 0x14;

    setBeacon(beacon_data);
    AdvertisePress();
}

//------------HalfPower --------------------
void HalfPower_BedRoom(char beacon_data[]) {
    beaconData_HalfPower(beacon_data);
    beaconData_BedRoom(beacon_data);

    beacon_data[19] = 0x6D; // Garbage 1
    
    beacon_data[21] = 0x88;
    beacon_data[22] = 0x5A;
    beacon_data[23] = 0xC5;
    beacon_data[24] = 0x2C;
    beacon_data[25] = 0xA8;
    beacon_data[26] = 0x0F;

    setBeacon(beacon_data);
    AdvertisePress();
}


void HalfPower_LabRoom(char beacon_data[]) {
    beaconData_HalfPower(beacon_data);
    beaconData_LabRoom(beacon_data);

    beacon_data[19] = 0xB7; // Garbage 1

    beacon_data[21] = 0x4B;
    beacon_data[22] = 0x99;
    beacon_data[23] = 0x60;
    beacon_data[24] = 0xEF;
    beacon_data[25] = 0x9A;
    beacon_data[26] = 0x2E;

    setBeacon(beacon_data);
    AdvertisePress();
}

void HalfPower_LivRoom(char beacon_data[]) {
    beaconData_HalfPower(beacon_data);
    beaconData_LivRoom(beacon_data);
 
    beacon_data[19] = 0x55; // Garbage 1

    beacon_data[21] = 0x30;
    beacon_data[22] = 0xE2;
    beacon_data[23] = 0x94;
    beacon_data[24] = 0x94;
    beacon_data[25] = 0xE2;
    beacon_data[26] = 0x8C;

    setBeacon(beacon_data);
    AdvertisePress();
}

//------------FullPower --------------------
void FullPower_BedRoom(char beacon_data[]) {
    beaconData_FullPower(beacon_data);
    beaconData_BedRoom(beacon_data);

    beacon_data[19] = 0xDD; // Garbage 1
    
    beacon_data[21] = 0x84;
    beacon_data[22] = 0x56;
    beacon_data[23] = 0xDC;
    beacon_data[24] = 0x20;
    beacon_data[25] = 0x03;
    beacon_data[26] = 0x88;

    setBeacon(beacon_data);
    AdvertisePress();
}


void FullPower_LabRoom(char beacon_data[]) {
    beaconData_FullPower(beacon_data);
    beaconData_LabRoom(beacon_data);

    beacon_data[19] = 0xCF; // Garbage 1

    beacon_data[21] = 0x30;
    beacon_data[22] = 0xE2;
    beacon_data[23] = 0xE2;
    beacon_data[24] = 0x94;
    beacon_data[25] = 0x58;
    beacon_data[26] = 0xBC;

    setBeacon(beacon_data);
    AdvertisePress();

}

void FullPower_LivRoom(char beacon_data[]) {
    beaconData_FullPower(beacon_data);
    beaconData_LivRoom(beacon_data);
 
    beacon_data[19] = 0x75; // Garbage 1

    beacon_data[21] = 0x0F;
    beacon_data[22] = 0xDD;
    beacon_data[23] = 0x41;
    beacon_data[24] = 0xAB;
    beacon_data[25] = 0xFA;
    beacon_data[26] = 0xB7;

    setBeacon(beacon_data);
    AdvertisePress();
}


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
  // Turn LAB ON
  Serial.println("Turning LAB ON..."); 
  PowerON_LabRoom(beacon_data);
  delay(4500);
  // Turn LAB OFF
  Serial.println("Turning LAB OFF...");
  PowerOFF_LabRoom(beacon_data);
  delay(4500);
  // Turn LAB ON
  Serial.println("Turning LAB ON..."); 
  PowerON_LabRoom(beacon_data);
  delay(4500);
  // Turn NIGHT MODE
  Serial.println("Turn NIGHT MODE...");
  NightMode_LabRoom(beacon_data);
  delay(4500);
  // Turn Mode LowTemp 
  Serial.println("Turn Mode LowTemp ...");
  Mode1_LabRoom(beacon_data);
  delay(4500);

  // Turn Mode HiTemp
  Serial.println("Turn Mode HiTemp...");
  Mode2_LabRoom(beacon_data);
  delay(4500); 

  // Turn Mode MidTemp
  Serial.println("Turn Mode MidTemp...");
  Mode3_LabRoom(beacon_data);
  delay(4500);  

  // Turn HalfPower
  Serial.println("Turn HalfPower...");
  HalfPower_LabRoom(beacon_data);
  delay(4500); 

  // Turn FullPower
  Serial.println("Turn FullPower...");
  FullPower_BedRoom(beacon_data);
  delay(4500);  
    

}