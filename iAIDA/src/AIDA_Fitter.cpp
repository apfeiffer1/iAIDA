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

 /**********************************************************************
  *                                                                    *
  * Copyright (c) 2002 Lorenzo Moneta, CERN/IT                       *
  *                   <Lorenzo.Moneta.cern.ch>                       *
  *                                                                    *
  * This library is free software; you can redistribute it and/or      *
  * modify it under the terms of the GNU Lesser General Public         *
  * License as published by the Free Software Foundation; either       *
  * version 2.1 of the License, or (at your option) any later version. *
  *                                                                    *
  * This library is distributed in the hope that it will be useful,    *
  * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   *
  * Lesser General Public License for more details.                    *
  *                                                                    *
  * You should have received a copy of the GNU Lesser General Public   *
  * License along with this library (see file COPYING); if not, write  *
  * to the Free Software Foundation, Inc., 59 Temple Place, Suite      *
  * 330, Boston, MA 02111-1307 USA, or contact the author.             *
  *                                                                    *
  **********************************************************************/

// Implementation file for class AIDA_Fitter
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Thu Oct  3 15:23:33 2002
// 
// Last update: Thu Oct  3 15:23:33 2002
// 

#include "AIDA_Fitter.h"
#include "AIDA_FitParameterSettings.h"
#include "AIDA_FitResult.h"
#include "AIDA_FitData.h"
#include "FitterOptionParser.h"
#include "FMLFitDataIteratorAdapter.h"
# include "IDevFitDataIteratorFMLBase.h"

#include "AIDA_Function/FMLFunctionAdapter.h"
#include "AIDA_Function/AIDA_DevFunctionFactory.h"
#include "AIDA_Function/AIDA_FunctionCatalog.h"
#include "AIDA_Function/AIDA_FunctionAdapter.h"
#include "AIDA_Function/AIDA_RangeSet.h"

#include "FML/FunctionCatalogue.h"
#include "FML/RangeSet.h"

#include "FML/Fitter.h"
#include "FML/Util/debug.h"
#include "FML/Util/util.h"

#include "AIDA/IBaseHistogram.h"
#include "AIDA/IHistogram1D.h"
#include "AIDA/IHistogram2D.h"
#include "AIDA/IHistogram3D.h"
#include "AIDA/IProfile1D.h"
#include "AIDA/IProfile2D.h"
#include "AIDA/ICloud1D.h"
#include "AIDA/ICloud2D.h"
#include "AIDA/ICloud3D.h"
#include "AIDA/IDataPointSet.h"


