# include "FML/FitMethod.h"


namespace fml
{
// namespace FML
// {

  FitMethodFunction::FitMethodFunction(bool provides_grad, std::string typename_id) : Function(1, 0, provides_grad, false, false, typename_id), m_model(0), m_data(0)
  {
  }

  void FitMethodFunction::connectModel(IFMLDataIterator& dataIt, IFMLFunction &model)
  {
    m_data = &dataIt;
    m_model = &model;

    setDimension(model.numberOfParameters());
    setNumberOfParameters(0);

    setProvidesGradient(providesGradient() && model.providesParameterGradient());
    setProvidesParameterGradient(false);

    for(int i=0; i<dimension(); ++i)
      setVarName(i, model.parameterNames()[i]);

    assert(static_cast<int> (m_grad_buf.size()) == model.numberOfParameters());

    //    DBG_VAL(m_grad_buf.size());

    //    setCodeletString(codeletString()); // already set    
    
  }

// }
}
