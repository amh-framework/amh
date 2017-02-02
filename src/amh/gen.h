#pragma once

namespace amh {
  template<class TIN>
  class gen : amh::algo<TIN,TIN> {
  public:
    gen() {}

    virtual void init() {}

    virtual void init(TIN&) {
      init();
    }

    virtual void key(int) {}

    virtual int key() {
      return -1;
    }

    virtual void next() {}

    virtual int max() {
      return -1;
    }

    virtual TIN operator()() {
      throw std::runtime_error("to be implemented");
    }

    virtual TIN operator()(TIN& _in) {
      return operator()();
    }
  };
}
