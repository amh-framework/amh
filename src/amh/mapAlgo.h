#pragma once

namespace amh {
  template<class TIN, class TFOO, class TBAR=TFOO, class TOUT=TIN>
  class mapAlgo : public algo<TIN,TOUT> {
  public:
    mapAlgo(algo<TFOO,TBAR>& _algo) : algo_(_algo) {}

    TOUT operator()(TIN& _in) {
      TOUT tmp;
      for (TFOO& foo : _in)
        tmp.push_back(algo_(foo));
      // TOUT out = retTypeHelper<TIN,TOUT>(_in);
      // out.resize(0);
      // out.replace(tmp);
      // return out;
      return tmp;
    }

  protected:
    algo<TFOO,TBAR>& algo_;
  };
}
