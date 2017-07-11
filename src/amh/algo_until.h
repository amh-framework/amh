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
  template<class TIN>
  class algo_until : public algo<TIN> {
  public:
    algo_until(algo<TIN>& _step, algo<TIN>& _check) :
      step(_step), check(_check) {}

    TIN operator()(TIN& _in) {
      TIN out = check(_in);
      while (!check.check(out))
        out = step(out);
      return out;
    }

    void operator<<(algo<TIN>& _algo) {
      add(_algo);
    }

  protected:
    algo<TIN>& step;
    algo<TIN>& check;
  };
}
