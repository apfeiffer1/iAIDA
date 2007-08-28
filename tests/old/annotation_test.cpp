#include <iostream>
#include <stdexcept>
#include <memory>
#include "AIDA_Annotation/AIDA_Annotation.h"
#include <fstream>

void annotationDump( const AIDA::IAnnotation& annotation, std::ostream& os )
{
  os << "****** Annotation Contents ***** START ******" << std::endl;
  for ( int i = 0; i < annotation.size(); ++i ) {
    os << annotation.key( i ) << " : " << annotation.value( i ) << std::endl;
  }
  os << "****** Annotation Contents ****** END *******" << std::endl;
}

int main( int, char** ) {
  std::ofstream fout( "annotation_test.out" );

  try {
    fout << "Creating an annotation object" << std::endl;
    std::auto_ptr< AIDA::IAnnotation > annotation( new iAIDA::AIDA_Annotation::AIDA_Annotation );
    if ( ! annotation.get() )
      throw std::runtime_error("Could not create an annotation object");

    fout << "Adding two sticky items" << std::endl;
    if ( ! annotation->addItem( "Title", "title value", true ) )
      throw std::runtime_error("Could not add new item" );
    if ( ! annotation->addItem( "Name", "name value", true ) )
      throw std::runtime_error("Could not add new item" );

    fout << "Adding two non-sticky items" << std::endl;
    if ( ! annotation->addItem( "data1 key", "data1 value" ) )
      throw std::runtime_error("Could not add new item" );
    if ( ! annotation->addItem( "data2 key", "data2 value" ) )
      throw std::runtime_error("Could not add new item" );
    // Listing the annotation items
    annotationDump( *annotation, fout );

    fout << "Removing an item" << std::endl;
    if ( ! annotation->removeItem( "data1 key" ) )
      throw std::runtime_error("Could not remove an item" );
    fout << "Changing the value of an item" << std::endl;
    annotation->setValue( "Name", "new name" );
    // Listing the annotation items
    annotationDump( *annotation, fout );

    fout << "Adding another non-sticky item" << std::endl;
    if ( ! annotation->addItem( "data3 key", "data3 value" ) )
      throw std::runtime_error("Could not add new item" );
    fout << "Value of data2 key : " << annotation->value( "data2 key" ) << std::endl;
    fout << "Value of Title : " << annotation->value( "Title" ) << std::endl;
    // Listing the annotation items
    annotationDump( *annotation, fout );

    fout << "Making Title non-sticky and the data2 key sticky" << std::endl;
    annotation->setSticky("Title", false);
    annotation->setSticky("data2 key", true);
    fout << "Reseting the annotation" << std::endl;
    annotation->reset();
    // Listing the annotation items
    annotationDump( *annotation, fout );

    fout << "Test succeeded." << std::endl;
    return 0;
  }
  catch( std::exception& e ) {
    fout << e.what() << std::endl;
    fout << "Test failed." << std::endl;
    return 1;
  }
}
