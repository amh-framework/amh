#pragma once

namespace amh {
  class controler {
  public:
    virtual void setup(int k) {}

    virtual void update() {}

    virtual int get_index() {
      return 0;
    }
  };
}
