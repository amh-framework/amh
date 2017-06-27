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
  class algo_term : public algo<TIN,TOUT> {
  public:
    algo_term(algo<TIN,TOUT>& _algo) : algo_tmp(&_algo), algo_(&algo_tmp) {}
    algo_term(algo<TIN,TOUT>* _algo) : algo_(&_algo) {}

    algo<TIN,TOUT>*& get() {
      return *algo_;
    }

    void init(TIN& _in) {
      (**algo_).init(_in);
    }

    TOUT operator()(TIN& _in) {
      return retTypeHelper<TIN,TOUT>(_in);
    }

    bool check(TIN& _in) {
      last_check = (**algo_).check(_in);
      return last_check;
    }

    bool check() {
      return last_check;
    }

    virtual std::string inspect() {
      std::string s = "term<";
      std::ostringstream oss;
      oss << this;
      s += oss.str();
      s += ">(";
      s += this->inspect_algo(**algo_);
      s += ")";
      return s;
    }

  protected:
    algo<TIN,TOUT>** algo_;
    algo<TIN,TOUT>* algo_tmp = nullptr;
    bool last_check = true;
  };
}
