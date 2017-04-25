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
  class func : public algo<TIN,TOUT> {
  public:
    func() {}
    func(std::function<TOUT (TIN&)> _op) :
      func_op(_op) {}
    func(std::function<TOUT (TIN&)> _op,
         std::function<bool (TIN&)> _check) :
      func_op(_op), func_check(_check) {}
    func(std::function<void (TIN&)> _init,
         std::function<TOUT (TIN&)> _op,
         std::function<bool (TIN&)> _check) :
      func_init(_init), func_op(_op), func_check(_check) {}

    virtual void init(TIN& _in) {
      if (func_init)
        func_init(_in);
    };

    virtual TOUT operator()(TIN& _in) {
      if (func_op)
        return func_op(_in);
      return retTypeHelper<TIN,TOUT>(_in);
    };

    virtual TOUT operator()(TIN&& _in) {
      return operator()(_in);
    };

    virtual bool check(TIN& _in) {
      last_check = !func_check || func_check(_in);
      return last_check;
    };

    virtual bool check() {
      return last_check;
    };

    virtual std::string inspect() {
      std::string s = "algo<";
      std::ostringstream oss;
      oss << this;
      s += oss.str();
      s += ">";
      return s;
    }

  protected:
    std::function<void (TIN&)> func_init;
    std::function<TOUT (TIN&)> func_op;
    std::function<bool (TIN&)> func_check;
    bool last_check = true;
  };

  template<class TIN1, class TIN2, class TOUT>
  class func_curry2 : public algo_curry2<TIN1,TIN2,TOUT> {
  public:
    func_curry2() {}
    func_curry2(std::function<TOUT (TIN1&, TIN2&)> _op) :
      func_op(_op) {}
    func_curry2(std::function<TOUT (TIN1&, TIN2&)> _op,
                std::function<bool (TIN1&, TIN2&)> _check) :
      func_op(_op), func_check(_check) {}
    func_curry2(std::function<void (TIN1&, TIN2&)> _init,
                std::function<TOUT (TIN1&, TIN2&)> _op,
                std::function<bool (TIN1&, TIN2&)> _check) :
      func_init(_init), func_op(_op), func_check(_check) {}

    virtual void init(TIN1& _in1, TIN2& _in2) {
      if (func_init)
        func_init(_in1, _in2);
    };

    virtual void init(std::pair<TIN1,TIN2>& p) {
      init(p.first, p.second);
    };

    virtual TOUT operator()(TIN1& _in1, TIN2& _in2) {
      if (func_op)
        return func_op(_in1, _in2);
      return TOUT();
    };

    virtual TOUT operator()(std::pair<TIN1,TIN2>& p) {
      return operator()(p.first, p.second);
    };

    virtual bool check(TIN1& _in1, TIN2& _in2) {
      last_check = !func_check || func_check(_in1, _in2);
      return last_check;
    };

    virtual bool check(std::pair<TIN1,TIN2>& p) {
      return check(p.first, p.second);
    };

    virtual bool check() {
      return last_check;
    };

    virtual std::string inspect() {
      std::string s = "algo<";
      std::ostringstream oss;
      oss << this;
      s += oss.str();
      s += ">";
      return s;
    }

  protected:
    std::function<void (TIN1&, TIN2&)> func_init;
    std::function<TOUT (TIN1&, TIN2&)> func_op;
    std::function<bool (TIN1&, TIN2&)> func_check;
    bool last_check = true;
  };

#if __cplusplus > 201103L
  template<class TOUT, class ...TIN>
  class func_curry : public algo_curry<TOUT,TIN...> {
  public:
    func_curry() {}
    func_curry(std::function<TOUT (TIN&...)> _op) :
      func_op(_op) {}
    func_curry(std::function<TOUT (TIN&...)> _op,
               std::function<bool (TIN&...)> _check) :
      func_op(_op), func_check(_check) {}
    func_curry(std::function<void (TIN&...)> _init,
               std::function<TOUT (TIN&...)> _op,
               std::function<bool (TIN&...)> _check) :
      func_init(_init), func_op(_op), func_check(_check) {}

    virtual void init(TIN&... _in) {
      if (func_init)
        func_init(_in...);
    };

    virtual void init(std::tuple<TIN...>& t) {
      splat(init, t);
    };

    virtual TOUT operator()(TIN&... _in) {
      if (func_op)
        return func_op(_in...);
      return TOUT();
    };

    virtual TOUT operator()(std::tuple<TIN...>& t) {
      splat(operator(), t);
    };

    virtual bool check(TIN&... _in) {
      last_check = !func_check || func_check(_in...);
      return last_check;
    };

    virtual bool check(std::tuple<TIN...>& t) {
      splat(check, t);
    };

    virtual bool check() {
      return last_check;
    };

    virtual std::string inspect() {
      std::string s = "algo<";
      std::ostringstream oss;
      oss << this;
      s += oss.str();
      s += ">";
      return s;
    }

  protected:
    std::function<void (TIN&...)> func_init;
    std::function<TOUT (TIN&...)> func_op;
    std::function<bool (TIN&...)> func_check;
    bool last_check = true;
  };
#endif

  template<class TIN, class TOUT=TIN>
  class check : public func<TIN,TOUT> {
  public:
    check(std::function<bool (TIN&)> _check) :
      func<TIN,TOUT>(NULL, _check) {}
    check(std::function<TOUT (TIN&)> _op,
          std::function<bool (TIN&)> _check) :
      func<TIN,TOUT>(_op, _check) {}
    check(std::function<void (TIN&)> _init,
          std::function<TOUT (TIN&)> _op,
          std::function<bool (TIN&)> _check) :
      func<TIN,TOUT>(_init, _op, _check) {}
  };

  template<class TIN>
  class buff : public func<TIN,TIN> {
  public:
    buff() {}
    buff(std::function<void (TIN&)> _op) :
      func_op(_op) {}
    buff(std::function<void (TIN&)> _op,
         std::function<bool (TIN&)> _check) :
      func_op(_op), func_check(_check) {}
    buff(std::function<void (TIN&)> _init,
         std::function<void (TIN&)> _op,
         std::function<bool (TIN&)> _check) :
      func_init(_init), func_op(_op), func_check(_check) {}

    virtual void init(TIN& _in) {
      if (func_init)
        func_init(_in);
    };

    virtual TIN operator()(TIN& _in) {
      if (func_op)
        func_op(_in);
      return _in;
    };

    virtual bool check(TIN& _in) {
      last_check = !func_check || func_check(_in);
      return last_check;
    };

    virtual bool check() {
      return last_check;
    };

    virtual std::string inspect() {
      std::string s = "buff<";
      s += std::to_string(*(int*)((void*) this));
      s += ">";
      return s;
    }

  protected:
    std::function<void (TIN&)> func_init;
    std::function<void (TIN&)> func_op;
    std::function<bool (TIN&)> func_check;
    bool last_check = true;
  };
}
