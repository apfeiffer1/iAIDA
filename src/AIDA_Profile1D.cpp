//
//    Copyright 2007 Andreas Pfeiffer
//
/*
 *  This file is part of iAIDA.
 *
 *  iAIDA is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as 
 *  published by the Free Software Foundation, either version 3 of 
 *  the License.
 *
 *  iAIDA is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public 
 *  License along with iAIDA.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "AIDA_Profile1D.h"
#include "EvenBinAxis.h"
#include "VariableBinAxis.h"
#include "ProfileBin1D.h"
#include "AIDA/IAnnotation.h"
#include <typeinfo>
#include <cmath>
#include <set>
#include "AnnotationNumberFormater.h"

static const unsigned int numberOfExtraBins = 2;
static const std::string meanKey = "Mean";
static const std::string rmsKey = "Rms";
static const std::string extra_entriesKey = "Extra Entries";
static const std::string overflowKey = "Overflow";
static const std::string underflowKey = "Underflow";
static const std::string emptyString = "";

iAIDA::AIDA_Histogram_native::AIDA_Profile1D::AIDA_Profile1D( const std::string& title,
							       int numberOfBins,
							       double lowEdge,
							       double highEdge ):
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram( title, "IProfile1D", 1 ),
  m_axis( new EvenBinAxis( std::abs( numberOfBins ), lowEdge, highEdge ) ),
  m_bins( std::abs( numberOfBins ) + numberOfExtraBins ),
  m_validStatistics( false )
{
  for ( unsigned int i = 0; i < m_bins.size(); ++i ) m_bins[ i ] = new iAIDA::AIDA_Histogram_native::ProfileBin1D;

  /// Define the items in the annotation.
  AIDA::IAnnotation& annotation = annotationNoUpdate();
  annotation.addItem( meanKey, emptyString, false );
  annotation.addItem( rmsKey, emptyString, false );
  annotation.addItem( extra_entriesKey, emptyString, false );
  annotation.addItem( overflowKey, emptyString, false );
  annotation.addItem( underflowKey, emptyString, false );
}


iAIDA::AIDA_Histogram_native::AIDA_Profile1D::AIDA_Profile1D( const std::string& title,
							       const std::vector< double >& edges ):
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram( title, "IProfile1D", 1 ),
  m_axis( new VariableBinAxis( edges ) ),
  m_bins( edges.size() - 1 + numberOfExtraBins ),
  m_validStatistics( false )
{
  for ( unsigned int i = 0; i < m_bins.size(); ++i ) m_bins[ i ] = new iAIDA::AIDA_Histogram_native::ProfileBin1D;

  /// Define the items in the annotation.
  AIDA::IAnnotation& annotation = annotationNoUpdate();
  annotation.addItem( meanKey, emptyString, false );
  annotation.addItem( rmsKey, emptyString, false );
  annotation.addItem( extra_entriesKey, emptyString, false );
  annotation.addItem( overflowKey, emptyString, false );
  annotation.addItem( underflowKey, emptyString, false );
}


iAIDA::AIDA_Histogram_native::AIDA_Profile1D::AIDA_Profile1D( const iAIDA::AIDA_Histogram_native::AIDA_Profile1D& h ):
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram( h.title(), "IProfile1D", 1 ),
  m_axis( 0 ),
  m_bins( h.m_bins.size() ),
  m_validStatistics( false )
{
  // Construct the axis first
  const AIDA::IAxis& otherAxis = h.axis();
  if ( otherAxis.isFixedBinning() ) {
    m_axis = new EvenBinAxis( otherAxis.bins(),
			      otherAxis.lowerEdge(),
			      otherAxis.upperEdge() );
  }
  else {
    std::vector< double > edges( otherAxis.bins() + 1);
    for ( int i = 0; i < otherAxis.bins(); ++i ) edges[i] = otherAxis.binLowerEdge( i );
    edges.back() = otherAxis.upperEdge();
    m_axis = new VariableBinAxis( edges );
  }

  // Copy the bin contents
  for ( unsigned int i = 0; i < m_bins.size(); ++i ) m_bins[i] = new ProfileBin1D( *( h.m_bins[i] ) );

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


iAIDA::AIDA_Histogram_native::AIDA_Profile1D::AIDA_Profile1D( const AIDA::IProfile1D& h ):
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram( h.title(), "IProfile1D", 1 ),
  m_axis( 0 ),
  m_bins( h.axis().bins() + numberOfExtraBins ),
  m_validStatistics( false )
{
  // Construct the axis first
  const AIDA::IAxis& otherAxis = h.axis();
  if ( h.axis().isFixedBinning() ) {
    m_axis = new EvenBinAxis( otherAxis.bins(),
			      otherAxis.lowerEdge(),
			      otherAxis.upperEdge() );
  }
  else {
    std::vector< double > edges( otherAxis.bins() + 1);
    for ( int i = 0; i < otherAxis.bins(); ++i ) edges[i] = otherAxis.binLowerEdge( i );
    edges.back() = otherAxis.upperEdge();
    m_axis = new VariableBinAxis( edges );
  }

  // Create the bins
  for ( unsigned int i = 0; i < m_bins.size(); ++i ) m_bins[i] = new ProfileBin1D;

  // Copy the in-range bin contents
  for ( unsigned int iBin = numberOfExtraBins; iBin < m_bins.size(); ++iBin ) {
    m_bins[iBin] = new ProfileBin1D;
    m_bins[iBin]->set( h.binEntries( iBin - numberOfExtraBins ),
		       h.binHeight( iBin - numberOfExtraBins ),
		       h.binError( iBin - numberOfExtraBins ),
		       h.binRms( iBin - numberOfExtraBins ),
		       h.binMean( iBin - numberOfExtraBins ) );
  }

  // Underflow bin:
  // LM : AIDA_UNDERFLOW is -2 
  //      need to add numberOfExtrabins when indexing m_bins  
  const int underFlowBin = static_cast< int >( AIDA::IAxis::UNDERFLOW_BIN );
  m_bins[ numberOfExtraBins + underFlowBin ]->set( h.binEntries( underFlowBin ),
			       h.binHeight( underFlowBin ),
			       h.binError( underFlowBin ),
			       h.binRms( underFlowBin ),
			       h.binMean( underFlowBin ) );

  // Overflow bin:
  const int overFlowBin = static_cast< int >( AIDA::IAxis::OVERFLOW_BIN );
  m_bins[ numberOfExtraBins + overFlowBin ]->set( h.binEntries( overFlowBin ),
			       h.binHeight( overFlowBin ),
			       h.binError( overFlowBin ),
			       h.binRms( overFlowBin ),
			       h.binMean( overFlowBin ) );

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


iAIDA::AIDA_Histogram_native::AIDA_Profile1D::~AIDA_Profile1D()
{
  if ( m_axis ) delete m_axis;
  for ( unsigned int i = 0; i < m_bins.size(); ++i ) {
    if ( m_bins[ i ] ) delete m_bins[ i ];
  }
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::reset()
{
  for ( unsigned int i = 0; i < m_bins.size(); ++i ) m_bins[ i ]->reset();
  m_validStatistics = false;
  setUpToDate( false );
  return true;
}


int
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::entries() const
{
  calculateStatistics();
  return m_entries;
}


int
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::allEntries() const
{
  return ( entries() + extraEntries() );
}


int
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::extraEntries() const
{
  calculateStatistics();
  return m_extraEntries;
}


void
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::updateAnnotation() const
{
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::updateAnnotation();
  const AIDA::IAnnotation& anno = annotationNoUpdate();
  AIDA::IAnnotation& annotation = const_cast< AIDA::IAnnotation& >( anno );

  annotation.setValue( meanKey, iAIDA_annotationNumberFormater.formatDouble( mean() ) );
  annotation.setValue( rmsKey, iAIDA_annotationNumberFormater.formatDouble( rms() ) );
  annotation.setValue( extra_entriesKey, iAIDA_annotationNumberFormater.formatInteger( extraEntries() ) );
  annotation.setValue( underflowKey, iAIDA_annotationNumberFormater.formatInteger( m_bins[numberOfExtraBins + AIDA::IAxis::UNDERFLOW_BIN]->entries() ) );
  annotation.setValue( overflowKey, iAIDA_annotationNumberFormater.formatInteger( m_bins[numberOfExtraBins + AIDA::IAxis::OVERFLOW_BIN]->entries() ) );
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::sumBinHeights() const
{
  calculateStatistics();
  return m_sumBinHeights;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::sumExtraBinHeights() const
{
  calculateStatistics();
  return m_sumExtraBinHeights;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::sumAllBinHeights() const
{
  return ( sumBinHeights() + sumExtraBinHeights() );
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::minBinHeight() const
{
  calculateStatistics();
  return m_minHeight;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::maxBinHeight() const
{
  calculateStatistics();
  return m_maxHeight;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::fill( double x, double y, double weight )
{
  // ignore Nan's filled in histograms 
  if (isNaN(x) || isNaN(y) || isNaN(weight) ) return false;  
  const int binIndex = m_axis->coordToIndex( x );
  m_bins[binIndex + numberOfExtraBins]->fill( weight, x, y );
  setUpToDate( false );  
  m_validStatistics = false;
  return true;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::binMean( int index ) const
{
  unsigned int realIndex = index + numberOfExtraBins;
  if (  realIndex>= m_bins.size() ) return 0;
  const iAIDA::AIDA_Histogram_native::ProfileBin1D& bin = * ( m_bins[ realIndex ] );
  if ( bin.entries() == 0 ) {
    if (index == AIDA::IAxis::UNDERFLOW_BIN)
      return m_axis->lowerEdge(  ) - 0.5 * m_axis->binWidth( 0 );
    else if (index == AIDA::IAxis::OVERFLOW_BIN)
      return m_axis->upperEdge(  ) + 0.5 *  m_axis->binWidth( m_axis->bins() - 1 );
    else return m_axis->binLowerEdge( index ) + 0.5 * m_axis->binWidth( index );
  }
  else return bin.centreOfGravityX();
}


int
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::binEntries( int index ) const
{
  unsigned int realIndex = index + numberOfExtraBins;
  if (  realIndex>= m_bins.size() ) return 0;
  return m_bins[ realIndex ]->entries();
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::binHeight( int index ) const
{
  unsigned int realIndex = index + numberOfExtraBins;
  if (  realIndex>= m_bins.size() ) return 0;
  return m_bins[ realIndex ]->value();
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::binError( int index ) const
{
  unsigned int realIndex = index + numberOfExtraBins;
  if (  realIndex>= m_bins.size() ) return 0;
  return m_bins[ realIndex ]->error();
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::binRms( int index ) const
{
  unsigned int realIndex = index + numberOfExtraBins;
  if (  realIndex>= m_bins.size() ) return 0;
  return m_bins[ realIndex ]->spread();
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::mean() const
{
  calculateStatistics();
  return m_mean;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::rms() const
{
  calculateStatistics();
  return m_rms;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::calculateStatistics() const
{
  if ( m_validStatistics ) return true;

  // For the in-range bins:
  m_sumBinHeights = 0;
  double swp = 0;
  double swpp = 0;
  m_entries = 0;
  m_maxHeight = m_minHeight = m_bins[ numberOfExtraBins ]->value();
  // LM: need to use sum of bin weights for calculating statistics 
  m_sumBinWeights = 0;
  for ( unsigned int i = numberOfExtraBins; i < m_bins.size(); ++i ) {
    const ProfileBin1D& bin = *( m_bins[ i ] );
    const double binMean = bin.centreOfGravityX();
    const double binHeight = bin.value();
    const double binWeight = bin.sumOfWeights();
    m_entries += bin.entries();
    m_sumBinHeights += binHeight;
    m_sumBinWeights += binWeight;
    swp += binMean * binWeight;
    swpp += binMean * binMean * binWeight;
    if ( m_maxHeight < binHeight ) m_maxHeight = binHeight;
    if ( m_minHeight > binHeight ) m_minHeight = binHeight;
  }

  if ( m_sumBinWeights == 0 ) {
    m_mean = m_rms = 0;
  }
  else {
    m_mean = swp / m_sumBinWeights;
    m_rms = std::sqrt( std::abs( swpp / m_sumBinWeights - m_mean * m_mean ) );
  }


  // For the out-of-range bins:
  m_sumExtraBinHeights = 0;
  m_extraEntries = 0;
  for ( unsigned int i = 0; i < numberOfExtraBins; ++i ) {
    const ProfileBin1D& bin = *( m_bins[ i ] );
    m_extraEntries += bin.entries();
    m_sumExtraBinHeights += bin.value();
  }

  // Done !
  m_validStatistics = true;
  return m_validStatistics;
}


const AIDA::IAxis&
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::axis() const
{
  return *m_axis;
}


int
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::coordToIndex( double coord ) const
{
  return m_axis->coordToIndex( coord );
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::add( const AIDA::IProfile1D & h )
{
  // First check that the profiles are compatible
  const AIDA::IAxis& otherAxis = h.axis();
  const AIDA::IAxis& myAxis = *m_axis;
  if ( otherAxis.bins() != myAxis.bins() ) return false;
  for ( int iBin = 0; iBin < myAxis.bins(); ++iBin ) {
    const double otherLowEdge = otherAxis.binLowerEdge( iBin );
    const double myLowEdge = myAxis.binLowerEdge( iBin );
    if ( std::abs(otherLowEdge - myLowEdge)/myLowEdge > 1.e-14 ) { return false; }
  }
  if ( otherAxis.upperEdge() != myAxis.upperEdge() ) return false;

  // OK, the profiles are compatible.
  // Let's check if the profiles are the PI ones, to make our life a bit easier.
  try {
    const iAIDA::AIDA_Histogram_native::AIDA_Profile1D& other = dynamic_cast<const iAIDA::AIDA_Histogram_native::AIDA_Profile1D&>( h );
    return increment( other );
  }
  catch( std::bad_cast ) {
    iAIDA::AIDA_Histogram_native::AIDA_Profile1D * transformed = new iAIDA::AIDA_Histogram_native::AIDA_Profile1D( h );
    bool ret = increment( *transformed );
    delete transformed;
    return ret;
  }
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::increment( const AIDA_Profile1D& h )
{
  for ( unsigned int i = 0; i < m_bins.size(); ++i ) {
    m_bins[ i ]->increment( *( h.m_bins[i] ) );
  }
  setUpToDate( false );  
  m_validStatistics = false;
  return true;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::setBinContents( int binIndex,
							       int entries,
							       double height,
							       double error,
							       double spread,
							       double centre )
{
  unsigned int realIndex = binIndex + numberOfExtraBins;
  if (  realIndex>= m_bins.size() ) return false;
  setUpToDate( false );  
  m_validStatistics = false;
  return m_bins[ realIndex ]->set( entries, height, error, spread, centre );
}

// new from AIDA 3.2.1 
void *
iAIDA::AIDA_Histogram_native::AIDA_Profile1D::cast(const std::string & className) const
{ 
  if (className.find(userLevelClassType()) != std::string::npos ) 
    return  dynamic_cast<AIDA::IProfile1D *>( const_cast<iAIDA::AIDA_Histogram_native::AIDA_Profile1D *> (this) );
  else 
    return 0;  
}
