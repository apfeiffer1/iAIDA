# ifndef RCS_debug_h__included__
# define RCS_debug_h__included__

/**
 * JTM_DEBUG defines the level of printing debug information at runtime.
 *
 * JTM_DEBUG undefined - no info printed
 * JTM_DEBUG == 0 - minimal 
 * JTM_DEBUG == 1 - maximal
 */

// PENDING: INFO_MSG() should NOT include trailing ; automatically
// PENDING: if(x) INFO_MSG(y); else INFO_MSG(z); will not complie (extra ;)
// PENDING: the same for other macros

/* IDEA:
   to be able to switch debug messages selectively one needs to group them
   DBG_STRING calls the messanger with identifier 
   (e.g. __LINE__, or typename(this)). the messanger may filter out
   unwanted groups -> environmental variable JTM_DEBUG_LIST is a list
   of wanted identifiers, e.g.
   setenv JTM_DEBUG_LIST myclass.cpp his.cpp
 */

# include <iostream>

# ifdef JTM_DEBUG

# if JTM_DEBUG == 0

# define DBG_VAL(x) \
  std::cerr << "DBG: " << __STRING(x) << " = " << (x) << std::endl; \

# define DBG_STRING(x) \
  std::cerr << "DBG: " << __STRING(x) << std::endl; \

# define DBG_EXEC(x) x;

# else

#if defined  __sun || defined _WIN32 

# define DBG_VAL(x) \
  std::cerr << "DBG: " << __STRING((x)) << " = " << (x) \
       << " File: " << __FILE__ \
       << ":" << __LINE__ \
       << std::endl; 

# define DBG_STRING(x) \
  std::cerr << "DBG: " << __STRING(x) \
       << " File: " << __FILE__ \
       << ":" << __LINE__ \
       << std::endl; 

#else 
// __PRETTY_FUNCTION__ is defined only in gcc > 2.6

# define DBG_VAL(x) \
  std::cerr << "DBG: " << __STRING((x)) << " = " << (x) \
       << " In: " << __PRETTY_FUNCTION__ \
       << " File: " << __FILE__ \
       << ":" << __LINE__ \
       << std::endl; 

# define DBG_STRING(x) \
  std::cerr << "DBG: " << __STRING(x) \
       << " In: " << __PRETTY_FUNCTION__ \
       << " File: " << __FILE__ \
       << ":" << __LINE__ \
       << std::endl; 
#endif // end __sun

# define DBG_EXEC(x) x;

# endif

# else

# define DBG_VAL(x)
# define DBG_STRING(x)
# define DBG_EXEC(x)
# endif

// only for compatibility with FML
# define DBG_PRINT(x) DBG_VAL(x)

#if defined __sun || defined _WIN32 
/// to issue error messages
# define ERROR_MSG(str) \
  std::cerr << "ERROR: " << str \
       << std::endl;
#else
/// to issue error messages
# define ERROR_MSG(str) \
  std::cerr << "ERROR: " << str \
       << " In: " << __PRETTY_FUNCTION__ \
       << std::endl;
#endif // if def __sun

# define INFO_MSG(str) \
  std::cerr << "INFO: " << str \
       << std::endl;

#endif
