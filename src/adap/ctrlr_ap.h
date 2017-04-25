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
  class ctrlr_ap : public ctrlr_feed<double> {
  public:
    ctrlr_ap(double _alpha, double _beta, double _pmin) :
      alpha(_alpha), beta(_beta), pmin(_pmin) {}

    void setup(int k) {
      if (k == 0)
        throw std::runtime_error("*** ERROR : zero options");
      size = k;
      est_payoffs.resize(0);
      //for (int i=0; i<size; i++)
      //  est_payoffs.push_back(1);
      sum_payoffs = 0;//size;
      if (pmin > 1.0/size)
        throw std::runtime_error("*** ERROR : pmin too big");
      pmax = 1-(size-1)*pmin;
    }

    void feed(double x) {
      int s = est_payoffs.size();
      if (s != size) {
        if (s != current_index)
          throw std::runtime_error("*** ERROR : bad init sequence");
        est_payoffs.push_back(x);
      } else {
        double tmp = est_payoffs[current_index];
        est_payoffs[current_index] = tmp + alpha*(x-tmp);
      }
      sum_payoffs = 0;
      for (unsigned int i=0; i<s; i++)
        sum_payoffs += est_payoffs[i];
    }

    double probability(int i) {
      double p = pmin+(1-size*pmin)*est_payoffs[i]/sum_payoffs;
      if (i == max_payoff_id)
        return p + beta*(pmax-p);
      else
        return p + beta*(pmin-p);
    }

    void update() {
      // update max_payoff_id
      if (est_payoffs.size() < size) {
        current_index = est_payoffs.size();
      } else {
        double max = -1;
        unsigned int max_i = -1;
        for (unsigned int i=0; i<size; i++) {
          double p = probability(i);
          if (p>=max) {
            max = p;
            max_i = i;
          }
        }
        max_payoff_id = max_i;
        // roulette
        double x = rng.uniform();
        for (unsigned int i=0; i<size; i++) {
          double p = probability(i);
          if (x<p) {
            current_index = i;
            break;
          } else {
            x -= p;
          }
        }
      }
    }

    int get_index() {
      return current_index;
    }

    void summary() {
      std::cout << "summary:\n";
      for (int i=0; i<size; i++) {
        std::cout << i << " " << est_payoffs[i] << " " << 100*(est_payoffs[i]/sum_payoffs + pmin) << "%" << std::endl;
      }
      std::cout << "sum " << sum_payoffs << std::endl;
    }

  protected:
    int current_index = 0;
    std::vector<double> est_payoffs;
    double sum_payoffs;
    double max_payoff_id;
    double alpha = 0.5;
    double beta = 0.5;
    double pmin = 0;
    double pmax = 1;
    int init = 0;
    int size;
  };
}
