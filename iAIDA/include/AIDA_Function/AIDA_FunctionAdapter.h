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

// Header file for class AIDA_FunctionAdapter
// 
// Authors  : Lorenzo Moneta , Jakub Moscicki
// Created  : Sun Oct  6 12:13:15 2002
// 
// Last update: Sun Oct  6 12:13:15 2002
// 
#ifndef IAIDA_AIDA_FUNCTION_AIDA_FUNCTIONADAPTER
#define IAIDA_AIDA_FUNCTION_AIDA_FUNCTIONADAPTER 1


#include "AIDA/IAnnotation.h"
#include "AIDA_Dev/IDevModelFunction.h"
#include "FML/IFMLFunction.h"
#include "FML/FunctionSetterHelper.h"

#include <memory>

namespace fml { 
     class RangeSet; 
 }


namespace iAIDA  { 
  namespace AIDA_Function { 


    /**
     * 
     * Adapt externally created implementation of FML::IFMLFunction to 
     * AIDA::IFunction interface.
     *
     * Wraps external pointer with optional ownership.
     */

    class AIDA_FunctionAdapter : virtual public AIDA::Dev::IDevModelFunction
    {

    public: 

      /// no-ownership transfer, simple wrapper
      AIDA_FunctionAdapter(fml::IFMLFunction *f);

      /// acquire ownerhip from auto_ptr and delete the wraped function object
      /// at the end.
      AIDA_FunctionAdapter(std::auto_ptr<fml::IFMLFunction> f);

      virtual ~AIDA_FunctionAdapter(); 

    private:
      // usually copying is non trivial, so we make this unaccessible
      AIDA_FunctionAdapter(const AIDA_FunctionAdapter &); 
      AIDA_FunctionAdapter & operator = (const AIDA_FunctionAdapter &); 

    public: 
      /// added methods 
      fml::IFMLFunction *getImplementation() { return m_function; }
      
      /// methods from AIDA interface
      std::string title() const; 
      bool setTitle(const std::string & title); 
      double value(const std::vector<double>& x) const;
      int dimension() const;
      bool isEqual(const IFunction & f) const;
      const std::vector<double>& gradient(const std::vector<double>& x)  const;  
      bool providesGradient() const;
      std::string variableName(int i) const;
      std::vector<std::string> variableNames() const;
      bool setParameters(const std::vector<double>& params);
      const std::vector<double>& parameters() const;
      int numberOfParameters() const;
      std::vector<std::string> parameterNames() const;
      bool setParameter(const std::string& name, double x);
      double parameter(const std::string& name) const;
      int indexOfParameter(const std::string& name) const;
      AIDA::IAnnotation& annotation();
      const AIDA::IAnnotation& annotation() const;
      std::string codeletString() const;
 
      void * cast(const std::string & className) const;
     
      // IModel Function

      // -- IModelFunction --
      bool providesNormalization() const;
      void normalize(bool on);
      bool isNormalized() const;
      const std::vector<double>  & parameterGradient(const std::vector<double>  & x) const;
      bool providesParameterGradient() const;
      AIDA::IRangeSet & normalizationRange(int iAxis); 
      void includeNormalizationAll();
      void excludeNormalizationAll();
      
      // developers interface

      void setCodeletString(std::string codelet);
      void setDimension(int dim);
      void setProvidesGradient(bool yes);
      bool setVariableNames(const std::vector<std::string>&);
      void setNumberOfParameters(int parnum);
      bool setParameterNames(const std::vector<std::string>&);
      void setAnnotation(AIDA::IAnnotation *ptr);

      // -- IDevModelFunction --

      bool setNormalization(bool provides, bool is_normalized);
      void setProvidesParameterGradient(bool yes);

      // from IManagedObject
      std::string name() const;

      /// Methods inherited from IDevManagedObject
      bool isUpToDate() const;
      void setUpToDate( bool isUpToDate );
      bool setName( const std::string& newName );
      const std::string& userLevelClassType() const;

      // added method for coherence with FML
      bool setNormalizationRange(int axis, const fml::RangeSet & rs); 

    protected: 

      AIDA::IAnnotation * createAnnotation(); 
      void updateAnnotation() const; 
      void createRanges(); 
      
    private: 

      bool m_owner;      
      fml::IFMLFunction *m_function;
      fml::FunctionSetterHelper m_fsh;
      AIDA::IAnnotation * m_annotation;
      std::vector<AIDA::IRangeSet * > m_ranges; 

      // for IDevManagedObject
      bool m_upToDate; 
      //std::string m_name;  ! not necessary : use annotation to store name
      mutable bool m_anno_upToDate;



    }; 

    /// Helper function.
    /// Try to get IFMLFunction from IFunction if possible,
    /// otherwise return 0.
    fml::IFMLFunction * getImplementation(AIDA::IFunction *);


  }    // end of namespace iAIDA::AIDA_Function
}
#endif /* AIDA_FUNCTIONADAPTER */
