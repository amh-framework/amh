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
  class algo_if : public algo<TIN,TOUT> {
  public:
    algo_if(algo<TIN>& _check, algo<TIN,TOUT>& _opt) :
      check(_check), op_true(_opt), op_false(def_algo) {}
    algo_if(algo<TIN>& _check, algo<TIN,TOUT>& _opt, algo<TIN,TOUT>& _opf) :
      check(_check), op_true(_opt), op_false(_opf) {}

    TOUT operator()(TIN& _in) {
      if (check.check(_in))
        return op_true(_in);
      else
        return op_false(_in);
    }

  protected:
    algo<TIN,TOUT>& op_true;
    algo<TIN,TOUT>& op_false;
    algo<TIN>& check;
    algo<TIN,TOUT> def_algo;
  };
}
