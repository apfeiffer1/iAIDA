#include "RootPathFormater.h"

const pi::AIDA_RootStore::RootPathFormater&
pi::AIDA_RootStore::RootPathFormater::theFormater()
{
  static pi::AIDA_RootStore::RootPathFormater formater;
  return formater;
}
