/* Emacs: -*- C++ -*- */

inline bool
iAIDA::AIDA_Annotation::AIDA_Annotation::addItem( const std::string & key, 
							  const std::string & value,
							  bool sticky )
{
  if ( m_identifiers.find( key ) != m_identifiers.end() ) return false;
  m_annotationItems.push_back( AnnotationItem( key, value, sticky ) );
  m_identifiers.insert( std::make_pair( key, m_annotationItems.size() - 1 ) );
  return true;
}

inline bool
iAIDA::AIDA_Annotation::AIDA_Annotation::removeItem( const std::string & key )
{
  std::map< std::string, unsigned int >::const_iterator iKey = m_identifiers.find( key );
  if ( iKey == m_identifiers.end() ) return false;
 
  unsigned int indexToBeRemoved = iKey->second;
  // check stickness

  if (  m_annotationItems[indexToBeRemoved].m_sticky ) return false; 
      
  // why rebuilding ?
   
  m_identifiers.clear();
  std::vector< AnnotationItem > m_annotationItemsNew;
  if ( m_annotationItems.size() > 1 ) m_annotationItemsNew.reserve( m_annotationItems.size() - 1 );
  for ( unsigned int iItem = 0; iItem < m_annotationItems.size(); ++iItem ) {
    if ( iItem == indexToBeRemoved ) continue;
    const AnnotationItem& item = m_annotationItems[ iItem ];
    m_annotationItemsNew.push_back( AnnotationItem( item.m_key, item.m_value, item.m_sticky ) );
    m_identifiers.insert( std::make_pair( item.m_key, m_annotationItemsNew.size() - 1 ) );
  }
  m_annotationItems = m_annotationItemsNew;
  
  

  return true;
}

inline std::string
iAIDA::AIDA_Annotation::AIDA_Annotation::value( const std::string & key) const
{
  std::map< std::string, unsigned int >::const_iterator iKey = m_identifiers.find( key );
  if ( iKey == m_identifiers.end() ) return emptyString;
  return ( m_annotationItems[ iKey->second ] ).m_value;
}

inline void
iAIDA::AIDA_Annotation::AIDA_Annotation::setValue( const std::string & key,
							   const std::string& value)
{
  std::map< std::string, unsigned int >::const_iterator iKey = m_identifiers.find( key );
  if ( iKey == m_identifiers.end() ) 
    // not found then add it 
    addItem(key,value); 
  else 
    ( m_annotationItems[ iKey->second ] ).m_value = value;
}

inline void
iAIDA::AIDA_Annotation::AIDA_Annotation::setSticky( const std::string & key,
							    bool sticky)
{
  std::map< std::string, unsigned int >::const_iterator iKey = m_identifiers.find( key );
  if ( iKey == m_identifiers.end() ) return;
  ( m_annotationItems[ iKey->second ] ).m_sticky = sticky;
}

inline int
iAIDA::AIDA_Annotation::AIDA_Annotation::size() const
{
  return m_annotationItems.size();
}

inline std::string
iAIDA::AIDA_Annotation::AIDA_Annotation::key(int index) const
{
  if ( index < 0 || index >= static_cast<int>(m_annotationItems.size()) ) return emptyString;
  return ( m_annotationItems[ index ] ).m_key;
}

inline std::string
iAIDA::AIDA_Annotation::AIDA_Annotation::value(int index) const
{
  if ( index < 0 || index >= static_cast<int>(m_annotationItems.size()) ) return emptyString;
  return ( m_annotationItems[ index ] ).m_value;
}

inline void
iAIDA::AIDA_Annotation::AIDA_Annotation::reset()
{
  // Collect the non-sticky items
  std::vector< std::string > itemsToRemove;
  itemsToRemove.reserve( size() );
  for ( int item = 0; item < size(); ++item ) {
    if ( ! ( m_annotationItems[ item ] ).m_sticky ) {
      itemsToRemove.push_back( ( m_annotationItems[ item ] ).m_key );
    }
  }

  for ( unsigned int i = 0; i < itemsToRemove.size(); ++i ) removeItem( itemsToRemove[i] );
}
