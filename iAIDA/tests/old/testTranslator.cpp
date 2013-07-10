#include <iostream>
#include <cstdlib>
#include <stdexcept>
//#include "AIDA_Dev/IDevAnalysisFactory.h"
//#include "AIDA_Dev/IStoreFactory.h"
//#include "AIDA_Dev/IStore.h"
//#include "AIDA_Dev/IBackingStore.h"
#include "AIDA_Dev/IBackingStore.h"
#include "AIDA_Dev/IDevTupleFactory.h"
#include "AIDA_Dev/IDevTuple.h"
#include "AIDA_Dev/ITupleHeader.h"
#include "AIDA_Dev/ITupleVariableDescription.h"
#include "AIDA_Dev/IDevTupleFactory.h"
#include "AIDA_Tuple_native/../src/TupleVariableDescriptionBuilder.h"
#include "AIDA_Tuple_native/AIDA_DevTupleFactory.h"

#include "AIDA_Histogram1D.h"
#include "AIDA_Cloud1D.h"
#include "AIDA_Histogram_native/AIDA_DevHistogramFactory.h"
#include "Translator.h"
#include "StoreTranslator.h"
#include "Histo1DTranslator.h"
#include "Cloud1DTranslator.h"
#include "TupleTranslator.h"
// for compression
//#include "gzstream.h"
//#include "AIDA_Dev/IDevHistogramFactory.h"
#include <fstream>
#include <string>
#include <memory>
#include <map>

// seal streams
#include "SealBase/File.h"
#include "SealBase/Filename.h"
#include "SealIOTools/StorageInputStream.h"
#include "SealIOTools/BufferInputStream.h"
#include "SealZip/GZIPInputStream.h"
#include "SealZip/BZIPInputStream.h"
#include "SealIOTools/StorageOutputStream.h"
#include "SealIOTools/BufferOutputStream.h"
#include "SealZip/GZIPOutputStream.h"
#include "SealZip/BZIPOutputStream.h"



#include "DummyBackingStore.h"


AIDA::IHistogram1D * createAndFillHisto() { 

    iAIDA::AIDA_Histogram_native::AIDA_Histogram1D * histo = new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D("my Histogram",100,0.,10.);

    AIDA::IHistogram1D * hist = dynamic_cast<AIDA::IHistogram1D *>(histo);

    // fill histo 
    for (int i = 0; i < 1000; ++i)  
      hist->fill(10*  static_cast<double>( std::rand() ) / RAND_MAX ); 
    
    return hist; 
}

AIDA::ICloud1D * createAndFillCloud() { 

    iAIDA::AIDA_Histogram_native::AIDA_Cloud1D * histo = new iAIDA::AIDA_Histogram_native::AIDA_Cloud1D("my Cloud");

    AIDA::ICloud1D * hist = dynamic_cast<AIDA::ICloud1D *>(histo);

    // fill histo 
    for (int i = 0; i < 1000; ++i)  
      hist->fill(10*  static_cast<double>( std::rand() ) / RAND_MAX ); 
    
    return hist; 
}


