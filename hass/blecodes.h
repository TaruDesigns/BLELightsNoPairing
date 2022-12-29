// --- Common Data
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


void beaconData_Null(char beacon_data[]) {
    //This is a function to "turn off" advert by setting random gibberish to the advertised data
    // This shouldn't be needed once the stop() method works properly
    beacon_data[0] = 0x00; // Begin starting sequence
    beacon_data[1] = 0x00;
    beacon_data[2] = 0x00;
    beacon_data[3] = 0x00;
    beacon_data[4] = 0x00;
    beacon_data[5] = 0x00;
    beacon_data[6] = 0x00;
    beacon_data[7] = 0x00;
    beacon_data[8] = 0x00;
    beacon_data[9] = 0x00;
    beacon_data[10] = 0x00;
    beacon_data[11] = 0x00;
    beacon_data[12] = 0x00; // End of "starting sequence"
    beacon_data[27] = 0x00; // Trailing bytes
    beacon_data[28] = 0x01; // Trailing bytes

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
     //Sets the Beacon Data for "Poer ON"
    beacon_data[13] = 0xD0; // Primary Function
    beacon_data[16] = 0x08; // Secondary Function 1
    beacon_data[17] = 0x24;// Secondary Function 2
    beacon_data[18] = 0xCB; // Secondary Function 3
    beacon_data[20] = 0xFC; // ThirdFunction
}

void beaconData_PowerOFF(char beacon_data[]) {
     //Sets the Beacon Data for "Power OFF"
    beacon_data[13] = 0x50; // Primary Function
    beacon_data[16] = 0x08; // Secondary Function 1
    beacon_data[17] = 0x24;// Secondary Function 2
    beacon_data[18] = 0xCB; // Secondary Function 3
    beacon_data[20] = 0xFC; // ThirdFunction

}

void beaconData_NightMode(char beacon_data[]) {
     //Sets the beacon data for "Night Mode"
    beacon_data[13] = 0x1C; // Primary Function
    beacon_data[16] = 0x08; // Secondary Function 1
    beacon_data[17] = 0x24;// Secondary Function 2
    beacon_data[18] = 0xCB; // Secondary Function 3
    beacon_data[20] = 0xFC; // ThirdFunction

}

void beaconData_Warm(char beacon_data[]) {
     //Sets the Beacon Data for "Mode 1" = warm white
    beacon_data[13] = 0x5C; // Primary Function
    beacon_data[16] = 0x08; // Secondary Function 1
    beacon_data[17] = 0xDB;// Secondary Function 2
    beacon_data[18] = 0xCB; // Secondary Function 3
    beacon_data[20] = 0xFE; // ThirdFunction

}

void beaconData_Cool(char beacon_data[]) {
     //Sets the Beacon Data for "Mode 2" = cool white
    beacon_data[13] = 0x5C; // Primary Function
    beacon_data[16] = 0xF7; // Secondary Function 1
    beacon_data[17] = 0x24;// Secondary Function 2
    beacon_data[18] = 0xCB; // Secondary Function 3
    beacon_data[20] = 0xFE; // ThirdFunction

}

void beaconData_Neutral(char beacon_data[]) {
     //Sets the Beacon Data for "Mode 1" = neutral white
    beacon_data[13] = 0x5C; // Primary Function
    beacon_data[16] = 0xF7; // Secondary Function 1
    beacon_data[17] = 0xDB;// Secondary Function 2
    beacon_data[18] = 0xCB; // Secondary Function 3
    beacon_data[20] = 0xFE; // ThirdFunction

}

void beaconData_HalfPower(char beacon_data[]) {
     //Sets the Beacon Data for half power
    beacon_data[13] = 0x5C; // Primary Function
    beacon_data[16] = 0xF6; // Secondary Function 1
    beacon_data[17] = 0xDA;// Secondary Function 2
    beacon_data[18] = 0xCB; // Secondary Function 3
    beacon_data[20] = 0x7C; // ThirdFunction

}


void beaconData_FullPower(char beacon_data[]) {
     //Sets the Beacon Data for full power
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

}


