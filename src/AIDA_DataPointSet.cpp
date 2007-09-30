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

#include "AIDA_DataPointSet.h"
#include "AIDA_DataPoint.h"
#include "AIDA/IAnnotation.h"
#include "AIDA/IMeasurement.h"
#include <set>

static const std::string titleKey = "Title"; // no more defined in Aida (3.2)
static const std::string nameKey = "Name";
static const std::string sizeKey = "Size";
static const std::string emptyString = "";

static const std::string g_userLevelClassType = "IDataPointSet";

iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::AIDA_DataPointSet( const std::string& title,
									int dimension,
                                                                        AIDA::IAnnotation* annotation ):
  m_annotation( annotation ),
  m_dimension( ( ( dimension > 0 ) ? dimension : 0  ) ),
  m_upToDate( false ),
  m_dataPoints()
{
  m_annotation->addItem( titleKey, title, true );
  m_annotation->addItem( nameKey, emptyString, true );
  m_annotation->addItem( sizeKey, emptyString, false );
}


iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::AIDA_DataPointSet( const iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet& dps,
									AIDA::IAnnotation* annotation ):
  m_annotation( annotation ),
  m_dimension( dps.dimension() ),
  m_upToDate( false ),
  m_dataPoints()
{
  // Define the annotation items
  std::set< std::string > existingAnnotationItems;
  m_annotation->addItem( titleKey, dps.title(), true );
  existingAnnotationItems.insert( titleKey );
  m_annotation->addItem( nameKey, emptyString, true );
  existingAnnotationItems.insert( nameKey );
  m_annotation->addItem( sizeKey, emptyString, false );
  existingAnnotationItems.insert( sizeKey );

  // Copy other annotation items
  const AIDA::IAnnotation& otherAnnotation = dps.annotation();
  for ( int itemNew = 0; itemNew < otherAnnotation.size(); ++itemNew ) {
    const std::string& key = otherAnnotation.key( itemNew );
    if ( existingAnnotationItems.find( key ) == existingAnnotationItems.end() ) {
      m_annotation->addItem( key, otherAnnotation.value( itemNew ), false );
    }
  }

  // Copy over the data points
  for ( int i = 0; i < dps.size(); ++i ) {
    m_dataPoints.push_back( new iAIDA::AIDA_DataPointSet_native::AIDA_DataPoint( m_dimension ) );
    const AIDA::IDataPoint* originalPoint = dps.point( i );
    AIDA::IDataPoint* copyPoint = m_dataPoints.back();
    for ( int j = 0; j < m_dimension; ++j ) {
      copyPoint->coordinate( j )->setValue( originalPoint->coordinate( j )->value() );
      copyPoint->coordinate( j )->setErrorPlus( originalPoint->coordinate( j )->errorPlus() );
      copyPoint->coordinate( j )->setErrorMinus( originalPoint->coordinate( j )->errorMinus() );
    } 
  }
}


iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::AIDA_DataPointSet( const AIDA::IDataPointSet& dps,
									AIDA::IAnnotation* annotation ):
  m_annotation( annotation ),
  m_dimension( dps.dimension() ),
  m_upToDate( false ),
  m_dataPoints()
{
  // Define the annotation items
  std::set< std::string > existingAnnotationItems;
  m_annotation->addItem( titleKey, dps.title(), true );
  existingAnnotationItems.insert( titleKey );
  m_annotation->addItem( nameKey, emptyString, true );
  existingAnnotationItems.insert( nameKey );
  m_annotation->addItem( sizeKey, emptyString, false );
  existingAnnotationItems.insert( sizeKey );

  // Copy other annotation items
  const AIDA::IAnnotation& otherAnnotation = dps.annotation();
  for ( int itemNew = 0; itemNew < otherAnnotation.size(); ++itemNew ) {
    const std::string& key = otherAnnotation.key( itemNew );
    if ( existingAnnotationItems.find( key ) == existingAnnotationItems.end() ) {
      m_annotation->addItem( key, otherAnnotation.value( itemNew ), false );
    }
  }

  // Copy over the data points
  for ( int i = 0; i < dps.size(); ++i ) {
    m_dataPoints.push_back( new iAIDA::AIDA_DataPointSet_native::AIDA_DataPoint( m_dimension ) );
    const AIDA::IDataPoint* originalPoint = dps.point( i );
    AIDA::IDataPoint* copyPoint = m_dataPoints.back();
    for ( int j = 0; j < m_dimension; ++j ) {
      copyPoint->coordinate( j )->setValue( originalPoint->coordinate( j )->value() );
      copyPoint->coordinate( j )->setErrorPlus( originalPoint->coordinate( j )->errorPlus() );
      copyPoint->coordinate( j )->setErrorMinus( originalPoint->coordinate( j )->errorMinus() );
    } 
  }
}


iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::~AIDA_DataPointSet()
{
  delete m_annotation;
  for ( unsigned int i = 0; i < m_dataPoints.size(); ++i ) {
    delete m_dataPoints[i];
  }
}


std::string
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::name() const
{
  return m_annotation->value( nameKey );
}


bool
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::isUpToDate() const
{
  return m_upToDate;
}


void
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::setUpToDate( bool isUpToDate )
{
  m_upToDate = isUpToDate;
}


bool
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::setName( const std::string& newName )
{
  m_annotation->setValue( nameKey, newName );
  return true;
}


const std::string&
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::userLevelClassType() const
{
  return g_userLevelClassType;
}


AIDA::IAnnotation&
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::annotation()
{
  updateAnnotation();
  return *m_annotation;
}


const AIDA::IAnnotation&
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::annotation() const
{
  updateAnnotation();
  return *m_annotation;
}


std::string
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::title() const
{
  return m_annotation->value( titleKey );
}


bool
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::setTitle( const std::string & newTitle )
{
  m_annotation->setValue( titleKey, newTitle );
  return true;
}


int
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::dimension() const
{
  return m_dimension;
}


void
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::clear()
{
  for ( unsigned int i = 0; i < m_dataPoints.size(); ++i ) {
    delete m_dataPoints[i];
  }
  m_dataPoints.clear();
}


int
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::size() const
{
  return static_cast< int >( m_dataPoints.size() );
}


AIDA::IDataPoint*
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::point( int index )
{
  if ( index < 0 || index >= size() ) return 0;
  return m_dataPoints[ index ];
}


const AIDA::IDataPoint*
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::point( int index ) const
{
  if ( index < 0 || index >= size() ) return 0;
  return m_dataPoints[ index ];
}


AIDA::IDataPoint * 
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::addPoint()
{
  iAIDA::AIDA_DataPointSet_native::AIDA_DataPoint * p = new  iAIDA::AIDA_DataPointSet_native::AIDA_DataPoint( m_dimension );
  m_dataPoints.push_back( p );
  return p;
}


bool
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::addPoint(const AIDA::IDataPoint & other)
{
  if ( other.dimension() != m_dimension ) return false;
  m_dataPoints.push_back( new iAIDA::AIDA_DataPointSet_native::AIDA_DataPoint( other ) );
  return true;
}


bool
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::removePoint( int index )
{
  if ( index < 0 || index >= size() ) return false;
  delete m_dataPoints[ index ];
  for ( unsigned int i = index + 1; i < m_dataPoints.size(); ++i ) {
    m_dataPoints[ i-1 ] = m_dataPoints[i];
  }
  m_dataPoints.pop_back();
  return true;
}


double
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::lowerExtent( int coordinate ) const
{
  if ( m_dataPoints.size() == 0 ) return 0;
  double low = m_dataPoints[0]->coordinate( coordinate )->value() - m_dataPoints[0]->coordinate( coordinate )->errorMinus();
  for ( unsigned int i = 1; i < m_dataPoints.size(); ++i ) {
    const double currentValue = m_dataPoints[i]->coordinate( coordinate )->value() - m_dataPoints[i]->coordinate( coordinate )->errorMinus();
    if ( currentValue < low ) low = currentValue;
  }
  return low;
}


double
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::upperExtent( int coordinate ) const
{
  if ( m_dataPoints.size() == 0 ) return 0;
  double high = m_dataPoints[0]->coordinate( coordinate )->value() + m_dataPoints[0]->coordinate( coordinate )->errorPlus();
  for ( unsigned int i = 1; i < m_dataPoints.size(); ++i ) {
    const double currentValue = m_dataPoints[i]->coordinate( coordinate )->value() + m_dataPoints[i]->coordinate( coordinate )->errorPlus();
    if ( currentValue > high ) high = currentValue;
  }
  return high;
}


bool
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::scale( double scaleFactor )
{
  for ( unsigned int i = 0; i < m_dataPoints.size(); ++i ) {
    AIDA::IDataPoint* point = m_dataPoints[i];
    for ( int iCoord = 0; iCoord < m_dimension; ++iCoord ) {
      AIDA::IMeasurement* measurement = point->coordinate( iCoord );
      measurement->setValue( measurement->value() * scaleFactor );
      measurement->setErrorPlus( measurement->errorPlus() * scaleFactor );
      measurement->setErrorMinus( measurement->errorMinus() * scaleFactor );
    }
  }
  return true;
}


