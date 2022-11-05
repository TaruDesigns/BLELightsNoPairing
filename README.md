# BLELightsNoPairing

This repo will have my results on reverse engineering some LED Lights I got off Amazon a while ago. My intention is to link these lights to HomeAssistant using a BLE device (ESP32 or the same Raspberry Pi 4 if i'm lucky)


# The Lights

The LED Lights are some indoor white LED lighting (Not RGB) with the ability to change the colour temperature and brightness. I went for this, but there are other similar models
![Lights](img/light1.jpg)
They are sold under many different brand names, but they have similar characteristics:
- Wall/Ceiling mounted, usually with a plate that is screwed onto the wall and the actual light being clipped onto that plate.
- Anywhere between 18 and 36W, with 24W being the most common
- Controlled with a remote
- Dimmable (with said remote) and also able to change temperature colour
These lights come with a remote to change the brightness, temperature, and a few other functions. Here's the remote on my lights:

![Remote](img/remote.jpg)

I haven't checked with other lights but I would guess they use a similar remote, most likely, they are using the same circuitry for both the lights and the remote, but they're just packaging it differently.

# The problem

The remote is not an IR remote like many LED strips use, but rather a BLE (Bluetooth Low Energy) device. The lights themselves however don't show up when you try to scan for bluetooth devices, and they don't have any button or anything to switch them to pairing mode.

# How "pairing" works for these lights:

When the lights are *powered on*, holding the "turn on" button will make the lights blink after ~3 seconds. From that moment, the remote is paired with that light.
If you have multiple, you can repeat the same process, eventually linking the same remote to a few different lights, so that the settings are all shared between them.

Alternatively, this same method can be used to link a phone using the [Lamp Smart App](https://play.google.com/store/apps/details?id=com.alllink.smart_lighting&hl=en&gl=GB) to those lights.

Note that when I say *powered on* I mean when the power switch (the switch on the wall) is flipped off and then back on again, not when the lights are turned off with the remote.


# Discoveries

I've used [nRF Connect](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&hl=en) to scan the packets the remote sends.
- Worthy of note, multiple devices can be "paired" with the lights. I've succesfully done the pairing method with the original remote, my phone using the Lamp Smart App, and the same phone simply redoing the "Pairing" packet previously captured by nRF. The lights don't seem to "forget" the previous devices, and I can control them with either the remote or my phone.
- Rather than having the lights be in discoverable mode, it seems that the lights are simply scanning for certain advertising packets at all times. I have tried to reverse engineer the packets sent by one of the remotes:
- First byte "Type" is always 0xFF
- Next 2 bytes are the "address". In my remotes it seems it's always "5655"
- The rest is custom data, examples:
  "MODE" Button
5655188752b65f2b5e00fc31515c729cf724cbd7fee331a24757716057 - Cool White Full
5655188752b65f2b5e00fc31515c729cf7dbcb73fe4d9f65e901896057 - Neutral White Full
5655188752b65f2b5e00fc31515c729c08dbcbf3fe4b9982ef89c96057 - Candlelight Full
"OFF" button
5655188752b65f2b5e00fc315150729c0824cb67fcce1c536ad0666057
"ON" Button
5655188752b65f2b5e00fc3151d0729c0824cb87fcc3114c678dd96057
"Full Power" Button
5655188752b65f2b5e00fc31515c729cf7dbcb47bc11c302b5f6fd6057
"half power" BUtton
5655188752b65f2b5e00fc31515c729cf6dacbc77cbe6c121adc9b6057
"Night power" Button
5655188752b65f2b5e00fc31511c729c0824cb79fca775c503c7756057

Using NRFConnect, I can set up the dongle as advertiser, 50ms advertising interval, and the packets will make the lights react accordingly.
- For the "mode" button in the remote, the remote itself is the one doing the "cycling", each mode has a distinct code associated with it.


After grabbing data from 3 different remotes:

- First Byte: "Type" is 0xFF
- Following bytes: 0x5655
- The next bytes are as follows:
  - 11 bytes of static data: 188752b65f2b5e00fc3151 -> Same for every remote
  - 1 Byte for the "Function":
    - 0xD0 for "ON"
    - 0x50 for "OFF"
    - 0x5C for "Change Mode" -> It seems that "FullBrightness" and "Half Brightness" are both in the same class as "Mode cycle" (Middle button)
    - 0x1C for "Night mode" -> This button goes to minimum brightness and colour temperature
  - 2 Bytes: -> Possibly a "unique address" for each remote
  - 3 Bytes: -> Related to each specific function
  - 1 Byte: -> Possibly some checksum or just random data. Completely different on each button press
  - 1 Byte: -> Related to each specific function
  - 6 Bytes: -> Possibly some checksum or just random data. Completely different on each button press
  - 2 Bytes: -> Always 0x6057