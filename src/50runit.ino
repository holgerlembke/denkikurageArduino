InputHandler input;

// Global state
ColorMode current_mode = PURPLE;
float user_y_offset = -110.0f;  // move the jf somewhat more up
float phase = 0;
float global_x_offset = 0.0f;
float global_y_offset = 0.0f;
float global_z_offset = 0.0f;
float angle_x = -0.5f, angle_y = 0.4f, angle_z = 0.0f;
float rotation_speed = 0.005f;
float target_rotation_speed = 0.005f;
unsigned long last_rotation_change = 0;
uint32_t last_frame_time = 0;
uint16_t framecount = 0;
float current_fps = 0;

Particle particles[NUM_PARTICLES];
Point2D curr_bell_2d[NUM_BELL_VERTICES];
Point2D curr_tentacles_2d[NUM_TENTACLES][TENTACLE_SEGMENTS];

inline void setupR() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Denki Kurage - Initted " __DATE__
                 " " __TIME__);

  setupGFX();
  setupClock();

  input.begin();
  input.loadSettings(current_mode);

  for (int i = 0; i < NUM_PARTICLES; i++) {
    particles[i].x = random(0, SCREEN_WIDTH);
    particles[i].y = random(0, SCREEN_HEIGHT);
    particles[i].speed = random(5, 15) / 10.0f;
    particles[i].brightness = random(40, 180);
  }
}

inline void loopR() {
  // Input
  canvas->fillScreen(RGB565_BLACK);

  displayclock();

  // Animation Math
  phase += 0.08f;

  // Slow autonomous rotation around body axis (Y-axis)
  // Randomize target speed every 45 seconds
  if (millis() - last_rotation_change > 45000) {
    // Range: -0.015 to 0.015 radians per frame
    target_rotation_speed = (random(-150, 150) / 10000.0f);
    last_rotation_change = millis();
  }
  // Smoothly interpolate current speed to target
  rotation_speed += (target_rotation_speed - rotation_speed) * 0.005f;
  angle_y += rotation_speed;

  global_x_offset = sinf(phase * 0.15f) * 35.0f + cosf(phase * 0.25f) * 15.0f;
  global_z_offset = sinf(phase * 0.1f) * 45.0f;
  float drift_y = sinf(phase * 0.4f) * 15.0f + cosf(phase * 0.2f) * 10.0f;
  float lift = cosf(phase) * 12.0f;
  global_y_offset = drift_y - lift + user_y_offset;

  // Particles
  int v_dir = input.getVerticalDir();
  for (int i = 0; i < NUM_PARTICLES; i++) {
    float p_speed = particles[i].speed;
    if (v_dir == -1)
      p_speed *= 4.0f;  // Moving UP: Fast upward flow
    else if (v_dir == 1)
      p_speed *= -2.5f;  // Moving DOWN: Reverse flow

    particles[i].y -= p_speed;
    particles[i].x += sinf(phase + i) * 0.3f;

    // Wrap around logic
    if (particles[i].y < 0) {
      particles[i].y = SCREEN_HEIGHT;
      particles[i].x = random(0, SCREEN_WIDTH);
    } else if (particles[i].y > SCREEN_HEIGHT) {
      particles[i].y = 0;
      particles[i].x = random(0, SCREEN_WIDTH);
    }

    uint16_t p_color = canvas->color565(0, particles[i].brightness, particles[i].brightness);
    //    if (canvas->created())
    int16_t px = particles[i].x;
    int16_t py = particles[i].y;
#ifdef biggerparticles
    int16_t pxp = particles[i].x + 1;
    int16_t pyp = particles[i].y + 1;
#endif
    canvas->drawPixel(px, py, p_color);
#ifdef biggerparticles
    canvas->drawPixel(pxp, pyp, p_color);
#endif
#ifdef evenbiggerparticles
    canvas->drawPixel(pxp, py, p_color);
    canvas->drawPixel(px, pyp, p_color);
#endif
  }

  // 3D Geometry
  updateRotationParams(angle_x, angle_y, angle_z);
  float expansion = 1.0f + sinf(phase) * 0.25f;

  // Bell Peak
  Point3D p3 = { 0, -35.0f, 0 };
  curr_bell_2d[0] = project(rotateFast(p3), global_x_offset, global_y_offset,
                            global_z_offset);

  for (int r = 0; r < BELL_RINGS; r++) {
    // Flattened bell geometry using sine curve
    float normalized_r = (float)(r + 1) / (float)BELL_RINGS;
    float ring_y = -20.0f + (float)r * 18.0f;
    float ring_radius = 120.0f * sinf(normalized_r * PI * 0.5f) * expansion;

    for (int i = 0; i < BELL_POINTS_PER_RING; i++) {
      float theta = (float)i * 2.0f * PI / (float)BELL_POINTS_PER_RING;
      p3 = { ring_radius * cosf(theta), ring_y, ring_radius * sinf(theta) };
      curr_bell_2d[1 + r * BELL_POINTS_PER_RING + i] =
        project(rotateFast(p3), global_x_offset, global_y_offset,
                global_z_offset);
    }
  }

  for (int t = 0; t < NUM_TENTACLES; t++) {
    float theta = (float)t * 2.0f * PI / (float)NUM_TENTACLES;
    float base_x = 35.0f * cosf(theta) * expansion;
    float base_z = 35.0f * sinf(theta) * expansion;
    float base_y = 30.0f;
    for (int s = 0; s < TENTACLE_SEGMENTS; s++) {
      float wave = sinf(phase - (float)s * 0.7f) * 20.0f;
      p3 = { base_x + wave * cosf(theta), base_y + (float)s * 35.0f,
             base_z + wave * sinf(theta) };
      curr_tentacles_2d[t][s] = project(rotateFast(p3), global_x_offset,
                                        global_y_offset, global_z_offset);
    }
  }

  // Render
  bool wireframe = input.isWireframeMode();
  drawJellyfish(canvas, curr_bell_2d, curr_tentacles_2d, current_mode, wireframe);
  canvas->flush();

// Update FPS
#ifdef framecounter
  framecount++;
  uint32_t td = millis() - last_frame_time;
  if (td > 1000) {
    Serial.printf("fps: %f\n", 1000.0f * framecount / td);

    last_frame_time = millis();
    framecount = 0;
  }
#endif

  delay(10);  // 18 fps without, 15 with 10, particlesize does not change much
}

//