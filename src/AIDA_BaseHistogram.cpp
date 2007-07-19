#include "AIDA_BaseHistogram.h"
#include "AIDA/IAnnotation.h"
#include "AnnotationNumberFormater.h"

// titleKey is no more defined in IAnnotation for AIDA 3.2.1
static const std::string titleKey = "Title";  
static const std::string nameKey = "Name";
static const std::string entriesKey = "Entries";
static const std::string emptyString = "";
static const std::string thisClassName = "AIDA::IBaseHistogram";

iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::AIDA_BaseHistogram( const std::string& title,
								       const std::string& classType,
								       int dimension ):
  m_annotation( createAnnotation() ),
  m_classType( classType ),
  m_dimension( dimension ),
  m_upToDate( true )
{
  m_annotation->addItem( titleKey, title, true );
  m_annotation->addItem( nameKey, emptyString, true );
  m_annotation->addItem( entriesKey, emptyString, false );
}

iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::~AIDA_BaseHistogram()
{
  if ( m_annotation ) delete m_annotation;
}


std::string
iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::title() const
{
  return m_annotation->value( titleKey );
}

bool
iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::setTitle( const std::string& title )
{
  m_annotation->setValue( titleKey, title );
  return true;
}

std::string
iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::name() const
{
  return m_annotation->value( nameKey );
}

bool
iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::setName( const std::string& name )
{
  m_annotation->setValue( nameKey, name );
  return true;
}

AIDA::IAnnotation&
iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::annotation()
{
  updateAnnotation();
  return *m_annotation;
}

const
AIDA::IAnnotation&
iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::annotation() const
{
  updateAnnotation();
  return *m_annotation;
}


void
iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::updateAnnotation() const
{
  AIDA::IAnnotation& anno = const_cast< AIDA::IAnnotation& >( *m_annotation );
  anno.setValue( entriesKey, iAIDA_annotationNumberFormater.formatInteger( entries() ) );
}


AIDA::IAnnotation&
iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::annotationNoUpdate()
{
  return *m_annotation;
}


const AIDA::IAnnotation&
iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::annotationNoUpdate() const
{
  return *m_annotation;
}

//#include <memory>
//#include <stdexcept>
#include "AIDA_Annotation/AIDA_Annotation.h"

AIDA::IAnnotation*
iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::createAnnotation()
{
  /*
  std::auto_ptr<AIDA::Dev::IDevAnalysisFactory> af(dynamic_cast<AIDA::Dev::IDevAnalysisFactory*>(AIDA_createAnalysisFactory() ));
  if ( ! af.get() ) throw std::runtime_error( "Could not create an analysis factory" );
  AIDA::Dev::IAnnotationFactory* anf = af->annotationFactory();
  if ( ! anf ) throw std::runtime_error( "Could not find an annotation factory" );
  */
  return new iAIDA::AIDA_Annotation::AIDA_Annotation( );
}

bool 
iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::isNaN(double x) { 
  // check if a number is a NaN 
  if ( x >= 0 ) 
    return false;
  else if ( x < 0) 
    return false; 
  else 
    // NaN is when both cond. are satisfied 
    return true; 
}
  
