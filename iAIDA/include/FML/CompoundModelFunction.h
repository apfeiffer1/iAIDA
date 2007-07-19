# ifndef FMLLIB__CompoundModelFunction_h__included__
# define FMLLIB__CompoundModelFunction_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//////////////////////////////////////////

# include "FML/Function.h"

namespace fml
{
// namespace FML
// {

/**
 * Compound model function is a set of model functions (member functions) 
 * clustered together to implement a functional operator. 
 *
 * The first member added with append() sets the dimensionality of the 
 * compound. Member functions are stored externally and are only
 * referenced by the compound. By default the memory management of the member
 * functions is done externally: compound will not delete its members.
 * However, of argument owned is explicitly set to true, then the memeber 
 * function will be deleted automatically (so it must be created using new
 * by the user).
 *
 * the dimension of the compound is set by the dimension of
 * the first member appended() and may not be changed afterwards.
 * 
 * The simplest example of the compound is a sum of model functions.
 *
 * PENDING:
 * For efficiency the parameters are cached. If parameters of 
 * member function are changed _externally_ this invalidates the cache.
 * For this purpose we need to make new method: updateParameters() which 
 * updates the cache from all member functions parameter caches.
 *
 */
class CompoundModelFunction : public Function
  {
  public:
    /**
     * Create compound without any member functions.
     * At least one member function must be appended.
     * If owned_members == true then memeber functions 
     * will be automatically deleted.
     */
    explicit CompoundModelFunction(bool owned_members = false);

    /**
     * If owned_members == true, delete all appended functions.
     */
    virtual ~CompoundModelFunction();

    /// @name Methods specific to compound functions.
                                                        //@{
    /*
     * Append new model function.
     * The dimension of new member must match the dimension() of the compound,
     * return false if function cannot be appended.
     * This is non-const function and changes the number of parameters etc.
     * Parameter values of member_fun are retained. This method has no
     * effect if member_fun == 0.
     * Derived classes may add some book-keeping code but they must
     * always call this base class implementation and propagate the result.
     */
    virtual bool append(IFMLFunction *member_fun);

    /// return a pointer to member, 0 if index out of range.
    /// valid range: 0 <= i < numberOfMembers()
    const IFMLFunction* member(int i) const;

    /// number of members in the compound
    int numberOfMembers() const;

                                                         //@}
    /// @name INHERITED INTERFACE METHODS
                                                         //@{
    /// must be defined in the subclass.
    virtual double value(const std::vector<double>& x) const = 0;

    /// the number of parameters changes if new member is append()'ed.
    bool setParameters(const std::vector<double>& params);
    bool setParameter(int ix, double x);
    bool setParameter(std::string nm, double x);
 
  private:
    
    std::vector<IFMLFunction*> m_members;
    bool m_owned_members;
    
    // @name efficiency cache
                                                                 //@{
    mutable std::vector<IFMLFunction*> m_param_owners;
    mutable std::vector<int> m_param_shift;
                                                                 //@}
};
  
// }

}

# endif
