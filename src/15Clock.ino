#ifdef cuseclock
#include <WiFi.h>         //
#include <WiFiMulti.h>    //
#include <esp_sntp.h>     //
#include <privatedata.h>  // https://github.com/holgerlembke/privatedata

enum sntprtcsyncprocess_t { srspwait,       // 0 wait for wifi connect
                            srspconnected,  // 1 wait for time sync
                            srspdoit,       // 2 time sync done, disconnect
                            srspwaittime    // 3 wait for next time sync while disconnected
} sntprtcsyncprocess = srspwait;

//*********************************************************************************************************************
void displayclock(void /**/) {
  // Time top left
  static time_t lastnow = -1;
  static int8_t lasthour = -1;
  static String times = "";

  time_t now = time(NULL);
  if (now != (time_t)(-1)) {
    if (lastnow != now) {
      lastnow = now;
      tm tm;
      localtime_r(&now, &tm);
      char buf[6];
      strftime(buf, sizeof(buf), "%H:%M", &tm);
      times = buf;

      if (lasthour == -1) {
        lasthour = tm.tm_hour;
      } else {
        if (lasthour != tm.tm_hour) {
          lasthour = tm.tm_hour;
          sntprtcsyncprocess = srspwait;
        }
      }
    }

    canvas->setCursor(cposx, cposy);
    canvas->println(times);
  }
}

//*********************************************************************************************************************
void rtcupdate(void* parameter /**/) {
  UBaseType_t lastwm = 50000;
  WiFiMulti wifiMulti;

  // Add your access point information here
  wifiMulti.addAP(privatedata_mySSID, privatedata_myWAP2);

  sntp_set_time_sync_notification_cb(  //
    [](struct timeval* tv) {
      sntprtcsyncprocess = srspdoit;
    }  //
  );
  sntp_set_sync_interval(5l * 60l * 1000l);  // force 5 minutes update time
  configTzTime(ntpTimeZone, ntpSource);

  sntprtcsyncprocess_t lastsntprtcsyncprocess = sntprtcsyncprocess;

  do {
    switch (sntprtcsyncprocess) {
      case srspwait:
        {
          if (WiFi.status() != WL_CONNECTED) {
            if (wifiMulti.run() != WL_CONNECTED) {
              Serial.println("WiFi: not connected");
              vTaskDelay((10000 + random(10000)) / portTICK_PERIOD_MS);  // random delay
            } else {
              Serial.print("IP: ");
              Serial.println(WiFi.localIP());
              sntp_restart();  // ??
              sntprtcsyncprocess = srspconnected;
            }
          } else {
            vTaskDelay(10000 / portTICK_PERIOD_MS);
          }
          break;
        }
      case srspdoit:
        {
          Serial.println("Disconnected.");
          WiFi.disconnect(true);
          sntprtcsyncprocess = srspwaittime;
          vTaskDelay(10000 / portTICK_PERIOD_MS);
          break;
        }
      case srspconnected:
      case srspwaittime:
        {
          vTaskDelay(10000 / portTICK_PERIOD_MS);
          break;
        }
    }

    if (lastsntprtcsyncprocess != sntprtcsyncprocess) {
      Serial.printf("sntprtcsyncprocess: %i -> %i\n", lastsntprtcsyncprocess, sntprtcsyncprocess);
      lastsntprtcsyncprocess = sntprtcsyncprocess;
    }

    /**/
    UBaseType_t cwm = uxTaskGetStackHighWaterMark(NULL);
    if (cwm < lastwm) {
      Serial.printf("rtcupdate StackHighWaterMark: %i\n", cwm);
      lastwm = cwm;
    }
    /**/
  } while (1);
}

//*********************************************************************************************************************
void setupClock(void /**/) {
  xTaskCreatePinnedToCore(
    rtcupdate,
    "rtcupdate",
    3500,  // Stack size ausgemessen via cwm
    NULL,
    1,
    NULL,
    0);  // core 0
}

#else
//*********************************************************************************************************************
void setupClock(void /**/) {}
void displayclock(void /**/) {}
#endif

//