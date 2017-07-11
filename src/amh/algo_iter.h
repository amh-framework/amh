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
  class algo_iter : public algo<TIN> {
  public:
    algo_iter(algo<TIN>& _step, algo<TIN>& _check, int& _i) :
      step(_step), check(_check), max_iter(_i) {}
    algo_iter(algo<TIN>& _step, algo<TIN>& _check, int&& _i) :
      step(_step), check(_check), max_iter(_i) {}
    algo_iter(algo<TIN>& _step, int& _i) :
      step(_step), check(def_check), max_iter(_i) {}
    algo_iter(algo<TIN>& _step, int&& _i) :
      step(_step), check(def_check), max_iter(_i) {}

    TIN operator()(TIN& _in) {
      int iter = 0;
      TIN out = _in;
      while (check.check(_in) && iter < max_iter) {
        iter++;
        out = step(out);
      }
      return out;
    }

    void operator<<(algo<TIN>& _algo) {
      add(_algo);
    }

  protected:
    algo<TIN>& step;
    algo<TIN>& check;
    algo<TIN> def_check;
    int max_iter;
  };
}
