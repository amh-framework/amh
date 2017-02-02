#pragma once

namespace amh {
  template < class ... FT >
  class feedControler : public controler {
  public:
    virtual void feed(FT ... f) {}
  };
}