namespace iAIDA  { 
  namespace AIDA_Fitter { 


AIDA_Fitter::AIDA_Fitter() 
{
  m_fitter = new fml::Fitter();
}

AIDA_Fitter::~AIDA_Fitter() 
{
  // delete parameter settings 
  for (FitParMap::iterator itr = m_parmap.begin(); itr != m_parmap.end(); ++itr)  
    delete itr->second; 

  m_parmap.clear(); 
  delete m_fitter; 
}

AIDA_Fitter::AIDA_Fitter(const AIDA_Fitter &) 
{
}

AIDA_Fitter & AIDA_Fitter::operator = (const AIDA_Fitter &rhs) 
{
   if (this == &rhs) return *this;  // time saving self-test

   return *this;
}

/// Fit an IFunction to an IFitData

AIDA::IFitResult * AIDA_Fitter::fit(AIDA::IFitData & d, AIDA::IFunction & f)
{
  // check dimensionality
  if (d.dimension() != f.dimension()) { 
    INFO_MSG("AIDA_Fitter: dimension mismatch: between function and data");
    return 0;
  }
  AIDA::IFitResult * result = 0; 

  // if f is a native FML function we don't need the adapter
  fml::IFMLFunction *fml_fun = dynamic_cast<fml::IFMLFunction*>(&f);
  
  if(fml_fun)
	result = fit(d, *fml_fun); 

  // f is not an FML function 
  else
    {
      
      // f needs to be a DeV::ModelFunction 
      // t.b.d : add support for all AIDA Model functions
      AIDA::Dev::IDevModelFunction *dmf = dynamic_cast<AIDA::Dev::IDevModelFunction*>(&f);
      if(!dmf) { 
	INFO_MSG("AIDA_Fitter::Function is not a AIDA::Dev::IDevModelFunction");
	return 0;
      }
      
      

      // if is a iAIDA::AIDA_Function::AIDA_FunctionAdapter is still NOT a foreign implementation 
      
      iAIDA::AIDA_Function::AIDA_FunctionAdapter * aida_fun_adapter = dynamic_cast<iAIDA::AIDA_Function::AIDA_FunctionAdapter *>(dmf);
      if (aida_fun_adapter) {
	fml_fun = aida_fun_adapter->getImplementation();
	if(!fml_fun) return 0; 
	// fit 
	result = fit(d, *fml_fun); 
      }
      else { 
	// create a FMLFunctionAdapter from a foreight AIDA::Dev;:IModelFunction

      // As suggested by Kuba 
      // I need to pass a function fuctory to the adapter. 
      // To do: function factory should be a singleton to be used 
      // in other classes of the package

	 iAIDA::AIDA_Function::AIDA_DevFunctionFactory * fun_factory = new iAIDA::AIDA_Function::AIDA_DevFunctionFactory(); 
	 // create function adapter (we retain owhnership of the function) 
	 iAIDA::AIDA_Function::FMLFunctionAdapter * ff = new iAIDA::AIDA_Function::FMLFunctionAdapter(dmf,fun_factory);

	 result = fit(d, *ff); 
	 delete ff;
	 delete fun_factory;
      }

    }
      
  // after fitting update annotation of AIDA::IFunction (it is a fitted function)
  AIDA_FitResult * aida_result = dynamic_cast<AIDA_FitResult * >(result); 
  if (aida_result) aida_result->fillAnnotation(&f);      
  return result;
    
}

/// private method used by all others to call FML:Fitter to fit

AIDA::IFitResult * AIDA_Fitter::fit(AIDA::IFitData& d, fml::IFMLFunction &f)
{

  // check first the dimension 
  if (d.dimension() != f.dimension()  ) { 
    INFO_MSG("AIDA_Fitter: dimension mismatch betwwen function and data ");
    return 0; 
  }
      

  AIDA::Dev::IDevFitData * dd = dynamic_cast<AIDA::Dev::IDevFitData *>(&d);
  
  if(!dd) { 
    INFO_MSG("AIDA_Fitter: FitData is not a AIDA::Dev::IDevFitData ");
    return 0;
  }

  AIDA::Dev::IDevFitDataIterator* aida_it = dd->dataIterator();
  if (!aida_it) { 
    INFO_MSG("AIDA_Fitter: Cannot get data iterator from FitData ");
    return 0;       
  }

  // check fit type 

  // operations for unbinned fits 
 
  if (dd->fitType() == AIDA::Dev::IDevFitter::UNBINNED_FIT) { 
    if (fitMethodName() != "UnbinnedML") { 
    // set method 
      INFO_MSG( "Warning: AIDA_Fitter: Setting fit type to UNBINNED" );
      setFitMethod("UnbinnedML");
      m_fitter->fitConfig().setErrorUP(0.5);
    }
    // check if function is normalizable
    if (!f.providesNormalization()) {
      INFO_MSG( " AIDA_Fitter: Warning - function does not provide normalization by itself - Be sure it is "); 
      //      return 0; 
    }
    // normalize
    f.normalize(true); 

    // set ranges 
    for (int i = 0; i < f.dimension(); ++i) { 
      AIDA::IRangeSet & r = dd->range(i);
      // dynamic cast to implementation
      iAIDA::AIDA_Function::AIDA_RangeSet * r_impl = dynamic_cast< iAIDA::AIDA_Function::AIDA_RangeSet *> (&r); 
      if (r_impl != 0 ) { 
	fml::RangeSet r_fml = r_impl->getFMLRangeSet();
	bool lret = f.setNormalizationRange(i,r_fml);
	if (lret) { 
	  std::string text = "";
	  for (int j = 0; j <  r_fml.size(); ++j)
	    text = "( " + Util::to_string(r_fml.lowerBounds()[j]) + " , " +  Util::to_string(r_fml.upperBounds()[j]) + " ) "; 
					 
	  //INFO_MSG( " Setting Function range to " + text);
	}
	else 
	  INFO_MSG( "AIDA_Fitter: Error setting a new iAIDA::FMLFunction range" );
      }
      else      
	INFO_MSG(" Error: Cannot get AIDA_Function::RangeSet - range cannot be set on functions "); 
    }
  }

  // operations for ML binned fits 
  if (fitMethodName() == "PoissonML")
      m_fitter->fitConfig().setErrorUP(0.5);
  
  IDevFitDataIteratorFMLBase *base_it = dynamic_cast<IDevFitDataIteratorFMLBase*>(aida_it);
  
  if(base_it)
    {
      // home implementation, use a faster path
      
      std::auto_ptr<fml::FitResult> r(m_fitter->fit(base_it->getIFMLDataIterator(), f));

      if (! r->isValid() ) return 0; 
 
      // who deletes fit result ? - user manages 
      return new AIDA_FitResult(r);

    }
  else // foregin implementation, use adapter
    {
      FMLFitDataIteratorAdapter it(aida_it);
      
      std::auto_ptr<fml::FitResult> r(m_fitter->fit(it, f));

      if (! r->isValid() ) return 0; 

      return new AIDA_FitResult(r);     
    }
  
}

/// Shortcut to fit an IBaseHistogram directly with an IFunction.

AIDA::IFitResult * AIDA_Fitter::fit(AIDA::IBaseHistogram & h, AIDA::IFunction & f) 
{
     // do volutaruy memory leak to avoid error with autopointers
     //AIDA_FitData * d = new AIDA_FitData();        
     AIDA_FitData d;
     if(!connect(d,h )) return 0;  

     // check dimensions..
     if (d.dimension() != f.dimension() ) { 
       INFO_MSG("AIDA_Fitter: dimension mismatch: between function and data");
       return 0;
     }

     // need to copy range of function in IFitData for unbinned fits and 
     if (d.fitType() == AIDA::Dev::IDevFitter::UNBINNED_FIT) {
     // function must be of type IModelFunction
      
       AIDA::IModelFunction * mf = dynamic_cast<AIDA::IModelFunction * > (&f); 
       if (mf) { 
       
	 for (int i = 0; i < d.dimension(); ++i) { 
	   AIDA::IRangeSet & range_d = d.range(i); 
	   range_d.excludeAll(); 
	   AIDA::IRangeSet & range_fun = mf->normalizationRange(i); 
	   // loop on function ranges 
	   for (int j = 0; j < range_fun.size(); ++j)
	     range_d.include(range_fun.lowerBounds()[j],range_fun.upperBounds()[j] ); 
	 }
       }
     }


     return fit(d,f);
}

/// Shortcut to fit an IBaseHistogram directly with predefined model

AIDA::IFitResult * AIDA_Fitter::fit(AIDA::IBaseHistogram & h, const std::string& model) 
{        
      std::auto_ptr<fml::IFMLFunction> f(fml::FunctionCatalogue::createFunction(model));
      if(!f.get()) return 0;

      AIDA_FitData d;
      if(!connect(d,h )) return 0; 

      return fit(d,*f.get());
}

/// Shortcut to fit an IBaseHistogram directly with model and initial parameters 


AIDA::IFitResult * AIDA_Fitter::fit(AIDA::IBaseHistogram & h, const std::string& model, const std::vector<double>  & initialParameters )
{ 
      std::auto_ptr<fml::IFMLFunction> f(fml::FunctionCatalogue::createFunction(model));
      if(!f.get()) return 0;

      f->setParameters(initialParameters); 

      AIDA_FitData d;
      if(!connect(d,h )) return 0; 

      return fit(d,*f.get()); 
}

/// Shortcut to fit an IDataPointSet directly with an IFunction.

AIDA::IFitResult * AIDA_Fitter::fit(AIDA::IDataPointSet & dps, AIDA::IFunction & f) 
{
     AIDA_FitData d;
     if(!connect(d,dps )) return 0; 
     
     return fit(d,f);
}

/// Shortcut to fit directly with predefined model

AIDA::IFitResult * AIDA_Fitter::fit(AIDA::IDataPointSet & dps, const std::string& model) 
{
        
      std::auto_ptr<fml::IFMLFunction> f(fml::FunctionCatalogue::createFunction(model));
      if(!f.get()) return 0;

      AIDA_FitData d;
      if(!connect(d,dps )) return 0; 

      return fit(d,*f.get());
}

/// Shortcut to fit directly with model and initial parameters 


AIDA::IFitResult * AIDA_Fitter::fit(AIDA::IDataPointSet & dps, const std::string& model, const std::vector<double>  & initialParameters )
{ 
      std::auto_ptr<fml::IFMLFunction> f(fml::FunctionCatalogue::createFunction(model));

      if(!f.get()) return 0;

      f->setParameters(initialParameters); 

      AIDA_FitData d;
      if(!connect(d,dps )) return 0; 

      return fit(d,*f.get()); 

}


