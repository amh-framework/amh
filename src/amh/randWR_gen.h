#pragma once

#include <vector>
#include <algorithm>

namespace amh {
  template<class TIN>
  class randWR_gen : public amh::gen<TIN> {
  public:
    randWR_gen(amh::gen<TIN>& _gen) : gen_(_gen) {
      if (max() == -1)
        throw std::runtime_error("cannot randomize infinite generator");
      init();
    }

    void init() {
      keys.resize(0);
      max_k = max();
      for (int k=0; k<max_k; k++)
        keys.push_back(k);
      std::random_shuffle(keys.begin(), keys.end());
    }

    void key(int k) {
      gen_.key(k);
    }

    int key() {
      return gen_.key();
    }

    void next() {
      if (max_k == 0)
        init();
      int k = keys[max_k];
      gen_.key(k);
      max_k--;
    }

    int max() {
      return gen_.max();
    }

    TIN operator()() {
      return gen_();
    }

    TIN operator()(TIN& _in) {
      return gen_(_in);
    }

  private:
    amh::gen<TIN>& gen_;
    std::vector<int> keys;
    int max_k;
  };
}
