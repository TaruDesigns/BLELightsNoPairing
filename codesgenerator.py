#Generate codes and output to CSV
import secrets



beginstring = "5655188752b65f2b5e00fc3151"

dFirstParameter = {
    "ON__POWER": "D0",
    "OFF_POWER": "50",
    "FULLPOWER": "5C",
    "HALFPOWER": "5C",
    "NIGHTMODE": "1C",
    "MODECYCLE_1": "5C",
    "MODECYCLE_2": "5C",
    "MODECYCLE_3": "5C",
}

dAddresses = {
    "LABROOM": "3F94",
    "LIVROOM": "729C",
    "BEDROOM": "F097",
}

dSecondParameter = {
    "ON__POWER": "0824CB",
    "OFF_POWER": "0824CB",
    "FULLPOWER": "F7DBCB",
    "HALFPOWER": "F6DACB",
    "NIGHTMODE": "0824CB",
    "MODECYCLE_1": "08DBCB",
    "MODECYCLE_2": "F724CB",
    "MODECYCLE_3": "F7DBCB"
}

firstchecksum = str(secrets.token_hex(1))

dFirstChecksum = {
    "LABROOM": "3F94",
    "LIVROOM": "729C",
    "BEDROOM": "F097",
}

dThirdParameter = {
    "ON__POWER": "FC",
    "OFF_POWER": "FC",
    "FULLPOWER": "BC",
    "HALFPOWER": "7C",
    "NIGHTMODE": "FC",
    "MODECYCLE_1": "FE",
    "MODECYCLE_2": "FE",
    "MODECYCLE_3": "FE"
}

secondchecksum = str(secrets.token_hex(6))

endingstring = "6057"

RemoteToUse = "LABROOM"

for key in dFirstParameter.keys():
    finalstring = beginstring \
    + dFirstParameter[key] \
    + dAddresses[RemoteToUse] \
    + dSecondParameter[key] \
    + firstchecksum \
    + dThirdParameter[key] \
    + secondchecksum \
    + endingstring
    print(key + " : " + finalstring.lower())

