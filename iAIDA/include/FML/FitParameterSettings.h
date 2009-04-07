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

#ifndef FML_FITPARAMETERSETTINGS_H
#define FML_FITPARAMETERSETTINGS_H

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//
//////////////////////////////////////////

# include <string>
#include <iostream>

# include "FML/Bound.h"

namespace fml
{
  // namespace FML
  // {

    class FitParameterSettings
    {
    public:

      FitParameterSettings(std::string name);

      /// Name of the parameter to which settings apply.
      std::string name() const;

      /// Step size allows to control the "sensitivity" of the change of the parameter
      /// when fitters looks for the optimal parameter value. Default is 1.0.
      double stepSize() const;

      /// Set the step size for the fitter.
      void setStepSize(double step);

      const Bound& bound() const;
      Bound& bound();

      /// Set bounds. If bounds not set, then default bound is (-INF,+INF).
      void setBounds(const Bound&b);

      /// Shortcut for setBounds(-INF,+INF).
      void removeBounds();

      /// Reset all settings to the default values (remove bounds, step size = 1.0, no fix).
      void reset();    

    private:

      std::string m_name;
      double m_step;

      Bound m_bound;
      
    };


    inline FitParameterSettings::FitParameterSettings(std::string name) : m_name(name)
    {
      reset();
    }

      /// Name of the parameter to which settings apply.
    inline std::string FitParameterSettings::name() const
    {
      return m_name;
    }

      /// Step size allows to control the "sensitivity" of the change of the parameter
      /// when fitters looks for the optimal parameter value. Default is 1.0.
      inline double FitParameterSettings::stepSize() const
      {
	return m_step;
      }

      /// Set the step size for the fitter.
      inline void FitParameterSettings::setStepSize(double step)
      {
	m_step = step;
      }

      inline const Bound& FitParameterSettings::bound() const
      {
	return m_bound;
      }

      inline Bound& FitParameterSettings::bound()
      {
	return m_bound;
      }

      inline void FitParameterSettings::setBounds(const Bound&b)
      {
	m_bound = b;
      }

      inline void FitParameterSettings::removeBounds()
      {
	m_bound = Bound();
      }

      /// Reset all settings to the default values (remove bounds, step size = 1.0, no fix).
      inline void FitParameterSettings::reset()
      {
	setStepSize(1.0);
	removeBounds();
      }

  // }
}

#endif
