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

#ifndef AIDA_CPP_GENFACTORY_H
#define AIDA_CPP_GENFACTORY_H

#include "AIDA_CPP/Exceptions.h"

#include <map>
#include <typeinfo>
#include <boost/shared_ptr.hpp>


namespace AIDA_CPP {

  /* a generic factory....
     
  **/
  class GenFactory {
  public:

    struct NotFound : public BaseException {
      NotFound(const std::string & typeName) : 
	BaseException(std::string("GenFactory: creator not found for ")+typeName){}
    };
    
    struct ACreate{ virtual ~ACreate(){}};
    
    template<typename I>
    struct ICreate : public ACreate {
      virtual I * operator()() const =0;
      virtual I * operator()(const I & rh) const =0;
    };
    
    template<typename I,typename T>
    struct Create : public ICreate<I> {
      I * operator()() const { return new T;}
      I * operator()(const I & rh) const { return new T(rh);}
    };
    
    typedef const std::type_info * pinfo;
    typedef boost::shared_ptr<ACreate> SP;
    typedef std::map<pinfo, SP> Creators;
    typedef Creators::const_iterator CI;

  public:
    
    virtual ~GenFactory(){}

    template<typename I,typename T >
    void put() { 
      SP p(new Create<I,T>); 
      creators.insert(std::make_pair(&typeid(I),p));
    }


    template<typename I> 
    I * create() const { return (*dynamic_cast<const ICreate<I> *>(get(&typeid(I))))(); }

    template<typename I> 
    I * create(const I & rh) const { return (*dynamic_cast<const ICreate<I> *>(get(&typeid(I))))(rh); }
    
  protected:

    const ACreate *  get(pinfo p) const {
      CI c = creators.find(p);
      if (c==creators.end()) throw NotFound((*p).name());
      return (*c).second.get();
    }      


  protected:
    
    Creators creators;

  };
}

#endif // AIDA_CPP_GENFACTORY_H
