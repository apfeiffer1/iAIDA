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

# ifndef FMLLIB_DataVector_h__included__
# define FMLLIB_DataVector_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

# include "FML/IFMLDataIterator.h"

# include <vector>
# include <string>
# include <memory>


namespace fml
{


//namespace FML 
//{

/**
 * Data point isolates from direct usage of IPoint.
 *
 * \ingroup FML_LIB
 */
class DataPoint
{
 public:

  DataPoint() {}

  DataPoint(const std::vector<double>& c, double val, double invErr);
  
  const std::vector<double>& coords() const { return m_coords; }
  double value() const { return m_value; }

  double error() const { return 1.0/m_invError; }

  double invError() const { return m_invError; }

  int dimension() const { return m_coords.size(); }

  // PENDING: to add here
  // in 1D case bin width abs(xPlus())+abs(xMinus())
  // in 2D case ? bin area?
  // double binVolume() const;

 private:
  
  std::vector<double> m_coords;
  double m_value;
  // keep inverse of errors since this what is used normally
  double m_invError;
};

inline DataPoint::DataPoint(const std::vector<double>& c, double val, double invErr) : m_coords(c), m_value(val), m_invError(invErr) {}

/**
 * The logical subrange of data vector.
 *
 * You can include/exclude points and ranges from the vector using
 * source indexing (index in source vector). DataRangeIterator
 * provides the _sequential_ access to the points which are in range.
 *
 * \ingroup FML_LIB
 */

class DataRange
{
 public:

  /// Create the range for n source points.
  /// All source points are in the range.
  explicit DataRange(int n);

  /// Number of points in the range.
  int nPoints() const;

  /// Number of source points. Source index (si) is
  /// valid in range 0...nSourcePoints()-1.
  int nSourcePoints() const;

  // PENDING: change names (delete "Source") to be consistent with rest
  /// Include single point. Return false if index out of bounds.
  bool includeSourcePoint(int si);

  /// Exclude single point. Return false if index out of bounds.
  bool excludeSourcePoint(int si);

  /// Include all points from iMin to iMax inclusively. 
  /// Return false if range invalid.
  bool includeSourceRange(int siMin, int siMax);

  /// Exclude all points from iMin to iMax inclusively.
  /// Return false if range invalid.
  bool excludeSourceRange(int siMin, int siMax);

  /// Include all points (remove all previous exclusions).
  void includeAll();

  /// Exclude all points. 
  /// Should be followed by some point inclusion otherwise the view contains 
  /// no points.
  void excludeAll();
 
  //  PENDING: Relative indexing not available but maybe added if requested
  /*
  /// Exclude point (using current range index).
  bool excludePointInCurrentView(int i);

  /// Exclude range (using current range index).
  bool excludeRangeInCurrentView(int iMin, int iMax);
  */
 private:

  friend class DataRangeIterator;
  
  bool markSourcePoint(int si, bool flag);
  bool markSourceRange(int siMin, int siMax, bool flag);

  bool checkSourceIndex(int si) const;

  std::vector<bool> m_restr_pattern;
  int m_npts;
};

/**
 * Subrange iterator.
 * Allows a sequential access to elements in the range.
 * Maps current position to source index. Source index may be used then
 * to retrieve a data point from a source data vector.
 *
 * \ingroup FML_LIB
 */

class DataRangeIterator
{
 public:
  DataRangeIterator(const DataRange& r);

  /// Number of points in range.
  int nPoints() const;

  /// Advance to next position.
  void next();

  /// False if past the end - all elements already traversed.
  bool hasMore() const;

  /// Map current position to source index.
  int sourceIndex() const;

 private:
  const DataRange &m_range;
  int m_ipos;
};

/**
 * Vector of experimental data with range restriction.
 *
 * DataVector keeps internally a copy of IVector implementation with
 * the data it acquired from original source (no longer referenced).
 *
 * Points are accessed via DataPoint structure, which maximally
 * isolates implementation from IVector interface. 
 *
 * Rationale:
 *   - there may be applications for which IVector is not 
 *     a good data structure for fitting problems (to be investigated 
 *     in the future) 
 *   - limit the number of places which depend on external interfaces 
 *     (this will make possible future changes easier).
 *
 * \ingroup FML_LIB
 */

class DataVector
{
 public:

  // this constructor is not accesible anymore, FML must not depend on external interfaces
  // and will provide its native implementation of data vector
  
  /**
   * Construct unrestricted view of source data.
   * Data is internally copied and the pointer is not used anymore.
   */
  //DataVector(const iAIDA::IVector *src);

  /// Construct empty vector.
  DataVector();

  /// Delete internal data copy.
  ~DataVector();

  /// Copy the data from another vector with all range restrictions.
  DataVector(const DataVector&);

  /// Copy the data from another vector with all range restrictions.
  DataVector& operator = (const DataVector&);

  /// Reserve space in advance to be more efficient
  void reserve(int n);
  
  /// Read in the data from an ASCII file.
  /// Old range is deleted. Return false if errors.
  bool fromAscii(std::string filename);

  /**
   * Read data from STL vector. This STL vector is a low-level 'interface'
   * which may be useful for sharing data between applications:
   * A point is a vector of (x, (ePlus, eMinus)).
   * A data vector is a vector of such points.
   * All inner vectors must have the same size (which is a dimension of data vector).
   */
  bool fromSTL(const std::vector< std::vector<std::pair<double, std::pair<double, double> > > >& v);

  /// Append a point at end. current range is not affected by this.
  /// Need to to setRange() at the end to syncronize
  void push_back(const std::vector<double>& coords, double value = 0, double error = 0);

  /// Access to points (via index 0...nPoints()-1)
  /// in the current range.
  const DataPoint& point(int i) const { return m_curr_data[i]; }

  /// Get number of coordinates (space dimension).
  int dimension() const;

  /// Number of points in current range.
  /// Access to points is via index 0...nPoints()-1.
  int nPoints() const;

  /// Set and update current range. 
  void setRange(const DataRange& r);

  /// Get current range.
  const DataRange& currentRange() const;

  std::auto_ptr<IFMLDataIterator> createIterator() const;
  std::auto_ptr<IFMLDataIterator> createIterator();

  friend class DataVectorIterator;

  const std::vector<DataPoint> & dataPoints() const { return m_curr_data; }

 private:

  // init and assign auxiliary
  //void _assign_from(const iAIDA::FML_internal::IVector *src, const DataRange* r);

  // range restriction
  DataRange m_range; 
  
  // a pointer to local source data copy 
  // PENDING: may be implemented with counted pointers 
  // for speed and memory saving
  // needed ???  
  const std::vector<DataPoint>  *m_src_data; 

  // data with range restrictions
  std::vector<DataPoint> m_curr_data; 

};
// }
}


# endif
