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

#include "AIDA_DataPointSet_native/AIDA_DevDataPointSetFactory.h"
#include "AIDA_DataPointSet.h"
#include "AIDA/IDataPoint.h"
#include "AIDA/IMeasurement.h"
#include "AIDA/IAxis.h"
#include "AIDA/IAnnotation.h"
#include "AIDA/IHistogram1D.h"
#include "AIDA/IHistogram2D.h"
#include "AIDA/IHistogram3D.h"
#include "AIDA/ICloud1D.h"
#include "AIDA/ICloud2D.h"
#include "AIDA/ICloud3D.h"
#include "AIDA/IProfile1D.h"
#include "AIDA/IProfile2D.h"
#include <set>
#include <cmath>
#include <typeinfo>

#include <memory>
#include <stdexcept>
#include "AIDA_Annotation/AIDA_Annotation.h"

AIDA::IAnnotation*
iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory::createAnnotation()
{
  /*  
  std::auto_ptr<AIDA::Dev::IDevAnalysisFactory> af(dynamic_cast<AIDA::Dev::IDevAnalysisFactory*>(AIDA_createAnalysisFactory() ));
  if ( ! af.get() ) throw std::runtime_error( "Could not create an analysis factory" );
  AIDA::Dev::IAnnotationFactory* anf = af->annotationFactory();
  if ( ! anf ) throw std::runtime_error( "Could not find an annotation factory" );
  return anf->createAnnotation();
  */ 
  return new iAIDA::AIDA_Annotation::AIDA_Annotation( );
}


static const std::string emptyString = "";

AIDA::Dev::IDevDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory::create( const std::string& title,
								       int dimensionOfPoint )
{
  return new iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet( title, dimensionOfPoint, createAnnotation() );
}


AIDA::Dev::IDevDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory::create( int dimensionOfPoint )
{
  return new iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet( emptyString, dimensionOfPoint, createAnnotation() );
}


AIDA::Dev::IDevDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory::createCopy( const AIDA::IDataPointSet & original )
{
  // LM: check first if original is an pi implementation
  try { 
    const iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet &dps = dynamic_cast< const iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet &>( original );
    return new iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet( dps, createAnnotation() );
  }
  // if cast failed is a foreign implementation
  catch( std::bad_cast ) {
    return new iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet( original, createAnnotation() );
  }
  
}


bool
iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory::destroy( AIDA::IDataPointSet * dataPointSet )
{
  if ( dataPointSet ) {
    delete dataPointSet;
    return true;
  }
  else return false;
}


AIDA::Dev::IDevDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory::create( const AIDA::IHistogram1D & h, const std::string & /*options*/ )
{
  AIDA::Dev::IDevDataPointSet* p = new iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet( h.title(), h.dimension() + 1, createAnnotation() );

  // Copy the data
  const AIDA::IAxis& xAxis = h.axis();
  for ( int i = 0; i < xAxis.bins(); ++i ) {
    p->addPoint();
    AIDA::IDataPoint* point = p->point(i);
    AIDA::IMeasurement* xMeasurement = point->coordinate( 0 );
    xMeasurement->setValue( h.binMean( i ) );
    xMeasurement->setErrorPlus( xAxis.binUpperEdge( i ) - xMeasurement->value() );
    xMeasurement->setErrorMinus( xMeasurement->value() - xAxis.binLowerEdge( i ) );

    AIDA::IMeasurement* yMeasurement = point->coordinate( 1 );
    const double height = h.binHeight( i );
    const double error = h.binError( i );
    yMeasurement->setValue( height );
    yMeasurement->setErrorPlus( error );
    yMeasurement->setErrorMinus( error );
  }

  // Copy the annotation items
  AIDA::IAnnotation& annotation = p->annotation();
  annotation.reset();

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

  return p;
}


