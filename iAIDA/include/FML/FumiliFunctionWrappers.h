# ifndef FUMILIFUNCTIONSWRAPPER_h__included__
# define FUMILIFUNCTIONSWRAPPER_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Lorenzo.Moneta@cern.ch
//
// CERN  Geneva, 2005
//////////////////////////////////////////


#include "FML/FitMethod.h"
#include "FML/IFMLFunction.h"
#include "FML/DataVectorIterator.h"
#include "FML/MinuitFCNWrapper.h"
#include "FML/UserModelFunction.h"

#include "Minuit2/FumiliChi2FCN.h"
#include "Minuit2/FumiliMaximumLikelihoodFCN.h"
#include "Minuit2/ParametricFunction.h"
#include "Minuit2/FunctionGradient.h"


#include "Minuit2/MnFcn.h"
#include "Minuit2/MnStrategy.h"
#include "Minuit2/MnUserParameterState.h"
#include "Minuit2/Numerical2PGradientCalculator.h"



#include "Minuit2/MnMachinePrecision.h"

// case use mathcore for derivatives 
#ifdef USE_MATHCORE
#include "MathCore/Derivator.h"
#endif

namespace fml 
{



#ifdef USE_MATHCORE
// transform derivatives of a multidim funciton in a single function
  // need to use mathcore derivatives algorithms efficiently
  class DerivFunction { 

  public: 

    DerivFunction() : m_index(0), m_func(0) {}

    DerivFunction(IFMLFunction * f) : m_index(0), m_func(f) { }

    DerivFunction(const DerivFunction & df) : m_index(df.m_index), m_func(df.m_func) {}

    DerivFunction & operator=( const  DerivFunction & df) { 
      m_index = df.m_index; 
      m_func = df.m_func; 
      return *this;
    }

    ~DerivFunction() {}

    double setCoordinatesAndParameters(const std::vector<double> & x, const std::vector<double> & params) { 
      m_x = x; 
      assert(m_func); 
      m_func->setParameters(params); 
    }

    void setIndex(int index) { m_index = index; }

    inline double operator() (double p) const { 
      m_func->setParameter(m_index, p); 
      return m_func->value(m_x);
    }
    
  private: 
    
    int m_index; 
    mutable IFMLFunction * m_func; 
    std::vector<double> m_x;

  };

#endif


/**
 * Wrapper around FML Fit method function (objective function)  to be used with Fumili of LCG Minuit
 * 
 * \ingroup FML_LIB
 */

  class FumiliFCNWrapper  : public MinuitFCNWrapper
  {


  public:

    /**
       constructor of fumili wrapper function taking a method function and error definition
    */
    FumiliFCNWrapper(const FitMethodFunction & f, double up);  
 
    /**
       destructor, no operation since the function is not managed by wrapper 
    */
    ~FumiliFCNWrapper()  {} 

  protected: 

    /**
       implement each chi2 contribution (not squared) 
    */
    virtual std::vector< double > m_elements (const std::vector< double > & ) const 
    { 
      //      std::cout << "requested elements" << std::endl; 
      return std::vector<double>(0);  
    }
    
    /**
       return obj function value
     */
    virtual double m_value(const std::vector<double> & v) const { return m_objf.value(v); }

    virtual int m_getNumberOfMeasurements() const { 
      //std::cout << "nmeasure" << (m_objf.dataIterator() )->numberOfEntries() << std::endl;
      return (m_objf.dataIterator() )->numberOfEntries(); 
    }


    double  up() const { return m_up; }

    void setError( double errorUp ) { m_up = errorUp; }

    std::vector<double> m_getMeasurement(int index) const {
      const std::vector<DataPoint> & points = dataPoints();
      return  points[index].coords(); 
    }

    const  std::vector<DataPoint>  & dataPoints() const { return *m_points; }

    IFMLFunction & modelFunction() { return *(m_objf.modelFunction()); }

    /**
       calculate at the same time the gradient of the function and the hessian 
     */
    void calculate_gradient_and_hessian(const std::vector<double> & p, int type); 

