#pragma once

namespace amh {
  template<class TIN, class TFOO, class TOUT>
  class chainAlgo;
  template<class TIN, class TFOO, class TOUT>
  class fragileChainAlgo;
  template<class TIN, class TFOO, class TOUT>
  class conjAlgo;
  template<class TIN, class TFOO, class TOUT>
  class disjAlgo;

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
      return retTypeHelper<TIN,TOUT>(_in);
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
    fragileChainAlgo<TIN,TOUT,TFOO> chain(algo<TOUT,TFOO>& _algo) {
      return fragileChainAlgo<TIN,TOUT,TFOO>(*this, _algo);
    }

    template<class TFOO>
    conjAlgo<TIN,TOUT,TFOO> conj(algo<TIN,TFOO>& _algo) {
      return conjAlgo<TIN,TOUT,TFOO>(*this, _algo);
    }

    template<class TFOO>
    disjAlgo<TIN,TOUT,TFOO> disj(algo<TIN,TFOO>& _algo) {
      return disjAlgo<TIN,TOUT,TFOO>(*this, _algo);
    }

    template<class TFOO>
    fragileChainAlgo<TIN,TOUT,TFOO> operator+(algo<TOUT,TFOO>& _algo) {
      return chain(_algo);
    }

    template<class TFOO>
    conjAlgo<TIN,TOUT,TFOO> operator&&(algo<TIN,TFOO>& _algo) {
      return conj(_algo);
    }

    template<class TFOO>
    disjAlgo<TIN,TOUT,TFOO> operator||(algo<TIN,TFOO>& _algo) {
      return disj(_algo);
    }

    virtual std::string inspect() {
      std::string s = "algo<";
      std::ostringstream oss;
      oss << this;
      s += oss.str();
      s += ">";
      return s;
    }
  };

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

  template<class T>
  using endo = algo<T,T>;

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
