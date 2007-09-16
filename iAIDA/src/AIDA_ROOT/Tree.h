#ifndef AIDA_ROOT_TREE_H
#define AIDA_ROOT_TREE_H 1

//  This file is part of the AIDA library
//  Copyright (C) 2002 by the AIDA team.  All rights reserved.
//  This library is free software and under the terms of the
//  GNU Library General Public License described in the LGPL.txt 

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "AIDA_Dev/IDevTree.h"
#include "AIDA/IManagedObject.h"
#include "AIDA/IHistogram1D.h"
#include "AIDA/IHistogram2D.h"
#include "AIDA/IHistogram3D.h"
#include "AIDA/IProfile1D.h"
#include "AIDA/IProfile2D.h"
#include "AIDA_ROOT/Histogram1D.h"
#include "AIDA_ROOT/Histogram2D.h"
#include "AIDA_ROOT/Histogram3D.h"
#include "AIDA_ROOT/Profile1D.h"
#include "AIDA_ROOT/Profile2D.h"

#include <boost/shared_ptr.hpp>


#include "TFile.h"
//class TDirectory; 

namespace AIDA_ROOT {


  static const std::string emptyString = ""; 

/**
 * User level interface to a Tree.
 *
 * All paths follow unix convention .., ., /, 
 * backslash is the escape character.
 * Relative paths are allowed.
 *
 * NOTE: - this tree keeps a current position(pwd) within the tree.
 * This may be implemented on a per-thread basis.
 *
 * @author The AIDA team (http://aida.freehep.org/)
 *
 */


/// Root implementation of an AIDA Tree: create a tree attached to a root file


class Tree : public AIDA::Dev::IDevTree {

public: 

  /// constructor 
    Tree(const std::string & storeName, bool readOnly = false, bool createNew = false, const std::string & options = "");

    /// Destructor.
    virtual ~Tree() { /* no op */ }
   

  /**
   * Get the name of the store.
   * @return The store's name.
   *
   */
    std::string  storeName() const { 
      return m_name; 
    }
  /**
   * Get the IManagedObject at a given path in the ITree. The path can either be
   * absolute or relative to the current working directory.
   * @param path The path.
   * @return     The corresponding IManagedObject.
   *
   */
  AIDA::IManagedObject * find(const std::string & /* path */) { return 0; } 

  /**
   * Get a mounted ITree at a given path in the current ITree. The path can either be
   * absolute or relative to the current working directory.
   * @param path The path.
   * @return     The corresponding ITree.
   *
   */
  ITree * findTree(const std::string & /* path */ ) { return 0; } 


  /**
   * Change to a given directory.
   * @param path The absolute or relative path of the directory we are changing to.
   * @return false If the path does not exist.
   *
   */
  bool cd(const std::string & /* path */ ) { return false; } 

  /**
   * Get the path of the current working directory.
   * @return The path of the current working directory.
   *
   */
     std::string  pwd() const { return emptyString; } 

  /** 
   * List, into a given output stream, all the IManagedObjects, including directories
   * (but not "." and ".."), in a given path. Directories end with "/". The list can be recursive.
   * @param path      The path where the list has to be performed (by default the current directory ".").
   * @param recursive If <code>true</code> the list is extended recursively
   *                  in all the directories under path (the default is <code>false</code>.
   * @param os        The output stream into which the list is dumped (by default the standard output).
   * @return false If the path does not exist.
   *
   */
     bool ls(const std::string & path = ".", bool recursive = false, std::ostream & os = std::cout) const;

  /**
   * Get the list of names of the IManagedObjects under a given path, including directories
   * (but not "." and ".."). Directories end with "/".
   * The returned names are appended to the given path unless the latter is ".".
   * @param path      The path where the list has to be performed (by default the current directory ".").
   * @param recursive If <code>true</code> the list is extended recursively
   *                  in all the directories under path (the default is <code>false</code>.
   *
   */
  std::vector<std::string>  listObjectNames(const std::string & /* path = "." */ , bool /* recursive = false */ ) const { return std::vector<std::string>(); }

  /**
   * Get the list of types of the IManagedObjects under a given path.
   * The types are the leaf class of the Interface, e.g. "IHistogram1D", "ITuple", etc.
   * Directories are marked with "dir".
   * The order of the types is the same as the order for the listObjectNames() method
   * to achieve a one-to-one correspondance between object names and types.
   * @param path      The path where the list has to be performed (by default the current directory ".").
   * @param recursive If <code>true</code> the list is extended recursively
   *                  in all the directories under path (the default is <code>false</code>.
   *
   */
  std::vector<std::string>  listObjectTypes(const std::string & /* path = "." */ , bool /* recursive = false */ ) const { return std::vector<std::string>(); }

