Arduino_Canvas *canvas = NULL;

//********************************************************************************************************************************
void setupGFX() {
  Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    13 /* CS */, 7 /* SCK */, 8 /* D0 */, 9 /* D1 */, 10 /* D2 */,
    11 /* D3 */, false /* is_shared_interface */);

  Arduino_GFX *graphics = new Arduino_ST77916(
    bus, 12 /* RST */, 0 /* rotation */, true /* IPS */, 360 /* width */, 360 /* height */,
    0 /* col offset 1 */, 0 /* row offset 1 */, 0 /* col offset 2 */, 0 /* row offset 2 */,
    st77916_150_init_operations, sizeof(st77916_150_init_operations));

  canvas = new Arduino_Canvas(360 /* width */, 360 /* height */, graphics);

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
  Serial.print("BL: ");
  Serial.println(GFX_BL);
#endif

  if (!canvas->begin()) {
    Serial.println("Panik: gfx->begin() failed");
  }
  canvas->fillScreen(RGB565_BLACK);
  canvas->setCursor(180, 180);
  canvas->setTextColor(RGB565_WHITE);
  canvas->println("Hello World");
  canvas->flush();

  // This sets the clock font parameters
  // canvas->setFont(u8g2_font_maniac_tr);
  canvas->setTextColor(cfontcolor);
  canvas->setTextSize(cfontsize, cfontsize, 2 /* pixel_margin */);
}

//