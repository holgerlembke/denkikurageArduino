// Dummy InputHandler because wo do not do touch
class InputHandler {
public:
  void begin() {}

  void loadSettings(ColorMode &mode) {
    mode = PURPLE;
    wireframe_mode = false;
    brightness_idx = 3;
  }

  void update(ColorMode &mode, float &user_y_offset, float &angle_y) {}

  uint8_t getBrightness() {
    const uint8_t levels[] = { 40, 100, 180, 255 };
    return levels[brightness_idx % 4];
  }

  bool isWireframeMode() const {
    return wireframe_mode;
  }

  int getVerticalDir() const {
    return vertical_dir;
  }  // -1: UP, 1: DOWN, 0: NONE

private:
  uint8_t brightness_idx;
  bool show_debug;
  bool wireframe_mode;
  int vertical_dir;
};

//