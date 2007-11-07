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

#ifndef AIDA_CPP_IPROFILE1D_H
#define AIDA_CPP_IPROFILE1D_H


#include "AIDA_Dev/IDevProfile1D.h"
#include<string>
#include<vector>

namespace AIDA_CPP {

  class IProfile1D : public AIDA::Dev::IDevProfile1D {
  public:
    typedef AIDA::IProfile Base;
    typedef AIDA::IProfile1D super;
  public:
    virtual void initialize(const std::string & title, int nBins, double lowerEdge, double upperEdge, const std::string & options = "")=0;

    virtual void initialize(const std::string & title, const std::vector<double>  & binEdges, const std::string & options = "")=0;

    virtual void initialize(const std::string & title, 
			    int nBins, double lowerEdge, double upperEdge, 
			    double lowerValue, double upperValue, 
			    const std::string & options = "")= 0;

    virtual void initialize(const std::string & title, 
			    const std::vector<double>  & binEdges,
			    double lowerValue, double upperValue, 
			    const std::string & options = "") =0;
  };

}
#endif  // AIDA_CPP_IPROFILE1D_H