AIDA::ITuple* createAndFillTuple(  AIDA::Dev::IBackingStore& store,
				   AIDA::Dev::IDevTupleFactory& factory,
				   const std::string& tuplePath )
{
  std::ostream & os = std::cout; 

  os << "Creating a tuple..." << std::endl;
  AIDA::Dev::IDevTuple * tuple = factory.create( &store, "Example tuple" );
  if ( ! tuple ) throw std::runtime_error( "Could not create a tuple" );

  tuple->header().setPathInStore( tuplePath );

  const std::string description = "float a, b; double x,y,z; int i; bool l; ITuple {int n; float a} t1, t2; ITuple { int n; ITuple { int n1, n2 } st } t3; string s";
  // const std::string description = "float a, b; double x,y,z; int i; bool l; string s";
  iAIDA::AIDA_Tuple_native::TupleVariableDescriptionBuilder builder( factory );
  if ( ! builder.buildDescription( tuple->header(), description ) ) {
    throw std::runtime_error( "Could not build the description" );
  }

  if ( ! store.writeTupleHeader( tuple->header() ) ) {
    throw std::runtime_error( "Could not write the header of the tuple in the store" );
  }

  const int ia = tuple->findColumn( "a" );
  const int ib = tuple->findColumn( "b" );
  const int ix = tuple->findColumn( "x" );
  const int iy = tuple->findColumn( "y" );
  const int iz = tuple->findColumn( "z" );
  const int ii = tuple->findColumn( "i" );
  const int il = tuple->findColumn( "l" );
  
  const int it1 = tuple->findColumn( "t1" );
  const int it2 = tuple->findColumn( "t2" );
  const int it3 = tuple->findColumn( "t3" );
  
  const int is = tuple->findColumn( "s" );

  os << "Filling the tuple..." << std::endl;
  for ( int i = 0; i < 100; ++i ) {
    if ( ! tuple->fill( ia, static_cast<float>( i ) ) ) {
      std::ostringstream error;
      error << "Could not fill the tuple for variable " << tuple->columnName( ia ) << " and row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }

    if ( ! tuple->fill( ib, static_cast<float>( i + 1 ) ) ) {
      std::ostringstream error;
      error << "Could not fill the tuple for variable " << tuple->columnName( ib ) << " and row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }

    if ( ! tuple->fill( ix, static_cast<double>( 2*i ) ) ) {
      std::ostringstream error;
      error << "Could not fill the tuple for variable " << tuple->columnName( ix ) << " and row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }

    if ( ! tuple->fill( iy, static_cast<double>( 3*i ) ) ) {
      std::ostringstream error;
      error << "Could not fill the tuple for variable " << tuple->columnName( iy ) << " and row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }

    if ( ! tuple->fill( iz, static_cast<double>( 4*i ) ) ) {
      std::ostringstream error;
      error << "Could not fill the tuple for variable " << tuple->columnName( iz ) << " and row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }

    if ( ! tuple->fill( ii, static_cast<int>( i - 1 ) ) ) {
      std::ostringstream error;
      error << "Could not fill the tuple for variable " << tuple->columnName( ii ) << " and row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }

    if ( ! tuple->fill( il, ( ( i%3 == 0 ) ? true : false ) ) ) {
      std::ostringstream error;
      error << "Could not fill the tuple for variable " << tuple->columnName( il ) << " and row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }

     
    AIDA::ITuple* subTuple1 = tuple->getTuple(it1);
    if ( ! subTuple1 ) throw std::runtime_error( "Could not retrieve the subtuple" );
    const int itn1 = subTuple1->findColumn( "n" );
    const int ita1 = subTuple1->findColumn( "a" );
    for ( int j1 = 0; j1 < 2; ++j1 ) {
      if ( ! subTuple1->fill( itn1, j1 ) ) throw std::runtime_error( "Could not fill the subtuple" );
      if ( ! subTuple1->fill( ita1, static_cast<float>( 2*j1 ) ) ) throw std::runtime_error( "Could not fill the subtuple" );
      if ( ! subTuple1->addRow() ) throw std::runtime_error( "Could not add a row in the subtuple" );
    }

    AIDA::ITuple* subTuple2 = tuple->getTuple(it2);
    if ( ! subTuple2 ) throw std::runtime_error( "Could not retrieve the subtuple" );
    const int itn2 = subTuple2->findColumn( "n" );
    const int ita2 = subTuple2->findColumn( "a" );
    for ( int j2 = 0; j2 < 5; ++j2 ) {
      if ( ! subTuple2->fill( itn2, 3*j2 ) ) throw std::runtime_error( "Could not fill the subtuple" );
      if ( ! subTuple2->fill( ita2, static_cast<float>( 4*j2 ) ) ) throw std::runtime_error( "Could not fill the subtuple" );
      if ( ! subTuple2->addRow() ) throw std::runtime_error( "Could not add a row in the subtuple" );
    }

    AIDA::ITuple* subTuple3 = tuple->getTuple(it3);
    if ( ! subTuple3 ) throw std::runtime_error( "Could not retrieve the subtuple" );
    const int in = subTuple3->findColumn( "n" );
    const int ist = subTuple3->findColumn( "st" );
    for ( int j3 = 0; j3 < 3; ++j3 ) {
      if ( ! subTuple3->fill( in, j3 ) ) throw std::runtime_error( "Could not fill the subtuple" );

      AIDA::ITuple* subTuple = subTuple3->getTuple(ist);
      if ( ! subTuple ) throw std::runtime_error( "Could not retrieve the sub-subtuple" );
      const int n1 = subTuple->findColumn( "n1" );
      const int n2 = subTuple->findColumn( "n2" );
      for ( int j4 = 0; j4 < 2; ++j4 ) {
	if ( ! subTuple->fill( n1, 10*j3 + j4 ) ) throw std::runtime_error( "Could not fill the sub-subtuple" );
	if ( ! subTuple->fill( n2, 100*j3 + j4 ) ) throw std::runtime_error( "Could not fill the sub-subtuple" );
	if ( ! subTuple->addRow() ) throw std::runtime_error( "Could not add a row in the sub-subtuple" );
      }

      if ( ! subTuple3->addRow() ) throw std::runtime_error( "Could not add a row in the subtuple" );
    }
    

    std::ostringstream oss;
    oss << "string_" << i << std::ends;
    const std::string s = oss.str();
    if ( ! tuple->fill( is, s ) ) {
      std::ostringstream error;
      error << "Could not fill the tuple for variable " << tuple->columnName( is ) << " and row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }

    if ( ! tuple->addRow() ) {
      std::ostringstream error;
      error << "Could not add in the tuple row " << i << std::ends;
      throw std::runtime_error( error.str() );
    }
  }

  os << "Rows written : " << tuple->rows() << std::endl;

  return tuple;
}

    
void writeHisto(const AIDA::IHistogram1D & h, const std::string & name, const std::string & path, const std::string & storeName) { 

    std::ostream& fout = std::cout;

    //  uncompress 
    //std::ofstream stream(storeName.c_str());
    // compress 
    //ogzstream stream(storeName.c_str());

    // uncompress using seal streams
    seal::File   output (storeName.c_str(), seal::IOFlags::OpenWrite
			 | seal::IOFlags::OpenCreate | seal::IOFlags::OpenTruncate ); 

    seal::StorageOutputStream soutput (&output);
    seal::BufferOutputStream  boutput (&soutput);


    iAIDA::AIDA_XMLStore::StoreTranslator * translator = 
      new iAIDA::AIDA_XMLStore::StoreTranslator(boutput); 

    fout << "Store created ..." << std::endl;

    translator->append(h,name,path); 
    fout << "Histogram appended ..." << std::endl;

    translator->write(); 
    fout << "File written ..." << std::endl;

    boutput.close();
    delete translator; 

}
 
void writeCloud(const AIDA::ICloud1D & h, const std::string & name, const std::string & path, const std::string & storeName) { 

    std::ostream& fout = std::cout;

    //  uncompress 
    //std::ofstream stream(storeName.c_str());
    // compress 
    //ogzstream stream(storeName.c_str());

    // uncompress using seal streams
    seal::File   output (storeName.c_str(), seal::IOFlags::OpenWrite
			 | seal::IOFlags::OpenCreate | seal::IOFlags::OpenTruncate ); 

    seal::StorageOutputStream soutput (&output);
    seal::BufferOutputStream  boutput (&soutput);

    iAIDA::AIDA_XMLStore::StoreTranslator * translator = 
      new iAIDA::AIDA_XMLStore::StoreTranslator(boutput); 

    fout << "Store created ..." << std::endl;

    translator->append(h,name,path); 
    fout << "Object appended ..." << std::endl;

    translator->write(); 
    fout << "File written ..." << std::endl;

    boutput.close();
    delete translator; 

}


 
void writeTuple(AIDA::ITuple & t, const std::string & name, const std::string & path, const std::string & storeName) { 

    std::ostream& fout = std::cout;

    //  uncompress 
    std::ofstream stream(storeName.c_str());
    // compress 
    //ogzstream stream(storeName.c_str());

    // uncompress using seal streams
    seal::File output (storeName.c_str(), seal::IOFlags::OpenWrite
			 | seal::IOFlags::OpenCreate | seal::IOFlags::OpenTruncate ); 
    seal::StorageOutputStream soutput (&output);
    seal::BufferOutputStream  boutput (&soutput);

    iAIDA::AIDA_XMLStore::StoreTranslator * translator = 
      new iAIDA::AIDA_XMLStore::StoreTranslator(boutput); 

    fout << "Store created ..." << std::endl;

    translator->append(t,name,path); 
    fout << "Object appended ..." << std::endl;

    translator->write(); 
    fout << "File written ..." << std::endl;

    boutput.close();
    delete translator; 

}

bool testHisto () { 

  std::ostream& fout = std::cout;

  std::string storeName("testHisto1.xml");

  AIDA::IHistogram1D * hist = createAndFillHisto(); 

  // test writing 

  std::string name="testHisto1D";
  std::string path="testDir/testName";    
  
  writeHisto(*hist,name,path,storeName);


  // Test Reading 

  fout << "Test reading now..." << std::endl;

  // uncompress
  //std::ifstream inputStream(storeName.c_str());
  // compress file
  //igzstream inputStream(storeName.c_str());

  // uncompress using seal streams

  // use now seal streams
  seal::File       input (storeName.c_str());
  seal::StorageInputStream sinput (&input);
  seal::BufferInputStream  binput (&sinput);

  iAIDA::AIDA_XMLStore::StoreTranslator * translator2 = 
    new iAIDA::AIDA_XMLStore::StoreTranslator(binput); 
  
  fout << "Store created ..." << std::endl;
  
  DataXML::DataObject aida_element; 
  translator2->read(aida_element); 
  const std::vector<DataXML::DataObject> elements = aida_element.children();   
  fout << "File read ... " << std::endl;
  fout << " number of objects found is  " << elements.size() << std::endl;

  for (std::vector<DataXML::DataObject>::const_iterator i = elements.begin(); i != elements.end(); ++i) { 
    fout << " Found element " << i->name() << std::endl;
    if (i->name() == "histogram1d") {
      iAIDA::AIDA_XMLStore::Histo1DTranslator ht(&(*i)); 
      iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory factory; 
      AIDA::Dev::IDevHistogram1D * h = ht.createFromXML(factory); 
      fout << " histogram read " << std::endl; 
      fout << " histo title " << h->title() << std::endl; 
	fout << " histo mean " << h->mean() << std::endl; 
	fout << " histo entries " << h->entries() << std::endl; 
      
	// now try to write back histogram 
	writeHisto(*h,"histoCopy","/histoCopy","testHisto2.xml");

    }
  }

  fout << "Test Histo succeeded !!!" << std::endl;
  return true;
}



bool testCloud () { 

  std::ostream& fout = std::cout;

  std::string storeName("testCloud1.xml");


  AIDA::ICloud1D * cloud = createAndFillCloud(); 
  // Test Reading 


  writeCloud(*cloud,"testCloud","/testCloud",storeName); 

  fout << "Test reading now..." << std::endl;

  // uncompress
  //std::ifstream inputStream(storeName.c_str());
  // compress file
  //igzstream inputStream(storeName.c_str());
  seal::File       input (storeName.c_str());
  seal::StorageInputStream sinput (&input);
  seal::BufferInputStream  binput (&sinput);
  


  iAIDA::AIDA_XMLStore::StoreTranslator * translator2 = 
    new iAIDA::AIDA_XMLStore::StoreTranslator(binput); 
  
  fout << "Store created ..." << std::endl;
  
  DataXML::DataObject aida_element; 
  translator2->read(aida_element); 
  const std::vector<DataXML::DataObject> elements = aida_element.children();   
  fout << "File read ... " << std::endl;
  fout << " number of objects found is  " << elements.size() << std::endl;

  for (std::vector<DataXML::DataObject>::const_iterator i = elements.begin(); i != elements.end(); ++i) { 
    fout << " Found element " << i->name() << std::endl;
    if (i->name() == "cloud1d") {
      iAIDA::AIDA_XMLStore::Cloud1DTranslator ht(&(*i)); 
      iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory factory; 
      AIDA::Dev::IDevCloud1D * h = ht.createFromXML(factory); 
      fout << " cloud read " << std::endl; 
      fout << " cloud title " << h->title() << std::endl; 
	fout << " cloud mean " << h->mean() << std::endl; 
	fout << " cloud entries " << h->entries() << std::endl; 
      
	// now try to write back histogram 
	writeCloud(*h,"cloudCopy","/cloudCopy","testCloud2.xml");

    }
  }

  fout << "Test Cloud succeeded !!!" << std::endl;
  return true;
}

bool testTuple() { 

std::cout << "Creating a tuple factory..." << std::endl;

    AIDA::Dev::IDevTupleFactory& factory = *(new iAIDA::AIDA_Tuple_native::AIDA_DevTupleFactory());

    std::cout << "Creating a dummy backing store..." << std::endl;
    std::auto_ptr<AIDA::Dev::IBackingStore> store( new DummyBackingStore( factory ) );
    if ( ! store.get() ) throw std::runtime_error( "Could not create a dummy store" );

    const std::string tuplePath = "/dir1/tuple1";

    AIDA::ITuple* tuple = createAndFillTuple( *store, factory, tuplePath );
 
    writeTuple(*tuple,"myTuple",tuplePath,"testTuple.xml");

    // test now the reading 

  std::cout << "Test reading now..." << std::endl;
  std::string storeName = "testTuple.xml"; 

  // uncompress
  //std::ifstream inputStream("testTuple.xml");
  // compress file
  //igzstream inputStream(storeName.c_str());
  //if (! inputStream ) { 
  //  std::cout  << "Error opening the file " << std::endl;  
  //  return false; 
  // }

  seal::File       input (storeName.c_str());
  seal::StorageInputStream sinput (&input);
  seal::BufferInputStream  binput (&sinput);

  iAIDA::AIDA_XMLStore::StoreTranslator * translator = 
    new iAIDA::AIDA_XMLStore::StoreTranslator(binput); 
  
  // get the root element of the store parent to all objects (aida element) 
  DataXML::DataObject rootElement; 
  if (!translator->read(rootElement)) { 
    std::cout  << "Error reading the file " << std::endl;  
    return false;
  }

  const std::vector<DataXML::DataObject> elements = rootElement.children();

  // check if any object is preset
  if (elements.size() == 0) return false; 
  
  AIDA::ITuple * readTuple = 0; 

  std::cout << "Creating a new dummy backing store..." << std::endl;
  std::auto_ptr<AIDA::Dev::IBackingStore> store2( new DummyBackingStore( factory ) );
  if ( ! store2.get() ) throw std::runtime_error( "Could not create a dummy store" );


  // loop on all elements and build map of types and objects
  for (std::vector<DataXML::DataObject>::const_iterator iObj = elements.begin(); iObj != elements.end(); ++iObj) { 
    if (iObj->name() == "tuple") { 
      iAIDA::AIDA_XMLStore::TupleTranslator t(&(*iObj)); 
      readTuple = t.createFromXML(factory,store2.get()); 
    }
  }


  if ( ! readTuple) {  
    std::cout << " Error reading Tuple " << std::endl; 
    return false; 
  }

  std::cout << " Number of tuple columns " << readTuple->columns() << std::endl; 
  std::cout << " Number of tuple rows " << readTuple->rows() << std::endl; 

  std::vector<std::string> colNames; 
  std::vector<std::string> colTypes; 
  for (int i = 0; i < readTuple->columns() ;  ++i)
    std::cout << " column " << readTuple->columnName(i) << " " 
	      <<  readTuple->columnType(i) << std::endl; 


  return true;
}





int main(int, char**) {
  std::ostream& fout = std::cout;

  try {
    
    if (testHisto()) 
      fout << " Test Histogram passed " << std::endl; 
    else 
      fout << "Error :  Test Histogram Failed !!! " << std::endl; 

    if (testCloud()) 
      fout << " Test Cloud passed " << std::endl; 
    else 
      fout << "Error :  Test Cloud Failed !!!!" << std::endl; 

    if (testTuple()) 
      fout << " Test Tuple passed " << std::endl; 
    else 
      fout << "Error :  Test Tuple Failed !!!! " << std::endl; 


    fout << "Full Test succeeded !!!" << std::endl;
    return 0;
  }
  catch( std::exception & e ) {
    fout << e.what() << std::endl;
    return 1;
  }
}

#include "DummyBackingStore.cpp"
#include "DummyPersistentTuple.cpp"
#include "DummyTupleData.cpp"
