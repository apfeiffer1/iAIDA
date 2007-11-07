/// extend tree for having methods to downcast from Managed objects to H1D

#include "AIDA/AIDA.h"

class CastConverter { 

 public: 
  CastConverter() {};

  static AIDA::IHistogram1D * toH1D(AIDA::IManagedObject * imo) { 
    return dynamic_cast< AIDA::IHistogram1D *> ( imo); 
  }
  static AIDA::IHistogram2D * toH2D(AIDA::IManagedObject * imo) { 
    return dynamic_cast< AIDA::IHistogram2D *> ( imo); 
  }
  static AIDA::IHistogram3D * toH3D(AIDA::IManagedObject * imo) { 
    return dynamic_cast< AIDA::IHistogram3D *> ( imo); 
  }
  static AIDA::IProfile1D * toP1D(AIDA::IManagedObject * imo) { 
    return dynamic_cast< AIDA::IProfile1D *> ( imo); 
  }
  static AIDA::IProfile2D * toP2D(AIDA::IManagedObject * imo) { 
    return dynamic_cast< AIDA::IProfile2D *> ( imo); 
  }
  static AIDA::ICloud1D * toC1D(AIDA::IManagedObject * imo) { 
    return dynamic_cast< AIDA::ICloud1D *> ( imo); 
  }
  static AIDA::ICloud2D * toC2D(AIDA::IManagedObject * imo) { 
    return dynamic_cast< AIDA::ICloud2D *> ( imo); 
  }
  static AIDA::ICloud3D * toC3D(AIDA::IManagedObject * imo) { 
    return dynamic_cast< AIDA::ICloud3D *> ( imo); 
  }
    static AIDA::ITuple * toTuple(AIDA::IManagedObject * imo) { 
    return dynamic_cast< AIDA::ITuple *> ( imo); 
  }
    static AIDA::IFunction * toFunction(AIDA::IManagedObject * imo) { 
    return dynamic_cast< AIDA::IFunction *> ( imo); 
  }
    static AIDA::IDataPointSet * toDPS(AIDA::IManagedObject * imo) { 
    return dynamic_cast< AIDA::IDataPointSet *> ( imo); 
  }


};
