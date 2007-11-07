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

#ifndef AIDA_CPP_IPROFILE2D_H
#define AIDA_CPP_IPROFILE2D_H


#include "AIDA_Dev/IDevProfile2D.h"
#include<string>
#include<vector>

namespace AIDA_CPP {

  class IProfile2D : public AIDA::Dev::IDevProfile2D {
  public:
    typedef AIDA::IProfile Base;
    typedef AIDA::IProfile2D super;
  public:
    virtual void initialize(const std::string & title, int binsX, double iminX, double imaxX, int binsY, double iminY, double imaxY, const std::string & options="") = 0;

    virtual void initialize(const std::string & title, const std::vector<double>  & binEdgesX, const std::vector<double>  & binEdgesY, const std::string & options="") = 0;

    virtual void initialize(const std::string & title, int binsX, double iminX, double imaxX, int binsY, double iminY, double imaxY,  double lowerValue, double upperValue, const std::string & options="") = 0;

    virtual void initialize(const std::string & title, const std::vector<double>  & binEdgesX, const std::vector<double>  & binEdgesY,  double lowerValue, double upperValue, const std::string & options="") = 0;

  };

}
#endif  // AIDA_CPP_IPROFILE2D_H
