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
  class gen_union : public amh::gen<TIN,TOUT> {
  public:
    gen_union(amh::gen<TIN,TOUT>& _lgen, amh::gen<TIN,TOUT>& _rgen) :
      gen_left(_lgen), gen_right(_rgen) {
      if (_lgen.max()*_rgen.max() < 0)
        throw std::runtime_error("cannot randomize infinite generator");
    }

    void init() {
      gen_left.init();
      gen_right.init();
    }

    void key(int k) {
      key_ = k;
      int m = gen_left.max();
      if (k < gen_left.max()) {
        gen_left.key(k);
        gen_right.key(0);
      } else {
        gen_left.key(0);
        gen_right.key(k-m);
      }
    }

    int key() {
      return key_;
    }

    void next() {
      key((key_+1)%max());
      //key_ = (key_+1)%max();
      // if (key_ < gen_left.max())
      //   gen_left.next();
      // else
      //   gen_right.next();
    }

    int max() {
      return gen_left.max()+gen_right.max();
    }

    TOUT operator()(TIN& _in) {
      if (key_ < gen_left.max())
        return gen_left(_in);
      else
        return gen_right(_in);
    }

  private:
    amh::gen<TIN,TOUT>& gen_left;
    amh::gen<TIN,TOUT>& gen_right;
    int key_ = 0;
  };
}
