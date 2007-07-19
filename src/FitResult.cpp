# include "FML/FitResult.h"

# include "FML/IFMLFunction.h"
# include "FML/Function.h"


# include "FML/Util/debug.h"

namespace fml
{
  // namespace FML
  // {

    class FakeFun : public Function
    {
    public:
      FakeFun() : Function(1,0,false,false,false,"NULL FUNCTION OBJECT") {}
      double value(const std::vector<double>& ) const { return 0; }
    };

    FitResult::FitResult() :  m_init_fun(new FakeFun), m_fitted_fun(new FakeFun), m_quality(0.0), m_ndf(0), m_valid(false), m_status(0) {}

     bool FitResult::isValid() const
    {
      return m_valid;
    }
    
     int FitResult::fitStatus() const
    {
      return m_status;
    }
    
     double FitResult::quality() const
    {
      return m_quality;
    }
    
    int FitResult::ndf() const
    {
      return m_ndf;
    }

    const std::vector<double> & FitResult::errors() const 
    { 
      return m_errors; 
    }

    const std::vector<double> & FitResult::errorsPlus() const 
    { 
      return m_errors_plus; 
    }

    const std::vector<double> & FitResult::errorsMinus() const 
    { 
      return m_errors_minus; 
    }

    const std::vector<double> & FitResult::parameters() const
    { 
      assert(m_fitted_fun.get());
      return m_fitted_fun->parameters(); 
    }

    const std::vector<std::string> & FitResult::parameterNames() const
    { 
      assert(m_fitted_fun.get());
      return m_fitted_fun->parameterNames(); 
    }

    IFMLFunction & FitResult::initialFunction() const
    {
      assert(m_init_fun.get());
      return *m_init_fun.get();
    }
    
    IFMLFunction & FitResult::fittedFunction() const
    {
      assert(m_fitted_fun.get());
      return *m_fitted_fun.get();
    }    
    /*
    const std::vector<double>& FitResult::initialParameters() const
    {
      return m_init_pars;
    }
    
    const std::vector<double>& FitResult::fittedParameters() const
    {
      return m_fit_pars;
    }
    */

    
     double FitResult::covMatrixElement(int i, int j) const
    {
      // here be careful that pnly free parameters are in the 
      // upper left and rest is all zero
      // It follows minuit tradition
      return m_cov_matrix[i][j];
    }
       
     std::string FitResult::dataDescription() const
    {
      return m_data_descr;
    }

     void FitResult::setFlags(bool valid, int status)
    {
      m_valid = valid;
      m_status = status;
    }



     void FitResult::setResult(const std::vector<double>& pars, double quality, int ndf)
    {
      m_fitted_fun->setParameters(pars);
      m_quality = quality;
      m_ndf = ndf;
    }

     void FitResult::setErrors(const std::vector<double>& errors,  const std::vector<double>& eMinus, const std::vector<double>& ePlus ) 
    {
      m_errors = errors;
      m_errors_plus = ePlus;
      m_errors_minus = eMinus;

    }


    void FitResult::setMatrix(const FMLMatrix & cv) 
    {
      m_cov_matrix = cv; 
    }

    bool FitResult::setConfig(const FitConfig& cfg, IFMLFunction &f, std::string data_descr)
    {
      m_config = cfg;

      {
	std::auto_ptr<IFMLFunction> nf(f.clone());

	if(!nf.get()) 
	  { 
	    std::string msg = "FML: fit config cannot clone function: ";
	    INFO_MSG(msg+f.functionName());
	    setFlags(false,-1);
	    return false;
	  }

	m_init_fun = nf;
      }

      {
	std::auto_ptr<IFMLFunction> nf(f.clone());

	if(!nf.get()) 
	  { 
	    std::string msg = "FML: fit config cannot clone function: ";
	    INFO_MSG(msg+f.functionName());
	    setFlags(false,-1);
	    return false;
	  }
	
	m_fitted_fun =  nf;
      }

      m_data_descr = data_descr;
      return true;
    }


    void FitResult::printOn(std::ostream& os) const
    {
      os << std::endl;

      std::string enginnm(fitConfig().engineName());

      if(enginnm == "") enginnm = "<default>";

      os << "*** FIT CONFIG ***" << std::endl
	 << "Minimizer          : " << enginnm << std::endl
	 << "Fit method         : " << fitConfig().fitMethodName() << std::endl
	 << "Data               : " << (dataDescription() == "" ? "unknown" : dataDescription()) << std::endl
	 << "Model function     : " << initialFunction().functionName() << std::endl 
	 << "Initial parameters : ";

      for(unsigned int i=0; i<initialFunction().parameters().size(); ++i)
	os << std::endl << initialFunction().parameterNames()[i] << " = " << initialFunction().parameters()[i];

      os << std::endl << std::endl;
      
      if(isValid())
	os << "*** FIT VALID ***" << std::endl;
      else
	{
	  os << "--->>> FIT FAILED <<<---" << std::endl;
	  return;
	}

      os << std::endl << "*** RESULT ***";
      os << std::endl << "Quality/" << fitConfig().fitMethodName() << "  : " << quality() 
	 << std::endl << "NDF              : " << ndf();


      os << std::endl << "Fitted parameters: ";

      for(unsigned int i=0; i<fittedFunction().parameters().size(); ++i)
	os << std::endl << fittedFunction().parameterNames()[i] << " = " << fittedFunction().parameters()[i] << "   +/-   " << errors()[i]; 


      os << std::endl;	

    }

      
  // }
}
