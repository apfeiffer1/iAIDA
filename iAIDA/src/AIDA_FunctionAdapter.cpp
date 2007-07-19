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

// Implementation file for class AIDA_FunctionAdapter
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Sun Oct  6 12:13:15 2002
// 
// Last update: Sun Oct  6 12:13:15 2002
// 

#include "AIDA_Function/AIDA_FunctionAdapter.h"

#include "AIDA_Annotation/AIDA_Annotation.h"

#include "AIDA_Function/AIDA_RangeSet.h"

#include "FML/FunctionSetterHelper.h"
#include "FML/Function.h"
#include "FML/RangeSet.h"

#include <assert.h>

#include "FML/Util/debug.h"
#include "FML/Util/util.h"

namespace iAIDA  { 
  namespace AIDA_Function { 

    // static members 
    static const std::string titleKey = "Title";
    static const std::string nameKey = "Name";
    static const std::string dimKey = "Dimension";
    static const std::string emptyString = "";

    static const std::string g_userLevelClassType = "IFunction";


    /// build from IFMLFunction autopointer

    AIDA_FunctionAdapter::AIDA_FunctionAdapter(std::auto_ptr<fml::IFMLFunction> f) : 
      m_owner(true), 
      m_function(f.release()), 
      m_fsh(m_function), 
      m_upToDate(false),
      m_anno_upToDate(false)
    {

      assert(m_function);
      m_annotation = createAnnotation(); 
      // create ranges wrapper 
      createRanges(); 
    }

    /// build from IFLFunction pointer
    
    AIDA_FunctionAdapter::AIDA_FunctionAdapter(fml::IFMLFunction *f) :
      m_owner(false), 
      m_function(f), 
      m_fsh(f),
      m_upToDate(false)
    {
      assert(m_function);
      m_annotation = createAnnotation(); 
      createRanges(); 
    }

    AIDA_FunctionAdapter::~AIDA_FunctionAdapter() 
    {
      if ( m_annotation ) delete m_annotation;
      // delete ranges 
      for (int i = 0; i < dimension(); ++i) 
	delete m_ranges[i]; 

      if(m_owner) delete m_function;
    }


    AIDA_FunctionAdapter & AIDA_FunctionAdapter::operator = (const AIDA_FunctionAdapter &rhs) 
    {
      if (this == &rhs) return *this;  // time saving self-test

      return *this;
    }

/// methods from AIDA interface

    std::string  AIDA_FunctionAdapter::title() const
    {
      if (!m_annotation) return emptyString; 
      return m_annotation->value( titleKey );
    }

    bool AIDA_FunctionAdapter::setTitle( const std::string& newTitle )
    {
      if (!m_annotation) return false; 
      m_annotation->setValue( titleKey, newTitle );
      return true;
    }

    double  AIDA_FunctionAdapter::value(const std::vector<double>& x) const
    {
      return m_function->value(x);
    }


    int AIDA_FunctionAdapter::dimension() const { return m_function->dimension(); }   
    
    const std::vector<double>& AIDA_FunctionAdapter::gradient(const std::vector<double>& x)  const  
    { 
      return m_function->gradient(x); 
    }

    bool AIDA_FunctionAdapter::providesGradient() const 
    { 
      return m_function->providesGradient(); 
    }

    std::string AIDA_FunctionAdapter::variableName(int i) const 
    { 
      return m_function->variableName(i); 
    }

    std::vector<std::string> AIDA_FunctionAdapter::variableNames() const 
    { 
      return m_function->variableNames(); 
    }

    bool AIDA_FunctionAdapter::setParameters(const std::vector<double>& params)
    {
      m_anno_upToDate = false; 
      return m_function->setParameters(params);
    }
    
    const std::vector<double>& AIDA_FunctionAdapter::parameters() const
    {
      return m_function->parameters();
    }
    
    int AIDA_FunctionAdapter::numberOfParameters() const
    {
      return m_function->numberOfParameters();
    }
    
    std::vector<std::string> AIDA_FunctionAdapter::parameterNames() const
    {
      return m_function->parameterNames();
    }
    
