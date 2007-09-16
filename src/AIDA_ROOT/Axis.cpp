#include "AIDA_ROOT/Axis.h"

AIDA_ROOT::Axis::Axis() : taxis_(0), fixed_(true){}
AIDA_ROOT::Axis::Axis(TAxis * itaxi, bool isfixed) : taxis_(itaxi), fixed_(isfixed){}

void AIDA_ROOT::Axis::initialize(TAxis * itaxi, bool isfixed) {
  taxis_ = itaxi;
  fixed_ = isfixed;
}


AIDA_ROOT::Axis::~Axis(){}
