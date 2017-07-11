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
  template<class TIN, class TOUT>
  class algo_disj : public algo<TIN,TOUT> {
  public:
    algo_disj(algo<TIN,TOUT>& _left, algo<TIN,TOUT>& _right, bool b=false) :
      algo_left_tmp(&_left), algo_left(&algo_left_tmp),
      algo_right_tmp(&_right), algo_right(&algo_right_tmp), leak(b) {}
    algo_disj(algo<TIN,TOUT>& _left, algo<TIN,TOUT>*& _right, bool b=false) :
      algo_left_tmp(&_left), algo_left(&algo_left_tmp), algo_right(&_right), leak(b) {}
    algo_disj(algo<TIN,TOUT>* _left, algo<TIN,TOUT>& _right, bool b=false) :
      algo_left(&_left), algo_right_tmp(&_right), algo_right(&algo_right_tmp), leak(b) {}
    algo_disj(algo<TIN,TOUT>* _left, algo<TIN,TOUT>*& _right, bool b=false) :
      algo_left(&_left), algo_right(&_right), leak(b) {}
    algo_disj(bool truelp, bool truerp, algo<TIN,TOUT>*& _left, algo<TIN,TOUT>*& _right, bool b=false) :
      algo_left_tmp(truelp ? nullptr : _left), algo_left(truelp ? &_left : &algo_left_tmp),
      algo_right_tmp(truerp ? nullptr : _right), algo_right(truerp ? &_right : &algo_right_tmp), leak(b) {
    }
    algo_disj(algo_disj_fragile<TIN,TOUT>&& _algo) :
      algo_disj(_algo.is_left_ptr(), _algo.is_right_ptr(), _algo.get_left(), _algo.get_right(), _algo.isLeaking()) {
      _algo.stealLeaks();
    }
    ~algo_disj() {
      if (leak)
        delete *algo_left;
    }

    algo<TIN,TOUT>*& get_left() {
      return *algo_left;
    }

    algo<TIN,TOUT>*& get_right() {
      return *algo_right;
    }

    bool is_left_ptr() {
      return algo_left_tmp == nullptr;
    }

    bool is_right_ptr() {
      return algo_right_tmp == nullptr;
    }

    bool isLeaking() {
      return leak;
    }

    virtual void stealLeaks() {
      leak = false;
    }

    void init(TIN& _in) {
      (**algo_left).init(_in);
      (**algo_right).init(_in);
    }

    TOUT operator()(TIN& _in) {
      return retTypeHelper<TIN,TOUT>(_in);
    }

    bool check(TIN& _in) {
      last_check = (**algo_left).check(_in) || (**algo_right).check(_in);
      return last_check;
    }

    bool check() {
      return last_check;
    }

    algo_disj_fragile<TIN,TOUT> operator||(algo<TIN,TOUT>& _algo) {
      return algo_disj_fragile<TIN,TOUT>(*this, _algo);
    }

    algo_disj_fragile<TIN,TOUT> operator||(algo<TIN,TOUT>*& _algo) {
      return algo_disj_fragile<TIN,TOUT>(*this, _algo);
    }

    virtual std::string inspect() {
      std::string s = "disj<";
      std::ostringstream oss;
      oss << this;
      s += oss.str();
      s += ">(";
      if (algo_left_tmp != nullptr) {
        s += this->inspect_algo(**algo_left);
      } else {
        oss.str("");
        oss.clear();
        oss << "pointer<";
        oss << *algo_left;
        oss << ">";
        s += oss.str();
      }
      s += " || ";
      if (algo_right_tmp != nullptr) {
        s += this->inspect_algo(**algo_right);
      } else {
        oss.str("");
        oss.clear();
        oss << "pointer<";
        oss << *algo_right;
        oss << ">";
        s += oss.str();
      }
      s += ")";
      return s;
    }

  protected:
    algo<TIN,TOUT>** algo_left;
    algo<TIN,TOUT>** algo_right;
    algo<TIN,TOUT>* algo_left_tmp = nullptr;
    algo<TIN,TOUT>* algo_right_tmp = nullptr;
    bool leak = false;
    bool last_check = true;
  };

  template<class TIN, class TOUT>
  class algo_disj_fragile : public algo_disj<TIN,TOUT> {
  public:
    algo_disj_fragile(algo<TIN,TOUT>& _left, algo<TIN,TOUT>& _right, bool b=false) :
      algo_disj<TIN,TOUT>(_left, _right),
      algo_left_tmp(&_left), algo_left(&algo_left_tmp),
      algo_right_tmp(&_right), algo_right(&algo_right_tmp), leak(b) {}
    algo_disj_fragile(algo<TIN,TOUT>& _left, algo<TIN,TOUT>*& _right, bool b=false) :
      algo_disj<TIN,TOUT>(_left, _right),
      algo_left_tmp(&_left), algo_left(&algo_left_tmp), algo_right(&_right), leak(b) {}
    algo_disj_fragile(algo<TIN,TOUT>* _left, algo<TIN,TOUT>& _right, bool b=false) :
      algo_disj<TIN,TOUT>(_left, _right),
      algo_left(&_left), algo_right_tmp(&_right), algo_right(&algo_right_tmp), leak(b) {}
    algo_disj_fragile(algo<TIN,TOUT>* _left, algo<TIN,TOUT>*& _right, bool b=false) :
      algo_disj<TIN,TOUT>(_left, _right),
      algo_left(&_left), algo_right(&_right), leak(b) {}
    ~algo_disj_fragile() {
      if (leak)
        delete *algo_left;
    }

    algo<TIN,TOUT>*& get_left() {
      return *algo_left;
    }

    algo<TIN,TOUT>*& get_right() {
      return *algo_right;
    }

    bool is_left_ptr() {
      return algo_left_tmp == nullptr;
    }

    bool is_right_ptr() {
      return algo_right_tmp == nullptr;
    }

    bool isLeaking() {
      return leak;
    }

    void stealLeaks() {
      leak = false;
    }

    void init(TIN& _in) {
      (**algo_left).init(_in);
      (**algo_right).init(_in);
    }

    TOUT operator()(TIN& _in) {
      return retTypeHelper<TIN,TOUT>(_in);
    }

    bool check(TIN& _in) {
      last_check = (**algo_left).check(_in) || (**algo_right).check(_in);
      return last_check;
    }

    bool check() {
      return last_check;
    }

    algo_conj_fragile<TIN,TOUT> operator&&(algo<TIN,TOUT>& _algo) {
      algo_conj<TIN,TOUT>* tmp;
      if (algo_left_tmp == nullptr && algo_right_tmp == nullptr)
        tmp = new algo_conj<TIN,TOUT>(*algo_left, *algo_right, leak);
      else if (algo_left_tmp == nullptr)
        tmp = new algo_conj<TIN,TOUT>(*algo_left, **algo_right, leak);
      else if (algo_right_tmp == nullptr)
        tmp = new algo_conj<TIN,TOUT>(**algo_left, *algo_right, leak);
      else
        tmp = new algo_conj<TIN,TOUT>(**algo_left, **algo_right, leak);
      leak = false;
      return algo_conj_fragile<TIN,TOUT>(*tmp, _algo, true);
    }

    algo_conj_fragile<TIN,TOUT> operator&&(algo<TIN,TOUT>*& _algo) {
      algo_conj<TIN,TOUT>* tmp;
      if (algo_left_tmp == nullptr && algo_right_tmp == nullptr)
        tmp = new algo_conj<TIN,TOUT>(*algo_left, *algo_right, leak);
      else if (algo_left_tmp == nullptr)
        tmp = new algo_conj<TIN,TOUT>(*algo_left, **algo_right, leak);
      else if (algo_right_tmp == nullptr)
        tmp = new algo_conj<TIN,TOUT>(**algo_left, *algo_right, leak);
      else
        tmp = new algo_conj<TIN,TOUT>(**algo_left, **algo_right, leak);
      leak = false;
      return algo_conj_fragile<TIN,TOUT>(*tmp, _algo, true);
    }

    algo_disj_fragile<TIN,TOUT> operator||(algo<TIN,TOUT>& _algo) {
      algo_disj<TIN,TOUT>* tmp;
      if (algo_left_tmp == nullptr && algo_right_tmp == nullptr)
        tmp = new algo_disj<TIN,TOUT>(*algo_left, *algo_right, leak);
      else if (algo_left_tmp == nullptr)
        tmp = new algo_disj<TIN,TOUT>(*algo_left, **algo_right, leak);
      else if (algo_right_tmp == nullptr)
        tmp = new algo_disj<TIN,TOUT>(**algo_left, *algo_right, leak);
      else
        tmp = new algo_disj<TIN,TOUT>(**algo_left, **algo_right, leak);
      leak = false;
      return algo_disj_fragile<TIN,TOUT>(*tmp, _algo, true);
    }

    algo_disj_fragile<TIN,TOUT> operator||(algo<TIN,TOUT>*& _algo) {
      algo_disj<TIN,TOUT>* tmp;
      if (algo_left_tmp == nullptr && algo_right_tmp == nullptr)
        tmp = new algo_disj<TIN,TOUT>(*algo_left, *algo_right, leak);
      else if (algo_left_tmp == nullptr)
        tmp = new algo_disj<TIN,TOUT>(*algo_left, **algo_right, leak);
      else if (algo_right_tmp == nullptr)
        tmp = new algo_disj<TIN,TOUT>(**algo_left, *algo_right, leak);
      else
        tmp = new algo_disj<TIN,TOUT>(**algo_left, **algo_right, leak);
      leak = false;
      return algo_disj_fragile<TIN,TOUT>(*tmp, _algo, true);
    }

    std::string inspect() {
      std::string s = "disj_fragile<";
      std::ostringstream oss;
      oss << this;
      s += oss.str();
      s += ">(";
      if (algo_left_tmp != nullptr) {
        s += this->inspect_algo(**algo_left);
      } else {
        oss.str("");
        oss.clear();
        oss << "pointer<";
        oss << *algo_left;
        oss << ">";
        s += oss.str();
      }
      s += " || ";
      if (algo_right_tmp != nullptr) {
        s += this->inspect_algo(**algo_right);
      } else {
        oss.str("");
        oss.clear();
        oss << "pointer<";
        oss << *algo_right;
        oss << ">";
        s += oss.str();
      }
      s += ")";
      return s;
    }

  protected:
    algo<TIN,TOUT>** algo_left;
    algo<TIN,TOUT>** algo_right;
    algo<TIN,TOUT>* algo_left_tmp = nullptr;
    algo<TIN,TOUT>* algo_right_tmp = nullptr;
    bool leak = false;
    bool last_check = true;
  };
}
