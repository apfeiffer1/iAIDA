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
