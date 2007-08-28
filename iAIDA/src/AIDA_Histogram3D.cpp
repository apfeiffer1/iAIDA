#include "AIDA_Histogram3D.h"
#include "EvenBinAxis.h"
#include "VariableBinAxis.h"
#include "GravityBin3D.h"
#include "AIDA/IAnnotation.h"
#include <typeinfo>
#include <cmath>
#include <set>
#include "AnnotationNumberFormater.h"


static const unsigned int numberOfExtraBins = 2;
static const std::string meanXKey = "MeanX";
static const std::string rmsXKey = "RmsX";
static const std::string meanYKey = "MeanY";
static const std::string rmsYKey = "RmsY";
static const std::string meanZKey = "MeanZ";
static const std::string rmsZKey = "RmsZ";
static const std::string extra_entriesKey = "Extra Entries";
static const std::string emptyString = "";


iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::AIDA_Histogram3D( const std::string& title,
								   int numberOfBinsX,
								   double lowEdgeX,
								   double highEdgeX,
								   int numberOfBinsY,
								   double lowEdgeY,
								   double highEdgeY,
								   int numberOfBinsZ,
								   double lowEdgeZ,
								   double highEdgeZ ):
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram( title, "IHistogram3D", 3 ),
  m_axisX( new EvenBinAxis( std::abs( numberOfBinsX ), lowEdgeX, highEdgeX ) ),
  m_axisY( new EvenBinAxis( std::abs( numberOfBinsY ), lowEdgeY, highEdgeY ) ),
  m_axisZ( new EvenBinAxis( std::abs( numberOfBinsZ ), lowEdgeZ, highEdgeZ ) ),
  m_bins( std::abs( numberOfBinsX ) + numberOfExtraBins, std::vector< std::vector< GravityBin3D* > >( std::abs( numberOfBinsY ) + numberOfExtraBins, std::vector< GravityBin3D* >( std::abs( numberOfBinsZ ) + numberOfExtraBins, 0 ) ) ),
  m_validStatistics( false )
{
  m_sumWeightTimesSquaredX = 0;
  m_sumWeightTimesSquaredY = 0;
  m_sumWeightTimesSquaredZ = 0;
  for ( unsigned int i = 0; i < m_bins.size(); ++i )
    for ( unsigned int j = 0; j < m_bins[0].size(); ++j )
      for ( unsigned int k = 0; k < m_bins[0][0].size(); ++k )
	m_bins[i][j][k] = new iAIDA::AIDA_Histogram_native::GravityBin3D;

  /// Define the items in the annotation.
  AIDA::IAnnotation& annotation = annotationNoUpdate();
  annotation.addItem( meanXKey, emptyString, false );
  annotation.addItem( rmsXKey, emptyString, false );
  annotation.addItem( meanYKey, emptyString, false );
  annotation.addItem( rmsYKey, emptyString, false );
  annotation.addItem( meanZKey, emptyString, false );
  annotation.addItem( rmsZKey, emptyString, false );
  annotation.addItem( extra_entriesKey, emptyString, false );
}

iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::AIDA_Histogram3D( const std::string& title,
								   const std::vector< double >& edgesX,
								   const std::vector< double >& edgesY,
								   const std::vector< double >& edgesZ ):
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram( title, "IHistogram3D", 3 ),
  m_axisX( new VariableBinAxis( edgesX ) ),
  m_axisY( new VariableBinAxis( edgesY ) ),
  m_axisZ( new VariableBinAxis( edgesZ ) ),
  m_bins( edgesX.size() - 1 + numberOfExtraBins, std::vector< std::vector< GravityBin3D* > >( edgesY.size() - 1 + numberOfExtraBins, std::vector< GravityBin3D* >( edgesZ.size() - 1 + numberOfExtraBins, 0 ) ) ),
  m_validStatistics( false )
{
  m_sumWeightTimesSquaredX = 0;
  m_sumWeightTimesSquaredY = 0;
  m_sumWeightTimesSquaredZ = 0;
  for ( unsigned int i = 0; i < m_bins.size(); ++i )
    for ( unsigned int j = 0; j < m_bins[0].size(); ++j )
      for ( unsigned int k = 0; k < m_bins[0][0].size(); ++k )
	m_bins[i][j][k] = new iAIDA::AIDA_Histogram_native::GravityBin3D;

  /// Define the items in the annotation.
  AIDA::IAnnotation& annotation = annotationNoUpdate();
  annotation.addItem( meanXKey, emptyString, false );
  annotation.addItem( rmsXKey, emptyString, false );
  annotation.addItem( meanYKey, emptyString, false );
  annotation.addItem( rmsYKey, emptyString, false );
  annotation.addItem( meanZKey, emptyString, false );
  annotation.addItem( rmsZKey, emptyString, false );
  annotation.addItem( extra_entriesKey, emptyString, false );
}


iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::AIDA_Histogram3D( const iAIDA::AIDA_Histogram_native::AIDA_Histogram3D& h ):
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram( h.title(), "IHistogram3D", 3 ),
  m_axisX( 0 ),
  m_axisY( 0 ),
  m_axisZ( 0 ),
  m_bins( h.m_bins.size(), std::vector< std::vector< GravityBin3D* > >( h.m_bins[0].size(), std::vector< GravityBin3D* >( h.m_bins[0][0].size(), 0 ) ) ),
  m_validStatistics( false )
{
  m_sumWeightTimesSquaredX = 0;
  m_sumWeightTimesSquaredY = 0;
  m_sumWeightTimesSquaredZ = 0;

  // Construct the axes first
  const AIDA::IAxis& otherAxisX = h.xAxis();
  if ( otherAxisX.isFixedBinning() ) {
    m_axisX = new EvenBinAxis( otherAxisX.bins(),
			       otherAxisX.lowerEdge(),
			       otherAxisX.upperEdge() );
  }
  else {
    std::vector< double > edges( otherAxisX.bins() + 1 );
    for ( int i = 0; i < otherAxisX.bins(); ++i ) edges[i] = otherAxisX.binLowerEdge( i );
    edges.back() = otherAxisX.upperEdge();
    m_axisX = new VariableBinAxis( edges );
  };
  const AIDA::IAxis& otherAxisY = h.yAxis();
  if ( otherAxisY.isFixedBinning() ) {
    m_axisY = new EvenBinAxis( otherAxisY.bins(),
			       otherAxisY.lowerEdge(),
			       otherAxisY.upperEdge() );
  }
  else {
    std::vector< double > edges( otherAxisY.bins() + 1 );
    for ( int i = 0; i < otherAxisY.bins(); ++i ) edges[i] = otherAxisY.binLowerEdge( i );
    edges.back() = otherAxisY.upperEdge();
    m_axisY = new VariableBinAxis( edges );
  };
  const AIDA::IAxis& otherAxisZ = h.zAxis();
  if ( otherAxisZ.isFixedBinning() ) {
    m_axisZ = new EvenBinAxis( otherAxisZ.bins(),
			       otherAxisZ.lowerEdge(),
			       otherAxisZ.upperEdge() );
  }
  else {
    std::vector< double > edges( otherAxisZ.bins() + 1 );
    for ( int i = 0; i < otherAxisZ.bins(); ++i ) edges[i] = otherAxisZ.binLowerEdge( i );
    edges.back() = otherAxisZ.upperEdge();
    m_axisZ = new VariableBinAxis( edges );
  }

  // Copy the bin contents
  for ( unsigned int i = 0; i < m_bins.size(); ++i )
    for ( unsigned int j = 0; j < m_bins[0].size(); ++j )
      for ( unsigned int k = 0; k < m_bins[0][0].size(); ++k )
	m_bins[i][j][k] = new GravityBin3D( *( h.m_bins[i][j][k] ) );
  setRms( h.rmsX(), h.rmsY(), h.rmsZ() );

  /// Copy the annotation items
  AIDA::IAnnotation& annotation = annotationNoUpdate();
  std::set< std::string > existingAnnotationItems;
  for ( int item = 0; item < annotation.size(); ++item ) {
      existingAnnotationItems.insert( annotation.key( item ) );
  }
  const AIDA::IAnnotation& otherAnnotation = h.annotation();
  for ( int itemNew = 0; itemNew < otherAnnotation.size(); ++itemNew ) {
      const std::string& key = otherAnnotation.key( itemNew );
      if ( existingAnnotationItems.find( key ) == existingAnnotationItems.end() ) {
	  annotation.addItem( key, otherAnnotation.value( itemNew ), false );
      }
  }

}


iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::AIDA_Histogram3D( const AIDA::IHistogram3D& h ):
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram( h.title(), "IHistogram3D", 3 ),
  m_axisX( 0 ),
  m_axisY( 0 ),
  m_axisZ( 0 ),
  m_bins( h.xAxis().bins() + numberOfExtraBins, std::vector< std::vector< GravityBin3D* > >( h.yAxis().bins() + numberOfExtraBins, std::vector< GravityBin3D* >( h.zAxis().bins() + numberOfExtraBins, 0 ) ) ),
  m_validStatistics( false )
{
  m_sumWeightTimesSquaredX = 0;
  m_sumWeightTimesSquaredY = 0;
  m_sumWeightTimesSquaredZ = 0;

  // Construct the axes first
  const AIDA::IAxis& otherAxisX = h.xAxis();
  if ( otherAxisX.isFixedBinning() ) {
    m_axisX = new EvenBinAxis( otherAxisX.bins(),
			       otherAxisX.lowerEdge(),
			       otherAxisX.upperEdge() );
  }
  else {
    std::vector< double > edges( otherAxisX.bins() + 1 );
    for ( int i = 0; i < otherAxisX.bins(); ++i ) edges[i] = otherAxisX.binLowerEdge( i );
    edges.back() = otherAxisX.upperEdge();
    m_axisX = new VariableBinAxis( edges );
  };
  const AIDA::IAxis& otherAxisY = h.yAxis();
  if ( otherAxisY.isFixedBinning() ) {
    m_axisY = new EvenBinAxis( otherAxisY.bins(),
			       otherAxisY.lowerEdge(),
			       otherAxisY.upperEdge() );
  }
  else {
    std::vector< double > edges( otherAxisY.bins() + 1 );
    for ( int i = 0; i < otherAxisY.bins(); ++i ) edges[i] = otherAxisY.binLowerEdge( i );
    edges.back() = otherAxisY.upperEdge();
    m_axisY = new VariableBinAxis( edges );
  };
  const AIDA::IAxis& otherAxisZ = h.zAxis();
  if ( otherAxisZ.isFixedBinning() ) {
    m_axisZ = new EvenBinAxis( otherAxisZ.bins(),
			       otherAxisZ.lowerEdge(),
			       otherAxisZ.upperEdge() );
  }
  else {
    std::vector< double > edges( otherAxisZ.bins() + 1 );
    for ( int i = 0; i < otherAxisZ.bins(); ++i ) edges[i] = otherAxisZ.binLowerEdge( i );
    edges.back() = otherAxisZ.upperEdge();
    m_axisZ = new VariableBinAxis( edges );
  }

  // Copy the bin contents
  for ( unsigned int iBin = 0; iBin < m_bins.size(); ++iBin ) {
    for ( unsigned int jBin = 0; jBin < m_bins[0].size(); ++jBin ) {
      for ( unsigned int kBin = 0; kBin < m_bins[0][0].size(); ++kBin ) {
	m_bins[iBin][jBin][kBin] = new GravityBin3D;
	m_bins[iBin][jBin][kBin]->set( h.binEntries( iBin - numberOfExtraBins, jBin - numberOfExtraBins, kBin - numberOfExtraBins ),
				       h.binHeight( iBin - numberOfExtraBins, jBin - numberOfExtraBins, kBin - numberOfExtraBins ),
				       h.binError( iBin - numberOfExtraBins, jBin - numberOfExtraBins, kBin - numberOfExtraBins ),
				       h.binMeanX( iBin - numberOfExtraBins, jBin - numberOfExtraBins, kBin - numberOfExtraBins ),
				       h.binMeanY( iBin - numberOfExtraBins, jBin - numberOfExtraBins, kBin - numberOfExtraBins ),
				       h.binMeanZ( iBin - numberOfExtraBins, jBin - numberOfExtraBins, kBin - numberOfExtraBins ) );
      }
    }
  }

  setRms( h.rmsX(), h.rmsY(), h.rmsZ() );

  /// Copy the annotation items
  AIDA::IAnnotation& annotation = annotationNoUpdate();
  std::set< std::string > existingAnnotationItems;
  for ( int item = 0; item < annotation.size(); ++item ) {
      existingAnnotationItems.insert( annotation.key( item ) );
  }
  const AIDA::IAnnotation& otherAnnotation = h.annotation();
  for ( int itemNew = 0; itemNew < otherAnnotation.size(); ++itemNew ) {
      const std::string& key = otherAnnotation.key( itemNew );
      if ( existingAnnotationItems.find( key ) == existingAnnotationItems.end() ) {
	  annotation.addItem( key, otherAnnotation.value( itemNew ), false );
      }
  }

}


iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::~AIDA_Histogram3D()
{
  if ( m_axisX ) delete m_axisX;
  if ( m_axisY ) delete m_axisY;
  if ( m_axisZ ) delete m_axisZ;
  for ( unsigned int i = 0; i < m_bins.size(); ++i ) {
    for ( unsigned int j = 0; j < m_bins[0].size(); ++j )
      for ( unsigned int k = 0; k < m_bins[0][0].size(); ++k )
	if ( m_bins[i][j][k] ) delete m_bins[i][j][k];
  }
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::reset()
{
  for ( unsigned int i = 0; i < m_bins.size(); ++i )
    for ( unsigned int j = 0; j < m_bins[0].size(); ++j )
      for ( unsigned int k = 0; k < m_bins[0][0].size(); ++k )
       m_bins[i][j][k]->reset();
  m_sumWeightTimesSquaredX = m_sumWeightTimesSquaredY = m_sumWeightTimesSquaredZ = 0;
  setUpToDate( false );  
  m_validStatistics = false;
  return true;
}


int
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::entries() const
{
  calculateStatistics();
  return m_entries;
}


int
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::allEntries() const
{
  return ( entries() + extraEntries() );
}


int
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::extraEntries() const
{
  calculateStatistics();
  return m_extraEntries;
}


void
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::updateAnnotation() const
{
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::updateAnnotation();
  const AIDA::IAnnotation& anno = annotationNoUpdate();
  AIDA::IAnnotation& annotation = const_cast< AIDA::IAnnotation& >( anno );

  annotation.setValue( meanXKey, iAIDA_annotationNumberFormater.formatDouble( meanX() ) );
  annotation.setValue( rmsXKey, iAIDA_annotationNumberFormater.formatDouble( rmsX() ) );
  annotation.setValue( meanYKey, iAIDA_annotationNumberFormater.formatDouble( meanY() ) );
  annotation.setValue( rmsYKey, iAIDA_annotationNumberFormater.formatDouble( rmsY() ) );
  annotation.setValue( meanZKey, iAIDA_annotationNumberFormater.formatDouble( meanZ() ) );
  annotation.setValue( rmsZKey, iAIDA_annotationNumberFormater.formatDouble( rmsZ() ) );
  annotation.setValue( extra_entriesKey, iAIDA_annotationNumberFormater.formatInteger( extraEntries() ) );
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::equivalentBinEntries() const
{
  calculateStatistics();
  return m_ebe;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::sumBinHeights() const
{
  calculateStatistics();
  return m_sumBinHeights;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::sumExtraBinHeights() const
{
  calculateStatistics();
  return m_sumExtraBinHeights;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::sumAllBinHeights() const
{
  return ( sumBinHeights() + sumExtraBinHeights() );
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::minBinHeight() const
{
  calculateStatistics();
  return m_minHeight;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::maxBinHeight() const
{
  calculateStatistics();
  return m_maxHeight;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::scale( double scaleFactor )
{
  for ( unsigned int i = 0; i < m_bins.size(); ++i )
    for ( unsigned int j = 0; j < m_bins[0].size(); ++j )
      for ( unsigned int k = 0; k < m_bins[0][0].size(); ++k )
      m_bins[i][j][k]->scale( scaleFactor );  
  m_sumWeightTimesSquaredX *= scaleFactor;
  m_sumWeightTimesSquaredY *= scaleFactor;
  m_sumWeightTimesSquaredZ *= scaleFactor;
  setUpToDate( false );
  m_validStatistics = false;
  return true;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::fill( double x, double y, double z, double weight )
{
  // ignore Nan's filled in histograms 
  if (isNaN(x) || isNaN(y) || isNaN(z) || isNaN(weight) ) return false;  
  const int binIndexX = m_axisX->coordToIndex( x );
  const int binIndexY = m_axisY->coordToIndex( y );
  const int binIndexZ = m_axisZ->coordToIndex( z );
  m_bins[binIndexX + numberOfExtraBins][binIndexY + numberOfExtraBins][binIndexZ + numberOfExtraBins]->fill( weight, x, y, z );
  if ( binIndexX >= 0 && binIndexY >= 0 && binIndexZ >= 0 ) {
    m_sumWeightTimesSquaredX += weight * x * x;
    m_sumWeightTimesSquaredY += weight * y * y;
    m_sumWeightTimesSquaredZ += weight * z * z;
  }
  setUpToDate( false );  
  m_validStatistics = false;
  return true;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::binMeanX( int indexX,  int indexY,  int indexZ  ) const
{
  unsigned int realIndexX = indexX + numberOfExtraBins;
  if (  realIndexX>= m_bins.size() ) return 0;
  unsigned int realIndexY = indexY + numberOfExtraBins;
  if (  realIndexY>= m_bins[0].size() ) return 0;
  unsigned int realIndexZ = indexZ + numberOfExtraBins;
  if (  realIndexZ>= m_bins[0][0].size() ) return 0;

  const GravityBin3D& bin = *( m_bins[ realIndexX ][ realIndexY ][ realIndexZ ] );
  if ( bin.entries() == 0 ) {
    if (indexX == AIDA::IAxis::UNDERFLOW_BIN)
      return m_axisX->lowerEdge(  ) - 0.5 * m_axisX->binWidth( 0 );
    else if (indexX == AIDA::IAxis::OVERFLOW_BIN)
      return m_axisX->upperEdge(  ) + 0.5 *  m_axisX->binWidth( m_axisX->bins() - 1 );
    else return m_axisX->binLowerEdge( indexX ) + 0.5 * m_axisX->binWidth( indexX );
  }
  return bin.centreOfGravityX();
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::binMeanY( int indexX,  int indexY,  int indexZ  ) const
{
  unsigned int realIndexX = indexX + numberOfExtraBins;
  if (  realIndexX>= m_bins.size() ) return 0;
  unsigned int realIndexY = indexY + numberOfExtraBins;
  if (  realIndexY>= m_bins[0].size() ) return 0;
  unsigned int realIndexZ = indexZ + numberOfExtraBins;
  if (  realIndexZ>= m_bins[0][0].size() ) return 0;

  const GravityBin3D& bin = *( m_bins[ realIndexX ][ realIndexY ][ realIndexZ ] );

  if ( bin.entries() == 0 ) {
    if (indexY == AIDA::IAxis::UNDERFLOW_BIN)
      return m_axisY->lowerEdge(  ) - 0.5 * m_axisY->binWidth( 0 );
    else if (indexY == AIDA::IAxis::OVERFLOW_BIN)
      return m_axisY->upperEdge(  ) + 0.5 *  m_axisY->binWidth( m_axisY->bins() - 1 );
    else return m_axisY->binLowerEdge( indexY ) + 0.5 * m_axisY->binWidth( indexY );
  }
  return bin.centreOfGravityY();
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::binMeanZ( int indexX,  int indexY,  int indexZ  ) const
{
  unsigned int realIndexX = indexX + numberOfExtraBins;
  if (  realIndexX>= m_bins.size() ) return 0;
  unsigned int realIndexY = indexY + numberOfExtraBins;
  if (  realIndexY>= m_bins[0].size() ) return 0;
  unsigned int realIndexZ = indexZ + numberOfExtraBins;
  if (  realIndexZ>= m_bins[0][0].size() ) return 0;

  const GravityBin3D& bin = *( m_bins[ realIndexX ][ realIndexY ][ realIndexZ ] );
  if ( bin.entries() == 0 ) {
    if (indexZ == AIDA::IAxis::UNDERFLOW_BIN)
      return m_axisZ->lowerEdge(  ) - 0.5 * m_axisZ->binWidth( 0 );
    else if (indexZ == AIDA::IAxis::OVERFLOW_BIN)
      return m_axisZ->upperEdge(  ) + 0.5 *  m_axisZ->binWidth( m_axisZ->bins() - 1 );
    else return m_axisZ->binLowerEdge( indexZ ) + 0.5 * m_axisZ->binWidth( indexZ );
  }
  return bin.centreOfGravityZ();
}


int
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::binEntries( int indexX, int indexY, int indexZ ) const
{
  unsigned int realIndexX = indexX + numberOfExtraBins;
  if (  realIndexX>= m_bins.size() ) return 0;
  unsigned int realIndexY = indexY + numberOfExtraBins;
  if (  realIndexY>= m_bins[0].size() ) return 0;
  unsigned int realIndexZ = indexZ + numberOfExtraBins;
  if (  realIndexZ>= m_bins[0][0].size() ) return 0;
  return m_bins[realIndexX][realIndexY][realIndexZ]->entries();
}


int
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::binEntriesX( int index ) const
{
  unsigned int realIndex = index + numberOfExtraBins;
  if ( realIndex>= m_bins.size() ) return 0;
  int e = 0;
  for ( unsigned int j = 0; j < m_bins[0].size(); ++j ) {
    for ( unsigned int k = 0; k < m_bins[0][0].size(); ++k ) {
      const GravityBin3D& bin = *( m_bins[ realIndex ][j][k] );
      e += bin.entries();
    }
  }
  return e;
}


int
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::binEntriesY( int index ) const
{
  unsigned int realIndex = index + numberOfExtraBins;
  if ( realIndex>= m_bins[0].size() ) return 0;
  int e = 0;
  for ( unsigned int j = 0; j < m_bins.size(); ++j ) {
    for ( unsigned int k = 0; k < m_bins[0][0].size(); ++k ) {
      const GravityBin3D& bin = *( m_bins[j][ realIndex ][k] );
      e += bin.entries();
    }
  }
  return e;
}


int
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::binEntriesZ( int index ) const
{
  unsigned int realIndex = index + numberOfExtraBins;
  if ( realIndex>= m_bins[0][0].size() ) return 0;
  int e = 0;
  for ( unsigned int j = 0; j < m_bins.size(); ++j ) {
    for ( unsigned int k = 0; k < m_bins[0].size(); ++k ) {
      const GravityBin3D& bin = *( m_bins[j][k][realIndex] );
      e += bin.entries();
    }
  }
  return e;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::binHeight( int indexX, int indexY, int indexZ ) const
{
  unsigned int realIndexX = indexX + numberOfExtraBins;
  if (  realIndexX>= m_bins.size() ) return 0;
  unsigned int realIndexY = indexY + numberOfExtraBins;
  if (  realIndexY>= m_bins[0].size() ) return 0;
  unsigned int realIndexZ = indexZ + numberOfExtraBins;
  if (  realIndexZ>= m_bins[0][0].size() ) return 0;
  return m_bins[realIndexX][realIndexY][realIndexZ]->height();
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::binHeightX( int index ) const
{
  unsigned int realIndex = index + numberOfExtraBins;
  if ( realIndex>= m_bins.size() ) return 0;
  double e = 0;
  for ( unsigned int j = 0; j < m_bins[0].size(); ++j ) {
    for ( unsigned int k = 0; k < m_bins[0][0].size(); ++k ) {
      const GravityBin3D& bin = *( m_bins[ realIndex ][j][k] );
      e += bin.height();
    }
  }
  return e;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::binHeightY( int index ) const
{
  unsigned int realIndex = index + numberOfExtraBins;
  if ( realIndex>= m_bins[0].size() ) return 0;
  double e = 0;
  for ( unsigned int j = 0; j < m_bins.size(); ++j ) {
    for ( unsigned int k = 0; k < m_bins[0][0].size(); ++k ) {
      const GravityBin3D& bin = *( m_bins[j][ realIndex ][k] );
      e += bin.height();
    }
  }
  return e;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::binHeightZ( int index ) const
{
  unsigned int realIndex = index + numberOfExtraBins;
  if ( realIndex>= m_bins[0][0].size() ) return 0;
  double e = 0;
  for ( unsigned int j = 0; j < m_bins.size(); ++j ) {
    for ( unsigned int k = 0; k < m_bins[0].size(); ++k ) {
      const GravityBin3D& bin = *( m_bins[j][k][ realIndex ] );
      e += bin.height();
    }
  }
  return e;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::binError( int indexX, int indexY, int indexZ ) const
{
  unsigned int realIndexX = indexX + numberOfExtraBins;
  if (  realIndexX>= m_bins.size() ) return 0;
  unsigned int realIndexY = indexY + numberOfExtraBins;
  if (  realIndexY>= m_bins[0].size() ) return 0;
  unsigned int realIndexZ = indexZ + numberOfExtraBins;
  if (  realIndexZ>= m_bins[0][0].size() ) return 0;
  return m_bins[realIndexX][realIndexY][realIndexZ]->error();
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::meanX() const
{
  calculateStatistics();
  return m_meanX;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::rmsX() const
{
  calculateStatistics();
  return m_rmsX;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::meanY() const
{
  calculateStatistics();
  return m_meanY;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::rmsY() const
{
  calculateStatistics();
  return m_rmsY;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::meanZ() const
{
  calculateStatistics();
  return m_meanZ;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::rmsZ() const
{
  calculateStatistics();
  return m_rmsZ;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::calculateStatistics() const
{
  if ( m_validStatistics ) return true;

  // For the in-range bins:
  m_sumBinHeights = 0;
  double sww = 0;
  double swx = 0;
  double swy = 0;
  double swz = 0;
  m_entries = 0;
  m_sumExtraBinHeights = 0;
  m_extraEntries = 0;
  m_maxHeight = m_minHeight = m_bins[numberOfExtraBins][numberOfExtraBins][numberOfExtraBins]->height();
  for ( unsigned int i = 0; i < m_bins.size(); ++i ) {
    for ( unsigned int j = 0; j < m_bins[0].size(); ++j ) {
      for ( unsigned int k = 0; k < m_bins[0][0].size(); ++k ) {
	const GravityBin3D& bin = *( m_bins[i][j][k] );
	if ( i < numberOfExtraBins || j < numberOfExtraBins || k < numberOfExtraBins ) {
	  m_extraEntries += bin.entries();
	  m_sumExtraBinHeights += bin.height();
	}
	else {
	  const double binMeanX = bin.centreOfGravityX();
	  const double binMeanY = bin.centreOfGravityY();
	  const double binMeanZ = bin.centreOfGravityZ();
	  const double binHeight = bin.height();
	  const double binErrorSquared = bin.errorSquared();
	  m_entries += bin.entries();
	  m_sumBinHeights += binHeight;
	  sww += binErrorSquared;
	  swx += binMeanX * binHeight;
	  swy += binMeanY * binHeight;
	  swz += binMeanZ * binHeight;
	  if ( m_maxHeight < binHeight ) m_maxHeight = binHeight;
	  if ( m_minHeight > binHeight ) m_minHeight = binHeight;
	}
      }
    }
  }

  if ( m_sumBinHeights == 0 ) {
    m_meanX = m_rmsX = m_meanY = m_rmsY = m_meanZ = m_rmsZ = 0;
  }
  else {
    m_meanX = swx / m_sumBinHeights;
    m_rmsX = std::sqrt( std::abs( m_sumWeightTimesSquaredX / m_sumBinHeights - m_meanX * m_meanX ) );
    m_meanY = swy / m_sumBinHeights;
    m_rmsY = std::sqrt( std::abs( m_sumWeightTimesSquaredY / m_sumBinHeights - m_meanY * m_meanY ) );
    m_meanZ = swz / m_sumBinHeights;
    m_rmsZ = std::sqrt( std::abs( m_sumWeightTimesSquaredZ / m_sumBinHeights - m_meanZ * m_meanZ ) );
  }

  if ( sww ) {
    m_ebe = m_sumBinHeights * m_sumBinHeights / sww;
  }
  else {
    m_ebe = 0;
  }

  // Done !
  m_validStatistics = true;
  return m_validStatistics;
}



const AIDA::IAxis&
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::xAxis() const
{
  return *m_axisX;
}

const AIDA::IAxis&
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::yAxis() const
{
  return *m_axisY;
}


const AIDA::IAxis&
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::zAxis() const
{
  return *m_axisZ;
}


int
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::coordToIndexX( double coord ) const
{
  return m_axisX->coordToIndex( coord );
}


int
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::coordToIndexY( double coord ) const
{
  return m_axisY->coordToIndex( coord );
}


int
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::coordToIndexZ( double coord ) const
{
  return m_axisZ->coordToIndex( coord );
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::add( const AIDA::IHistogram3D & h )
{
  // First check that the histograms are compatible
  const AIDA::IAxis& otherAxisX = h.xAxis();
  const AIDA::IAxis& myAxisX = *m_axisX;
  if ( otherAxisX.bins() != myAxisX.bins() ) return false;
  for ( int iBin = 0; iBin < myAxisX.bins(); ++iBin ) {
    const double otherLowEdge = otherAxisX.binLowerEdge( iBin );
    const double myLowEdge = myAxisX.binLowerEdge( iBin );
    if ( std::abs(otherLowEdge - myLowEdge)/myLowEdge > 1.e-14 ) { return false; }
  }
  if ( otherAxisX.upperEdge() != myAxisX.upperEdge() ) return false;

  const AIDA::IAxis& otherAxisY = h.yAxis();
  const AIDA::IAxis& myAxisY = *m_axisY;
  if ( otherAxisY.bins() != myAxisY.bins() ) return false;
  for ( int iBin = 0; iBin < myAxisY.bins(); ++iBin ) {
    const double otherLowEdge = otherAxisY.binLowerEdge( iBin );
    const double myLowEdge = myAxisY.binLowerEdge( iBin );
    if ( std::abs(otherLowEdge - myLowEdge)/myLowEdge > 1.e-14 ) { return false; }
  }
  if ( otherAxisY.upperEdge() != myAxisY.upperEdge() ) return false;

  const AIDA::IAxis& otherAxisZ = h.zAxis();
  const AIDA::IAxis& myAxisZ = *m_axisZ;
  if ( otherAxisZ.bins() != myAxisZ.bins() ) return false;
  for ( int iBin = 0; iBin < myAxisZ.bins(); ++iBin ) {
    const double otherLowEdge = otherAxisZ.binLowerEdge( iBin );
    const double myLowEdge = myAxisZ.binLowerEdge( iBin );
    if ( std::abs(otherLowEdge - myLowEdge)/myLowEdge > 1.e-14 ) { return false; }
  }
  if ( otherAxisZ.upperEdge() != myAxisZ.upperEdge() ) return false;



  // OK, the histograms are compatible.
  // Let's check if the histograms are the PI ones, to make our life a bit easier.
  try {
    const iAIDA::AIDA_Histogram_native::AIDA_Histogram3D& other = dynamic_cast<const iAIDA::AIDA_Histogram_native::AIDA_Histogram3D&>( h );
    return increment( other );
  }
  catch( std::bad_cast ) {
    iAIDA::AIDA_Histogram_native::AIDA_Histogram3D * transformed = new iAIDA::AIDA_Histogram_native::AIDA_Histogram3D( h );
    bool ret = increment( *transformed );
    delete transformed;
    return ret;
  }
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::increment( const AIDA_Histogram3D& h )
{
  for ( unsigned int i = 0; i < m_bins.size(); ++i ) {
    for ( unsigned int j = 0; j < m_bins[0].size(); ++j ) {
      for ( unsigned int k = 0; k < m_bins[0][0].size(); ++k ) {
	m_bins[i][j][k]->increment( *( h.m_bins[i][j][k] ) );
      }
    }
  }
  m_sumWeightTimesSquaredX += h.m_sumWeightTimesSquaredX;
  m_sumWeightTimesSquaredY += h.m_sumWeightTimesSquaredY;
  m_sumWeightTimesSquaredZ += h.m_sumWeightTimesSquaredZ;
  setUpToDate( false );  
  m_validStatistics = false;
  return true;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::setBinContents( int binIndexX,
								 int binIndexY,
								 int binIndexZ,
								 int entries,
								 double height,
								 double error,
								 double centreX,
								 double centreY,
								 double centreZ )
{
  unsigned int realIndexX = binIndexX + numberOfExtraBins;
  if (  realIndexX >= m_bins.size() ) return false;
  unsigned int realIndexY = binIndexY + numberOfExtraBins;
  if (  realIndexY >= m_bins[0].size() ) return false;
  unsigned int realIndexZ = binIndexZ + numberOfExtraBins;
  if (  realIndexZ >= m_bins[0][0].size() ) return false;
  // set also sumwx2 in case setRms will not be called
  if (binIndexX >= 0) m_sumWeightTimesSquaredX += height*centreX*centreX;
  if (binIndexY >= 0) m_sumWeightTimesSquaredY += height*centreY*centreY;
  if (binIndexZ >= 0) m_sumWeightTimesSquaredZ += height*centreZ*centreZ;
  setUpToDate( false );  
  m_validStatistics = false;
  return m_bins[realIndexX][realIndexY][realIndexZ]->set( entries, height, error, centreX, centreY, centreZ );
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::setRms( double rmsX, double rmsY, double rmsZ )
{
  const double sw = sumBinHeights();
  const double mnX = meanX();
  m_sumWeightTimesSquaredX = ( mnX*mnX + rmsX*rmsX) * sw;
  const double mnY = meanY();
  m_sumWeightTimesSquaredY = ( mnY*mnY + rmsY*rmsY) * sw;
  const double mnZ = meanZ();
  m_sumWeightTimesSquaredZ = ( mnZ*mnZ + rmsZ*rmsZ) * sw;
  m_validStatistics = false;
  return true;
}

// new from AIDA 3.2.1 
void *
iAIDA::AIDA_Histogram_native::AIDA_Histogram3D::cast(const std::string & className) const
{ 
  if (className.find(userLevelClassType()) != std::string::npos ) 
    return  dynamic_cast<AIDA::IHistogram3D *>( const_cast<iAIDA::AIDA_Histogram_native::AIDA_Histogram3D *> (this) );
  else 
    return 0;  
}
