#include <iostream>
#include "HistogramUnitTester.h"
#include "AIDA/IAnnotation.h"
#include "AIDA/IHistogram.h"
#include "AIDA/IBaseHistogram.h"
#include "AIDA/ICloud.h"
#include "AIDA/IProfile.h"
#include "AIDA_Dev/IDevManagedObject.h"


void
HistogramUnitTester::dumpBaseHistogramInfo( const AIDA::IBaseHistogram& h, std::ostream& os ) const
{
  os << "Histogram name : " << dynamic_cast<const AIDA::IManagedObject&>( h ).name() << std::endl
     << "Type : " << dynamic_cast<const AIDA::Dev::IDevManagedObject&>( h ).userLevelClassType() << std::endl
     << "Title : " << h.title() << std::endl
     << "Dimension : " << h.dimension() << std::endl
     << "Entries : " << h.entries() << std::endl
     << std::endl;

  os << "*** Annotation ** START ** "<< std::endl;
  const AIDA::IAnnotation& annotation = h.annotation();
  for (  int i = 0; i < annotation.size(); ++i ) {
    os << annotation.key( i ) << " : " << annotation.value( i ) << std::endl;
  }
  os << "*** Annotation *** END ***" << std::endl << std::endl;
}


void
HistogramUnitTester::dumpHistogramInfo( const AIDA::IHistogram& h, std::ostream& os ) const
{
  dumpBaseHistogramInfo( h, os );
  os << "All entries : " << h.allEntries() << std::endl
     << "Extra entries : " << h.extraEntries() << std::endl
     << "Equivalent bin entries : " << h.equivalentBinEntries() << std::endl
     << "Sum of bin heights : " << h.sumBinHeights() << std::endl
     << "Sum of extra bin heights : " << h.sumExtraBinHeights() << std::endl
     << "Sum of all bin heights : " << h.sumAllBinHeights() << std::endl
     << "Minimum bin height : " << h.minBinHeight() << std::endl
     << "Maximum bin height : " << h.maxBinHeight() << std::endl
     << std::endl;
}


void
HistogramUnitTester::dumpCloudInfo( const AIDA::ICloud& h, std::ostream& os ) const
{
  dumpBaseHistogramInfo( h, os );
  os << "Sum of weights : " << h.sumOfWeights() << std::endl;
  if ( h.isConverted() ) os << "Cloud has been converted into a histogram" << std::endl;
  else  os << "Cloud has NOT been converted yet into a histogram" << std::endl;
  os << std::endl;
}


void
HistogramUnitTester::dumpProfileInfo( const AIDA::IProfile& h, std::ostream& os ) const
{
  dumpBaseHistogramInfo( h, os );
  os << "All entries : " << h.allEntries() << std::endl
     << "Extra entries : " << h.extraEntries() << std::endl
     << "Sum of bin heights : " << h.sumBinHeights() << std::endl
     << "Sum of extra bin heights : " << h.sumExtraBinHeights() << std::endl
     << "Sum of all bin heights : " << h.sumAllBinHeights() << std::endl
     << "Minimum bin height : " << h.minBinHeight() << std::endl
     << "Maximum bin height : " << h.maxBinHeight() << std::endl
     << std::endl;
}
