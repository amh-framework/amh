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
  class gen : amh::algo<TIN,TOUT> {
  public:
    gen() {}

    virtual void init() {}

    virtual void init(TIN&) {
      init();
    }

    virtual void key(int) {}

    virtual int key() {
      return -1;
    }

    virtual void next() {}

    virtual int max() {
      return -1;
    }

    virtual TOUT operator()() {
      throw std::runtime_error("in `amh::gen': method `()' to be implemented");
    }

    virtual TOUT operator()(TIN& _in) {
      return operator()();
    }

    virtual bool check() {
      int m = max();
      return m == -1 || key() < m;
    }

    virtual bool check(TIN&) {
      return check();
    }
};
}
