#include <iostream>
#include <memory>
#include <stdexcept>
#include <fstream>

#include "AIDA_Histogram_native/AIDA_DevHistogramFactory.h"
#include "HistogramUnitTester.h"

int main( int, char** )
{
  std::ofstream fout( "histo_test.out" );
  try {
    std::auto_ptr< AIDA::Dev::IDevHistogramFactory > hfdev( new iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory );
    if ( ! hfdev.get() ) throw std::runtime_error( "No dev Histogram factory created" );
    HistogramUnitTester tester( *hfdev );

    fout << "Testing 1d histograms..." << std::endl;
    std::ofstream fh1d( "fh1d.out" );
    tester.testHistogram1D( fh1d );
    fh1d.flush();

    fout << "Testing merging of histograms..." << std::endl;
    std::ofstream fmh( "fmh.out" );
    tester.testMergeHistogram( fmh );
    fmh.flush();

    fout << "Testing 2d histograms..." << std::endl;
    std::ofstream fh2d( "fh2d.out" );
    tester.testHistogram2D( fh2d );
    fh2d.flush();

    fout << "Testing 3d histograms..." << std::endl;
    std::ofstream fh3d( "fh3d.out" );
    tester.testHistogram3D( fh3d );
    fh3d.flush();

    fout << "Testing 1d clouds..." << std::endl;
    std::ofstream fc1d( "fc1d.out" );
    tester.testCloud1D( fc1d );
    fc1d.flush();

    fout << "Testing 2d clouds..." << std::endl;
    std::ofstream fc2d( "fc2d.out" );
    tester.testCloud2D( fc2d );
    fc2d.flush();

    fout << "Testing 3d clouds..." << std::endl;
    std::ofstream fc3d( "fc3d.out" );
    tester.testCloud3D( fc3d );
    fc3d.flush();

    fout << "Testing 1d profiles..." << std::endl;
    std::ofstream fp1d( "fp1d.out" );
    tester.testProfile1D( fp1d );
    fp1d.flush();

    fout << "Testing 2d profiles..." << std::endl;
    std::ofstream fp2d( "fp2d.out" );
    tester.testProfile2D( fp2d );
    fp2d.flush();

    fout << "Testing histogram projections..." << std::endl;
    std::ofstream fproj( "fproj.out" );
    tester.testProjections( fproj );
    fproj.flush();

    fout << "Testing histogram slices..." << std::endl;
    std::ofstream fsl( "fsl.out" );
    tester.testSlices( fsl );
    fsl.flush();

    fout << "Testing cloud-to-histogram projections..." << std::endl;
    std::ofstream fch( "fch.out" );
    tester.testCloudToHistogram( fch );
    fch.flush();

    fout << "Test succeeded!" << std::endl;
    return 0;
  }
  catch ( std::exception & e ) {
    fout << e.what() << std::endl;
    fout << "Test failed!" << std::endl;
    return 0;
  }
}
