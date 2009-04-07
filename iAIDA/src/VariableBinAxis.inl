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

inline bool
iAIDA::AIDA_Histogram_native::VariableBinAxis::isFixedBinning() const
{
  return false;
}

inline double
iAIDA::AIDA_Histogram_native::VariableBinAxis::lowerEdge() const
{
  return m_binEdges.front();
}

inline double
iAIDA::AIDA_Histogram_native::VariableBinAxis::upperEdge() const
{
  return m_binEdges.back();
}

inline int
iAIDA::AIDA_Histogram_native::VariableBinAxis::bins() const
{
  return m_binEdges.size() - 1;
}

inline int
iAIDA::AIDA_Histogram_native::VariableBinAxis::coordToIndex( double coord ) const
{
  return m_binUpperEdges.upper_bound( coord )->second;
}
