#pragma once

namespace amh {
  template<class TIN, class TOUT=void, class TFOO=void>
  class conjAlgo : public algo<TIN,TOUT> {
  public:
    // chainAlgo(algo<TIN,TFOO>& _left, algo<TFOO,TOUT>& _right) :
    //   algo_left(_left), algo_right(_right) {}
    conjAlgo(algo<TIN,TOUT>& _left, algo<TIN,TFOO>& _right, bool b1=false, bool b2=false) :
      algo_left(_left), algo_right(_right), leak_left(b1), leak_right(b2) {}
    conjAlgo(conjAlgo<TIN,TFOO,TOUT>& _algo) :
      algo_left(_algo.get_left()), algo_right(_algo.get_right()) {}
    conjAlgo(conjAlgo<TIN,TFOO,TOUT>&& _algo) :
      algo_left(_algo.get_left()), algo_right(_algo.get_right()) {}

    ~conjAlgo() {
      if (leak_left)
        free(&algo_left);
      if (leak_right)
        free(&algo_right);
    }

    algo<TIN,TOUT>& get_left() {
      return algo_left;
    }

    algo<TIN,TFOO>& get_right() {
      return algo_right;
    }

    void init(TIN& _in) {
      algo_left.init(_in);
      algo_right.init(_in);
    }

    TOUT operator()(TIN& _in) {
      return retTypeHelper<TIN,TOUT>(_in);
    }

    bool check(TIN& _in) {
      last_check = algo_left.check(_in) && algo_right.check(_in);
      return last_check;
    }

    bool check() {
      return last_check;
    }

    template<class TBAR>
    conjAlgo<TIN,TOUT,TFOO> conj(algo<TIN,TBAR>& _algo) {
      auto foo = new conjAlgo<TIN,TOUT,TFOO>(*this);
      return conjAlgo<TIN,TOUT,TBAR>(*foo, _algo, true);
    }

    template<class TBAR>
    disjAlgo<TIN,TOUT,TFOO> disj(algo<TIN,TBAR>& _algo) {
      auto foo = new conjAlgo<TIN,TOUT,TFOO>(*this);
      return disjAlgo<TIN,TOUT,TBAR>(*foo, _algo, true);
    }

    template<class TBAR>
    conjAlgo<TIN,TOUT,TFOO> operator&&(algo<TIN,TBAR>& _algo) {
      return conj(_algo);
    }

    template<class TBAR>
    conjAlgo<TIN,TOUT,TFOO> operator||(algo<TIN,TBAR>& _algo) {
      return disj(_algo);
    }

    template<class TBAR>
    chainAlgo<TIN,TFOO,TBAR> operator+(algo<TOUT,TBAR>& _algo) {
      throw std::runtime_error("what");
    }

  protected:
    algo<TIN,TOUT>& algo_left;
    algo<TIN,TFOO>& algo_right;
    bool leak_left = false;
    bool leak_right = false;
    bool last_check = true;
  };
}
