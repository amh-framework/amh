#pragma once

namespace amh {
  class seqControler : public controler {
  public:
    void setup(int k) {
      max = k;
    }

    void update() {
      current_index = current_index + 1 % max;
    }

    int get_index() {
      return current_index;
    }

  protected:
    int current_index = 0;
    int max = 0;
  };
}
