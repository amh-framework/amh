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
  class ctrlr_ucb : public ctrlr_feed<double> {
  public:
    void setup(int k) {
      if (k == 0)
        throw std::runtime_error("*** ERROR : zero options");
      size = k;
      sum_payoffs.resize(0);
      time_chosen.resize(0);
      for (int i=0; i<size; i++) {
        sum_payoffs.push_back(0);
        time_chosen.push_back(0);
      }
      init = 0;
      time = 0;
    }

    void feed(double x) {
      time_chosen[current_index] += 1;
      sum_payoffs[current_index] += x;
      time += 1;
    }

    double expectation(int i) {
      double x;
      x = sum_payoffs[i] / time_chosen[i]; // expectation
      x += sqrt(2*log(time+1) / time_chosen[i]); // upper bound
      return x;
    }

    void update() {
      if (init < size) {
        current_index = init;
        init++;
      } else {
        double max_expected = -1;
        int max_i = -1;
        for (int i = 0; i<size; i++) {
          double x = expectation(i);
          if (x >= max_expected) {
            max_expected = x;
            max_i = i;
          }
        }
        current_index = max_i;
      }
    }

    int get_index() {
      return current_index;
    }

    void summary() {
      std::cout << "summary:\n";
      for (int i=0; i<size; i++) {
        int t = time_chosen[i];
        std::cout << i << " " << t << " " << sum_payoffs[i]/t << std::endl;
      }
    }

  protected:
    int current_index = 0;
    std::vector<double> sum_payoffs;
    std::vector<int> time_chosen;
    int size = 1;
    int init = 0;
    int time = 0;
  };
}