 /// Shortcut for the predefined models.

AIDA::IFitResult * AIDA_Fitter::fit(AIDA::IFitData & d, const std::string& model ) 
{
      std::auto_ptr<fml::IFMLFunction> f(fml::FunctionCatalogue::createFunction(model));
      if(!f.get()) return 0;

      return fit(d,*f.get());
}
    
AIDA::IFitResult * AIDA_Fitter::fit(AIDA::IFitData & d, const std::string& model, const std::vector<double>  & initialParameters) 
{ 
      std::auto_ptr<fml::IFMLFunction> f(fml::FunctionCatalogue::createFunction(model));
      if(!f.get()) return 0;
      f->setParameters(initialParameters); 

      return fit(d,*f.get());
}


bool AIDA_Fitter::setEngine(const std::string& name)
{
      return m_fitter->fitConfig().setEngine(name);
}

std::string AIDA_Fitter::engineName()
{
      return m_fitter->fitConfig().engineName();
}

AIDA::IFitParameterSettings & AIDA_Fitter::fitParameterSettings(const std::string& name)
{
  AIDA_FitParameterSettings * ps = new AIDA_FitParameterSettings(&m_fitter->fitConfig().fitParameterSettings(name));
      m_parmap[name] = ps;
      return *m_parmap[name];
}

std::vector<std::string>  AIDA_Fitter::listParameterSettings()
{
      return m_fitter->fitConfig().listParameterSettings();
}

void AIDA_Fitter::resetParameterSettings()
{
      return m_fitter->fitConfig().resetParameterSettings();
}

bool AIDA_Fitter::setConstraint(const std::string& /* expr */ )
{
      // to implement this you must create a parser for constraint expressions
      // create a Linear or NonlinearConstraint object and here you go - the rest is done.
      INFO_MSG("AIDA_Fitter::setConstraint not implemented");
      return false; 
}

std::vector<std::string>   AIDA_Fitter::constraints()
{
     return m_constr;       
}

void AIDA_Fitter::resetConstraints()
{
}

bool AIDA_Fitter::setFitMethod(const std::string& name)
{
      return m_fitter->fitConfig().setFitMethod(name);
}

std::string AIDA_Fitter::fitMethodName()
{
      return m_fitter->fitConfig().fitMethodName();
}

AIDA::IDataPointSet * AIDA_Fitter::createScan1D(AIDA::IFitData & /* d */, AIDA::IFunction & /* f */, const std::string& /* par */ , int /* npts */ , double /* pmin */ , double /* pmax */)
{
      INFO_MSG("AIDA_Fitter::createScan1D not implemented");
      return 0;
}

AIDA::IDataPointSet * AIDA_Fitter::createContour(AIDA::IFitData & /* d */ , AIDA::IFitResult & /* r */ , const std::string& /* par1 */ , const std::string& /* par2 */ , int /* npts */ , double /* up */)
{
      INFO_MSG("AIDA_Fitter::createContour not implemented");
      return 0;     
}
    
void AIDA_Fitter::setUseFunctionGradient( bool /* useGrad */ )
{
      INFO_MSG("AIDA_Fitter::setUseFunctionGradient not implemented");
}
     
bool AIDA_Fitter::useFunctionGradient()
{
      return true;
}


// set fitter options

bool AIDA_Fitter::setOptions(const std::string & options)
{ 
  if (!m_fitter) return false; 

  FitterOptionParser parser(options);
  /* 
  std::cout <<  " Fitter options... " << std::endl;
  std::cout << " printLevel " << parser.printLevel()  << std::endl;
  std::cout << " useMINOS " << parser.useMinos()  << std::endl;
  std::cout << " errorUp " << parser.errorUP() << std::endl;
  */

  m_fitter->fitConfig().setPrintLevel(parser.printLevel());
  m_fitter->fitConfig().switchMinosErrors(parser.useMinos());
  m_fitter->fitConfig().setErrorUP(parser.errorUP());

  return true; 
}


bool AIDA_Fitter::connect(AIDA::IFitData& d , AIDA::IBaseHistogram &h)
{

  bool lret; 
  int dim = h.dimension();
  switch(dim)
    {
    case 1:
      {
	AIDA::IHistogram1D *h1 = dynamic_cast<AIDA::IHistogram1D*>(&h);
	if (h1) { 
	  lret = d.create1DConnection(*h1);
	  break; 
	}
	AIDA::IProfile1D *p1 = dynamic_cast<AIDA::IProfile1D*>(&h);
	if (p1) { 
	  lret = d.create1DConnection(*p1);
	  break; 
	}
	AIDA::ICloud1D *c1 = dynamic_cast<AIDA::ICloud1D*>(&h);
	if (c1) { 
	  lret = d.create1DConnection(*c1);
	  break; 
	}
	// I should never be here
	INFO_MSG("AIDA_Fitter: Unknown type of AIDA::IBaseHistogram"); 
	return false;
      }
      
    case 2:
      {
	AIDA::IHistogram2D *h2 = dynamic_cast<AIDA::IHistogram2D*>(&h);
	if (h2) { 
	  lret = d.create2DConnection(*h2);
	  break; 
	}
	AIDA::IProfile2D *p2 = dynamic_cast<AIDA::IProfile2D*>(&h);
	if (p2) { 
	  lret = d.create2DConnection(*p2);
	  break; 
	}
	AIDA::ICloud2D *c2 = dynamic_cast<AIDA::ICloud2D*>(&h);
	if (c2) { 
	  lret = d.create2DConnection(*c2);
	  break; 
	}
	// I should never be here
	INFO_MSG("AIDA_Fitter: Unknown type of AIDA::IBaseHistogram"); 
	return false;
      }

    case 3:
      {
	AIDA::IHistogram3D *h3 = dynamic_cast<AIDA::IHistogram3D*>(&h);
	if (h3) { 
	  lret = d.create3DConnection(*h3);
	  break; 
	}
	AIDA::ICloud3D *c3 = dynamic_cast<AIDA::ICloud3D*>(&h);
	if (c3) { 
	  lret = d.create3DConnection(*c3);
	  break; 
	}
	// I should never be here
	INFO_MSG("AIDA_Fitter: Unknown type of AIDA::IBaseHistogram"); 
	return false;
      }

    default: 
      {
	return false;
      }
    }
  
  return lret;
}

// connect of DatapointSet to data 
bool AIDA_Fitter::connect(AIDA::IFitData& d , AIDA::IDataPointSet &dps)
{
  // connect using defaults indeces (0,1,2,...) for coordinate and last one
  // for values 
  std::vector<int> indices; 
  for (int i = 0; i < dps.dimension()-1; ++i) 
    indices.push_back(i); 

  int valIndex = dps.dimension()-1; 
  
  return d.createConnection(dps,indices,valIndex); 
}



  }     // end of namespace iAIDA::AIDA_Fitter
} 
