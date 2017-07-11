/*
  Copyright (C) ORKAD team, CRIStAL, University of Lille, 2017
  Aymeric Blot

  This software is governed by the CeCILL-C license.
  You can use, modify and/ or redistribute the software under the
  terms of the CeCILL-C license as circulated by CEA, CNRS and INRIA
  at the following URL "http://www.cecill.info".
 */

#pragma once

namespace amh {
  template<class TIN, class TOUT=TIN>
  class gen_rand : public amh::gen<TIN,TOUT> {
  public:
    gen_rand(amh::gen<TIN,TOUT>& _gen) : gen_(_gen) {
      max_k = max();
      if (max_k == -1)
        throw std::runtime_error("cannot randomize infinite generator");
      next();
    }

    void init() {
      gen_.init();
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

    TOUT operator()() {
      return gen_();
    }

    TOUT operator()(TIN& _in) {
      return gen_(_in);
    }

  private:
    amh::gen<TIN>& gen_;
    int max_k = -1;
  };
}
