#pragma once

namespace amh {
  template<class TIN>
  class doWhileAlgo : public algo<TIN> {
  public:
    doWhileAlgo(algo<TIN>& _step,
                algo<TIN>& _check) :
      step(_step), check(_check) {}

    TIN operator()(TIN& _in) {
      TIN out = _in;
      do {
        out = step(out);
      } while (check.check(out));
      return out;
    }

    void operator<<(algo<TIN>& _algo) {
      add(_algo);
    }

  protected:
    algo<TIN>& step;
    algo<TIN>& check;
  };
}
