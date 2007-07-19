#ifndef FMLLIB_patches__h_included__
#define FMLLIB_patches__h_included__

// Compiler patches

// pending: this switch is obviously temporary
# ifdef  FML_SOLARIS_COMPILER

typedef int bool;

# define false 0
# define true 1

# endif

#endif

