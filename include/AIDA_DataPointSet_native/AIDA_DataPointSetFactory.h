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

#ifndef IAIDA_AIDA_DATAPOINTSET_AIDADATAPOINTSETFACTORY
#define IAIDA_AIDA_DATAPOINTSET_AIDADATAPOINTSETFACTORY 1

// includes
#include "AIDA/IDataPointSetFactory.h"

#include "AIDA_Dev/IDevTree.h"
#include "AIDA_Dev/IDevDataPointSetFactory.h"

namespace iAIDA {
  namespace AIDA_DataPointSet_native {

/// Implementation of the AIDA::IDataPointSetFactory interface class

class AIDA_DataPointSetFactory : virtual public AIDA::IDataPointSetFactory
{
public:
  AIDA_DataPointSetFactory( AIDA::Dev::IDevTree& tree, AIDA::Dev::IDevDataPointSetFactory& factory );
  /// destructor
  ~AIDA_DataPointSetFactory() { /*nop*/; }

  AIDA::IDataPointSet* create( const std::string& name, const std::string& title, int dimensionOfPoint );
  AIDA::IDataPointSet* create( const std::string& name, int dimensionOfPoint );

  // new methods from Aida 3.2.1
  AIDA::IDataPointSet * createY(const std::string & path, const std::string & title, const std::vector<double>  & y, const std::vector<double>  & ey);
  AIDA::IDataPointSet * createY(const std::string & path, const std::string & title, const std::vector<double>  & y, const std::vector<double>  & eyp, const std::vector<double>  & eym);
  AIDA::IDataPointSet * createY(const std::string & pathAndTitle, const std::vector<double>  & y, const std::vector<double>  & ey);
  AIDA::IDataPointSet * createY(const std::string & pathAndTitle, const std::vector<double>  & y, const std::vector<double>  & eyp, const std::vector<double>  & eym);
  AIDA::IDataPointSet * createX(const std::string & path, const std::string & title, const std::vector<double>  & x, const std::vector<double>  & ex);
  AIDA::IDataPointSet * createX(const std::string & path, const std::string & title, const std::vector<double>  & x, const std::vector<double>  & exp, const std::vector<double>  & exm);
  AIDA::IDataPointSet * createX(const std::string & pathAndTitle, const std::vector<double>  & x, const std::vector<double>  & ex);
  AIDA::IDataPointSet * createX(const std::string & pathAndTitle, const std::vector<double>  & x, const std::vector<double>  & exp, const std::vector<double>  & exm);
  AIDA::IDataPointSet * createXY(const std::string & path, const std::string & title, const std::vector<double>  & x, const std::vector<double>  & y, const std::vector<double>  & exp, const std::vector<double>  & eyp, const std::vector<double>  & exm, const std::vector<double>  & eym);
  AIDA::IDataPointSet * createXY(const std::string & path, const std::string & title, const std::vector<double>  & x, const std::vector<double>  & y, const std::vector<double>  & ex, const std::vector<double>  & ey);
  AIDA::IDataPointSet * createXY(const std::string & pathAndTitle, const std::vector<double>  & x, const std::vector<double>  & y, const std::vector<double>  & exp, const std::vector<double>  & eyp, const std::vector<double>  & exm, const std::vector<double>  & eym);
  AIDA::IDataPointSet * createXY(const std::string & pathAndTitle, const std::vector<double>  & x, const std::vector<double>  & y, const std::vector<double>  & ex, const std::vector<double>  & ey); 
  AIDA::IDataPointSet * createXYZ(const std::string & path, const std::string & title, const std::vector<double>  & x, const std::vector<double>  & y, const std::vector<double>  & z, const std::vector<double>  & exp, const std::vector<double>  & eyp, const std::vector<double>  & ezp, const std::vector<double>  & exm, const std::vector<double>  & eym, const std::vector<double>  & ezm);
  AIDA::IDataPointSet * createXYZ(const std::string & path, const std::string & title, const std::vector<double>  & x, const std::vector<double>  & y, const std::vector<double>  & z, const std::vector<double>  & ex, const std::vector<double>  & ey, const std::vector<double>  & ez);
  AIDA::IDataPointSet * createXYZ(const std::string & pathAndTitle, const std::vector<double>  & x, const std::vector<double>  & y, const std::vector<double>  & z, const std::vector<double>  & exp, const std::vector<double>  & eyp, const std::vector<double>  & ezp, const std::vector<double>  & exm, const std::vector<double>  & eym, const std::vector<double>  & ezm);
  AIDA::IDataPointSet * createXYZ(const std::string & pathAndTitle, const std::vector<double>  & x, const std::vector<double>  & y, const std::vector<double>  & z, const std::vector<double>  & ex, const std::vector<double>  & ey, const std::vector<double>  & ez);  

  AIDA::IDataPointSet* createCopy( const std::string& name, const AIDA::IDataPointSet & original );
  bool destroy( AIDA::IDataPointSet * dataPointSet );
  AIDA::IDataPointSet* create( const std::string & name, const AIDA::IHistogram1D & h, const std::string & options = "" );
  AIDA::IDataPointSet* create( const std::string & name, const AIDA::IHistogram2D & h , const std::string & options = "");
  AIDA::IDataPointSet* create( const std::string & name, const AIDA::IHistogram3D & h , const std::string & options = "");
  AIDA::IDataPointSet* create( const std::string & name, const AIDA::ICloud1D & c , const std::string & options = "");
  AIDA::IDataPointSet* create( const std::string & name, const AIDA::ICloud2D & c, const std::string & options = "" );
  AIDA::IDataPointSet* create( const std::string & name, const AIDA::ICloud3D & c , const std::string & options = "" );
  AIDA::IDataPointSet* create( const std::string & name, const AIDA::IProfile1D & p , const std::string & options = "" );
  AIDA::IDataPointSet* create( const std::string & name, const AIDA::IProfile2D & p , const std::string & options = "" );
  AIDA::IDataPointSet* add( const std::string & name, const AIDA::IDataPointSet& a, const AIDA::IDataPointSet& b );
  AIDA::IDataPointSet* subtract( const std::string & name, const AIDA::IDataPointSet& a, const AIDA::IDataPointSet& b );
  AIDA::IDataPointSet* multiply( const std::string & name, const AIDA::IDataPointSet& a, const AIDA::IDataPointSet& b );
  AIDA::IDataPointSet* divide( const std::string & name, const AIDA::IDataPointSet& a, const AIDA::IDataPointSet& b );
  AIDA::IDataPointSet* weightedMean( const std::string & name, const AIDA::IDataPointSet& a, const AIDA::IDataPointSet& b );

private:
  /// Data members : reference to the tree and the real factory
  AIDA::Dev::IDevTree&                 m_tree;
  AIDA::Dev::IDevDataPointSetFactory&  m_factory;

  /// Private method
  template< class T >
  T* registerObject( T* p, const std::string& name );
};

  }
}

template< class T >
inline T* iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::registerObject( T* p, const std::string& name ) {
  if ( p ) {
    p->setName( name );
    if ( ! m_tree.add( p ) ) {
      m_factory.destroy( p );
      p = 0;
    }
  }
  return p;
}

#endif /* ifndef AIDA_IDATAPOINTSETFACTORY */
