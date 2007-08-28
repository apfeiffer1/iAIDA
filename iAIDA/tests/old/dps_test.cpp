#include <iostream>
#include "AIDA_DataPointSet_native/AIDA_DevDataPointSetFactory.h"
#include <memory>
#include <stdexcept>
#include "AIDA_Dev/IDevDataPointSetFactory.h"
#include "AIDA_Dev/IDevDataPointSet.h"
#include "AIDA/IDataPoint.h"
#include "AIDA/IMeasurement.h"
#include <cmath>
#include <fstream>

void dumpDataPointSetInfo( const AIDA::Dev::IDevDataPointSet& dps, std::ostream& os );

int main( int, char** )
{
  std::ofstream fout( "dsp_test.out" );
  try {
    std::auto_ptr< AIDA::Dev::IDevDataPointSetFactory > dpsfp( new iAIDA::AIDA_DataPointSet_native::AIDA_DevDataPointSetFactory() );
    if ( ! dpsfp.get() ) throw std::runtime_error( "No DataPointSet factory created" );
    AIDA::Dev::IDevDataPointSetFactory& dpsf = *(dpsfp.get());

    std::ostream& out = fout;

    out << "Creating a datapointset of dimension 1" << std::endl;
    std::auto_ptr< AIDA::Dev::IDevDataPointSet > dpsa( dpsf.create( "Title a", 1 ) );
    if ( ! dpsa.get() ) throw std::runtime_error( "Could not create a data point set" );
    dpsa->setName( "dpsa" );
    int nPoints = 4;
    for ( int i = 0; i < nPoints; ++i ) {
      dpsa->addPoint();
      AIDA::IDataPoint* point = dpsa->point( i );
      for ( int dim = 0; dim < point->dimension(); ++dim ) {
	AIDA::IMeasurement* m = point->coordinate( dim );
	m->setValue( i * std::pow( 10.0, dim ) );
	m->setErrorPlus( std::pow( 10.0, dim - 1 ) );
	m->setErrorMinus( std::pow( 10.0, dim - 1 ) );
      }
    }
    dumpDataPointSetInfo( *dpsa, out );

    out << "Creating a datapointset of dimension 2" << std::endl;
    std::auto_ptr< AIDA::Dev::IDevDataPointSet > dpsb( dpsf.create( 2 ) );
    if ( ! dpsb.get() ) throw std::runtime_error( "Could not create a data point set" );
    dpsb->setName( "dpsb" );
    dpsb->setTitle( "Title b" );
    for ( int i = 0; i < nPoints; ++i ) {
      if ( ! dpsb->addPoint() ) throw std::runtime_error( "Could not add a new point" );
      AIDA::IDataPoint* point = dpsb->point( i );
      for ( int dim = 0; dim < point->dimension(); ++dim ) {
	AIDA::IMeasurement* m = point->coordinate( dim );
	m->setValue( i * std::pow( 10.0, dim ) );
	m->setErrorPlus( std::pow( 10.0, dim - 1 ) );
	m->setErrorMinus( std::pow( 10.0, dim - 1 ) );
      }
    }
    for ( int i = 0; i < nPoints; ++i ) {
      dpsb->addPoint( *( dpsb->point( i ) ) );
    }
    dumpDataPointSetInfo( *dpsb, out );

    out << "Creating a copy of the last d.p.s." << std::endl;
    std::auto_ptr< AIDA::Dev::IDevDataPointSet > dpsc( dpsf.createCopy( *dpsb ) );
    if ( ! dpsc.get() ) throw std::runtime_error( "Could not create a data point set" );
    dpsc->setName( "dpsc" );
    dpsc->setTitle( "Title c" );
    dumpDataPointSetInfo( *dpsc, out );

    out << "Removing a point in the middle" << std::endl;
    dpsc->removePoint( dpsc->size() / 2 );
    dumpDataPointSetInfo( *dpsc, out );

    out << "Removing the last point" << std::endl;
    dpsc->removePoint( dpsc->size() - 1 );
    dumpDataPointSetInfo( *dpsc, out );

    out << "Removing the first point" << std::endl;
    dpsc->removePoint( 0 );
    dumpDataPointSetInfo( *dpsc, out );

    out << "Adding an empty point" << std::endl;
    dpsc->addPoint();
    dumpDataPointSetInfo( *dpsc, out );

    out << "Clearing the set" << std::endl;
    dpsc->clear();
    dumpDataPointSetInfo( *dpsc, out );

    out << "Copying half of the points from the previous set." << std::endl;
    for ( int i = 0; i < nPoints; ++i ) {
      dpsc->addPoint( *( dpsb->point( i ) ) );
    }
    dumpDataPointSetInfo( *dpsc, out );

    const double scaleFactor = 2;
    out << "Scaling the values by a factor of " << scaleFactor << std::endl;
    dpsc->scaleValues( scaleFactor );
    dumpDataPointSetInfo( *dpsc, out );

    out << "Scaling the erros by a factor of " << 1/scaleFactor << std::endl;
    dpsc->scaleErrors( 1/scaleFactor );
    dumpDataPointSetInfo( *dpsc, out );

    out << "Scaling the values and the erros by a factor of " << scaleFactor << std::endl;
    dpsc->scale( scaleFactor );
    dumpDataPointSetInfo( *dpsc, out );

    out << "Trying to insert an incompatible point" << std::endl;
    if ( dpsc->addPoint( * dpsa->point( 0 ) ) ) throw std::runtime_error( "Inserted a point with wrong dimension!!!" );
    dumpDataPointSetInfo( *dpsc, out );

    // Test the factory operations ( for each test a valid one and two invalid : wrong size/dimension )

    // add
    out << "Creating an addition d.p.s" << std::endl;
    std::auto_ptr< AIDA::Dev::IDevDataPointSet > dpsd( dpsf.add( *dpsc, *dpsc ) );
    if ( ! dpsd.get() ) throw std::runtime_error( "Could not add two compatible data point sets" );
    dpsd->setName( "dpsd" );
    dpsd->setTitle( "Title d" );
    dumpDataPointSetInfo( *dpsd, out );

    out << "Attemptimg to add two incompatible dps (in length)..." << std::endl;
    std::auto_ptr< AIDA::Dev::IDevDataPointSet > dpsW1( dpsf.add( *dpsc, *dpsb ) );
    if ( dpsW1.get() ) throw std::runtime_error( "Added two incompatible data point sets!!!" );
    out << "No effect, correct." << std::endl;

    out << "Attemptimg to add two incompatible dps (in dimension)..." << std::endl;
    std::auto_ptr< AIDA::Dev::IDevDataPointSet > dpsW2( dpsf.add( *dpsc, *dpsa ) );
    if ( dpsW2.get() ) throw std::runtime_error( "Added two incompatible data point sets!!!" );
    out << "No effect, correct." << std::endl;

    // subtract
    out << "Creating an subtraction d.p.s" << std::endl;
    std::auto_ptr< AIDA::Dev::IDevDataPointSet > dpse( dpsf.subtract( *dpsd, *dpsc ) );
    if ( ! dpse.get() ) throw std::runtime_error( "Could not subtract two compatible data point sets" );
    dpse->setName( "dpse" );
    dpse->setTitle( "Title e" );
    dumpDataPointSetInfo( *dpse, out );

    out << "Attemptimg to subtract two incompatible dps (in length)..." << std::endl;
    std::auto_ptr< AIDA::Dev::IDevDataPointSet > dpsW3( dpsf.subtract( *dpsc, *dpsb ) );
    if ( dpsW3.get() ) throw std::runtime_error( "Subtracted two incompatible data point sets!!!" );
    out << "No effect, correct." << std::endl;

    out << "Attemptimg to subtract two incompatible dps (in dimension)..." << std::endl;
    std::auto_ptr< AIDA::Dev::IDevDataPointSet > dpsW4( dpsf.subtract( *dpsc, *dpsa ) );
    if ( dpsW4.get() ) throw std::runtime_error( "Subtracted two incompatible data point sets!!!" );
    out << "No effect, correct." << std::endl;

    // multiply
    out << "Creating an multiplication d.p.s" << std::endl;
    std::auto_ptr< AIDA::Dev::IDevDataPointSet > dpsg( dpsf.multiply( *dpsd, *dpsc ) );
    if ( ! dpsg.get() ) throw std::runtime_error( "Could not multiply two compatible data point sets" );
    dpsg->setName( "dpsg" );
    dpsg->setTitle( "Title g" );
    dumpDataPointSetInfo( *dpsg, out );

    out << "Attemptimg to multiply two incompatible dps (in length)..." << std::endl;
    std::auto_ptr< AIDA::Dev::IDevDataPointSet > dpsW5( dpsf.multiply( *dpsc, *dpsb ) );
    if ( dpsW5.get() ) throw std::runtime_error( "Multiplied two incompatible data point sets!!!" );
    out << "No effect, correct." << std::endl;

    out << "Attemptimg to multiply two incompatible dps (in dimension)..." << std::endl;
    std::auto_ptr< AIDA::Dev::IDevDataPointSet > dpsW6( dpsf.multiply( *dpsc, *dpsa ) );
    if ( dpsW6.get() ) throw std::runtime_error( "Multiplied two incompatible data point sets!!!" );
    out << "No effect, correct." << std::endl;

    // divide
    out << "Creating a division d.p.s" << std::endl;
    std::auto_ptr< AIDA::Dev::IDevDataPointSet > dpsh( dpsf.divide( *dpsg, *dpsg ) );
    if ( ! dpsh.get() ) throw std::runtime_error( "Could not divide two compatible data point sets" );
    dpsh->setName( "dpsh" );
    dpsh->setTitle( "Title h" );
    dumpDataPointSetInfo( *dpsh, out );

    out << "Attemptimg to divide two incompatible dps (in length)..." << std::endl;
    std::auto_ptr< AIDA::Dev::IDevDataPointSet > dpsW7( dpsf.divide( *dpsc, *dpsb ) );
    if ( dpsW7.get() ) throw std::runtime_error( "Divided two incompatible data point sets!!!" );
    out << "No effect, correct." << std::endl;

    out << "Attemptimg to divide two incompatible dps (in dimension)..." << std::endl;
    std::auto_ptr< AIDA::Dev::IDevDataPointSet > dpsW8( dpsf.divide( *dpsc, *dpsa ) );
    if ( dpsW8.get() ) throw std::runtime_error( "Divided two incompatible data point sets!!!" );
    out << "No effect, correct." << std::endl;

    // weighted mean
    out << "Copying and scaling the last d.p.s" << std::endl;
    std::auto_ptr< AIDA::Dev::IDevDataPointSet > dpsi( dpsf.createCopy( *dpsh ) );
    if ( ! dpsi.get() ) throw std::runtime_error( "Could not create a copy" );
    dpsi->scale( scaleFactor );
    dumpDataPointSetInfo( *dpsi, out );
    
    out << "Retrieving the weighted mean of the last two" << std::endl;
    std::auto_ptr< AIDA::Dev::IDevDataPointSet > dpsj( dpsf.weightedMean( *dpsi, *dpsh ) );
    if ( ! dpsj.get() ) throw std::runtime_error( "Could not retrieve the weighted mean of two compatible data point sets" );
    dpsj->setName( "dpsj" );
    dpsj->setTitle( "Title j" );
    dumpDataPointSetInfo( *dpsj, out );

    out << "Attemptimg to retrieve the weighted mean of two incompatible dps (in length)..." << std::endl;
    std::auto_ptr< AIDA::Dev::IDevDataPointSet > dpsW9( dpsf.weightedMean( *dpsc, *dpsb ) );
    if ( dpsW9.get() ) throw std::runtime_error( "Retrieved the weighted mean of two incompatible data point sets!!!" );
    out << "No effect, correct." << std::endl;

    out << "Attemptimg to retrieve the weighted mean of two incompatible dps (in dimension)..." << std::endl;
    std::auto_ptr< AIDA::Dev::IDevDataPointSet > dpsW10( dpsf.weightedMean( *dpsc, *dpsa ) );
    if ( dpsW10.get() ) throw std::runtime_error( "Retrieved the weighted mean of two incompatible data point sets!!!" );
    out << "No effect, correct." << std::endl;

    out << "Test succeeded!" << std::endl;
    return 0;
  }
  catch ( std::exception & e ) {
    fout << e.what() << std::endl;
    fout << "Test failed!" << std::endl;
    return 1;
  }

}


