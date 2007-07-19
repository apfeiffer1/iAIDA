#include "HBookPathFormater.h"

const pi::AIDA_HBookStore::HBookPathFormater&
pi::AIDA_HBookStore::HBookPathFormater::theFormater()
{
  static pi::AIDA_HBookStore::HBookPathFormater formater;
  return formater;
}
