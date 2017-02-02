#pragma once

namespace amh {
  template<class TIN, class TOUT=TIN>
  class adapAlgo : public algo<TIN,TOUT> {
  public:
    adapAlgo(controler& _model) : model(_model) {}

    void add(algo<TIN,TOUT> & _algo) {
      algo_list.push_back(&_algo);
    }

    void operator()(TIN & _in) {
      int k = algo_list.size();
      if (k > 0) {
        if (!setuped) {
          model.setup(k);
          setuped = true;
        }
        int i = model.get_index();
        return (*algo_list[i])(_in);
      } else
        return retTypeHelper<TIN,TOUT>(_in);
    }

    void operator<<(algo<TIN,TOUT> & _algo) {
      add(_algo);
    }

  protected:
    std::vector<algo<TIN,TOUT>*> algo_list;
    bool setuped = false;
    controler& model;
  };
}