  /**
   * Create a new directory. Given a path only the last directory
   * in it is created if all the intermediate subdirectories already exist.
   * @param path The absolute or relative path of the new directory.
   * @return false If a subdirectory within the path does
   *             not exist or it is not a directory. Also if the directory already exists.
   *
   */   
  bool mkdir(const std::string & /* path */ ) { return false; }

  /**
   * Create a directory recursively. Given a path the last directory
   * and all the intermediate non-existing subdirectories are created.
   * @param path The absolute or relative path of the new directory.
   * @return false If an intermediate subdirectory
   *             is not a directory, or if the directory already exists.
   *
   */
  bool mkdirs(const std::string & /* path */) { return false; } 

  /**
   * Remove a directory and all the contents underneeth.
   * @param path The absolute or relative path of the directory to be removed.
   * @return false If path does not exist or if it is not
   *             a directory.
   *
   */
  bool rmdir(const std::string & /* path */ ) { return false; } 

  /**
   * Remove an IManagedObject by specifying its path.
   * If the path points to a mount point, the mount point should first commit, then 
   * close and delete the tree object.
   * @param path The absolute or relative path of the IManagedObject to be removed.
   * @return false If path does not exist.
   *
   */
  bool rm(const std::string & /* path */ ) { return false; } 

  /**
   * Get the full path of an IManagedObject.
   * @param object The IManagedObject whose path is to be returned.
   * @return       The object's absolute path.
   *               In C++ if the object does not exist, an empty string is returned.
   *
   */
  std::string findPath(const AIDA::IManagedObject & /* object */ ) const { return emptyString; } 

  /**
   * Move an IManagedObject or a directory from one directory to another.
   * @param oldPath The path of the IManagedObject or direcoty to be moved.
   * @param newPath The path of the diretory in which the object has to be moved to.
   * @return false If either path does not exist.
   *
   */
  bool mv(const std::string & /* oldPath */ , const std::string & /* newPath */ ) { return false; } 

  /**
   * Commit any open transaction to the underlying store(s).
   * It flushes objects into the disk for non-memory-mapped stores.
   * @return false If the underlying store cannot be written out.
   *
   */
     bool commit();

  /**
   * Set the strategy of what should happen if two objects have the same path.
   * Default is overwrite.
   * @param overwrite <code>true</code> to enable overwriting.
   *
   */
  void setOverwrite(bool /* overwrite = true */ ) { } 

  /**
   * Copy an object from a path to another.
   * @param oldPath   The path of the object to be copied.
   * @param newPath   The path where the object is to be copied.
   * @param recursive <code>true</code> if a recursive copy has to be performed.
   * @return false If either path does not exist.
   *
   */
  bool cp(const std::string & /* oldPath */ , const std::string & /* newPath */ , bool /* recursive = false */) { return false; } 

  /**
   * Create a symbolic link to an object in the ITree.
   * @param path  The absolute or relative path of the object to be linked.
   * @param alias The absolute or relative name of the link.
   * @return false If path or any
   *              subidrectory within path does not exist.
   *
   */
  bool symlink(const std::string & /* path */ , const std::string & /* alias */ ) { return false; } 

  /**
   * Mounts a tree within another (target) tree. A tree can only be mounted once.
   * Example:
   * <pre>
   *     target.mount("/home/tonyj",tree,"/");
   * </pre>
   * @param path     The path in the target tree
   * @param tree     The tree to mount within the target tree
   * @param treePath The mount point within the tree to be mounted.
   * @return false If something does not exist.
   *
   */
  bool mount(const std::string & /* path */ , ITree & /* tree */ , const std::string & /* treePath */) { return false; } 

  /**
   * Unmount a subtree at a given path (mount point).
   * Whenever a tree is destroyed it first unmounts all dependent trees.
   * @param path The path of the subtree to be unmounted.
   * @return false If path does not exist.
   *
   */
  bool unmount(const std::string & /* path */ ) { return false; } 

  /**
   * Closes the underlying store.
   * Changes will be saved only if commit() has been called before.
   * The call is propagated to the dependent mounted trees.
   * @return false If there are problems writing out
   *         the underlying store.
   *
   */
     bool close();  

