#pragma once

namespace amh {
  template<class TIN>
  class iterAlgo : public algo<TIN> {
  public:
    iterAlgo(algo<TIN>& _step, algo<TIN>& _check, int& _i) :
      step(_step), check(_check), max_iter(_i) {}
    iterAlgo(algo<TIN>& _step, algo<TIN>& _check, int&& _i) :
      step(_step), check(_check), max_iter(_i) {}
    iterAlgo(algo<TIN>& _step, int& _i) :
      step(_step), check(def_check), max_iter(_i) {}
    iterAlgo(algo<TIN>& _step, int&& _i) :
      step(_step), check(def_check), max_iter(_i) {}

    TIN operator()(TIN& _in) {
      int iter = 0;
      TIN out = _in;
      while (check.check(_in) && iter < max_iter) {
        iter++;
        out = step(out);
      }
      return out;
    }

    void operator<<(algo<TIN>& _algo) {
      add(_algo);
    }

  protected:
    algo<TIN>& step;
    algo<TIN>& check;
    algo<TIN> def_check;
    int max_iter;
  };
}