AIDA::Dev::IDevDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory::create( const AIDA::IHistogram2D & h, const std::string & /*options*/ )
{
  AIDA::Dev::IDevDataPointSet* p = new iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet( h.title(), h.dimension() + 1, createAnnotation() );

  // Copy the data
  const AIDA::IAxis& xAxis = h.xAxis();
  const AIDA::IAxis& yAxis = h.yAxis();
  for ( int i = 0; i < xAxis.bins(); ++i ) {
    for ( int j = 0; j < yAxis.bins(); ++j ) {
      p->addPoint();
      AIDA::IDataPoint* point = p->point( p->size() - 1 );

      AIDA::IMeasurement* xMeasurement = point->coordinate( 0 );
      xMeasurement->setValue( h.binMeanX( i, j ) );
      xMeasurement->setErrorPlus( xAxis.binUpperEdge( i ) - xMeasurement->value() );
      xMeasurement->setErrorMinus( xMeasurement->value() - xAxis.binLowerEdge( i ) );

      AIDA::IMeasurement* yMeasurement = point->coordinate( 1 );
      yMeasurement->setValue( h.binMeanY( i, j ) );
      yMeasurement->setErrorPlus( yAxis.binUpperEdge( j ) - yMeasurement->value() );
      yMeasurement->setErrorMinus( yMeasurement->value() - yAxis.binLowerEdge( j ) );

      AIDA::IMeasurement* zMeasurement = point->coordinate( 2 );
      const double height = h.binHeight( i, j );
      const double error = h.binError( i, j );
      zMeasurement->setValue( height );
      zMeasurement->setErrorPlus( error );
      zMeasurement->setErrorMinus( error );
    }
  }

  // Copy the annotation items
  AIDA::IAnnotation& annotation = p->annotation();
  annotation.reset();

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

  return p;
}


AIDA::Dev::IDevDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory::create( const AIDA::IHistogram3D & h, const std::string & /*options*/ )
{
  AIDA::Dev::IDevDataPointSet* p = new iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet( h.title(), h.dimension() + 1, createAnnotation() );

  // Copy the data
  const AIDA::IAxis& xAxis = h.xAxis();
  const AIDA::IAxis& yAxis = h.yAxis();
  const AIDA::IAxis& zAxis = h.zAxis();
  for ( int i = 0; i < xAxis.bins(); ++i ) {
    for ( int j = 0; j < yAxis.bins(); ++j ) {
      for ( int k = 0; k < zAxis.bins(); ++k ) {
	p->addPoint();
	AIDA::IDataPoint* point = p->point( p->size() - 1 );

	AIDA::IMeasurement* xMeasurement = point->coordinate( 0 );
	xMeasurement->setValue( h.binMeanX( i, j, k ) );
	xMeasurement->setErrorPlus( xAxis.binUpperEdge( i ) - xMeasurement->value() );
	xMeasurement->setErrorMinus( xMeasurement->value() - xAxis.binLowerEdge( i ) );
	
	AIDA::IMeasurement* yMeasurement = point->coordinate( 1 );
	yMeasurement->setValue( h.binMeanY( i, j, k ) );
	yMeasurement->setErrorPlus( yAxis.binUpperEdge( j ) - yMeasurement->value() );
	yMeasurement->setErrorMinus( yMeasurement->value() - yAxis.binLowerEdge( j ) );

	AIDA::IMeasurement* zMeasurement = point->coordinate( 2 );
	zMeasurement->setValue( h.binMeanZ( i, j, k ) );
	zMeasurement->setErrorPlus( zAxis.binUpperEdge( k ) - zMeasurement->value() );
	zMeasurement->setErrorMinus( zMeasurement->value() - zAxis.binLowerEdge( k ) );

	AIDA::IMeasurement* nMeasurement = point->coordinate( 3 );
	const double height = h.binHeight( i, j, k );
	const double error = h.binError( i, j, k );
	nMeasurement->setValue( height );
	nMeasurement->setErrorPlus( error );
	nMeasurement->setErrorMinus( error );
      }
    }
  }

  // Copy the annotation items
  AIDA::IAnnotation& annotation = p->annotation();
  annotation.reset();

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

  return p;
}