//------------Warm --------------------
void Warm_BedRoom(char beacon_data[]) {
    beaconData_Warm(beacon_data);
    beaconData_BedRoom(beacon_data);

    beacon_data[19] = 0x75; // Garbage 1
    
    beacon_data[21] = 0x6C;
    beacon_data[22] = 0xBE;
    beacon_data[23] = 0x73;
    beacon_data[24] = 0xC8;
    beacon_data[25] = 0x78;
    beacon_data[26] = 0x97;

}


void Warm_LabRoom(char beacon_data[]) {
    beaconData_Warm(beacon_data);
    beaconData_LabRoom(beacon_data);

    beacon_data[19] = 0x27; // Garbage 1

    beacon_data[21] = 0x1B;
    beacon_data[22] = 0xC9;
    beacon_data[23] = 0x1A;
    beacon_data[24] = 0xBF;
    beacon_data[25] = 0xFD;
    beacon_data[26] = 0x51;


}

void Warm_LivRoom(char beacon_data[]) {
    beaconData_Warm(beacon_data);
    beaconData_LivRoom(beacon_data);
 
    beacon_data[19] = 0x85; // Garbage 1

    beacon_data[21] = 0x7B;
    beacon_data[22] = 0xA9;
    beacon_data[23] = 0xBF;
    beacon_data[24] = 0xDF;
    beacon_data[25] = 0x1F;
    beacon_data[26] = 0xCE;


}

//------------Cool --------------------
void Cool_BedRoom(char beacon_data[]) {
    beaconData_Cool(beacon_data);
    beaconData_BedRoom(beacon_data);

    beacon_data[19] = 0x35; // Garbage 1
    
    beacon_data[21] = 0x92;
    beacon_data[22] = 0x40;
    beacon_data[23] = 0xD6;
    beacon_data[24] = 0x36;
    beacon_data[25] = 0x1E;
    beacon_data[26] = 0xD1;

}


void Cool_LabRoom(char beacon_data[]) {
    beaconData_Cool(beacon_data);
    beaconData_LabRoom(beacon_data);

    beacon_data[19] = 0xA7; // Garbage 1

    beacon_data[21] = 0xC5;
    beacon_data[22] = 0x17;
    beacon_data[23] = 0x7B;
    beacon_data[24] = 0x61;
    beacon_data[25] = 0x14;
    beacon_data[26] = 0x0E;

}

void Cool_LivRoom(char beacon_data[]) {
    beaconData_Cool(beacon_data);
    beaconData_LivRoom(beacon_data);
 
    beacon_data[19] = 0x45; // Garbage 1

    beacon_data[21] = 0x92;
    beacon_data[22] = 0x40;
    beacon_data[23] = 0xB8;
    beacon_data[24] = 0x36;
    beacon_data[25] = 0xD2;
    beacon_data[26] = 0xBE;


}


//------------Neutral --------------------
void Neutral_BedRoom(char beacon_data[]) {
    beaconData_Neutral(beacon_data);
    beaconData_BedRoom(beacon_data);

    beacon_data[19] = 0xB5; // Garbage 1
    
    beacon_data[21] = 0xCA;
    beacon_data[22] = 0x18;
    beacon_data[23] = 0xFB;
    beacon_data[24] = 0x6E;
    beacon_data[25] = 0x69;
    beacon_data[26] = 0xBF;

}


void Neutral_LabRoom(char beacon_data[]) {
    beaconData_Neutral(beacon_data);
    beaconData_LabRoom(beacon_data);

    beacon_data[19] = 0xD7; // Garbage 1

    beacon_data[21] = 0xC3;
    beacon_data[22] = 0x11;
    beacon_data[23] = 0xA4;
    beacon_data[24] = 0x67;
    beacon_data[25] = 0xC1;
    beacon_data[26] = 0x8C;


}

void Neutral_LivRoom(char beacon_data[]) {
    beaconData_Neutral(beacon_data);
    beaconData_LivRoom(beacon_data);
 
    beacon_data[19] = 0x05; // Garbage 1

    beacon_data[21] = 0x78;
    beacon_data[22] = 0xAA;
    beacon_data[23] = 0x08;
    beacon_data[24] = 0xDC;
    beacon_data[25] = 0x5B;
    beacon_data[26] = 0x14;


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


}
