# ifndef IAIDA_AIDA_FITTER_FMLFitDataIteratorAdapter_h__included__
# define IAIDA_AIDA_FITTER_FMLAFitDataIteratorAdapter_h__included__

/**
 * \file Implementation for AIDA fitting component
 *
 * \author Jakub.Moscicki@cern.ch, CERN/IT 2001
 */

# include "AIDA_Dev/IDevFitDataIterator.h"
# include "FML/IFMLDataIterator.h"

# include <memory>
# include <vector>

namespace iAIDA 
{
  namespace AIDA_Fitter
  {

    /**
     * Adapt the AIDA data iterator to IDevFitDataIterator
     *
     */
    
    class FMLFitDataIteratorAdapter : virtual public fml::IFMLDataIterator
    {
    public:

      FMLFitDataIteratorAdapter(AIDA::Dev::IDevFitDataIterator *  it);

      void start();
      bool current() const;
      void next();
      int numberOfEntries() const;
      const std::vector<double> &coords() const;
      double value() const;
      double vErrorPlus() const;
      double vErrorMinus() const;

    private:
      AIDA::Dev::IDevFitDataIterator *  m_it;
      bool m_curr;
    };
 
  }
}


# endif
