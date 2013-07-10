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
iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::dimension() const
{
  return m_dimension;
}

inline bool
iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::isUpToDate() const
{
  return m_upToDate;
}

inline void
iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::setUpToDate(bool isUpToDate)
{
  m_upToDate=isUpToDate;
}

inline const std::string&
iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::userLevelClassType() const
{
  return m_classType;
}
