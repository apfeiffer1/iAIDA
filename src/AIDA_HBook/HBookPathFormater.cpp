#include "HBookPathFormater.h"

const iAIDA::AIDA_HBookStore::HBookPathFormater&
iAIDA::AIDA_HBookStore::HBookPathFormater::theFormater()
{
  static iAIDA::AIDA_HBookStore::HBookPathFormater formater;
  return formater;
}
