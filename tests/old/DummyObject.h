#ifndef DUMMYOBJECT_H
#define DUMMYOBJECT_H 1

#include "AIDA_Dev/IDevManagedObject.h"

namespace iAIDA {
  namespace aida_tree {

class DummyObject : virtual public AIDA::Dev::IDevManagedObject {

public:
  DummyObject( std::string nameNew = "", std::string typeNew = "DummyObject" ): m_name( nameNew ), m_utd( false ), m_type( typeNew ) {}
  ~DummyObject() {}

  std::string name() const {return m_name;}
  void * cast(const std::string & className) const { return 0; }
  bool isUpToDate() const {return m_utd;}
  void setUpToDate( bool isUpToDate ) {m_utd=isUpToDate;}
  bool setName( const std::string& newName ) {m_name=newName; return true;}
  const std::string& userLevelClassType() const {return m_type;}
  

private:
  std::string m_name;
  bool m_utd;
  std::string m_type;
};

  }
}


#endif
