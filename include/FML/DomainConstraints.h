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

# ifndef FMLLIB_DomainConstraints_h__included__
# define FMLLIB_DomainConstraints_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

# include "FML/LinearConstraint.h"
# include "FML/NonlinearConstraint.h"

# include <vector>
# include <map>

namespace fml
{

// namespace FML {

class DomainConstraints;

/**
 * Iterator over the heterogenious collection of constraints.
 * For read only access. \ingroup FML_LIB
 */
class DomainConstraintsIterator
{
 public:
  
  /// Types of objects in the collection.  
  /// Field all_types indicates that actual type does not matter.
  enum object_type {all_types, linear_type, nonlinear_type};
  
  /// Initializes the iterator.
  /// Choosing object_type other than default allows to iterate only
  /// through the objects of indicated type.
  /// By default (all_types) objects of all types are iterated
  /// (this is called multitype iteration).
  
  DomainConstraintsIterator(const DomainConstraints& c, object_type t = all_types);

  /// Get current linear constraint or 0 if multitype iteration
  /// and current object is of different type. Also 0 if no objects
  /// in collection or iteration finished.
  const LinearConstraint* currLinear() const;
  
  /// Get current nonlinear constraint or 0 if multitype iteration
  /// and current object is of different type. Also 0 if no objects
  /// in collection or iteration finished.
  const NonlinearConstraint* currNonlinear() const;
  
  /// Check if there are any objects remaining.
  bool hasMore() const;
  
  /// Advance to the next object.
  void next();

  /// Reset iterator (and change the iterated type).

  void reset(object_type t);

 private:

  const DomainConstraints & m_c;
  object_type m_t;

  typedef std::map<int, LinearConstraint, std::less<int> > LCMap;
  typedef std::map<int, NonlinearConstraint, std::less<int> > NLCMap;

  LCMap::const_iterator m_lc_iter;
  NLCMap::const_iterator m_nlc_iter;
};


/**
 * DomainConstraints correspond to the set of constraints that may be
 * imposed on a general n-dimensional domain space.
 *
 * Constraints may be of the following kinds:
 *
 *  - linear constraints -
 *      there may be any number of linear constraints
 *
 *  - non-linear constraints -
 *      there may be any number of non-linear constraints
 *
 * Current implementation limits the number of constraints according to the
 * length of integer number (so for 2 byte int it is 64K).
 *
 * Identifier 0 has special meaning (error handling) and is never assigned to 
 * constraints.
 *
 * \ingroup FML_LIB
 */

class DomainConstraints
{
 public:

  /// Construct empty constraints collection on the constraints space
  /// with given dimensionality. 
  explicit DomainConstraints(int ndim);

  /// Ignore dimensionality.
  DomainConstraints();

  ~DomainConstraints();

  /// Set linear constraint and return its id.
  /// Return 0 if not set.
  int set(LinearConstraint c);

  /// Set non-linear constraint and return its id.
  /// Return 0 if not set.
  int set(NonlinearConstraint c);

  /// Retrieve LinearConstraint by id identifier.
  /// Return 0 if not found.
  const LinearConstraint* linearConstraint(int id) const;

  /// Retrieve NonlinearConstraint by id identifier.
  /// Return 0 if not found.
  const NonlinearConstraint* nonlinearConstraint(int id) const;

  /// Remove any space constraint with id identifier.
  bool removeConstraint(int id);

  /*
  /// Get iterator for the constraints collection.
  // DomainConstraintsIterator iterator() const;
  */

  /// Number of linear constraints.

  int numberOfLinearConstraints() const { return m_lc.size(); }

  /// Number of non-linear constraints.
  
  int numberOfNonlinearConstraints() const { return m_nlc.size(); }

  /// Return true if no constraints were set.
  bool isUnconstrained() const { return m_lc.size()==0 && m_nlc.size()==0; }

  /// Dimensionality of the contraints space.
  int dimension() const { return m_dim; }

  friend class DomainConstraintsIterator;

 private:

  typedef std::map<int, LinearConstraint, std::less<int> > LCMap;
  typedef std::map<int, NonlinearConstraint, std::less<int> > NLCMap;

  int m_dim;
  LCMap m_lc;
  NLCMap m_nlc;
  int current_id;
};

// }
}

# endif
