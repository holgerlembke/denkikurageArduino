// Colors
#define CL_BG 0x0000  // TFT_BLACK

// Screen dimensions
#define SCREEN_WIDTH 360
#define SCREEN_HEIGHT 360


// Touch Calibration values
const uint16_t TOUCH_MIN_X = 200;
const uint16_t TOUCH_MAX_X = 3800;
const uint16_t TOUCH_MIN_Y = 250;
const uint16_t TOUCH_MAX_Y = 3850;

// Geometry configuration
const int BELL_RINGS = 4;
const int BELL_POINTS_PER_RING = 12;
const int NUM_TENTACLES = 12;
const int TENTACLE_SEGMENTS = 10;
const int NUM_BELL_VERTICES = BELL_RINGS * BELL_POINTS_PER_RING + 1;

const float scalefactor = 1.2;

// Particles
#define NUM_PARTICLES 50
#define biggerparticles
#define evenbiggerparticles

#ifdef evenbiggerparticles
#define biggerparticles
#endif

//#define framecounter

// Clock-Stuff
#define cuseclock
#define cfontsize 8
#define cfontcolor RGB565(25, 25, 25)
#define cposx 65
#define cposy 145
#define ntpSource "pool.ntp.org"
// https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
#define ntpTimeZone "CET-1CEST,M3.5.0,M10.5.0/3"



