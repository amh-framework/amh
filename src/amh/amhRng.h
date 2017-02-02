#pragma once

namespace amh {
  class amhRng {
  public:
    amhRng() {
      std::random_device rd;
      reseed(rd());
    }
    amhRng(int seed) {
      reseed(seed);
    }

    void reseed(int s) {
      mt = std::mt19937(s);
    }

    int random(int max) {
      std::uniform_int_distribution<> dis(0, max-1);
      return dis(mt);
    }

    double uniform(double max=1.0) {
      return uniform(0.0, max);
    }

    double uniform(double min, double max) {
      std::uniform_real_distribution<double> dis(min, max);
      return dis(mt);
    }

  protected:
    std::mt19937 mt;
  };
}
