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
  template<class TIN, class TFOO, class TOUT>
  class algo_chain;
  template<class TIN, class TFOO, class TOUT>
  class algo_chain_fragile;
  template<class TIN, class TOUT>
  class algo_conj;
  template<class TIN, class TOUT>
  class algo_conj_fragile;
  template<class TIN, class TOUT>
  class algo_disj;
  template<class TIN, class TOUT>
  class algo_disj_fragile;

  template <typename TIN, typename TOUT>
  TOUT retTypeHelper(TIN& _in) {
    if (std::is_same<TIN,TOUT>::value)
      return *(TOUT*)((void*)&_in);
    else
      return TOUT();
  }

  template <typename TIN, typename TOUT>
  TOUT retTypeHelper(TIN&& _in) {
    if (std::is_same<TIN,TOUT>::value)
      return *(TOUT*)((void*)&_in);
    else
      return TOUT();
  }

  template<class TIN, class TOUT=TIN>
  class algo {
  public:
    algo() {}

    virtual void init(TIN& _in) {};

    virtual TOUT operator()(TIN& _in) {
      //return retTypeHelper<TIN,TOUT>(_in);
    };

    virtual TOUT operator()(TIN&& _in) {
      return operator()(_in);
    };

    virtual bool check(TIN& _in) {
      return true;
    };

    virtual bool check() {
      return true;
    };

    template<class TFOO>
    algo_chain_fragile<TIN,TOUT,TFOO> operator+(algo<TOUT,TFOO>& _algo) {
      return algo_chain_fragile<TIN,TOUT,TFOO>(*this, _algo);
    }

    template<class TFOO>
    algo_chain_fragile<TIN,TOUT,TFOO> operator+(algo<TOUT,TFOO>*& _algo) {
      return algo_chain_fragile<TIN,TOUT,TFOO>(*this, _algo);
    }

    algo_conj_fragile<TIN,TOUT> operator&&(algo<TIN>& _algo) {
      return algo_conj_fragile<TIN,TOUT>(*this, _algo);
    }

    algo_disj_fragile<TIN,TOUT> operator||(algo<TIN,TOUT>& _algo) {
      return algo_disj_fragile<TIN,TOUT>(*this, _algo);
    }

    virtual std::string inspect() {
      std::string s = "algo<";
      std::ostringstream oss;
      oss << this;
      s += oss.str();
      s += ">";
      return s;
    }

    template<class TFOO, class TBAR>
    std::string inspect_algo(algo<TFOO,TBAR>& _algo) {
      if (&_algo == this) {
        std::string s = "self<";
        std::ostringstream oss;
        oss << this;
        s += oss.str();
        s += ">";
        return s;
      } else
        return _algo.inspect();
    }
  };

  template<class T>
  using endo = algo<T,T>;

  template<class TIN1, class TIN2, class TOUT>
  class algo_curry2 : public algo<std::pair<TIN1,TIN2>,TOUT> {
  public:
    algo_curry2() {}

    virtual void init(TIN1&, TIN2&) {
    };

    virtual void init(std::pair<TIN1,TIN2>&) {
    };

    virtual TOUT operator()(TIN1&, TIN2&) {
      return TOUT();
    };

    virtual TOUT operator()(std::pair<TIN1,TIN2>&) {
      return TOUT();
    };

    virtual bool check(TIN1&, TIN2&) {
      return true;
    };

    virtual bool check(std::pair<TIN1,TIN2>&) {
      return true;
    };

    virtual bool check() {
      return true;
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
    template<class Fun, class Pair>
    decltype(auto) splat(Fun&& fun, Pair&& pair) {
      return fun(pair.first, pair.second);
    }
};

#if __cplusplus > 201103L
  template<class TOUT, class ...TIN>
  class algo_curry : public algo<std::tuple<TIN...>,TOUT> {
  public:
    algo_curry() {}

    virtual void init(TIN&...) {
    };

    virtual void init(std::tuple<TIN...>&) {
    };

    virtual TOUT operator()(TIN&...) {
      return TOUT();
    };

    virtual TOUT operator()(std::tuple<TIN...>&) {
      return TOUT();
    };

    virtual bool check(TIN&...) {
      return true;
    };

    virtual bool check(std::tuple<TIN...>&) {
      return true;
    };

    virtual bool check() {
      return true;
    };

    virtual std::string inspect() {
      std::string s = "algo<";
      std::ostringstream oss;
      oss << this;
      s += oss.str();
      s += ">";
      return s;
    }

  private:
    template<class F, class T, std::size_t... I>
    decltype(auto) splat_helper(F&& fun, T&& tuple, std::index_sequence<I...>) {
      return fun(std::get<I>(std::forward<T>(tuple))...);
    }
    template<class F, class T>
    decltype(auto) splat(F&& f, T&& t) {
      constexpr auto S = std::tuple_size<typename std::decay<T>::type>::value;
      splat_helper(std::forward<F>(f), std::forward<T>(t), std::make_index_sequence<S>{});
    }
  };
#endif
}
