#include <iostream>
#include <cstdlib>
#include <memory>

#include "AIDA/AIDA.h"

// Class declarations from CLHEP
#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Random/DRand48Engine.h"


int main( int, char** )
{
  // Creating the analysis factory
  std::auto_ptr<AIDA::IAnalysisFactory> af( AIDA_createAnalysisFactory() );

  // Creating the tree factory
  std::auto_ptr<AIDA::ITreeFactory> tf( af->createTreeFactory() );

  // Creating a tree mapped to an hbook file
  bool readOnly = false;
  bool createNew = true;
  std::auto_ptr<AIDA::ITree> tree( tf->create( "tuple.hbook", "hbook", readOnly, createNew, "lrec = 2048" ) );

  // Creating a tuple factory associated to the tree
  std::auto_ptr<AIDA::ITupleFactory> tpf( af->createTupleFactory( *tree ) );

  // Defining the description of the tuple columns
  std::string description = "int nTracks; double beamEnergy; bool goodTrigger;";
  description += "Tuple{ double px } px;";
  description += "Tuple{ double py } py;";
  description += "Tuple{ double pz } pz;";
  description += "Tuple{ double mass } mass";

  // Options (HBOOK specific)
  std::string options = "nTracks[0,100] px(nTracks) py(nTracks) pz(nTracks) mass(nTracks)";

  // Creating the actual tuple (HBOOK CWN)
  AIDA::ITuple& tuple = *( tpf->create( "100", "example tuple", description, options ) );

  DRand48Engine randomEngine;
  RandGauss rBeamEnergy( randomEngine, 90, 5 );
  RandGauss rTracksSpread( randomEngine, 0, 2 );
  RandGauss rMomentum( randomEngine, 0, 3 );
  RandGauss rMass( randomEngine, 1, 0.1 );

  // Filling the tuple
  int i_nTracks = tuple.findColumn( "nTracks" );
  int i_beamEnergy = tuple.findColumn( "beamEnergy" );
  int i_goodTrigger = tuple.findColumn( "goodTrigger" );
  int i_px = tuple.findColumn( "px" );
  int i_py = tuple.findColumn( "py" );
  int i_pz = tuple.findColumn( "pz" );
  int i_mass = tuple.findColumn( "mass" );

  const double tracksPerEnergy = 0.5;
  for ( unsigned int i = 0; i < 10000; ++i ) {
    double beamEnergy = rBeamEnergy.fire();
    int numberOfTracks = static_cast<int>( tracksPerEnergy * beamEnergy + rTracksSpread.fire() );
    if ( numberOfTracks < 0 ) numberOfTracks = 0;
    if ( numberOfTracks > 100 ) numberOfTracks = 100;
    tuple.fill( i_nTracks, numberOfTracks );
    tuple.fill( i_beamEnergy, beamEnergy );
    tuple.fill( i_goodTrigger, ( (rMomentum.fire() > 0)?true:false) );
    AIDA::ITuple* tpx = tuple.getTuple( i_px );
    AIDA::ITuple* tpy = tuple.getTuple( i_py );
    AIDA::ITuple* tpz = tuple.getTuple( i_pz );
    AIDA::ITuple* tmass = tuple.getTuple( i_mass );

    for ( int iTrack = 0; iTrack < numberOfTracks; ++iTrack ) {
      tpx->fill( 0,  rMomentum.fire() );
      tpx->addRow();
      tpy->fill( 0,  rMomentum.fire() );
      tpy->addRow();
      tpz->fill( 0,  rMomentum.fire() );
      tpz->addRow();
      tmass->fill( 0, rMass.fire() );
      tmass->addRow();
    }

    tuple.addRow();
  }

  std::cout << "Filled the tuple with " << tuple.rows() << " rows" << std::endl;

  // committing and closing the file
  tree->commit();
  tree->close();

  return 0;
}
