#pragma once

namespace amh {
  class rng_helper {
  public:
    rng_helper() {
      std::random_device rd;
      reseed(rd());
    }
    rng_helper(int seed) {
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

    template<class T>
    void shuffle(T begin, T end) {
      std::shuffle(begin, end, mt);
    }

  protected:
    std::mt19937 mt;
  };
}
