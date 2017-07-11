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
  template<class TIN, class TFOO, class TOUT>
  class algo_chain : public algo<TIN,TOUT> {
  public:
    algo_chain(algo<TIN,TFOO>& _left, algo<TFOO,TOUT>& _right, bool b=false) :
      algo_left_tmp(&_left), algo_left(&algo_left_tmp),
      algo_right_tmp(&_right), algo_right(&algo_right_tmp), leak(b) {}
    algo_chain(algo<TIN,TFOO>& _left, algo<TFOO,TOUT>*& _right, bool b=false) :
      algo_left_tmp(&_left), algo_left(&algo_left_tmp), algo_right(&_right), leak(b) {}
    algo_chain(algo<TIN,TFOO>* _left, algo<TFOO,TOUT>& _right, bool b=false) :
      algo_left(&_left), algo_right_tmp(&_right), algo_right(&algo_right_tmp), leak(b) {}
    algo_chain(algo<TIN,TFOO>* _left, algo<TFOO,TOUT>*& _right, bool b=false) :
      algo_left(&_left), algo_right(&_right), leak(b) {}
    algo_chain(bool truelp, bool truerp, algo<TIN,TFOO>*& _left, algo<TFOO,TOUT>*& _right, bool b=false) :
      algo_left_tmp(truelp ? nullptr : _left), algo_left(truelp ? &_left : &algo_left_tmp),
      algo_right_tmp(truerp ? nullptr : _right), algo_right(truerp ? &_right : &algo_right_tmp), leak(b) {
    }
    algo_chain(algo_chain_fragile<TIN,TFOO,TOUT>&& _algo) :
      algo_chain(_algo.is_left_ptr(), _algo.is_right_ptr(), _algo.get_left(), _algo.get_right(), _algo.isLeaking()) {
      _algo.stealLeaks();
    }
    ~algo_chain() {
      if (leak)
        delete *algo_left;
    }

    algo<TIN,TFOO>*& get_left() {
      return *algo_left;
    }

    algo<TFOO,TOUT>*& get_right() {
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
      // if (std::is_same<TIN,TFOO>::value)
      //   (**algo_right).init(_in);
    }

    TOUT operator()(TIN& _in) {
      TFOO tmp = (**algo_left)(_in);
      return (**algo_right)(tmp);
    }

    bool check(TIN& _in) {
      throw std::runtime_error("what");
    }

    bool check() {
      throw std::runtime_error("what");
    }

    template<class TBAR>
    algo_chain_fragile<TIN,TOUT,TBAR> operator+(algo<TOUT,TBAR>& _algo) {
      return algo_chain_fragile<TIN,TOUT,TBAR>(*this, _algo);
    }

    template<class TBAR>
    algo_chain_fragile<TIN,TOUT,TBAR> operator+(algo<TOUT,TBAR>*& _algo) {
      return algo_chain_fragile<TIN,TOUT,TBAR>(*this, _algo);
    }

    virtual std::string inspect() {
      std::string s = "chain<";
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
      s += " -> ";
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
    algo<TIN,TFOO>** algo_left;
    algo<TFOO,TOUT>** algo_right;
    algo<TFOO,TOUT>* algo_left_tmp = nullptr;
    algo<TFOO,TOUT>* algo_right_tmp = nullptr;
    bool leak = false;
  };

  template<class TIN, class TFOO, class TOUT>
  class algo_chain_fragile : public algo_chain<TIN,TFOO,TOUT> {
  public:
    algo_chain_fragile(algo<TIN,TFOO>& _left, algo<TFOO,TOUT>& _right, bool b=false) :
      algo_chain<TIN,TFOO,TOUT>(_left, _right),
      algo_left_tmp(&_left), algo_left(&algo_left_tmp),
      algo_right_tmp(&_right), algo_right(&algo_right_tmp), leak(b) {}
    algo_chain_fragile(algo<TIN,TFOO>& _left, algo<TFOO,TOUT>*& _right, bool b=false) :
      algo_chain<TIN,TFOO,TOUT>(_left, _right),
      algo_left_tmp(&_left), algo_left(&algo_left_tmp), algo_right(&_right), leak(b) {}
    algo_chain_fragile(algo<TIN,TFOO>* _left, algo<TFOO,TOUT>& _right, bool b=false) :
      algo_chain<TIN,TFOO,TOUT>(_left, _right),
      algo_left(&_left), algo_right_tmp(&_right), algo_right(&algo_right_tmp), leak(b) {}
    algo_chain_fragile(algo<TIN,TFOO>* _left, algo<TFOO,TOUT>*& _right, bool b=false) :
      algo_chain<TIN,TFOO,TOUT>(_left, _right),
      algo_left(&_left), algo_right(&_right), leak(b) {}
    ~algo_chain_fragile() {
      if (leak)
        delete *algo_left;
    }

    algo<TIN,TFOO>*& get_left() {
      return *algo_left;
    }

    algo<TFOO,TOUT>*& get_right() {
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
      // if (std::is_same<TIN,TFOO>::value)
      //   algo_right->init(_in);
    }

    TOUT operator()(TIN& _in) {
      TFOO tmp = (**algo_left)(_in);
      return (**algo_right)(tmp);
    }

    bool check(TIN& _in) {
      throw std::runtime_error("what");
    }

    bool check() {
      throw std::runtime_error("what");
    }

    template<class TBAR>
    algo_chain_fragile<TIN,TOUT,TBAR> operator+(algo<TOUT,TBAR>& _algo) {
      algo_chain<TIN,TFOO,TOUT>* tmp;
      if (algo_left_tmp == nullptr && algo_right_tmp == nullptr)
        tmp = new algo_chain<TIN,TFOO,TOUT>(*algo_left, *algo_right, leak);
      else if (algo_left_tmp == nullptr)
        tmp = new algo_chain<TIN,TFOO,TOUT>(*algo_left, **algo_right, leak);
      else if (algo_right_tmp == nullptr)
        tmp = new algo_chain<TIN,TFOO,TOUT>(**algo_left, *algo_right, leak);
      else
        tmp = new algo_chain<TIN,TFOO,TOUT>(**algo_left, **algo_right, leak);
      leak = false;
      return algo_chain_fragile<TIN,TOUT,TBAR>(*tmp, _algo, true);
    }

    template<class TBAR>
    algo_chain_fragile<TIN,TOUT,TBAR> operator+(algo<TOUT,TBAR>*& _algo) {
      algo_chain<TIN,TFOO,TOUT>* tmp;
      if (algo_left_tmp == nullptr && algo_right_tmp == nullptr)
        tmp = new algo_chain<TIN,TFOO,TOUT>(*algo_left, *algo_right, leak);
      else if (algo_left_tmp == nullptr)
        tmp = new algo_chain<TIN,TFOO,TOUT>(*algo_left, **algo_right, leak);
      else if (algo_right_tmp == nullptr)
        tmp = new algo_chain<TIN,TFOO,TOUT>(**algo_left, *algo_right, leak);
      else
        tmp = new algo_chain<TIN,TFOO,TOUT>(**algo_left, **algo_right, leak);
      leak = false;
      return algo_chain_fragile<TIN,TOUT,TBAR>(*tmp, _algo, true);
    }

    std::string inspect() {
      std::string s = "chain_fragile<";
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
      s += " -> ";
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
    algo<TIN,TFOO>** algo_left;
    algo<TFOO,TOUT>** algo_right;
    algo<TFOO,TOUT>* algo_left_tmp = nullptr;
    algo<TFOO,TOUT>* algo_right_tmp = nullptr;
    bool leak = false;
  };
}
