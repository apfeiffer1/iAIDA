// Emacs -*- C++ -*-
#ifndef AIDANATIVE_AIDADEVHISTOGRAMFACTORY
#define AIDANATIVE_AIDADEVHISTOGRAMFACTORY 1

#include "AIDA_Dev/IDevHistogramFactory.h"

namespace iAIDA {
  namespace AIDA_Histogram_native {

/// Implementation of the AIDA::Dev::IDevHistogramFactory interface class

class AIDA_DevHistogramFactory : public AIDA::Dev::IDevHistogramFactory {

public:
  /// Constructor
  AIDA_DevHistogramFactory() {};
  /// Destructor
  ~AIDA_DevHistogramFactory() {};

  /// Methods inherited from IDevHistoFactory
  bool destroy( AIDA::IBaseHistogram * histo );
  AIDA::Dev::IDevCloud1D* createCloud1D( const std::string & title, int nMax = -1, const std::string & options = "" );
  AIDA::Dev::IDevCloud1D* createCloud1D();
  AIDA::Dev::IDevCloud1D* createCopy( const AIDA::ICloud1D & original );
  AIDA::Dev::IDevCloud2D* createCloud2D( const std::string & title, int nMax = -1, const std::string & options = "" );
  AIDA::Dev::IDevCloud2D* createCloud2D();
  AIDA::Dev::IDevCloud2D* createCopy( const AIDA::ICloud2D & original );
  AIDA::Dev::IDevCloud3D* createCloud3D( const std::string & title, int nMax = -1, const std::string & options = "" );
  AIDA::Dev::IDevCloud3D* createCloud3D();
  AIDA::Dev::IDevCloud3D* createCopy( const AIDA::ICloud3D & original );
  AIDA::Dev::IDevHistogram1D* createHistogram1D( const std::string & title, int nBins, double lowerEdge, double upperEdge, const std::string & options = "");
  AIDA::Dev::IDevHistogram1D* createHistogram1D( const std::string & title, const std::vector< double > & binEdge, const std::string & options );
  AIDA::Dev::IDevHistogram1D* createHistogram1D( int nBins, double lowerEdge, double upperEdge );
  AIDA::Dev::IDevHistogram1D* createCopy( const AIDA::IHistogram1D & original );
  AIDA::Dev::IDevHistogram2D* createHistogram2D( const std::string & title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, const std::string & options  = "");
  AIDA::Dev::IDevHistogram2D* createHistogram2D( const std::string & title, const std::vector< double > & binEdgeX, const std::vector< double > & binEdgeY, const std::string & options );
  AIDA::Dev::IDevHistogram2D* createHistogram2D( int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY );
  AIDA::Dev::IDevHistogram2D* createCopy( const AIDA::IHistogram2D & original );
  AIDA::Dev::IDevHistogram3D* createHistogram3D( const std::string & title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, int nBinsZ, double lowerEdgeZ, double upperEdgeZ, const std::string & options = "" );
  AIDA::Dev::IDevHistogram3D* createHistogram3D( const std::string & title, const std::vector< double > & binEdgeX, const std::vector< double > & binEdgeY, const std::vector< double > & binEdgeZ, const std::string & options );
  AIDA::Dev::IDevHistogram3D* createHistogram3D( int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, int nBinsZ, double lowerEdgeZ, double upperEdgeZ );
  AIDA::Dev::IDevHistogram3D* createCopy( const AIDA::IHistogram3D & original );
  AIDA::Dev::IDevProfile1D* createProfile1D( const std::string & title, int nBins, double lowerEdge, double upperEdge, const std::string & options = "");
  AIDA::Dev::IDevProfile1D* createProfile1D( const std::string & title, int nBins, double lowerEdge, double upperEdge, double lowerValue, double upperValue, const std::string & options = "");
  AIDA::Dev::IDevProfile1D* createProfile1D( const std::string & title, const std::vector< double > & binEdge, const std::string & options );
  AIDA::Dev::IDevProfile1D* createProfile1D( const std::string & title, const std::vector< double > & binEdge, double lowerValue, double upperValue, const std::string & options );
  AIDA::Dev::IDevProfile1D* createProfile1D( int nBins, double lowerEdge, double upperEdge );
  AIDA::Dev::IDevProfile1D* createProfile1D( int nBins, double lowerEdge, double upperEdge, double lowerValue, double upperValue );
  AIDA::Dev::IDevProfile1D* createCopy( const AIDA::IProfile1D & original );
  AIDA::Dev::IDevProfile2D* createProfile2D( const std::string & title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, const std::string & options = "");
  AIDA::Dev::IDevProfile2D* createProfile2D( const std::string & title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, double lowerValue, double upperValue, const std::string & options = "");
  AIDA::Dev::IDevProfile2D* createProfile2D( const std::string & title, const std::vector< double > & binEdgeX, const std::vector< double > & binEdgeY, const std::string & options );
  AIDA::Dev::IDevProfile2D* createProfile2D( const std::string & title, const std::vector< double > & binEdgeX, const std::vector< double > & binEdgeY, double lowerValue, double upperValue, const std::string & options );
  AIDA::Dev::IDevProfile2D* createProfile2D( int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY );
  AIDA::Dev::IDevProfile2D* createProfile2D( int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, double lowerValue, double upperValue );
  AIDA::Dev::IDevProfile2D* createCopy( const AIDA::IProfile2D & original );
  AIDA::Dev::IDevHistogram1D* add( const AIDA::IHistogram1D & a, const AIDA::IHistogram1D & b );
  AIDA::Dev::IDevHistogram1D* subtract( const AIDA::IHistogram1D & a, const AIDA::IHistogram1D & b );
  AIDA::Dev::IDevHistogram1D* multiply( const AIDA::IHistogram1D & a, const AIDA::IHistogram1D & b );
  AIDA::Dev::IDevHistogram1D* divide( const AIDA::IHistogram1D & a, const AIDA::IHistogram1D & b );
  AIDA::Dev::IDevHistogram2D* add( const AIDA::IHistogram2D & a, const AIDA::IHistogram2D & b );
  AIDA::Dev::IDevHistogram2D* subtract( const AIDA::IHistogram2D & a, const AIDA::IHistogram2D & b );
  AIDA::Dev::IDevHistogram2D* multiply( const AIDA::IHistogram2D & a, const AIDA::IHistogram2D & b );
  AIDA::Dev::IDevHistogram2D* divide( const AIDA::IHistogram2D & a, const AIDA::IHistogram2D & b );
  AIDA::Dev::IDevHistogram3D* add( const AIDA::IHistogram3D & a, const AIDA::IHistogram3D & b );
  AIDA::Dev::IDevHistogram3D* subtract( const AIDA::IHistogram3D & a, const AIDA::IHistogram3D & b );
  AIDA::Dev::IDevHistogram3D* multiply( const AIDA::IHistogram3D & a, const AIDA::IHistogram3D & b );
  AIDA::Dev::IDevHistogram3D* divide( const AIDA::IHistogram3D & a, const AIDA::IHistogram3D & b );
  AIDA::Dev::IDevHistogram1D* projectionX( const AIDA::IHistogram2D & h );
  AIDA::Dev::IDevHistogram1D* projectionY( const AIDA::IHistogram2D & h );
  AIDA::Dev::IDevHistogram1D* sliceX( const AIDA::IHistogram2D & h, int indexY );
  AIDA::Dev::IDevHistogram1D* sliceY( const AIDA::IHistogram2D & h, int indexX );
  AIDA::Dev::IDevHistogram1D* sliceX( const AIDA::IHistogram2D & h, int indexY1, int indexY2 );
  AIDA::Dev::IDevHistogram1D* sliceY( const AIDA::IHistogram2D & h, int indexX1, int indexX2 );
  AIDA::Dev::IDevHistogram2D* projectionXY( const AIDA::IHistogram3D & h );
  AIDA::Dev::IDevHistogram2D* projectionXZ( const AIDA::IHistogram3D & h );
  AIDA::Dev::IDevHistogram2D* projectionYZ( const AIDA::IHistogram3D & h );
  AIDA::Dev::IDevHistogram2D* sliceXY( const AIDA::IHistogram3D & h, int indexZ1, int indexZ2 );
  AIDA::Dev::IDevHistogram2D* sliceXZ( const AIDA::IHistogram3D & h, int indexY1, int indexY2 );
  AIDA::Dev::IDevHistogram2D* sliceYZ( const AIDA::IHistogram3D & h, int indexX1, int indexX2 );

};

  }
}

#endif /* ifndef AIDANATIVE_AIDADEVHISTOGRAMFACTORY */
