# include "FML/DomainConstraints.h"

namespace fml
{

// namespace FML {

DomainConstraints::DomainConstraints() : m_dim(0), current_id(1)
{}

DomainConstraints::DomainConstraints(int ndim) : m_dim(ndim), current_id(1)
{}

DomainConstraints::~DomainConstraints() {}

bool DomainConstraints::removeConstraint(int id)
{
  return !(m_lc.erase(id) == 0 && m_nlc.erase(id) == 0);
}

int DomainConstraints::set(LinearConstraint c) 
{
  if(dimension()!=0 &&
    (c.dimension() != dimension())) return 0;

  LCMap::iterator pos;

  for(pos = m_lc.begin(); pos != m_lc.end(); ++pos)
    if((*pos).second == c) return 0;

  m_lc.insert(std::make_pair(++current_id,c));
  return current_id;
}

int DomainConstraints::set(NonlinearConstraint c) 
{ 
  if(dimension()!=0 &&
    (c.dimension() != dimension())) return 0;

  NLCMap::iterator pos;

  for(pos = m_nlc.begin(); pos != m_nlc.end(); ++pos)
    if((*pos).second == c) return 0;

  m_nlc.insert(std::make_pair(++current_id,c));
  return current_id;
}

const LinearConstraint *DomainConstraints::linearConstraint(int id) const 
{ 
  LCMap::const_iterator elem = m_lc.find(id);

  return (elem == m_lc.end()) ? 0 : &(*elem).second;
}

const NonlinearConstraint *DomainConstraints::nonlinearConstraint(int id) const
{
  NLCMap::const_iterator elem = m_nlc.find(id);

  return (elem == m_nlc.end()) ? 0 : &(*elem).second;
}
/*
  DomainConstraintsIterator DomainConstraints::iterator() const
  {
  return DomainConstraintsIterator(*this); 
  } 
*/


DomainConstraintsIterator::DomainConstraintsIterator(const DomainConstraints& c, object_type t) : m_c(c)
{
  reset(t);
}
/*
  impl of currLinear() and currNonlinear() are not symmetric because
  we use very simple ordering: first we go thru nonlinear constraints
  and then thru linear constraints. the order is arbitrary from the user
  point of view, so we implement it in easy way, see next()
*/
const LinearConstraint* DomainConstraintsIterator::currLinear() const
{
  /// still iterating nonlinear constraints
  if(m_nlc_iter != m_c.m_nlc.end()) return 0;

  if(m_lc_iter == m_c.m_lc.end()) return 0;
  else
    return &(*m_lc_iter).second;
}

const NonlinearConstraint* DomainConstraintsIterator::currNonlinear() const
{
  if(m_nlc_iter == m_c.m_nlc.end()) return 0;
  else
    return &(*m_nlc_iter).second;
}

bool DomainConstraintsIterator::hasMore() const
{
  return !(m_nlc_iter == m_c.m_nlc.end() && m_lc_iter == m_c.m_lc.end());
}

void DomainConstraintsIterator::next()
{
  // first iterate nonlinear constraints then the other
  // this is simple & dirty implementation

  if(m_nlc_iter != m_c.m_nlc.end())
    ++m_nlc_iter;
  else
    if(m_lc_iter != m_c.m_lc.end())
      ++m_lc_iter;
}

void DomainConstraintsIterator::reset(object_type t)
{
  m_t = t;
  m_lc_iter = m_c.m_lc.begin();
  m_nlc_iter = m_c.m_nlc.begin();
}

// }
}
