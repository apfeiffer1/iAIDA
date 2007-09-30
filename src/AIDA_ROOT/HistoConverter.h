 /**********************************************************************
  *                                                                    *
  * Copyright (c) 2002 Lorenzo Moneta, CERN/IT                       *
  *                   <Lorenzo.Moneta.cern.ch>                       *
  *                                                                    *
  * This library is free software; you can redistribute it and/or      *
  * modify it under the terms of the GNU Lesser General Public         *
  * License as published by the Free Software Foundation; either       *
  * version 2.1 of the License, or (at your option) any later version. *
  *                                                                    *
  * This library is distributed in the hope that it will be useful,    *
  * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   *
  * Lesser General Public License for more details.                    *
  *                                                                    *
  * You should have received a copy of the GNU Lesser General Public   *
  * License along with this library (see file COPYING); if not, write  *
  * to the Free Software Foundation, Inc., 59 Temple Place, Suite      *
  * 330, Boston, MA 02111-1307 USA, or contact the author.             *
  *                                                                    *
  **********************************************************************/

// Header file for class Histo1DConverter
// 
// Created by: moneta  17/6/2003
// 
 

/*    
 *    class for transforming from AIDA to a Root histogram 
 * 
 */

#ifndef IAIDA_AIDA_ROOTCONVERTER_HISTOCONVERTER
#define IAIDA_AIDA_ROOTCONVERTER_HISTOCONVERTER 1


#include <string>
#include <vector>

class TAxis;
class TH1;
class TH2;
class TH3;
class TH1D;
class TH2D;
class TH3D;
class TProfile;
class TProfile2D;

namespace AIDA { 
   class IAxis;
   class IHistogram1D;
   class IHistogram2D;
   class IHistogram3D;
   class IProfile1D;
   class IProfile2D;
   namespace Dev { 
     class IDevHistogram1D;
     class IDevHistogram2D;
     class IDevHistogram3D;
     class IDevProfile1D;
     class IDevProfile2D;
     class IDevHistogramFactory;
   }
 }

namespace iAIDA { 
   namespace AIDA_RootConverter { 


class HistoConverter  {

public: 
  // constructor 
  HistoConverter() {} 

  virtual ~HistoConverter() {}

private:
// usually copying is non trivial, so we make this unaccessible
  HistoConverter(const HistoConverter &); 
  HistoConverter & operator = (const HistoConverter &); 

public: 

  /// transform to Root 
  static TH1D *  copyToRoot( const AIDA::IHistogram1D & histo, const std::string & name);
  static TH2D *  copyToRoot( const AIDA::IHistogram2D & histo, const std::string & name);
  static TH3D *  copyToRoot( const AIDA::IHistogram3D & histo, const std::string & name);
  static TProfile   *  copyToRoot( const AIDA::IProfile1D & histo, const std::string & name);
  static TProfile2D *  copyToRoot( const AIDA::IProfile2D & histo, const std::string & name);

  /// create from Root 

  static AIDA::Dev::IDevProfile1D *   copyToAida(AIDA::Dev::IDevHistogramFactory& factory, const TProfile & tprof ); 
  static AIDA::Dev::IDevProfile2D *   copyToAida(AIDA::Dev::IDevHistogramFactory& factory, const TProfile2D & tprof ); 
  static AIDA::Dev::IDevHistogram3D * copyToAida(AIDA::Dev::IDevHistogramFactory& factory, const TH3 & thist ); 
  static AIDA::Dev::IDevHistogram2D * copyToAida(AIDA::Dev::IDevHistogramFactory& factory, const TH2 & thist ); 
  static AIDA::Dev::IDevHistogram1D * copyToAida(AIDA::Dev::IDevHistogramFactory& factory, const TH1 & thist ); 

 protected: 

  static void  getBinEdges(const AIDA::IAxis & axis, std::vector<double> & edges);
  static void  getBinEdges(const TAxis & axis, std::vector<double> & edges);
  static bool isEqBinSize(const TAxis * axis);

}; 

   }      // end of namespace iAIDA::AIDA_RootStore
}

#endif /* HISTOCONVERTER */