AIDA::Dev::IDevDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory::create( const AIDA::ICloud1D & c, const std::string & /*options*/ )
{
  if ( c.isConverted() ) return create( c.histogram() );

  AIDA::Dev::IDevDataPointSet* p = new iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet( c.title(), c.dimension() + 1, createAnnotation() );
  for ( int i = 0; i < c.entries(); ++i ) {
    p->addPoint();
    AIDA::IDataPoint* point = p->point( p->size() - 1 );
    point->coordinate( 0 )->setValue( c.value( i ) );
    point->coordinate( 1 )->setValue( c.weight( i ) );
  }

  // Copy the annotation items
  AIDA::IAnnotation& annotation = p->annotation();
  annotation.reset();

  std::set< std::string > existingAnnotationItems;
  for ( int item = 0; item < annotation.size(); ++item ) {
      existingAnnotationItems.insert( annotation.key( item ) );
  }
  const AIDA::IAnnotation& otherAnnotation = c.annotation();
  for ( int itemNew = 0; itemNew < otherAnnotation.size(); ++itemNew ) {
      const std::string& key = otherAnnotation.key( itemNew );
      if ( existingAnnotationItems.find( key ) == existingAnnotationItems.end() ) {
	  annotation.addItem( key, otherAnnotation.value( itemNew ), false );
      }
  }

  return p;
}


AIDA::Dev::IDevDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory::create( const AIDA::ICloud2D & c, const std::string & /*options*/ )
{
  if ( c.isConverted() ) return create( c.histogram() );

  AIDA::Dev::IDevDataPointSet* p = new iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet( c.title(), c.dimension() + 1, createAnnotation() );
  for ( int i = 0; i < c.entries(); ++i ) {
    p->addPoint();
    AIDA::IDataPoint* point = p->point( p->size() - 1 );
    point->coordinate( 0 )->setValue( c.valueX( i ) );
    point->coordinate( 1 )->setValue( c.valueY( i ) );
    point->coordinate( 2 )->setValue( c.weight( i ) );
  }

  // Copy the annotation items
  AIDA::IAnnotation& annotation = p->annotation();
  annotation.reset();

  std::set< std::string > existingAnnotationItems;
  for ( int item = 0; item < annotation.size(); ++item ) {
      existingAnnotationItems.insert( annotation.key( item ) );
  }
  const AIDA::IAnnotation& otherAnnotation = c.annotation();
  for ( int itemNew = 0; itemNew < otherAnnotation.size(); ++itemNew ) {
      const std::string& key = otherAnnotation.key( itemNew );
      if ( existingAnnotationItems.find( key ) == existingAnnotationItems.end() ) {
	  annotation.addItem( key, otherAnnotation.value( itemNew ), false );
      }
  }

  return p;
}


AIDA::Dev::IDevDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory::create( const AIDA::ICloud3D & c, const std::string & /*options*/ )
{
  if ( c.isConverted() ) return create( c.histogram() );

  AIDA::Dev::IDevDataPointSet* p = new iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet( c.title(), c.dimension() + 1, createAnnotation() );
  for ( int i = 0; i < c.entries(); ++i ) {
    p->addPoint();
    AIDA::IDataPoint* point = p->point( p->size() - 1 );
    point->coordinate( 0 )->setValue( c.valueX( i ) );
    point->coordinate( 1 )->setValue( c.valueY( i ) );
    point->coordinate( 2 )->setValue( c.valueZ( i ) );
    point->coordinate( 3 )->setValue( c.weight( i ) );
  }

  // Copy the annotation items
  AIDA::IAnnotation& annotation = p->annotation();
  annotation.reset();

  std::set< std::string > existingAnnotationItems;
  for ( int item = 0; item < annotation.size(); ++item ) {
      existingAnnotationItems.insert( annotation.key( item ) );
  }
  const AIDA::IAnnotation& otherAnnotation = c.annotation();
  for ( int itemNew = 0; itemNew < otherAnnotation.size(); ++itemNew ) {
      const std::string& key = otherAnnotation.key( itemNew );
      if ( existingAnnotationItems.find( key ) == existingAnnotationItems.end() ) {
	  annotation.addItem( key, otherAnnotation.value( itemNew ), false );
      }
  }

  return p;
}