#include "AIDA/IAnnotation.h"

void
dumpDataPointSetInfo( const AIDA::Dev::IDevDataPointSet& dps, std::ostream& os )
{
  os << "******* DATA POINT SET ******** START *********************" << std::endl;
  os << "DataPointSet name : " << dps.name() << std::endl
     << "Type : " << dps.userLevelClassType() << std::endl
     << "Title : " << dps.title() << std::endl
     << "Dimension : " << dps.dimension() << std::endl
     << "Size : " << dps.size() << std::endl
     << std::endl;

  os << "Lower extent :";
  for ( int dim = 0; dim < dps.dimension(); ++dim ) os << " " << dps.lowerExtent( dim ) << " /";
  os << std::endl;

  os << "Upper extent :";
  for ( int dim = 0; dim < dps.dimension(); ++dim ) os << " " << dps.upperExtent( dim ) << " /";
  os << std::endl;

  os << "*** Annotation ** START ** "<< std::endl;
  const AIDA::IAnnotation& annotation = dps.annotation();
  for (  int i = 0; i < annotation.size(); ++i ) {
    os << annotation.key( i ) << " : " << annotation.value( i ) << std::endl;
  }
  os << "*** Annotation *** END ***" << std::endl << std::endl;

  os << "Elements :" << std::endl;
  for ( int i = 0; i < dps.size(); ++i ) {
    os << i << " : ";
    const AIDA::IDataPoint* dp = dps.point( i );
    for ( int dim = 0; dim < dp->dimension(); ++dim ) {
      const AIDA::IMeasurement* m = dp->coordinate( dim );
      os << " " << m->value() << "+" << m->errorPlus() << "-" << m->errorMinus() << " /";
    }
    os << std::endl;
  }
  os << std::endl;
  os << "******* DATA POINT SET ********* END **********************" << std::endl;
}
