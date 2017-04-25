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
  template<class TIN, class TFOO, class TBAR=TFOO, class TOUT=TIN>
  class algo_map : public algo<TIN,TOUT> {
  public:
    algo_map(algo<TFOO,TBAR>& _algo) : algo_(_algo) {}

    TOUT operator()(TIN& _in) {
      TOUT tmp;
      for (TFOO& foo : _in)
        tmp.push_back(algo_(foo));
      return tmp;
    }

  protected:
    algo<TFOO,TBAR>& algo_;
  };
}
