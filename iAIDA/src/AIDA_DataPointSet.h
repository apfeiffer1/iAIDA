// Emacs -*- C++ -*-
#ifndef IAIDA_AIDA_DATAPOINTSET_AIDADATAPOINTSET
#define IAIDA_AIDA_DATAPOINTSET_AIDADATAPOINTSET 1

#include "AIDA_Dev/IDevDataPointSet.h"
#include <vector>


namespace iAIDA {
  namespace AIDA_DataPointSet_native {

/**
 * Implementation of the AIDA IDataPointSet class
 *
 * Annotation items defined:
 *
 * "Title" (sticky), "Name" (sticky), "Dimension", "Size"
 *
 */

class AIDA_DataPointSet : virtual public AIDA::Dev::IDevDataPointSet
{
public:
  /// Constructor
  AIDA_DataPointSet( const std::string& title,
		     int dimension,
		     AIDA::IAnnotation* annotation );
  /// Copy constructor
  AIDA_DataPointSet( const AIDA_DataPointSet& dps,
		     AIDA::IAnnotation* annotation );
  /// Copy constructor from another AIDA IDataPointSet
  AIDA_DataPointSet( const AIDA::IDataPointSet& dps,
		     AIDA::IAnnotation* annotation );
  /// Destructor
  ~AIDA_DataPointSet();

  /// Methods inherited from IManagedObject
  std::string name() const;

  /// Methods inherited from IDevManagedObject
  bool isUpToDate() const;
  void setUpToDate( bool isUpToDate );
  bool setName( const std::string& newName );
  const std::string& userLevelClassType() const;

  /// Methods inherited from IDataPointSet
  AIDA::IAnnotation& annotation();
  const AIDA::IAnnotation & annotation() const;
  std::string title() const;
  bool setTitle( const std::string & newTitle );
  int dimension() const;
  void clear();
  int size() const;
  AIDA::IDataPoint* point( int index );
  const AIDA::IDataPoint* point( int index ) const;
  bool setCoordinate(int coord, const std::vector<double>  & val, const std::vector<double>  & err);
  bool setCoordinate(int coord, const std::vector<double>  & val, const std::vector<double>  & errp, const std::vector<double>  & errm);
  AIDA::IDataPoint *  addPoint(); 
  bool addPoint(const AIDA::IDataPoint & other);
  bool removePoint( int index );
  double lowerExtent( int coordinate ) const;
  double upperExtent( int coordinate ) const;
  bool scale( double scaleFactor );
  bool scaleValues( double scaleFactor );
  bool scaleErrors( double scaleFactor );  
  // from AIDA 3.2.1
  void * cast(const std::string & className) const;

private:
  mutable AIDA::IAnnotation*       m_annotation;
  int                              m_dimension;
  bool                             m_upToDate;
  std::vector< AIDA::IDataPoint* > m_dataPoints;

  // Private methods
  void updateAnnotation() const;
  AIDA::IAnnotation* createAnnotation();

  // No assignment operator
  AIDA_DataPointSet& operator=( const AIDA_DataPointSet& );
};

  }
}


#endif /* ifndef IAIDA_AIDA_DATAPOINTSET_AIDADATAPOINTSET */
