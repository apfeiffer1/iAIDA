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

inline double
iAIDA::AIDA_DataPointSet_native::AIDA_Measurement::value() const
{
  return m_value;
}

inline double
iAIDA::AIDA_DataPointSet_native::AIDA_Measurement::errorPlus() const
{
  return m_errorPlus;
}

inline double
iAIDA::AIDA_DataPointSet_native::AIDA_Measurement::errorMinus() const
{
  return m_errorMinus;
}

inline bool
iAIDA::AIDA_DataPointSet_native::AIDA_Measurement::setValue( double v )
{
  m_value = v;
  return true;
}

inline bool
iAIDA::AIDA_DataPointSet_native::AIDA_Measurement::setErrorPlus( double ep )
{
  m_errorPlus = ep;
return true;
}

inline bool
iAIDA::AIDA_DataPointSet_native::AIDA_Measurement::setErrorMinus( double em )
{
  m_errorMinus = em;
return true;
}
