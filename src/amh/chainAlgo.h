#pragma once

namespace amh {
  template<class TIN, class TFOO, class TOUT>
  class chainAlgo : public algo<TIN,TOUT> {
  public:
    chainAlgo(algo<TIN,TFOO>& _left, algo<TFOO,TOUT>& _right, bool b1=false, bool b2=false) :
      algo_left(_left), algo_right(_right), leak_left(b1), leak_right(b2) {}
    chainAlgo(chainAlgo<TIN,TFOO,TOUT>&& _algo) : chainAlgo(_algo.getLeft(), _algo.getRight(), _algo.getLeakLeft(), _algo.getLeakRight()) {
      _algo.stealLeaks();
    }
    chainAlgo(fragileChainAlgo<TIN,TFOO,TOUT>&& _algo) : chainAlgo(_algo.getLeft(), _algo.getRight(), _algo.getLeakLeft(), _algo.getLeakRight()) {
      _algo.stealLeaks();
    }

    ~chainAlgo() {
      if (leak_left)
        free(&algo_left);
      if (leak_right)
        free(&algo_right);
    }

    algo<TIN,TFOO>& getLeft() {
      return algo_left;
    }

    algo<TFOO,TOUT>& getRight() {
      return algo_right;
    }

    bool getLeakLeft() {
      return leak_left;
    }

    bool getLeakRight() {
      return leak_right;
    }

    virtual void stealLeaks() {
      leak_left = false;
      leak_right = false;
    }

    void init(TIN& _in) {
      algo_left.init(_in);
      // if (std::is_same<TIN,TOUT>::value)
      //   algo_right.init(_in);
    }

    TOUT operator()(TIN& _in) {
      TFOO tmp = algo_left(_in);
      return algo_right(tmp);
    }

    bool check(TIN& _in) {
      throw std::runtime_error("what");
    }

    bool check() {
      throw std::runtime_error("what");
    }

    template<class TBAR>
    fragileChainAlgo<TIN,TOUT,TBAR> chain(algo<TOUT,TBAR>& _algo) {
      return fragileChainAlgo<TIN,TOUT,TBAR>(*this, _algo);
    }

    template<class TBAR>
    fragileChainAlgo<TIN,TOUT,TBAR> operator+(algo<TOUT,TBAR>& _algo) {
      return chain(_algo);
    }

    virtual std::string inspect() {
      std::string s = "chain(";
      s += algo_left.inspect();
      s += " -> ";
      s += algo_right.inspect();
      s += ")<";
      std::ostringstream oss;
      oss << this;
      s += oss.str();
      s += ">";
      return s;
    }

  protected:
    algo<TIN,TFOO>& algo_left;
    algo<TFOO,TOUT>& algo_right;
    bool leak_left = false;
    bool leak_right = false;
  };

  template<class TIN>
  class chainAlgo<TIN,TIN,TIN> : public algo<TIN> {
  public:
    chainAlgo(std::vector<algo<TIN>*> list) : algo_list(list) {}
    chainAlgo(algo<TIN>& _left, algo<TIN>& _right) {
      algo_list.push_back(&_left);
      algo_list.push_back(&_right);
    }
    chainAlgo(chainAlgo<TIN,TIN,TIN>&& _algo) : chainAlgo(_algo.getList()) {}
    chainAlgo(fragileChainAlgo<TIN,TIN,TIN>&& _algo) : chainAlgo(_algo.getList()) {}

    std::vector<algo<TIN>*>& getList() {
      return algo_list;
    }

    void init(TIN& _in) {
      for (auto algo : algo_list)
        (*algo).init(_in);
    }

    TIN operator()(TIN& _in) {
      TIN tmp = _in;
      for (auto algo : algo_list)
        tmp = (*algo)(tmp);
      return tmp;
    }

    bool check(TIN& _in) {
      throw std::runtime_error("what");
    }

    bool check() {
      throw std::runtime_error("what");
    }

    template<class TOUT>
    fragileChainAlgo<TIN,TIN,TOUT> chain(algo<TIN,TOUT>& _algo) {
      return fragileChainAlgo<TIN,TIN,TOUT>(*this, _algo);
    }

    template<class TOUT>
    chainAlgo<TIN,TIN,TOUT> operator+(algo<TIN,TOUT>& _algo) {
      return chain(_algo);
    }

    virtual std::string inspect() {
      std::string s = "chainEndo(";
      bool b = false;
      for (auto algo : algo_list) {
        if (b)
          s += " -> ";
        if (algo == this) {
          s += "self<";
          std::ostringstream oss;
          oss << this;
          s += oss.str();
          s += ">";
        } else
          s += (*algo).inspect();
        b = true;
      }
      s += ")<";
      std::ostringstream oss;
      oss << this;
      s += oss.str();
      s += ">";
      return s;
    }

  protected:
    std::vector<algo<TIN>*> algo_list;
  };