AIDA::Dev::IDevDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory::create( const AIDA::IProfile1D & h, const std::string & /*options*/ )
{
  AIDA::Dev::IDevDataPointSet* p = new iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet( h.title(), h.dimension() + 1, createAnnotation() );

  // Copy the data
  const AIDA::IAxis& xAxis = h.axis();
  for ( int i = 0; i < xAxis.bins(); ++i ) {
    p->addPoint();
    AIDA::IDataPoint* point = p->point(i);
    AIDA::IMeasurement* xMeasurement = point->coordinate( 0 );
    xMeasurement->setValue( h.binMean( i ) );
    xMeasurement->setErrorPlus( xAxis.binUpperEdge( i ) - xMeasurement->value() );
    xMeasurement->setErrorMinus( xMeasurement->value() - xAxis.binLowerEdge( i ) );

    AIDA::IMeasurement* yMeasurement = point->coordinate( 1 );
    const double height = h.binHeight( i );
    const double error = h.binRms( i );
    yMeasurement->setValue( height );
    yMeasurement->setErrorPlus( error );
    yMeasurement->setErrorMinus( error );
  }

  // Copy the annotation items
  AIDA::IAnnotation& annotation = p->annotation();
  annotation.reset();

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

  return p;
}


AIDA::Dev::IDevDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory::create( const AIDA::IProfile2D & h, const std::string & /*options*/ )
{
  AIDA::Dev::IDevDataPointSet* p = new iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet( h.title(), h.dimension() + 1, createAnnotation() );

  // Copy the data
  const AIDA::IAxis& xAxis = h.xAxis();
  const AIDA::IAxis& yAxis = h.yAxis();
  for ( int i = 0; i < xAxis.bins(); ++i ) {
    for ( int j = 0; j < yAxis.bins(); ++j ) {
      p->addPoint();
      AIDA::IDataPoint* point = p->point( p->size() - 1 );

      AIDA::IMeasurement* xMeasurement = point->coordinate( 0 );
      xMeasurement->setValue( h.binMeanX( i, j ) );
      xMeasurement->setErrorPlus( xAxis.binUpperEdge( i ) - xMeasurement->value() );
      xMeasurement->setErrorMinus( xMeasurement->value() - xAxis.binLowerEdge( i ) );

      AIDA::IMeasurement* yMeasurement = point->coordinate( 1 );
      yMeasurement->setValue( h.binMeanY( i, j ) );
      yMeasurement->setErrorPlus( yAxis.binUpperEdge( j ) - yMeasurement->value() );
      yMeasurement->setErrorMinus( yMeasurement->value() - yAxis.binLowerEdge( j ) );

      AIDA::IMeasurement* zMeasurement = point->coordinate( 2 );
      const double height = h.binHeight( i, j );
      const double error = h.binRms( i, j );
      zMeasurement->setValue( height );
      zMeasurement->setErrorPlus( error );
      zMeasurement->setErrorMinus( error );
    }
  }

  // Copy the annotation items
  AIDA::IAnnotation& annotation = p->annotation();
  annotation.reset();

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

  return p;
}


