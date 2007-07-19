# include "FML/DataVector.h"
# include "FML/DataVectorIterator.h"


# include <assert.h>

namespace fml
{

// namespace FML {



DataRange::DataRange(int n) : m_restr_pattern(std::vector<bool>(n,true)), m_npts(n)
{}

int DataRange::nSourcePoints() const
{
  return m_restr_pattern.size();
}

bool DataRange::checkSourceIndex(int si) const
{
  return si >=0 && si<nSourcePoints();
}

bool DataRange::markSourcePoint(int si, bool flag)
{
  if(!checkSourceIndex(si)) return false;

  if(m_restr_pattern[si] != flag) 
    {
      m_npts += (flag ? 1 : -1);
      m_restr_pattern[si] = flag;
    }

  return true;
}

bool DataRange::includeSourcePoint(int si)
{
  return markSourcePoint(si,true);
}

bool DataRange::excludeSourcePoint(int si)
{ 
  return markSourcePoint(si,false);
}

bool DataRange::markSourceRange(int siMin, int siMax, bool flag)
{
  if(!checkSourceIndex(siMin)) return false;
  if(!checkSourceIndex(siMax)) return false;

  int diff = flag ? 1 : -1;

  for(int i=siMin; i<=siMax; i++)
    if(m_restr_pattern[i] != flag) 
      {
	m_npts += diff;
	m_restr_pattern[i] = flag;
      }

  return true;
}

bool DataRange::includeSourceRange(int siMin, int siMax)
{
  return markSourceRange(siMin,siMax, true);
}

bool DataRange::excludeSourceRange(int siMin, int siMax)
{
  return markSourceRange(siMin,siMax, false);
}

void DataRange::includeAll()
{
  // PENDING: assign should work but probably not implmented in this STL
  //m_restr_pattern.assign(nSourcePoints(),true);
  m_restr_pattern = std::vector<bool>(nSourcePoints(),true);
  m_npts = nSourcePoints();
}

void DataRange::excludeAll()
{
  // PENDING: assign should work but probably not implmented in this STL
  //  m_restr_pattern.assign(nSourcePoints(),false);
  m_restr_pattern = std::vector<bool>(nSourcePoints(),false);
  m_npts = 0;
}
 
int DataRange::nPoints() const
{
  return m_npts;
}


/// DataRangeIterator 

DataRangeIterator::DataRangeIterator(const DataRange& r) : m_range(r), m_ipos(0)
{
  if(m_range.nSourcePoints()==0) return;

  while(m_range.m_restr_pattern[m_ipos] == false)
    m_ipos++;
}

int DataRangeIterator::nPoints() const
{
  return m_range.nPoints();
}

void DataRangeIterator::next()
{
  if(hasMore())
    while(++m_ipos<m_range.nSourcePoints() && 
	  m_range.m_restr_pattern[m_ipos] == false);  
}


bool DataRangeIterator::hasMore() const
  {
    return m_ipos < m_range.nSourcePoints();
  }


int DataRangeIterator::sourceIndex() const
{
  return m_ipos;
}

DataVector::DataVector() : 
  m_range(0), 
  m_curr_data()
{
  m_src_data = new std::vector<DataPoint>; 
}



DataVector::DataVector(const DataVector& d) : 
  m_range(0), 
  m_src_data(0), 
  m_curr_data()
{  
  m_src_data = const_cast<std::vector<DataPoint> *>(d.m_src_data);

  setRange(d.m_range);
  
}

DataVector& DataVector::operator = (const DataVector& d)
{
  if(this != &d)
    {
      if (!m_src_data) delete m_src_data;
  
      m_src_data = const_cast<std::vector<DataPoint> * >(d.m_src_data);

      setRange(d.m_range);
    }


  return *this;
}


DataVector::~DataVector()
{
  if (!m_src_data) delete m_src_data;
}

// reserve space in advance to be more efficient 
void DataVector::reserve(int n) 
{
  (const_cast<std::vector<DataPoint> * >( m_src_data) )->reserve(n); 
  m_curr_data.reserve(n); 
}

bool DataVector::fromAscii(std::string /* filename */)
{
  bool ok = false;
  // to be implemented 

  return ok;
}

// create and read from STL vectors

bool DataVector::fromSTL(const std::vector< std::vector<std::pair<double, std::pair<double, double> > > >& /* v */ )
{
  // to be implemented
  return false;
}

/// Append a point at end. current range is not affected by this. Need to be 
/// set by the user 
void DataVector::push_back(const std::vector<double>& coords, double value, double error) { 

  // fill point 
  if (error == 0) { 
    if (value == 0) 
      error = 1.0; 
    else 
      error = value; 
  }

  double invError = 1.0/error; 
  DataPoint point(coords,value,invError);
  // check if vector is empty 
  if (!m_src_data) return; 
  // need to remove  away constness 
  (const_cast<std::vector<DataPoint> *>(m_src_data) )->push_back(point); 
  // update also curr data 
  m_curr_data.push_back(point);
  
}

// dimensionality of the point 

int DataVector::dimension() const 
{ 
  if (!m_src_data) return 0; 
  if (m_src_data->size() == 0) return 0; 
  return (*m_src_data)[0].dimension(); 
}


int DataVector::nPoints() const { if (!m_src_data) return 0; return  m_curr_data.size(); }


// set range : copy in curr data points within the set range 

void DataVector::setRange(const DataRange& r)
{
  m_range = r;

  if(m_src_data!=0)
    {
      m_curr_data.clear();
      // to be more efficient reserve the size 
      m_curr_data.reserve(m_src_data->size());

      int dim = dimension();
      std::vector<double> coords(dim);
      
      for(DataRangeIterator it(m_range); it.hasMore(); it.next())
	{
	  unsigned int si = it.sourceIndex();
	  
	  // if source index out of source vector - ignore it
	  if( si< (*m_src_data).size())
	    {
	      const DataPoint pt = (*m_src_data)[si];
	      
	      m_curr_data.push_back(pt);
	    }
	}
    }
}

const DataRange& DataVector::currentRange() const
{
  return m_range; 
}

std::auto_ptr<IFMLDataIterator> DataVector::createIterator() const
{
  return std::auto_ptr<IFMLDataIterator>( new DataVectorIterator(*this) );
}

std::auto_ptr<IFMLDataIterator> DataVector::createIterator()
{
  return std::auto_ptr<IFMLDataIterator>( new DataVectorIterator(*this) );
}

// }
}
