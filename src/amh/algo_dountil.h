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
  template<class TIN>
  class algo_dountil : public algo<TIN> {
  public:
    algo_dountil(algo<TIN>& _step, algo<TIN>& _check) :
      step(_step), check(_check) {}

    TIN operator()(TIN& _in) {
      TIN out = _in;
      do {
        out = step(out);
      } while (!check.check(out));
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