AIDA::Dev::IDevDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory::add( const AIDA::IDataPointSet& a,
								    const AIDA::IDataPointSet& b,
								    std::string )
{
  if ( a.dimension() != b.dimension() || a.size() != b.size() ) return 0;
  AIDA::Dev::IDevDataPointSet* p = new iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet( a.title(), a.dimension(), createAnnotation() );
  for ( int iPoint = 0; iPoint < a.size(); ++iPoint ) {
    p->addPoint();
    AIDA::IDataPoint* point = p->point( p->size() - 1 );
    const AIDA::IDataPoint* pointA = a.point( iPoint );
    const AIDA::IDataPoint* pointB = b.point( iPoint );
    for ( int i = 0; i < point->dimension(); ++i ) {
      AIDA::IMeasurement* measurement = point->coordinate( i );
      const AIDA::IMeasurement* measurementA = pointA->coordinate( i );
      const AIDA::IMeasurement* measurementB = pointB->coordinate( i );
      const double valueA = measurementA->value();
      const double valueB = measurementB->value();
      const double errorPlusA = measurementA->errorPlus();
      const double errorPlusB = measurementB->errorPlus();
      const double errorMinusA = measurementA->errorMinus();
      const double errorMinusB = measurementB->errorMinus();

      const double value = valueA + valueB;
      const double errorPlus = std::sqrt( errorPlusA*errorPlusA + errorPlusB*errorPlusB );
      const double errorMinus = std::sqrt( errorMinusA*errorMinusA + errorMinusB*errorMinusB );

      measurement->setValue( value );
      measurement->setErrorPlus( errorPlus );
      measurement->setErrorMinus( errorMinus );
    }
  }

  return p;
}


AIDA::Dev::IDevDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory::subtract( const AIDA::IDataPointSet& a,
									 const AIDA::IDataPointSet& b,
									 std::string )
{
  if ( a.dimension() != b.dimension() || a.size() != b.size() ) return 0;
  AIDA::Dev::IDevDataPointSet* p = new iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet( a.title(), a.dimension(), createAnnotation() );
  for ( int iPoint = 0; iPoint < a.size(); ++iPoint ) {
    p->addPoint();
    AIDA::IDataPoint* point = p->point( p->size() - 1 );
    const AIDA::IDataPoint* pointA = a.point( iPoint );
    const AIDA::IDataPoint* pointB = b.point( iPoint );
    for ( int i = 0; i < point->dimension(); ++i ) {
      AIDA::IMeasurement* measurement = point->coordinate( i );
      const AIDA::IMeasurement* measurementA = pointA->coordinate( i );
      const AIDA::IMeasurement* measurementB = pointB->coordinate( i );
      const double valueA = measurementA->value();
      const double valueB = measurementB->value();
      const double errorPlusA = measurementA->errorPlus();
      const double errorPlusB = measurementB->errorPlus();
      const double errorMinusA = measurementA->errorMinus();
      const double errorMinusB = measurementB->errorMinus();

      const double value = valueA - valueB;
      const double errorPlus = std::sqrt( errorPlusA*errorPlusA + errorPlusB*errorPlusB );
      const double errorMinus = std::sqrt( errorMinusA*errorMinusA + errorMinusB*errorMinusB );

      measurement->setValue( value );
      measurement->setErrorPlus( errorPlus );
      measurement->setErrorMinus( errorMinus );
    }
  }

  return p;
}


AIDA::Dev::IDevDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory::multiply( const AIDA::IDataPointSet& a,
									 const AIDA::IDataPointSet& b,
									 std::string )
{
  if ( a.dimension() != b.dimension() || a.size() != b.size() ) return 0;
  AIDA::Dev::IDevDataPointSet* p = new iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet( a.title(), a.dimension(), createAnnotation() );
  for ( int iPoint = 0; iPoint < a.size(); ++iPoint ) {
    p->addPoint();
    AIDA::IDataPoint* point = p->point( p->size() - 1 );
    const AIDA::IDataPoint* pointA = a.point( iPoint );
    const AIDA::IDataPoint* pointB = b.point( iPoint );
    for ( int i = 0; i < point->dimension(); ++i ) {
      AIDA::IMeasurement* measurement = point->coordinate( i );
      const AIDA::IMeasurement* measurementA = pointA->coordinate( i );
      const AIDA::IMeasurement* measurementB = pointB->coordinate( i );
      const double valueA = measurementA->value();
      const double valueB = measurementB->value();
      const double errorPlusA = measurementA->errorPlus();
      const double errorPlusB = measurementB->errorPlus();
      const double errorMinusA = measurementA->errorMinus();
      const double errorMinusB = measurementB->errorMinus();

      const double value = valueA * valueB;
      const double errorPlus = std::sqrt( errorPlusA*errorPlusA*valueB*valueB + errorPlusB*errorPlusB*valueA*valueA );
      const double errorMinus = std::sqrt( errorMinusA*errorMinusA*valueB*valueB + errorMinusB*errorMinusB*valueA*valueA );

      measurement->setValue( value );
      measurement->setErrorPlus( errorPlus );
      measurement->setErrorMinus( errorMinus );
    }
  }

  return p;
}


