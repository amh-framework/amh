#pragma once

namespace amh {
  template<class TIN, class TOUT=TIN>
  class unlessAlgo : public algo<TIN,TOUT> {
  public:
    unlessAlgo(algo<TIN,TOUT>& _check, algo<TIN,TOUT>& _opt) :
      check(_check), op_true(_opt), op_false(def_algo) {}
    unlessAlgo(algo<TIN,TOUT>& _check, algo<TIN,TOUT>& _opt, algo<TIN,TOUT>& _opf) :
      check(_check), op_true(_opt), op_false(_opf) {}

    TOUT operator()(TIN& _in) {
      if (check.check(_in))
        return op_false(_in);
      else
        return op_true(_in);
    }

  protected:
    algo<TIN,TOUT>& op_true;
    algo<TIN,TOUT>& op_false;
    algo<TIN,TOUT>& check;
    algo<TIN,TOUT> def_algo;
  };
}
