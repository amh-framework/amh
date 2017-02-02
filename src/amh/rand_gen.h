#pragma once

#include <vector>
#include <algorithm>

namespace amh {
  template<class TIN>
  class rand_gen : public amh::gen<TIN> {
  public:
    rand_gen(amh::gen<TIN>& _gen) : gen_(_gen) {
      max_k = max();
      if (max_k == -1)
        throw std::runtime_error("cannot randomize infinite generator");
      next();
    }

    void key(int k) {
      gen_.key(k);
    }

    int key() {
      return gen_.key();
    }

    void next() {
      int k = rng.random(gen_.max());
      gen_.set(k);
    }

    int max() {
      return max_k;
    }

    void max(int m) {
      max_k = m;
    }

    TIN operator()() {
      return gen_();
    }

    TIN operator()(TIN& _in) {
      return gen_(_in);
    }

  private:
    amh::gen<TIN>& gen_;
    int max_k = -1;
  };
}
