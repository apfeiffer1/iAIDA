#include "AIDA_Fitter/AIDA_FitFactory.h"
#include "AIDA_Fitter.h"
#include "AIDA_FitData.h"

iAIDA::AIDA_Fitter::AIDA_FitFactory::AIDA_FitFactory()
{}

iAIDA::AIDA_Fitter::AIDA_FitFactory::~AIDA_FitFactory()
{}


AIDA::IFitData*
iAIDA::AIDA_Fitter::AIDA_FitFactory::createFitData()
{
  return new iAIDA::AIDA_Fitter::AIDA_FitData;
}


AIDA::IFitter*
iAIDA::AIDA_Fitter::AIDA_FitFactory::createFitter( const std::string& fitterType,
						const std::string&  engineType,
						const std::string& options)
{
  iAIDA::AIDA_Fitter::AIDA_Fitter* fitter = new iAIDA::AIDA_Fitter::AIDA_Fitter;
  if ( engineType != "" ) fitter->setEngine( engineType );
  if ( fitterType != "" ) fitter->setFitMethod( fitterType );
  // treat some control parameter from the options
  if ( options    != "")  fitter->setOptions(options); 
  
  return fitter;
}
