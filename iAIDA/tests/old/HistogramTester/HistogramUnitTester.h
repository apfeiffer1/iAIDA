#ifndef HISTOGRAMUNITTESTER_H
#define HISTOGRAMUNITTESTER_H 1

#include "AIDA_Dev/IDevHistogramFactory.h"
#include <iostream>

namespace AIDA {
  class IBaseHistogram;
  class IHistogram;
  class IHistogram1D;
  class IHistogram2D;
  class IHistogram3D;
  class ICloud;
  class ICloud1D;
  class ICloud2D;
  class ICloud3D;
  class IProfile;
  class IProfile1D;
  class IProfile2D;
}

class HistogramUnitTester {

public:
  HistogramUnitTester( AIDA::Dev::IDevHistogramFactory& hf ): m_hf( hf ) {};
  ~HistogramUnitTester(){};

  void testHistogram1D( std::ostream& out );
  void testHistogram2D( std::ostream& out );
  void testHistogram3D( std::ostream& out );
  void testCloud1D( std::ostream& out );
  void testCloud2D( std::ostream& out );
  void testCloud3D( std::ostream& out );
  void testProfile1D( std::ostream& out );
  void testProfile2D( std::ostream& out );
  void testProjections( std::ostream& out );
  void testSlices( std::ostream& out );
  void testCloudToHistogram( std::ostream& out );
  void testMergeHistogram( std::ostream& out );

private:
  AIDA::Dev::IDevHistogramFactory& m_hf;

  // Private methods
  void dumpBaseHistogramInfo( const AIDA::IBaseHistogram& h, std::ostream& os ) const;
  void dumpHistogramInfo( const AIDA::IHistogram& h, std::ostream& os ) const;
  void dumpHistogramInfo1D( const AIDA::IHistogram1D& h, std::ostream& os ) const;
  void dumpHistogramInfo2D( const AIDA::IHistogram2D& h, std::ostream& os ) const;
  void dumpHistogramInfo3D( const AIDA::IHistogram3D& h, std::ostream& os ) const;
  void dumpCloudInfo( const AIDA::ICloud& h, std::ostream& os ) const;
  void dumpCloudInfo1D( const AIDA::ICloud1D& h, std::ostream& os ) const;
  void dumpCloudInfo2D( const AIDA::ICloud2D& h, std::ostream& os ) const;
  void dumpCloudInfo3D( const AIDA::ICloud3D& h, std::ostream& os ) const;
  void dumpProfileInfo( const AIDA::IProfile& h, std::ostream& os ) const;
  void dumpProfileInfo1D( const AIDA::IProfile1D& h, std::ostream& os ) const;
  void dumpProfileInfo2D( const AIDA::IProfile2D& h, std::ostream& os ) const;

};

#endif
