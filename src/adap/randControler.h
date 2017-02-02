#pragma once

namespace amh {
  class randControler : public controler {
  public:
    void setup(int k) {
      max = k;
    }

    void update() {
      current_index = rng.random(max);
    }

    int get_index() {
      return current_index;
    }

  protected:
    int current_index = 0;
    int max = 0;
  };
}
