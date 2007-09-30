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

# ifndef MINFIT_IVariable_h__included__
# define MINFIT_IVariable_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

# include <iostream>


# include <iomanip>
# include <vector>
# include <string>

namespace fml {

// namespace FML {

/**
 * Implementation of Variable.
 *
 * Variable is any scalar, settable value.
 *
 * \ingroup FML_LIB
 */

class Variable 
{
 public:
  Variable(double v = 0.0) : m_value(v) {}

  double value() const { return m_value; }
  void setValue(double v) { m_value = v; }

  /**
   * Print contents in tabular format.
   * Current width value in stream is column width.
   * Current width is preserved by this function.
   */
  virtual void tabPrint(std::ostream& os) const;

  /**
   * Print column names in tabular format.
   * Current width value in stream is column width.
   * Current width is preserved by this function.
   */
  static void tabPrintNames(std::ostream& os);

 private:

  double m_value;
};


class VoidFunction
  {
  public:
    int operator() () { return 0; }
  };

  /**
   * Print a vector of variables as a table.
   * Type VAR must support tabPrint() and tabPrintNames(). 
   * See FML::Variable for details.
   * You can specify additional column by passing non-empty 'name' for
   * the name of the column. printTable will call nmf() function object
   * to obtain elements of the column. So value of first nmf() call will
   * be << inserted in the first row, the value of second nmf() call in the
   * second row etc.
   */
template<class VAR, class NAME_FUN>
  void printTable(const std::vector<VAR>& v, std::ostream& os, std::string name, NAME_FUN nmf)
  {
    std::ios::fmtflags externalFlags = os.flags();  // store external cout flags

    os.setf(std::ios::scientific, std::ios::floatfield);
    os.setf(std::ios::right, std::ios::adjustfield);
    
    const int COL_WIDTH = 12;
    
    os.precision(3);
    
    os << std::endl 
       << std::setw(COL_WIDTH) << "Index";

    // PENDING: probably bug in iostream: ostream << string ignores width()
    // using ostream << (const char*) instead
    if(name!="") os << std::setw(COL_WIDTH) << name.c_str();

    os << std::setw(COL_WIDTH);
    
    VAR::tabPrintNames(os);
    
    os << std::endl;
    
    for(unsigned int i=0; i<v.size(); i++)
      {
	const VAR& vi = v[i];
	
	os << std::endl << std::setw(COL_WIDTH) << i;

	if(name!="")
	  os << std::setw(COL_WIDTH) << nmf();
	
	os.width(COL_WIDTH);
	vi.tabPrint(os);
      }	  
    
    os << std::endl;
    
    os.flags(externalFlags);  // reset saved stream flags 
    
  }

/**
 * Print a vector of variables as a table without additional column.
 */
template<class VAR> 
 void printTable(const std::vector<VAR>& v, std::ostream& os)
  {
    printTable(v,os,"",VoidFunction());
  }

// }
}

# endif
