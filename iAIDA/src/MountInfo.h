// Emacs -*- C++ -*-
#ifndef IAIDA_MOUNTINFO
#define IAIDA_MOUNTINFO 1

#include <string>

namespace AIDA {
  namespace Dev {
    class IDevTree;
  }
}

namespace iAIDA {
  namespace aida_tree {

/// Class keeping the information for a mount point on a tree

class MountInfo {

public:
  /// Constructor
  MountInfo( AIDA::Dev::IDevTree* targetTree,
             const std::string& targetPath,
             const std::string& mountPoint ):
    m_targetTree( targetTree ),
    m_targetPath( targetPath ),
    m_mountPoint( mountPoint )
  {}

  /// Destructor
  ~MountInfo(){}

  /// Returns the target tree
  AIDA::Dev::IDevTree* targetTree() const {return m_targetTree;}

  /// Returns the target path
  const std::string targetPath() const {return m_targetPath;}

  /// Returns the mount point
  const std::string mountPoint() const {return m_mountPoint;}

private:
  AIDA::Dev::IDevTree*  m_targetTree;
  std::string           m_targetPath;
  std::string           m_mountPoint;
};

  }
}

#endif /* ifndef IAIDA_MOUNTINFO */
