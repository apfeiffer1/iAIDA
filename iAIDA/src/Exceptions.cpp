
#include "AIDA_CPP/Exceptions.h"

using namespace AIDA_CPP;

BaseException::BaseException() throw() {}
BaseException::BaseException(const std::string & mess) throw() : message(mess){}
BaseException::~BaseException() throw() {}