AIDA::Dev::IDevDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory::divide( const AIDA::IDataPointSet& a,
								       const AIDA::IDataPointSet& b,
								       std::string )
{
  if ( a.dimension() != b.dimension() || a.size() != b.size() ) return 0;
  AIDA::Dev::IDevDataPointSet* p = new iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet( a.title(), a.dimension(), createAnnotation() );
  for ( int iPoint = 0; iPoint < a.size(); ++iPoint ) {
    p->addPoint();
    AIDA::IDataPoint* point = p->point( p->size() - 1 );
    const AIDA::IDataPoint* pointA = a.point( iPoint );
    const AIDA::IDataPoint* pointB = b.point( iPoint );
    for ( int i = 0; i < point->dimension(); ++i ) {
      AIDA::IMeasurement* measurement = point->coordinate( i );
      const AIDA::IMeasurement* measurementA = pointA->coordinate( i );
      const AIDA::IMeasurement* measurementB = pointB->coordinate( i );
      const double valueA = measurementA->value();
      const double valueB = measurementB->value();
      const double errorPlusA = measurementA->errorPlus();
      const double errorPlusB = measurementB->errorPlus();
      const double errorMinusA = measurementA->errorMinus();
      const double errorMinusB = measurementB->errorMinus();

      double value = 0;
      double errorPlus = 0;
      double errorMinus = 0;
      if ( valueB != 0 ) {
	value = valueA / valueB;
	const double valueA2 = valueA * valueA;
	const double valueB2 = valueB * valueB;
	const double valueB4 = valueB2 * valueB2;
	errorPlus = std::sqrt( errorPlusA * errorPlusA / valueB2 + errorPlusB * errorPlusB * valueA2 / valueB4 );
	errorMinus = std::sqrt( errorMinusA * errorMinusA / valueB2 + errorMinusB * errorMinusB * valueA2 / valueB4 );
      }

      measurement->setValue( value );
      measurement->setErrorPlus( errorPlus );
      measurement->setErrorMinus( errorMinus );
    }
  }

  return p;
}


AIDA::Dev::IDevDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory::weightedMean( const AIDA::IDataPointSet& a,
									     const AIDA::IDataPointSet& b,
									     std::string )
{
  if ( a.dimension() != b.dimension() || a.size() != b.size() ) return 0;
  AIDA::Dev::IDevDataPointSet* p = new iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSet( a.title(), a.dimension(), createAnnotation() );
  for ( int iPoint = 0; iPoint < a.size(); ++iPoint ) {
    p->addPoint();
    AIDA::IDataPoint* point = p->point( p->size() - 1 );
    const AIDA::IDataPoint* pointA = a.point( iPoint );
    const AIDA::IDataPoint* pointB = b.point( iPoint );
    for ( int i = 0; i < point->dimension(); ++i ) {
      AIDA::IMeasurement* measurement = point->coordinate( i );
      const AIDA::IMeasurement* measurementA = pointA->coordinate( i );
      const AIDA::IMeasurement* measurementB = pointB->coordinate( i );
      const double valueA = measurementA->value();
      const double valueB = measurementB->value();
      const double errorPlusA = measurementA->errorPlus();
      const double errorPlusB = measurementB->errorPlus();
      const double errorMinusA = measurementA->errorMinus();
      const double errorMinusB = measurementB->errorMinus();

      // We will assume symmetric errors
      const double errorA2 = 0.5 * ( errorPlusA*errorPlusA + errorMinusA*errorMinusA );
      const double errorB2 = 0.5 * ( errorPlusB*errorPlusB + errorMinusB*errorMinusB );

      double value = 0;
      double error = 0;
      if ( errorA2 != 0 && errorB2 != 0 ) {
	const double wA = 1 / errorA2;
	const double wB = 1 / errorB2;
	value = ( valueA * wA + valueB * wB ) / ( wA + wB );
	error = ( std::sqrt( wA ) + std::sqrt( wB ) ) / ( wA + wB );
      }

      measurement->setValue( value );
      measurement->setErrorPlus( error );
      measurement->setErrorMinus( error );
    }
  }

  return p;
}