    bool AIDA_FunctionAdapter::setParameter(const std::string& name, double x)
    {
      bool ret = m_function->setParameter(name,x);     
      m_anno_upToDate = false; 
      return ret; 
    }
    
    double AIDA_FunctionAdapter::parameter(const std::string& name) const
    {
      return m_function->parameter(name);
    }
    
    int AIDA_FunctionAdapter::indexOfParameter(const std::string& name) const
    {
     return m_function->indexOfParameter(name);
    }
    
    bool AIDA_FunctionAdapter::isEqual(const AIDA::IFunction & f) const
    {
      const AIDA_FunctionAdapter *f0 = dynamic_cast<const AIDA_FunctionAdapter*>(&f);

      if(!f0) return false;

      const fml::Function *f1 = dynamic_cast<const fml::Function*>(f0->m_function);

      if(!f1) return false;

      return m_function->isEqual(f1);
    }


    AIDA::IAnnotation& AIDA_FunctionAdapter::annotation()
    {
      assert(m_annotation);
      if (!m_anno_upToDate) updateAnnotation(); 
      return *m_annotation;
    }
    
    const AIDA::IAnnotation& AIDA_FunctionAdapter::annotation() const
    {
      assert(m_annotation);
      if (!m_anno_upToDate) updateAnnotation(); 
      return *m_annotation;
    }


    std::string AIDA_FunctionAdapter::codeletString() const
    {
      return m_function->codeletString();
    }


    // new from AIDA 3.2.1 

    void * AIDA_FunctionAdapter::cast(const std::string & className) const
    { 
      if (className.find(userLevelClassType()) != std::string::npos ) 
	return const_cast<AIDA_FunctionAdapter *> (this);
      else 
	return 0;  
    }


    //////////// DEVELOPER'S INTERFACE //////////////


    void AIDA_FunctionAdapter::setCodeletString(std::string codelet)
    {
      m_fsh.setCodeletString(codelet);
    }

    void AIDA_FunctionAdapter::setDimension(int dim)
    {
      m_fsh.setDimension(dim);
    }

    void AIDA_FunctionAdapter::setProvidesGradient(bool yes)
    {
      m_fsh.setProvidesGradient(yes);
    }


    void AIDA_FunctionAdapter::setNumberOfParameters(int parnum)
    {
      m_fsh.setNumberOfParameters(parnum);
    }

    bool AIDA_FunctionAdapter::setParameterNames(const std::vector<std::string>& v)
    {
      for(unsigned int i=0; i< v.size(); ++i)
	if(!m_fsh.setParName(i,v[i]))
	  return false;
      return true;
    }

    bool AIDA_FunctionAdapter::setVariableNames(const std::vector<std::string>&v)
    {
      for(unsigned int i=0; i<v.size(); ++i)
	if(!m_fsh.setVarName(i,v[i]))
	  return false;
      return true;
    }

    void AIDA_FunctionAdapter::setAnnotation(AIDA::IAnnotation *anno_ptr)
    {
      // delete already existing annotatio 
      if (m_annotation) delete m_annotation; 
      // set update flag to true (if you set you expect to be correct)
      m_anno_upToDate = true; 
      m_annotation = anno_ptr;
    }

    bool AIDA_FunctionAdapter::providesNormalization() const
    {
      return m_function->providesNormalization();
    }

    void AIDA_FunctionAdapter::normalize(bool on)
    {
      m_function->normalize(on);
    }

    bool AIDA_FunctionAdapter::isNormalized() const
    {
      return m_function->isNormalized();
    }

    const std::vector<double>  & AIDA_FunctionAdapter::parameterGradient(const std::vector<double>  & x) const
    {
      return m_function->parameterGradient(x);
    }

    bool AIDA_FunctionAdapter::providesParameterGradient() const
    {
      return m_function->providesParameterGradient();
    }

    AIDA::IRangeSet & AIDA_FunctionAdapter::normalizationRange(int iAxis) 
    { 
      return *m_ranges[iAxis]; 
    }

