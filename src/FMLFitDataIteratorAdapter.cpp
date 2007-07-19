# include "FMLFitDataIteratorAdapter.h"

#include <assert.h>


namespace iAIDA  
{

  namespace AIDA_Fitter
  {

    FMLFitDataIteratorAdapter::FMLFitDataIteratorAdapter(AIDA::Dev::IDevFitDataIterator* it) : m_it(it), m_curr(false) 
    {
      assert(m_it);
    }

    void FMLFitDataIteratorAdapter::start()
    {
      m_it->start();
      m_curr = m_it->next();
    }
    
    bool FMLFitDataIteratorAdapter::current() const
    {
      return m_curr;
    }
    
    void FMLFitDataIteratorAdapter::next()
    {
      m_curr = m_it->next();      
    }

    int FMLFitDataIteratorAdapter::numberOfEntries() const
    {
      return m_it->entries();
    }
    
    const std::vector<double> &FMLFitDataIteratorAdapter::coords() const
    {
      return m_it->vars();
    }
    
    double FMLFitDataIteratorAdapter::value() const
    {
      return m_it->value();
    }
    
    double FMLFitDataIteratorAdapter::vErrorPlus() const
    {      
      return m_it->error();
    }
    
    double FMLFitDataIteratorAdapter::vErrorMinus() const
    {
      return m_it->minusError();
    }

  }
}