    /**
       return cached value of the gradient
     */
    const std::vector<double> & get_gradient() const { 
      return m_gradient; 
    }


    /**
       return cached value of the hessian
     */
    inline double  get_hessian(unsigned int row, unsigned int col) const{ 
      if(row > col) 
	return m_hessian[col+row*(row+1)/2];
      else
	return m_hessian[row+col*(col+1)/2];
    }



    /**
       numerical gradient calculation
       use simple 2 point method to estimate the model function parametric gradient from know point
    */
    void calculate_numerical_gradient( IFMLFunction & func, const std::vector<double> & x, double f0);  

    /**
       calculate numerical gradient quickly in case of having a user model function
     */
    void calculate_fast_numerical_gradient( const std::vector<double> & x,  double f0);

    /**
       calculate i-th contribution due to the i-th data element to the objective function
     */
    double calculate_element(int index, const std::vector<double> & p, int type) const;

    /**
       calculate derivative of the i-th element of the objective function
       \param type is 1 for chi2 objective function and 2 for likelihood's 
     */
    const std::vector<double> & calculate_derivatives(int index, const std::vector<double> & p, int type = 1) const;  


  protected: 


  private:
    const FitMethodFunction & m_objf;
    // vector of fixed parameters (for Minuit to FML convention ) 
    //std::vector<bool> m_par_isFixed; 
    // vector of initial values (to be used when one parameter is fixed)
    //std::vector<double> m_par_initValues; 
    // number of free parameters 
    //int m_numbOfFreeParams;
    double m_up;
    
#ifdef USE_MATHCORE
    mutable mathlib::Derivator m_derivator;     
    mutable DerivFunction m_derivFunc; 
#endif

    const std::vector<DataPoint> * m_points;    


    mutable std::vector<double> m_modelFun_grad;

    std::vector<double> m_gradient;
    std::vector<double> m_hessian;
    // cache parameters for derivatives
    std::vector<double> m_p2;


    bool m_providesGradient;
    UserModelFunction * m_userModelFunction;
    double m_precision;

  };

  /**
     wrapper for Fumili Chi2 methods 
   */

  class FumiliChi2FCNWrapper : public ROOT::Minuit2::FumiliFCNBase , public FumiliFCNWrapper { 

  public: 

    /// construct Fumili Chi2 FCN Wrapper from a Chi2 Fit Method function
    FumiliChi2FCNWrapper(const FitMethodFunction & f) : FumiliFCNWrapper(f,1.0) {} 
    ~FumiliChi2FCNWrapper() {}

    std::vector< double > elements (const std::vector< double > & par) const { return m_elements(par); }
    
    double operator() (const std::vector<double> & par) const { return m_value(par); }
      

    void EvaluateAll( const std::vector<double> & p)  
    { 
      calculate_gradient_and_hessian(p,1); 
    }

     
    const std::vector<double> & Gradient() const { 
      return get_gradient(); 
    }

    double Hessian(unsigned int i, unsigned int j) const { 
      return get_hessian(i,j); 
    }

    double Up() const { return 1.0; } 
 
  };

  /**
     wrapper for Fumili Likelihood methods 
   */
  class FumiliMLFCNWrapper : public ROOT::Minuit2::FumiliFCNBase, public FumiliFCNWrapper { 

  public: 
    /// construct Fumili Chi2 FCN Wrapper from a Unbinned ML Fit Method function
    FumiliMLFCNWrapper(const FitMethodFunction & f) : FumiliFCNWrapper(f,0.5) {} 
    ~FumiliMLFCNWrapper() {}

    std::vector< double > elements (const std::vector< double > & par) const { return m_elements(par); }
    
    double operator() (const std::vector<double> & par) const { return m_value(par); }


    void EvaluateAll( const std::vector<double> & p)  
    { return calculate_gradient_and_hessian(p,2); }

    inline     
    const std::vector<double> & Gradient() const { 
      return get_gradient(); 
    }

    inline     
    double Hessian(unsigned int i, unsigned int j) const { 
      return get_hessian(i,j); 
    }

    double Up() const { return 0.5; } 

  };



}  // end fml namespace


# endif