bool
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::scaleValues( double scaleFactor )
{
  for ( unsigned int i = 0; i < m_dataPoints.size(); ++i ) {
    AIDA::IDataPoint* point = m_dataPoints[i];
    for ( int iCoord = 0; iCoord < m_dimension; ++iCoord ) {
      AIDA::IMeasurement* measurement = point->coordinate( iCoord );
      measurement->setValue( measurement->value() * scaleFactor );
    }
  }
  return true;
}


bool
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::scaleErrors( double scaleFactor )
{
  for ( unsigned int i = 0; i < m_dataPoints.size(); ++i ) {
    AIDA::IDataPoint* point = m_dataPoints[i];
    for ( int iCoord = 0; iCoord < m_dimension; ++iCoord ) {
      AIDA::IMeasurement* measurement = point->coordinate( iCoord );
      measurement->setErrorPlus( measurement->errorPlus() * scaleFactor );
      measurement->setErrorMinus( measurement->errorMinus() * scaleFactor );
    }
  }
  return true;
}

// new from AIDA 3.2.1 : set coordinate for dps. If empty create points 
// otherwise replace with new points 

bool 
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::setCoordinate(int iCoord, const std::vector<double>  & val, const std::vector<double>  & err) 
{
  // checks 
  if (iCoord >= m_dimension) return false; 
  // set is empty : create points 
  if (m_dataPoints.size() == 0) {
    m_dataPoints.reserve(val.size()); 
    for ( unsigned int i = 0; i < val.size(); ++i ) {
      AIDA::IDataPoint* p = addPoint(); 
      AIDA::IMeasurement* measurement = p->coordinate( iCoord );
      measurement->setValue(val[i]);
      if ( i < err.size() ) { 
	measurement->setErrorPlus(err[i]); 
	measurement->setErrorMinus(err[i]);
      } 
    }
  }
  // replace contents of existing points 
  else if ( m_dataPoints.size() == val.size() ) { 
    for ( unsigned int i = 0; i < val.size(); ++i ) {
      AIDA::IDataPoint* p = point(i); 
      AIDA::IMeasurement* measurement = p->coordinate( iCoord );
      measurement->setValue(val[i]);
      if ( i < err.size() ) { 
	measurement->setErrorPlus(err[i]); 
	measurement->setErrorMinus(err[i]);
      } 
    }
  }
  else
    return false; 

  return true;      
}

bool 
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::setCoordinate(int iCoord, const std::vector<double>  & val, const std::vector<double>  & errp, const std::vector<double>  & errm)
{
  // checks 
  if (iCoord >= m_dimension) return false; 
  // set is empty : create points 
  if (m_dataPoints.size() == 0) {
    m_dataPoints.reserve(val.size()); 
    for ( unsigned int i = 0; i < val.size(); ++i ) {
      AIDA::IDataPoint* p = addPoint(); 
      AIDA::IMeasurement* measurement = p->coordinate( iCoord );
      measurement->setValue(val[i]);
      if ( i < errp.size() ) 
	measurement->setErrorPlus(errp[i]); 
      if ( i < errm.size() )
	measurement->setErrorMinus(errm[i]); 
    }
  }
  // replace contents of existing points 
  else if ( m_dataPoints.size() == val.size() ) { 
    for ( unsigned int i = 0; i < val.size(); ++i ) {
      AIDA::IDataPoint* p = point(i); 
      AIDA::IMeasurement* measurement = p->coordinate( iCoord );
      measurement->setValue(val[i]);
      if ( i < errp.size() )  
	measurement->setErrorPlus(errp[i]); 
      if ( i < errm.size() )  
	measurement->setErrorMinus(errm[i]);      
    }
  }
  else
    return false; 

  return true;      
}

void * 
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::cast(const std::string & className) const
{
  if (className.find(userLevelClassType()) != std::string::npos ) 
    return const_cast<iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet *> (this);
  else 
    return 0;  
}


#ifdef OLDSTREAMS
# include <strstream>
# define ostringstream ostrstream
#else
# include <sstream>
#endif

void
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet::updateAnnotation() const
{
  std::ostringstream osSize;
  osSize << size();
#ifndef BADENDS
  osSize << std::ends;
#endif
  m_annotation->setValue( sizeKey, osSize.str() );
}
