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

inline int
iAIDA::AIDA_Histogram_native::ProfileBin1D::entries() const
{
  return m_entries;
}

inline bool
iAIDA::AIDA_Histogram_native::ProfileBin1D::reset()
{
  m_sumOfWeights = m_sumOfWeightTimesExtraCoordinate = m_sumOfWeightTimesSquaredExtraCoordinate = m_sumOfWeightTimesCoordinate = m_entries = 0;
  return true;
}


inline bool
iAIDA::AIDA_Histogram_native::ProfileBin1D::scale( double scaleFactor )
{
  m_sumOfWeights *= scaleFactor;
  m_sumOfWeightTimesExtraCoordinate *= scaleFactor;
  m_sumOfWeightTimesSquaredExtraCoordinate *= scaleFactor;
  m_sumOfWeightTimesCoordinate *= scaleFactor;
  return true;
}


inline bool
iAIDA::AIDA_Histogram_native::ProfileBin1D::fill( double weight,
						   double x,
						   double y )
{
  ++m_entries;
  m_sumOfWeights += weight;
  m_sumOfWeightTimesExtraCoordinate += weight * y;
  m_sumOfWeightTimesSquaredExtraCoordinate += weight * y * y;
  m_sumOfWeightTimesCoordinate += weight * x;
  return true;
}

inline double
iAIDA::AIDA_Histogram_native::ProfileBin1D::sumOfWeights() const
{
  return m_sumOfWeights;
}
