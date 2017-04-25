/*
  Copyright (C) Dolphin project-team, CRIStAL, 2017
  Aymeric Blot

  This software is governed by the CeCILL-C license.
  You can use, modify and/ or redistribute the software under the
  terms of the CeCILL-C license as circulated by CEA, CNRS and INRIA
  at the following URL "http://www.cecill.info".
 */

#pragma once

namespace amh {
  template<class TIN, class TOUT=TIN>
  class gen_randworep : public amh::gen<TIN,TOUT> {
  public:
    gen_randworep(amh::gen<TIN,TOUT>& _gen) : gen_(_gen) {
      if (max() == -1)
        throw std::runtime_error("cannot randomize an infinite generator");
      init();
    }

    void init() {
      gen_.init();
      keys.resize(0);
      max_k = max()-1;
      for (int k=0; k<max_k+1; k++)
        keys.push_back(k);
      rng.shuffle(keys.begin(), keys.end());
    }

    void key(int k) {
      gen_.key(k);
    }

    int key() {
      return gen_.key();
    }

    void next() {
      // if (max_k == -1)
      //   init();
      int k = keys[max_k];
      gen_.key(k);
      max_k--;
    }

    int max() {
      return gen_.max();
    }

    TOUT operator()() {
      return gen_();
    }

    TOUT operator()(TIN& _in) {
      return gen_(_in);
    }

  protected:
    amh::gen<TIN,TOUT>& gen_;
    std::vector<int> keys;
    int max_k;
  };
}
