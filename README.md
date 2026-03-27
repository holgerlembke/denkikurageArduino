# `Denki Kurage` (ja: 電気くらげ, electric jellyfish)

<img src="./resources/front.jpg">

This is a remix of [Denki Kurage](https://github.com/likeablob/denki-kurage) that is compilable with the Arduino IDE.

<video src="https://github.com/user-attachments/assets/1f0c8e98-27a2-492e-a7ed-1c2e2f27417f" autoplay loop playsInline></video>

## Changes
* moved to [Arduino_GFX_Library](https://github.com/moononournation/Arduino_GFX) from moononournation because display is qspi
(it has more display types, too)
* a "little" bit reorganization due to IDE compile process
* removed debug mode
* "scalefactor" for zooming jellyfish bigger/smaller
* "biggerparticles" and "evenbiggerparticles" for particle size
* added a clock (because we all love clocks) 
  Wifi for sync, only connected once a hour

## Case

<img src="./resources/cad.jpg">

[Keili](https://github.com/holgerlembke/OpenSCAD-Modelle/blob/main/denkikurage.scad)

## the inside

ESP32-S3 Zero 4+2,"1.53 Inch round ST77916 qspi tft", Uhu Montage Ultra

<img src="./resources/innen.jpg">

The Zero has a strip of Kaptone tape at iss back. That is then glued to the base.
