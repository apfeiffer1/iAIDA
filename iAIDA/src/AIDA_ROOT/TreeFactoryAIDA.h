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

#ifndef AIDA_ROOT_TREEFACTORY_H
#define AIDA_ROOT_TREEFACTORY_H 1

//  This file is part of the AIDA library
//  Copyright (C) 2002 by the AIDA team.  All rights reserved.
//  This library is free software and under the terms of the
//  GNU Library General Public License described in the LGPL.txt 

#include <string>

#include "AIDA/ITreeFactory.h"
#include "AIDA_ROOT/Tree.h"
#include "AIDA_CPP/DummyTree.h"

namespace AIDA_ROOT {


/**
 * The creator of trees.
 *
 * @author The AIDA team (http://aida.freehep.org/)
 */
 
class TreeFactoryAIDA : public AIDA::ITreeFactory {

public: 
    /// Destructor.
    virtual ~TreeFactoryAIDA() { /* nop */; }

    /**
     * Creates a new tree that is not associated with a store.
     */
     AIDA::ITree * create() 
       {
	 return new AIDA_CPP::DummyTree();
       }



    /**
     * Creates a new tree and associates it with a store.
     * The store is assumed to be read/write.
     * The store will be created if it does not exist.
     * @param storeName The name of the store, if empty (""), the tree is created
     *                  in memory and therefore will not be associated with a file.
     * @param storeType Implementation specific string, may control store type
     * @param readOnly If true the store is opened readonly, an exception if it does not exist
     * @param createNew If false the file must exist, if true the file will be created
     * @param options Other options, currently are not specified
     *
     */
     AIDA::ITree * create(const std::string & storeName, const std::string & storeType = "", bool readOnly = false, bool createNew = false, const std::string & options = "") 
      { 
	if (storeType == "Root" || storeType == "ROOT") 
	  return new Tree( storeName, readOnly,createNew,options); 
	else
	  return create();
      }

}; // class
}  // namespace AIDA_ROOT
#endif /* ifndef AIDA_ROOT_TREEFACTORY_H */