  template<class TIN, class TFOO, class TOUT>
  class fragileChainAlgo : public chainAlgo<TIN,TFOO,TOUT> {
  public:
    fragileChainAlgo(algo<TIN,TFOO>& _left, algo<TFOO,TOUT>& _right, bool b1=false, bool b2=false) :
      chainAlgo<TIN,TFOO,TOUT>(_left, _right),
      algo_left(_left), algo_right(_right), leak_left(b1), leak_right(b2) {}
    fragileChainAlgo(chainAlgo<TIN,TFOO,TOUT>&& _algo) :
      fragileChainAlgo<TIN,TFOO,TOUT>(_algo.getLeft(), _algo.getRight(), _algo.getLeakLeft(), _algo.getLeakRight()) {
      _algo.stealLeaks();
    }
    fragileChainAlgo(fragileChainAlgo<TIN,TFOO,TOUT>&& _algo) :
      fragileChainAlgo<TIN,TFOO,TOUT>(_algo.getLeft(), _algo.getRight(), _algo.getLeakLeft(), _algo.getLeakRight()) {
      _algo.stealLeaks();
    }

    ~fragileChainAlgo() {
      if (leak_left)
        free(&algo_left);
      if (leak_right)
        free(&algo_right);
    }

    algo<TIN,TFOO>& getLeft() {
      return algo_left;
    }

    algo<TFOO,TOUT>& getRight() {
      return algo_right;
    }

    bool getLeakLeft() {
      return leak_left;
    }

    bool getLeakRight() {
      return leak_right;
    }

    void stealLeaks() {
      leak_left = false;
      leak_right = false;
    }

    void init(TIN& _in) {
      algo_left.init(_in);
    }

    TOUT operator()(TIN& _in) {
      TFOO tmp = algo_left(_in);
      return algo_right(tmp);
    }

    bool check(TIN& _in) {
      throw std::runtime_error("what");
    }

    bool check() {
      throw std::runtime_error("what");
    }

    template<class TBAR>
    fragileChainAlgo<TIN,TOUT,TBAR> chain(algo<TOUT,TBAR>& _algo) {
      auto foo = new chainAlgo<TIN,TFOO,TOUT>(algo_left, algo_right);
      return fragileChainAlgo<TIN,TOUT,TBAR>(*foo, _algo, true, false);
    }

    template<class TBAR>
    fragileChainAlgo<TIN,TOUT,TBAR> operator+(algo<TOUT,TBAR>& _algo) {
      return chain(_algo);
    }

    std::string inspect() {
      std::string s = "fragileChain(";
      s += algo_left.inspect();
      s += " -> ";
      s += algo_right.inspect();
      s += ")<";
      std::ostringstream oss;
      oss << this;
      s += oss.str();
      s += ">";
      return s;
    }

  protected:
    algo<TIN,TFOO>& algo_left;
    algo<TFOO,TOUT>& algo_right;
    bool leak_left = false;
    bool leak_right = false;
  };

  template<class TIN>
  class fragileChainAlgo<TIN,TIN,TIN> : public chainAlgo<TIN,TIN,TIN> {
  public:
    fragileChainAlgo(std::vector<algo<TIN>*> list) :
      chainAlgo<TIN,TIN,TIN>(list), algo_list(list) {}
    fragileChainAlgo(algo<TIN>& _left, algo<TIN>& _right) :
      chainAlgo<TIN,TIN,TIN>(_left, _right) {
      algo_list.push_back(&_left);
      algo_list.push_back(&_right);
    }
    fragileChainAlgo(chainAlgo<TIN,TIN,TIN>&& _algo) :
      fragileChainAlgo<TIN,TIN,TIN>(_algo.getList()) {}
    fragileChainAlgo(fragileChainAlgo<TIN,TIN,TIN>&& _algo) :
      fragileChainAlgo<TIN,TIN,TIN>(_algo.getList()) {}

    std::vector<algo<TIN>*>& getList() {
      return algo_list;
    }

    void init(TIN& _in) {
      for (auto algo : algo_list)
        (*algo).init(_in);
    }

    TIN operator()(TIN& _in) {
      TIN tmp = _in;
      for (auto algo : algo_list)
        tmp = (*algo)(tmp);
      return tmp;
    }

    bool check(TIN& _in) {
      throw std::runtime_error("what");
    }

    bool check() {
      throw std::runtime_error("what");
    }

    template<class TOUT>
    fragileChainAlgo<TIN,TIN,TOUT> chain(algo<TIN,TOUT>& _algo) {
      return fragileChainAlgo<TIN,TIN,TOUT>(*this, _algo);
    }

    fragileChainAlgo<TIN,TIN,TIN> chain(algo<TIN>& _algo) {
      auto tmp(algo_list);
      tmp.push_back(&_algo);
      return fragileChainAlgo<TIN,TIN,TIN>(tmp);
    }

    template<class TOUT>
    fragileChainAlgo<TIN,TIN,TOUT> operator+(algo<TIN,TOUT>& _algo) {
      return chain(_algo);
    }

    std::string inspect() {
      std::string s = "fragileChainEndo(";
      bool b = false;
      for (auto algo : algo_list) {
        if (b)
          s += " -> ";
        s += (*algo).inspect();
        b = true;
      }
      s += ")<";
      std::ostringstream oss;
      oss << this;
      s += oss.str();
      s += ">";
      return s;
    }

  protected:
    std::vector<algo<TIN>*> algo_list;
  };
}
