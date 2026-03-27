/*
    denki-kurage Remix

    https://github.com/likeablob/denki-kurage

        * a "little" bit reorganization due to IDE compile process
        * moved to Arduino_GFX_Library because display is qspi
        * removed debug mode
        * "scalefactor" for zooming jellyfish bigger/smaller
        * "biggerparticles" and "evenbiggerparticles" for particle size
        * added a clock (because we all love clocks)
          (Wifi for sync, only connected once a hour)

    ESP32-S3 Zero 4+2
        usb cdc on boot: enabled
        flash size: 4 mb
        psram: QSPI (2mb)
        partition scheme: default 1.2 app, 1.5 fatfs

    tft: 1.53 Inch round ST77916 qspi tft, no touch

    lembke@gmail.com 2026-03-24/26
*/

#pragma GCC optimize("Ofast")

//#include <XPT2046_Touchscreen_TT.h>  // https://github.com/tedtoal/XPT2046_Touchscreen_TT
#include <Arduino_GFX_Library.h>  // https://github.com/moononournation/Arduino_GFX

void setup() {
  setupR();
}

void loop() {
  loopR();
}

//