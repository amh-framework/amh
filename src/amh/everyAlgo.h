#pragma once

namespace amh {
  template<class TIN, class TOUT=TIN>
  class everyAlgo : public algo<TIN,TOUT> {
  public:
    everyAlgo(int& _i, algo<TIN,TOUT>& _opt, int k=0) :
      max_i(_i), op_true(_opt), op_false(def_algo), has_else(false) {
      if (k != 0)
        setup(k);
    }
    everyAlgo(int& _i, algo<TIN,TOUT>& _opt, algo<TIN>& _opf, int k=0) :
      max_i(_i), op_true(_opt), op_false(_opf), has_else(true) {
      if (k != 0)
        setup(k);
    }
    everyAlgo(int&& _i, algo<TIN,TOUT>& _opt, int k=0) :
      max_i(_i), op_true(_opt), op_false(def_algo), has_else(false) {
      if (k != 0)
        setup(k);
    }
    everyAlgo(int&& _i, algo<TIN,TOUT>& _opt, algo<TIN,TOUT>& _opf, int k=0) :
      max_i(_i), op_true(_opt), op_false(_opf), has_else(true) {
      if (k != 0)
        setup(k);
    }

    void setup(int i) {
      current_i = i%max_i;
    }

    TOUT operator()(TIN& _in) {
      current_i++;
      if (current_i >= max_i)
        current_i = 0;
      if (current_i == 1)
        return op_true(_in);
      else if (has_else)
        return op_false(_in);
      return retTypeHelper<TIN,TOUT>(_in);
    }

    // std::string inspect(bool inline_=true, int indent=0) {
    //   std::string s = "todo";
    //   return s;
    // }

  protected:
    algo<TIN,TOUT>& op_true;
    algo<TIN,TOUT>& op_false;
    algo<TIN,TOUT> def_algo;
    bool has_else = false;
    int current_i = 0;
    int max_i = 0;
  };
}
