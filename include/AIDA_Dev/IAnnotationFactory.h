// Emacs -*- C++ -*-
#ifndef AIDA_IANNOTATIONFACTORY
#define AIDA_IANNOTATIONFACTORY 1

namespace AIDA {
  class IAnnotation;

  namespace Dev {

/// Abstract interface class for a annotation factory

class IAnnotationFactory {

public:
  /// Destructor
  virtual ~IAnnotationFactory() {/*nop*/;}
  /// Creates a new annotation
  virtual AIDA::IAnnotation* createAnnotation() = 0;
  /// Deletes a annotation object
  virtual bool destroyAnnotation( AIDA::IAnnotation * annotation ) = 0;
};

  }
}

#endif /* ifndef AIDA_IANNOTATIONFACTORY */
