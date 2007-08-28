# ifndef AIDA_Function_FML_FMLFunctionAdapter_h__included__
# define AIDA_Function_FML_FMLFunctionAdapter_h__included__

/**
 * \file Implementation for AIDA function component
 *
 * \author Jakub.Moscicki@cern.ch, CERN/IT 2001
 */

# include "FML/IFMLFunction.h"

# include "AIDA_Dev/IDevModelFunction.h"
# include "AIDA_Dev/IDevFunctionFactory.h"

# include <memory>

namespace fml
{
    class Function;
    class FunctionCatalogue;
    class RangeSet; 
}

namespace iAIDA 
{
  namespace AIDA_Function
  {

    /**
     * Adapt externally created implementation of AIDA::Dev::IDevModelFunction
     * to the FML::IFMLFunction. This way you may fit any implementation
     * of AIDA functions with FML library directly.
     *
     * Wraps external pointer with optional ownership.
     */
    
    class FMLFunctionAdapter : virtual public fml::IFMLFunction
    {
    public:

      /**
       * No-ownership transfer, simple wrapper.
       * Use factory for cloning.
       * If no factory -> use function catalogue as set by the call to
       * useCatalogueIfNoFactory().
       */
      FMLFunctionAdapter(AIDA::Dev::IDevModelFunction *f, AIDA::Dev::IDevFunctionFactory * factory = 0);

      /**
       * Acquire ownerhip from auto_ptr and delete the wraped function object
       * at the end.
       * Factory usage: see above.
       */

      FMLFunctionAdapter(std::auto_ptr<AIDA::Dev::IDevModelFunction> f, AIDA::Dev::IDevFunctionFactory * factory = 0);
      
      ~FMLFunctionAdapter();

      /**
       * Use given catalogue to clone() but only if factory was not given 
       * at construction time. If null then do not use catalogue.
       * This method is provided for an alternative scenario, when
       * the foreign implementation is able to register its functions into
       * the persistent catalogue (or transient registry) but you do not
       * have its factory.
       *
       * This can work for simple function types only (E,G) but not for
       * complex types like P6 or E+G. In complex cases the codelet string
       * does not hold the full function type information. This needs to
       * be modified in order to achieve fully coherent expandable scheme.
       * PENDING: modify codelet definition and implementation in FML to
       * PENDING: include the info from IFMLFunction::functionName()
       */
      void useCatalogueIfNoFactory(fml::FunctionCatalogue *cat);

      // ---------------------------
      // implementation of interface

      double value(const std::vector<double>& x) const;
      int dimension() const;
      bool isEqual(const IFMLFunction * f) const;
      const std::vector<double>& gradient(const std::vector<double>& x)  const;  
      bool providesGradient() const;
      std::string variableName(int i) const;
      const std::vector<std::string>& variableNames() const;
      bool setParameters(const std::vector<double>& params);
      const std::vector<double>& parameters() const;
      int numberOfParameters() const;
      const std::vector<std::string>& parameterNames() const;
      bool setParameter(std::string name, double x);
      bool setParameter(int ix, double x);
      double parameter(std::string name) const;
      int indexOfParameter(std::string name) const;
      std::string codeletString() const;

      /// return typenameId from the codelet.
      /// we do not have access to function's name, how to do cloning then?
      std::string functionName() const;


      /* the method below are specific  AIDA::IDevModelFunction*/
      bool providesNormalization() const;
      void normalize(bool on);
      bool isNormalized() const;
      const std::vector<double>& parameterGradient(const std::vector<double>& x) const;
      
      bool providesParameterGradient() const;

      fml::RangeSet * normalizationRange(int iAxis) const; 
      void includeNormalizationAll();
      void excludeNormalizationAll();

      bool setNormalizationRange(int axis, const fml::RangeSet & rs); 
  
      /* 
       * Q: how to do cloning without function's factory?
       * A: relying on the function catalogue - but this will not work
       * for implementations which do not register into FML function registry!
       */
      std::auto_ptr<IFMLFunction> clone() const;

      /// Yes. By default we can stream these AIDA::Function objects.
      bool isStreamable() const;

    protected:
      // developers interface

      bool setDimension(int dimx);
      bool setNumberOfParameters(int parnum);
      bool setProvidesGradient(bool provides_gradient);
      bool setParName(int ix, std::string name);
      bool setVarName(int ix, std::string name);
      bool setCodeletString(std::string cds);
      bool setFunctionName(std::string fname);
      bool setProvidesParameterGradient(bool yes);
      bool setNormalization(bool provides, bool is_normalized);
      /// do nothing.
      bool _constructThis(std::string functionName, const fml::FunctionCatalogue* cat = 0);

    private:

      /// helper method 
      /// PENDING: to be factored out to the common base class
      /// PENDING: in FML, exactly the same code is in FML::Function!
      bool _copy_state_to(fml::IFMLFunction &target) const;

      bool m_owner;      
      AIDA::Dev::IDevModelFunction *m_function;

      AIDA::Dev::IDevFunctionFactory *m_factory;
      fml::FunctionCatalogue *m_catalogue;

      //  AIDA returns by value vect of string, FML by const ref: need to cache them
      mutable std::vector<std::string> m_variableNames;
      mutable std::vector<std::string> m_parameterNames;
    };
    
  }
}


# endif
