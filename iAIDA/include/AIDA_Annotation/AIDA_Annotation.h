/* Emacs: -*- C++ -*- */
#ifndef AIDA_ANNOTATION_H
#define AIDA_ANNOTATION_H 1

// Include files
#include <vector>
#include <string>
#include <map>

#include "AIDA/IAnnotation.h"

namespace iAIDA {
  namespace AIDA_Annotation {

///  Implementation of the AIDA IAnnotation interface class

class  AIDA_Annotation  : virtual public AIDA::IAnnotation {

public:
  /// Constructor
  AIDA_Annotation(){ /* nop */ }

  /// Destructor
  ~AIDA_Annotation(){ /* nop */ }

  /// Add a key/value pair with a given sticky.
  bool addItem( const std::string & key, 
		const std::string & value,
		bool sticky = false);

  /// Remove the item indicated by a given key
  bool removeItem( const std::string & key );

  /// Retrieve the value for a given key
  std::string value( const std::string & key) const;

  /// Set value for a given key
  void setValue( const std::string & key,
		 const std::string& value);

  /// Set sticky for a given key
  void setSticky( const std::string & key,
		  bool sticky);

  /// Get the number of items in the Annotation
  int size() const;

  /// Individual access to the Annotation-items
  std::string  key(int index) const;
  std::string  value(int index) const;

  /// Remove all the non-sticky items
  void reset();

private:
  /// Internal private annotation item class
  class AnnotationItem {
  public:
    AnnotationItem( std::string k = "",
		    std::string v = "",
		    bool vis = true):
      m_key( k ), m_value( v ), m_sticky( vis )
    {/* nop */};

    ~AnnotationItem(){ /* nop */};

    std::string m_key;
    std::string m_value;
    bool        m_sticky;
  };

  /// The vector of the annotation items
  std::vector< AnnotationItem >         m_annotationItems;

  /// The map of strings to identifiers
  std::map< std::string, unsigned int > m_identifiers;

  std::string emptyString;
};

  }
}

#include "AIDA_Annotation.inl"

#endif
