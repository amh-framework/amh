/*
  Copyright (C) Dolphin project-team, CRIStAL, 2017
  Aymeric Blot

  This software is governed by the CeCILL-C license.
  You can use, modify and/ or redistribute the software under the
  terms of the CeCILL-C license as circulated by CEA, CNRS and INRIA
  at the following URL "http://www.cecill.info".
 */

#pragma once

// necessary headers for amh
#include <algorithm>
#include <functional>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
#include <sstream>
#include <string>
#include <utility>
#include <tuple>
#include <vector>

// debug helper
template <class TH, class...TT>
void dbgaux(TH head) {
  std::cerr << head << " ";
}
template <class TE, class...TT>
void dbgaux(std::vector<TE> head) {
  std::cerr << "[ ";
  for (TE& e : head)
    dbgaux(e);
  //std::copy(head.begin(), head.end(), std::ostream_iterator<TE>(std::cerr, " "));
  std::cerr << "]";
}
template <class TH, class...TT>
void dbgaux(TH head, TT...tail) {
  dbgaux(head);
  dbgaux(tail...);
}
template <class ...Args>
void dbg(Args ...args) {
  std::cerr << "[debug: ";
  dbgaux(args...);
  std::cerr << "]" << std::endl;
}
void dbg() {
  std::cerr << "[debug]" << std::endl;
}

// helper for random operations
#include <amh/rng_helper.h>
namespace amh {
  // global variable to uniformize random seeding
  rng_helper rng;
}

// cli parser
#include <amh/parser.h>


// base algorithm
#include <amh/algo.h>
#include <amh/func.h>

// composition
#include <amh/algo_chain.h>
#include <amh/algo_conj.h>
#include <amh/algo_disj.h>

// generators
#include <amh/gen.h>
#include <amh/gen_rand.h>
#include <amh/gen_randworep.h>
#include <amh/gen_union.h>

// flow
#include <amh/algo_if.h>
#include <amh/algo_unless.h>
#include <amh/algo_while.h>
#include <amh/algo_dowhile.h>
#include <amh/algo_until.h>
#include <amh/algo_dountil.h>
#include <amh/algo_iter.h>
#include <amh/algo_every.h>
#include <amh/algo_map.h>
