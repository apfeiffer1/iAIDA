#ifndef AIDA_CPP_EXCEPTIONS_H
#define AIDA_CPP_EXCEPTIONS_H

#include<exception>
#include<string>

namespace AIDA_CPP {
  typedef std::exception ExceptionI;

  /** base generic exception for AIDA_CPP
   */
  class BaseException: public ExceptionI  {
  public:
    BaseException() throw();
    BaseException(const std::string & mess) throw();
    virtual ~BaseException() throw();
    virtual const char* what() const throw() { return message.c_str();}
    
  private:
    std::string message;
  };
  
  
}

#endif // AIDA_CPP_EXCEPTIONS_H
