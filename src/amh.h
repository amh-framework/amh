#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include <functional>

template <class TH, class...TT>
void dbgaux(TH head) {
  std::cout << head << " ";
}
template <class TH, class...TT>
void dbgaux(TH head, TT...tail) {
  dbgaux(head);
  dbgaux(tail...);
}
template <class ...Args>
void dbg(Args ...args) {
  std::cout << "[debug: ";
  dbgaux(args...);
  std::cout << "]" << std::endl;
}

#include <amh/amhRng.h>
namespace amh {
  amhRng rng;
}

#include <amh/algo.h>
#include <amh/chainAlgo.h>
#include <amh/conjAlgo.h>
#include <amh/disjAlgo.h>

#include <amh/gen.h>
#include <amh/rand_gen.h>
#include <amh/randWR_gen.h>
#include <amh/union_gen.h>

#include <amh/ifAlgo.h>
#include <amh/whileAlgo.h>
#include <amh/doWhileAlgo.h>
#include <amh/untilAlgo.h>
#include <amh/doUntilAlgo.h>
#include <amh/iterAlgo.h>
#include <amh/everyAlgo.h>
#include <amh/mapAlgo.h>

#include <adap/controler.h>
#include <adap/adapAlgo.h>
#include <adap/randControler.h>
#include <adap/seqControler.h>
#include <adap/feedControler.h>
#include <adap/pmControler.h>
#include <adap/apControler.h>
#include <adap/ucbControler.h>