    void AIDA_FunctionAdapter::includeNormalizationAll()
    {
      m_function->includeNormalizationAll();
    }

    void AIDA_FunctionAdapter::excludeNormalizationAll()
    {
      m_function->excludeNormalizationAll();
    }

    bool AIDA_FunctionAdapter::setNormalizationRange(int axis, const fml::RangeSet & rs)
    {
      return m_fsh.setNormalizationRange(axis,rs); 
    } 
 
    bool AIDA_FunctionAdapter::setNormalization(bool provides, bool is_normalized)
    {
      return m_fsh.setNormalization(provides,is_normalized);
    }

    void AIDA_FunctionAdapter::setProvidesParameterGradient(bool yes)
    {
      m_fsh.setProvidesParameterGradient(yes);
    }

    /// from IManagedObject and IDevManagedObject


    std::string  AIDA_FunctionAdapter::name() const
    {
      if (!m_annotation) return emptyString; 
      return m_annotation->value( nameKey );
    }


    bool AIDA_FunctionAdapter::isUpToDate() const
    {
      return m_upToDate;
    }


    void AIDA_FunctionAdapter::setUpToDate( bool isUpToDate )
    {
      m_upToDate = isUpToDate;
    }


    bool AIDA_FunctionAdapter::setName( const std::string& newName )
    {
      if (!m_annotation) return false; 
      m_annotation->setValue( nameKey, newName );
      return true;
    }


    const std::string& AIDA_FunctionAdapter::userLevelClassType() const
    {
      return g_userLevelClassType;
    }


/// protected methods 

/// create annotation

    AIDA::IAnnotation* AIDA_FunctionAdapter::createAnnotation()
    {
      // create pi annotation
      iAIDA::AIDA_Annotation::AIDA_Annotation * anno = new iAIDA::AIDA_Annotation::AIDA_Annotation( );

      // add items to annotation 
      anno->addItem( titleKey, m_function->functionName(), true );
      anno->addItem( nameKey, emptyString, true );
      // need to add the others (dimension- parameters, ....)
      std::string t_dim = Util::to_string(dimension() );
      anno->addItem(dimKey, t_dim, false ); 
      // parameters 
      std::vector<std::string> parNames = parameterNames(); 
      std::vector<double> parValues = parameters(); 
      for (int i = 0; i < numberOfParameters() ; ++ i) { 
	std::string txt = Util::to_string( parValues[i] );
	anno->addItem( parNames[i], txt , false ); 
      }

      m_anno_upToDate = true; 
      return anno; 
    }

/// update annotation with new parameter values 

    void AIDA_FunctionAdapter::updateAnnotation() const
    {
      std::vector<std::string> parNames = parameterNames(); 
      std::vector<double> parValues = parameters(); 
      for (int i = 0; i < numberOfParameters() ; ++ i) { 
	std::string txt = Util::to_string( parValues[i] );
	m_annotation->setValue( parNames[i], txt ); 
      }
      m_anno_upToDate = true;
    }

    // create vector of normalization ranges (wrapper of FML rangeSet) 

    void AIDA_FunctionAdapter::createRanges()
    {
      for (int i = 0; i < dimension(); ++i) { 
	fml::RangeSet * range = m_function->normalizationRange(i);
	// if range does not exists create default ones (-inf + inf) 
	if (range) 
	  m_ranges.push_back( new AIDA_RangeSet(range) );
	else 
	  m_ranges.push_back( new AIDA_RangeSet() ); 
      } 
    }


/// Helper function 

    fml::IFMLFunction * getImplementation(AIDA::IFunction *aidaf)
    {
      fml::IFMLFunction *fmlf = dynamic_cast<fml::IFMLFunction*>(aidaf);

      if(fmlf) return fmlf;
      
      AIDA_FunctionAdapter *wrapf = dynamic_cast<AIDA_FunctionAdapter*>(aidaf);

      if(wrapf) return wrapf->getImplementation();
      
      return 0;
    }




  }     // end of namespace iAIDA::AIDA_Function
} 