// new creation methods from AIDA 3.2
//===========================================================================
// createY

AIDA::Dev::IDevDataPointSet * 
iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory::createY(const std::string & title, const std::vector<double>  & y, const std::vector<double>  & eyp, const std::vector<double>  & eym) {
  // create empty 2d-data set 
  AIDA::Dev::IDevDataPointSet * dps = create(title, 2); 
  unsigned int npoints = y.size(); 
  // x vector are indices 
  std::vector<double> x;
  x.reserve( npoints ); 
  for (unsigned int i = 0; i < npoints; ++i)  
    x.push_back( static_cast<double> (i) ); 

  std::vector<double> ex( npoints ); 
  // set coordinate
  if (!dps->setCoordinate(0, x, ex ) ) return 0;
  if (!dps->setCoordinate(1, y, eyp, eym ) ) return 0;
  return dps; 
}

//===========================================================================
// createX


AIDA::Dev::IDevDataPointSet * 
iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory::createX(const std::string & title, const std::vector<double>  & x, const std::vector<double>  & exp, const std::vector<double>  & exm) { 

  // create empty 2d-data set 
  AIDA::Dev::IDevDataPointSet * dps = create(title, 2); 
  unsigned int npoints = x.size(); 
  // y vector are indices 
  std::vector<double> y;
  y.reserve( npoints ); 
  for (unsigned int i = 0; i < npoints; ++i)  
    y.push_back( static_cast<double> (i) ); 

  std::vector<double> ey( npoints ); 
  // set coordinate
  if (!dps->setCoordinate(0, x, exp, exm ) ) return 0;
  if (!dps->setCoordinate(1, y, ey ) ) return 0;
  return dps; 

}

//===========================================================================
// createXY


AIDA::Dev::IDevDataPointSet * 
iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory::createXY(const std::string & title, const std::vector<double>  & x, const std::vector<double>  & y, const std::vector<double>  & exp, const std::vector<double>  & eyp, const std::vector<double>  & exm, const std::vector<double>  & eym) { 

  // create empty 2d-data set 
  AIDA::Dev::IDevDataPointSet * dps = create(title, 2); 
  // set coordinate
  if (!dps->setCoordinate(0, x, exp, exm ) ) return 0;
  if (!dps->setCoordinate(1, y, eyp, eym ) ) return 0;
  return dps; 

}

//===========================================================================
// createXYZ

AIDA::Dev::IDevDataPointSet * 
iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory::createXYZ(const std::string & title, const std::vector<double>  & x, const std::vector<double>  & y, const std::vector<double>  & z, const std::vector<double>  & exp, const std::vector<double>  & eyp, const std::vector<double>  & ezp, const std::vector<double>  & exm, const std::vector<double>  & eym, const std::vector<double>  & ezm) { 

  // create empty 3d-data set 
  AIDA::Dev::IDevDataPointSet * dps = create(title, 3); 
  // set coordinate
  if (!dps->setCoordinate(0, x, exp, exm ) ) return 0;
  if (!dps->setCoordinate(1, y, eyp, eym ) ) return 0;
  if (!dps->setCoordinate(2, z, ezp, ezm ) ) return 0;
  return dps; 

}
