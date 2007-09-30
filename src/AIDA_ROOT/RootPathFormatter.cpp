#include "RootPathFormatter.h"

const iAIDA::AIDA_RootStore::RootPathFormatter&
iAIDA::AIDA_RootStore::RootPathFormatter::theFormatter()
{
  static iAIDA::AIDA_RootStore::RootPathFormatter formatter;
  return formatter;
}
