// Emacs -*- C++ -*-
#ifndef AIDA_ROOT_AIDAHISTOGRAMFACTORY_H 
#define AIDA_ROOT_AIDAHISTOGRAMFACTORY_H 1

// includes 
#include "AIDA/IHistogramFactory.h"
// keep dependency on aida_root tree but can be removed 
#include "AIDA_ROOT/Tree.h"

#include "AIDA_Dev/IDevHistogramFactory.h"
#include "AIDA_Dev/IDevTree.h"

namespace AIDA_ROOT {

/// Implementation of the AIDA::IHistogramFactory interface class

class AIDA_HistogramFactory : virtual public AIDA::IHistogramFactory
{
public:
  /// Constructor
  AIDA_HistogramFactory( AIDA::Dev::IDevTree& tree, AIDA::Dev::IDevHistogramFactory& factory );
  /// Destructor
  ~AIDA_HistogramFactory() { /* nop */; }
  /// Methods inherited by IHistogramFactory
  bool destroy( AIDA::IBaseHistogram* histo );
  AIDA::ICloud1D* createCloud1D( const std::string& name, const std::string& title, int nMax = -1, const std::string& options = "" );
  AIDA::ICloud1D* createCloud1D( const std::string& name );
  AIDA::ICloud1D* createCopy( const std::string& name, const AIDA::ICloud1D& original );
  AIDA::ICloud2D* createCloud2D( const std::string& name, const std::string& title, int nMax = -1, const std::string& options = "" );
  AIDA::ICloud2D* createCloud2D( const std::string& name );
  AIDA::ICloud2D* createCopy( const std::string& name, const AIDA::ICloud2D& original );
  AIDA::ICloud3D* createCloud3D( const std::string& name, const std::string& title, int nMax = -1, const std::string& options = "" );
  AIDA::ICloud3D* createCloud3D( const std::string& name );
  AIDA::ICloud3D* createCopy( const std::string& name, const AIDA::ICloud3D& original );
  AIDA::IHistogram1D* createHistogram1D( const std::string& name, const std::string& title, int nBins, double lowerEdge, double upperEdge, const std::string& options = "");
  AIDA::IHistogram1D* createHistogram1D( const std::string& name, const std::string& title, const std::vector< double >& binEdge, const std::string& options = "" );
  AIDA::IHistogram1D* createHistogram1D( const std::string& name, int nBins, double lowerEdge, double upperEdge );
  AIDA::IHistogram1D* createCopy( const std::string& name, const AIDA::IHistogram1D& original );
  AIDA::IHistogram2D* createHistogram2D( const std::string& name, const std::string& title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, const std::string& options  = "");
  AIDA::IHistogram2D* createHistogram2D( const std::string& name, const std::string& title, const std::vector< double >& binEdgeX, const std::vector< double >& binEdgeY, const std::string& options = "" );
  AIDA::IHistogram2D* createHistogram2D( const std::string& name, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY );
  AIDA::IHistogram2D* createCopy( const std::string& name, const AIDA::IHistogram2D& original );
  AIDA::IHistogram3D* createHistogram3D( const std::string& name, const std::string& title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, int nBinsZ, double lowerEdgeZ, double upperEdgeZ, const std::string& options = "" );
  AIDA::IHistogram3D* createHistogram3D( const std::string& name, const std::string& title, const std::vector< double >& binEdgeX, const std::vector< double >& binEdgeY, const std::vector< double >& binEdgeZ, const std::string& options = "" );
  AIDA::IHistogram3D* createHistogram3D( const std::string& name, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, int nBinsZ, double lowerEdgeZ, double upperEdgeZ );
  AIDA::IHistogram3D* createCopy( const std::string& name, const AIDA::IHistogram3D& original );
  AIDA::IProfile1D* createProfile1D( const std::string& name, const std::string& title, int nBins, double lowerEdge, double upperEdge, const std::string& options = "");
  AIDA::IProfile1D* createProfile1D( const std::string& name, const std::string& title, int nBins, double lowerEdge, double upperEdge, double lowerValue, double upperValue, const std::string& options = "");
  AIDA::IProfile1D* createProfile1D( const std::string& name, const std::string& title, const std::vector< double >& binEdge, const std::string& options = "" );
  AIDA::IProfile1D* createProfile1D( const std::string& name, const std::string& title, const std::vector< double >& binEdge, double lowerValue, double upperValue, const std::string& options = "" );
  AIDA::IProfile1D* createProfile1D( const std::string& name, int nBins, double lowerEdge, double upperEdge );
  AIDA::IProfile1D* createProfile1D( const std::string& name, int nBins, double lowerEdge, double upperEdge, double lowerValue, double upperValue );
  AIDA::IProfile1D* createCopy( const std::string& name, const AIDA::IProfile1D& original );
  AIDA::IProfile2D* createProfile2D( const std::string& name, const std::string& title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, const std::string& options = "" );
  AIDA::IProfile2D* createProfile2D( const std::string& name, const std::string& title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, double lowerValue, double upperValue, const std::string& options = "");
  AIDA::IProfile2D* createProfile2D( const std::string& name, const std::string& title, const std::vector< double >& binEdgeX, const std::vector< double >& binEdgeY, const std::string& options = "" );
  AIDA::IProfile2D* createProfile2D( const std::string& name, const std::string& title, const std::vector< double >& binEdgeX, const std::vector< double >& binEdgeY, double lowerValue, double upperValue, const std::string& options = "" );
  AIDA::IProfile2D* createProfile2D( const std::string& name, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY );
  AIDA::IProfile2D* createProfile2D( const std::string& name, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, double lowerValue, double upperValue );
  AIDA::IProfile2D* createCopy( const std::string& name, const AIDA::IProfile2D& original );
  AIDA::IHistogram1D* add( const std::string& name, const AIDA::IHistogram1D& a, const AIDA::IHistogram1D& b );
  AIDA::IHistogram1D* subtract( const std::string& name, const AIDA::IHistogram1D& a, const AIDA::IHistogram1D& b );
  AIDA::IHistogram1D* multiply( const std::string& name, const AIDA::IHistogram1D& a, const AIDA::IHistogram1D& b );
  AIDA::IHistogram1D* divide( const std::string& name, const AIDA::IHistogram1D& a, const AIDA::IHistogram1D& b );
  AIDA::IHistogram2D* add( const std::string& name, const AIDA::IHistogram2D& a, const AIDA::IHistogram2D& b );
  AIDA::IHistogram2D* subtract( const std::string& name, const AIDA::IHistogram2D& a, const AIDA::IHistogram2D& b );
  AIDA::IHistogram2D* multiply( const std::string& name, const AIDA::IHistogram2D& a, const AIDA::IHistogram2D& b );
  AIDA::IHistogram2D* divide( const std::string& name, const AIDA::IHistogram2D& a, const AIDA::IHistogram2D& b );
  AIDA::IHistogram3D* add( const std::string& name, const AIDA::IHistogram3D& a, const AIDA::IHistogram3D& b );
  AIDA::IHistogram3D* subtract( const std::string& name, const AIDA::IHistogram3D& a, const AIDA::IHistogram3D& b );
  AIDA::IHistogram3D* multiply( const std::string& name, const AIDA::IHistogram3D& a, const AIDA::IHistogram3D& b );
  AIDA::IHistogram3D* divide( const std::string& name, const AIDA::IHistogram3D& a, const AIDA::IHistogram3D& b );
  AIDA::IHistogram1D* projectionX( const std::string& name, const AIDA::IHistogram2D& h );
  AIDA::IHistogram1D* projectionY( const std::string& name, const AIDA::IHistogram2D& h );
  AIDA::IHistogram1D* sliceX( const std::string& name, const AIDA::IHistogram2D& h, int indexY );
  AIDA::IHistogram1D* sliceY( const std::string& name, const AIDA::IHistogram2D& h, int indexX );
  AIDA::IHistogram1D* sliceX( const std::string& name, const AIDA::IHistogram2D& h, int indexY1, int indexY2 );
  AIDA::IHistogram1D* sliceY( const std::string& name, const AIDA::IHistogram2D& h, int indexX1, int indexX2 );
  AIDA::IHistogram2D* projectionXY( const std::string& name, const AIDA::IHistogram3D& h );
  AIDA::IHistogram2D* projectionXZ( const std::string& name, const AIDA::IHistogram3D& h );
  AIDA::IHistogram2D* projectionYZ( const std::string& name, const AIDA::IHistogram3D& h );
  AIDA::IHistogram2D* sliceXY( const std::string& name, const AIDA::IHistogram3D& h, int indexZ1, int indexZ2 );
  AIDA::IHistogram2D* sliceXZ( const std::string& name, const AIDA::IHistogram3D& h, int indexY1, int indexY2 );
  AIDA::IHistogram2D* sliceYZ( const std::string& name, const AIDA::IHistogram3D& h, int indexX1, int indexX2 );

private:
  /// Data members : reference to the tree and the real factory
  AIDA::Dev::IDevTree&              m_tree;
  AIDA::Dev::IDevHistogramFactory&  m_factory;

  /// Private method
  template< class T >
  T* registerObject( T* p, const std::string& name )
  {
    if ( p ) {
      // set name of object
      p->setName( name );
      
     
      AIDA_ROOT::Tree * impTree = dynamic_cast<AIDA_ROOT::Tree*> (&m_tree); 
      bool iret = false; 
      if ( impTree == 0) 
	iret =  m_tree.add(p); 
      else 
	iret = impTree->add(p,name); 

      if ( ! iret ) { 
	m_factory.destroy( p );
	p = 0;
      }
    }
    return p;
  }
  
  

};

}
#endif /* ifndef AIDA_ROOT_AIDAHISTOGRAMFACTORY_H */ 