  /**
   *  See IManagedObject for a description.
   * @param className The name of the class to cast on.
   * @return The right pointer. Return 0 if failure.
   */ 
  void * cast(const std::string & /* className */ ) const { return 0; }


     // methods from IDevTree
     /// Methods inherited from the IDevTree class
     

     bool copyAndAdd( AIDA::IManagedObject * /* object */ ,
		      const std::string& /* newPath */ ) { return false; } 

  AIDA::Dev::IStore* store( const std::string& /* path = "." */ ) { return 0; }

     AIDA::Dev::IStore* nativeStore() { return 0; } 

  std::string pathInStore( const AIDA::IManagedObject& /* object */ ) const 
       { std::string empty; return empty; } 

  bool existsDirectory( const std::string& /* directory */ ) const { return false;}

     bool isMounted() const { return false; } 
  bool setParentTree( AIDA::Dev::IDevTree * /* parentTree */ ) { return false; } 
  bool unmountTree( AIDA::Dev::IDevTree * /* daughterTree */ ) { return false; } 



     // add methods to tree

     template <class T> 
     bool add(T* /* h */, const std::string & name ) { 
       std::cerr << "Error: cannot  add  generic object " << name << " to the tree " << std::endl; 
       return false; 
     } 
     
     // specialized for H1D
     bool add(AIDA::Dev::IDevHistogram1D* h, const std::string & name ) { 
       if (manage<Histogram1D> (h) ) return false;     
       m_H1D.insert( std::make_pair( name, SH1D(h) ) ); 
       return true; 
     } 
     // Specialized for H2D
     bool add(AIDA::Dev::IDevHistogram2D* h, const std::string & name ) { 
       if (manage<Histogram2D> (h) ) return false;     
       m_H2D.insert( std::make_pair( name, SH2D(h) ) ); 
       return true; 
     } 
     // specialized for H3D
     bool add(AIDA::Dev::IDevHistogram3D* h, const std::string & name ) { 
       if (manage<Histogram3D> (h) ) return false;     
       m_H3D.insert( std::make_pair( name, SH3D(h) ) ); 
       return true; 
     } 
     // specialized for Profile1D
     bool add(AIDA::Dev::IDevProfile1D* h, const std::string & name ) { 
       if (manage<Profile1D> (h) ) return false;     
       m_P1D.insert( std::make_pair( name, SP1D(h) ) ); 
       return true; 
     } 
     // specialized for Profile2D
     bool add(AIDA::Dev::IDevProfile2D* h, const std::string & name ) { 
       if (manage<Profile2D> (h) ) return false;     
       m_P2D.insert( std::make_pair( name, SP2D(h) ) ); 
       return true; 
     }


     // specialized for MO  (for dev    
     bool add( AIDA::IManagedObject * object,
	       const std::string& /* path = "." */ ) {  
       std::cerr << "Tree:: cannot add a generic managed object " << object->name() << std::endl; 
       return false; 
     }



    template <class T, class B>
    bool manage(B* base) { 
      T * h = dynamic_cast<T *>(base);
      if (!h) return false; 
      // tell root to manage histo with the file
      (h->representation()).SetDirectory(static_cast<TDirectory *> (rootFile.get() ));
      return true; 
    }


 private: 

     // root file 
     boost::shared_ptr<TFile>  rootFile; 
     mutable std::string m_name; 
     // map of objects
     typedef boost::shared_ptr<AIDA::Dev::IDevHistogram1D> SH1D;
     typedef boost::shared_ptr<AIDA::Dev::IDevHistogram2D> SH2D;
     typedef boost::shared_ptr<AIDA::Dev::IDevHistogram3D> SH3D;
     typedef boost::shared_ptr<AIDA::Dev::IDevProfile1D> SP1D; 
     typedef boost::shared_ptr<AIDA::Dev::IDevProfile2D> SP2D; 

     typedef std::map<std::string, SH1D > H1DMap;
     typedef std::map<std::string, SH2D > H2DMap;
     typedef std::map<std::string, SH3D > H3DMap;
     typedef std::map<std::string, SP1D > P1DMap;
     typedef std::map<std::string, SP2D > P2DMap;

     H1DMap m_H1D; 
     H2DMap m_H2D; 
     H3DMap m_H3D; 
     P1DMap m_P1D; 
     P2DMap m_P2D; 

}; // class
}  // namespace AIDA_ROOT
#endif /* ifndef AIDA_ROOT_TREE_H */
