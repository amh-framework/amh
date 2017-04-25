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
  class ctrlr_rand : public ctrlr_base {
  public:
    void setup(int k) {
      max = k;
    }

    void update() {
      current_index = rng.random(max);
    }

    int get_index() {
      return current_index;
    }

  protected:
    int current_index = 0;
    int max = 0;
  };
}
