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
  template<class TIN, class TALGO>
  class algo_ctrl : public algo<TIN,TIN> {
  public:
    algo_ctrl(ctrlr_base& _model, TALGO*& p) : model(_model), algop(p) {}

    void add(TALGO& _algo) {
      algo_list.push_back(&_algo);
    }

    void setup(std::vector<TALGO*>& v) {
      algo_list = v;
      model.setup(v.size());
      setuped = true;
    }

    void update() {
      int k = algo_list.size();
      if (k > 0) {
        if (!setuped) {
          model.setup(k);
          setuped = true;
        }
        model.update();
        int i = model.get_index();
        algop = algo_list[i];
      }
    }

    TIN operator()(TIN& _in) {
      update();
      return _in;
    }

    void operator<<(TALGO& _algo) {
      add(_algo);
      setuped = false;
    }

  protected:
    std::vector<TALGO*> algo_list;
    TALGO*& algop;
    bool setuped = false;
    ctrlr_base& model;
  };
}
