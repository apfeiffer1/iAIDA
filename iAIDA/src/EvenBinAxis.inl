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
iAIDA::AIDA_Histogram_native::EvenBinAxis::isFixedBinning() const
{
  return true;
}

inline double
iAIDA::AIDA_Histogram_native::EvenBinAxis::lowerEdge() const
{
  return m_lowEdge;
}

inline double
iAIDA::AIDA_Histogram_native::EvenBinAxis::upperEdge() const
{
  return m_highEdge;
}

inline int
iAIDA::AIDA_Histogram_native::EvenBinAxis::bins() const
{
  return m_bins;
}

inline double
iAIDA::AIDA_Histogram_native::EvenBinAxis::binWidth( int ) const
{
  return m_binSize;
}
