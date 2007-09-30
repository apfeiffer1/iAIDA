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
