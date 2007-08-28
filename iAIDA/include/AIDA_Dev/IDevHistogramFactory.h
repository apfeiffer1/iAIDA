// Emacs -*- C++ -*-
#ifndef AIDA_IDEVHISTOGRAMFACTORY
#define AIDA_IDEVHISTOGRAMFACTORY 1

#include <string>
#include <vector>

namespace AIDA {
  class IBaseHistogram;
  class IHistogram1D;
  class IHistogram2D;
  class IHistogram3D;
  class ICloud1D;
  class ICloud2D;
  class ICloud3D;
  class IProfile1D;
  class IProfile2D;

  namespace Dev {
    class IDevHistogram1D;
    class IDevHistogram2D;
    class IDevHistogram3D;
    class IDevCloud1D;
    class IDevCloud2D;
    class IDevCloud3D;
    class IDevProfile1D;
    class IDevProfile2D;

/// Histogram factory creating objects that are not attached to a tree

class IDevHistogramFactory {

public:
  /// Destructor
  virtual ~IDevHistogramFactory() {/*nop*/;}
  /// Destroys an object
  virtual bool destroy( AIDA::IBaseHistogram * histo ) = 0;
  /**
   * Creation of 1-dimensional unbinned histograms (clouds)
   * nMax = -1 indicates no (user-specified) limit of entries before autoconversion into a histogram.
   */
  virtual IDevCloud1D* createCloud1D( const std::string & title, int nMax = -1, const std::string & options = "" ) = 0;
  virtual IDevCloud1D* createCloud1D() = 0;
  virtual IDevCloud1D* createCopy( const AIDA::ICloud1D & original ) = 0;
  /**
   * Creation of 2-dimensional unbinned histograms (clouds)
   * nMax = -1 indicates no (user-specified) limit of entries before autoconversion into a histogram.
   */
  virtual IDevCloud2D* createCloud2D( const std::string & title, int nMax = -1, const std::string & options = "" ) = 0;
  virtual IDevCloud2D* createCloud2D() = 0;
  virtual IDevCloud2D* createCopy( const AIDA::ICloud2D & original ) = 0;
  /**
   * Creation of 3-dimensional unbinned histograms (clouds)
   * nMax = -1 indicates no (user-specified) limit of entries before autoconversion into a histogram.
   */
  virtual IDevCloud3D* createCloud3D( const std::string & title, int nMax = -1, const std::string & options = "" ) = 0;
  virtual IDevCloud3D* createCloud3D() = 0;
  virtual IDevCloud3D* createCopy( const ICloud3D & original ) = 0;
  /**
   * Creation of 1-dimensional binned histograms
   */
  virtual IDevHistogram1D* createHistogram1D( const std::string & title, int nBins, double lowerEdge, double upperEdge, const std::string & options = "") = 0;
  virtual IDevHistogram1D* createHistogram1D( const std::string & title, const std::vector< double > & binEdge, const std::string & options ) = 0;
  virtual IDevHistogram1D* createHistogram1D( int nBins, double lowerEdge, double upperEdge ) = 0;
  virtual IDevHistogram1D* createCopy( const AIDA::IHistogram1D & original ) = 0;
  /**
   * Creation of 2-dimensional binned histograms
   */                             
  virtual IDevHistogram2D* createHistogram2D( const std::string & title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, const std::string & options  = "") = 0;
  virtual IDevHistogram2D* createHistogram2D( const std::string & title, const std::vector< double > & binEdgeX, const std::vector< double > & binEdgeY, const std::string & options ) = 0;
  virtual IDevHistogram2D* createHistogram2D( int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY ) = 0;
  virtual IDevHistogram2D* createCopy( const AIDA::IHistogram2D & original ) = 0;
  /**
   * Creation of 3-dimensional binned histograms
   */                             
  virtual IDevHistogram3D* createHistogram3D( const std::string & title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, int nBinsZ, double lowerEdgeZ, double upperEdgeZ, const std::string & options = "" ) = 0;
  virtual IDevHistogram3D* createHistogram3D( const std::string & title, const std::vector< double > & binEdgeX, const std::vector< double > & binEdgeY, const std::vector< double > & binEdgeZ, const std::string & options ) = 0;
  virtual IDevHistogram3D* createHistogram3D( int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, int nBinsZ, double lowerEdgeZ, double upperEdgeZ ) = 0;
  virtual IDevHistogram3D* createCopy( const AIDA::IHistogram3D & original ) = 0;
  /**
   * Creation of 1-dimensional profile histograms
   */
  virtual IDevProfile1D* createProfile1D( const std::string & title, int nBins, double lowerEdge, double upperEdge, const std::string & options = "") = 0;
  virtual IDevProfile1D* createProfile1D( const std::string & title, int nBins, double lowerEdge, double upperEdge, double lowerValue, double upperValue, const std::string & options = "") = 0;
  virtual IDevProfile1D* createProfile1D( const std::string & title, const std::vector< double > & binEdge, const std::string & options ) = 0;
  virtual IDevProfile1D* createProfile1D( const std::string & title, const std::vector< double > & binEdge, double lowerValue, double upperValue, const std::string & options ) = 0;
  virtual IDevProfile1D* createProfile1D( int nBins, double lowerEdge, double upperEdge ) = 0;
  virtual IDevProfile1D* createProfile1D( int nBins, double lowerEdge, double upperEdge, double lowerValue, double upperValue ) = 0;
  virtual IDevProfile1D* createCopy( const AIDA::IProfile1D & original ) = 0;
  /**
   * Creation of 2-dimensional profile histograms
   */
  virtual IDevProfile2D* createProfile2D( const std::string & title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, const std::string & options = "") = 0;
  virtual IDevProfile2D* createProfile2D( const std::string & title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, double lowerValue, double upperValue, const std::string & options = "") = 0;
  virtual IDevProfile2D* createProfile2D( const std::string & title, const std::vector< double > & binEdgeX, const std::vector< double > & binEdgeY, const std::string & options ) = 0;
  virtual IDevProfile2D* createProfile2D( const std::string & title, const std::vector< double > & binEdgeX, const std::vector< double > & binEdgeY, double lowerValue, double upperValue, const std::string & options ) = 0;
  virtual IDevProfile2D* createProfile2D( int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY ) = 0;
  virtual IDevProfile2D* createProfile2D( int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, double lowerValue, double upperValue ) = 0;
  virtual IDevProfile2D* createCopy( const AIDA::IProfile2D & original ) = 0;
  // Methods which create histograms by performing operations on existing histograms
  // 1D Histograms 
  /**
   * Adds two 1D histograms
   *
   * @returns a+b
   * @throws IllegalArgumentException if histogram binnings are incompatible
   */
  virtual IDevHistogram1D* add( const AIDA::IHistogram1D & a, const AIDA::IHistogram1D & b ) = 0;
  /**
   * Subtracts two 1D histograms
   *
   * @returns a-b
   * @throws IllegalArgumentException if histogram binnings are incompatible
   */
  virtual IDevHistogram1D* subtract( const AIDA::IHistogram1D & a, const AIDA::IHistogram1D & b ) = 0;
  /**
   * Multiplies two 1D histograms
   *
   * @returns a*b
   * @throws IllegalArgumentException if histogram binnings are incompatible
   */
  virtual IDevHistogram1D* multiply( const AIDA::IHistogram1D & a, const AIDA::IHistogram1D & b ) = 0;
  /**
   * Divides two 1D histograms
   *
   * @returns a/b
   * @throws IllegalArgumentException if histogram binnings are incompatible
   */
   virtual IDevHistogram1D* divide( const AIDA::IHistogram1D & a, const AIDA::IHistogram1D & b ) = 0;
    
   // 2D Histograms
   /**
    * Adds two 2D histograms
    *
    * @returns a+b
    * @throws IllegalArgumentException if histogram binnings are incompatible
    */
   virtual IDevHistogram2D* add( const AIDA::IHistogram2D & a, const AIDA::IHistogram2D & b ) = 0;
   /**
    * Subtracts two 2D histograms
    *
    * @returns a-b
    * @throws IllegalArgumentException if histogram binnings are incompatible
    */
   virtual IDevHistogram2D* subtract( const AIDA::IHistogram2D & a, const AIDA::IHistogram2D & b ) = 0;
   /**
    * Multiplies two 2D histograms
    *
    * @returns a*b
    * @throws IllegalArgumentException if histogram binnings are incompatible
    */
   virtual IDevHistogram2D* multiply( const AIDA::IHistogram2D & a, const AIDA::IHistogram2D & b ) = 0;
   /**
    * Divides two 2D histograms
    *
    * @returns a/b
    * @throws IllegalArgumentException if histogram binnings are incompatible
    */
   virtual IDevHistogram2D* divide( const AIDA::IHistogram2D & a, const AIDA::IHistogram2D & b ) = 0;
   // 3D Histograms
   /**
    * Adds two 3D histograms
    *
    * @returns a+b
    * @throws IllegalArgumentException if histogram binnings are incompatible
    */
   virtual IDevHistogram3D* add( const AIDA::IHistogram3D & a, const AIDA::IHistogram3D & b ) = 0;
   /**
    * Subtracts two 3D histograms
    *
    * @returns a-b
    * @throws IllegalArgumentException if histogram binnings are incompatible
    */
   virtual IDevHistogram3D* subtract( const AIDA::IHistogram3D & a, const AIDA::IHistogram3D & b ) = 0;
   /**
    * Multiplies two 3D histograms
    *
    * @returns a*b
    * @throws IllegalArgumentException if histogram binnings are incompatible
    */
   virtual IDevHistogram3D* multiply( const AIDA::IHistogram3D & a, const AIDA::IHistogram3D & b ) = 0;
   /**
    * Divides two 3D histograms
    *
    * @returns a/b
    * @throws IllegalArgumentException if histogram binnings are incompatible
    */
   virtual IDevHistogram3D* divide( const AIDA::IHistogram3D & a, const AIDA::IHistogram3D & b ) = 0;
   
   // 2D Projections
   /**
    * Create a projection from a 2D histogram parallel to the X axis.
    * Equivalent to <tt>sliceX(UNDERFLOW_BIN,OVERFLOW_BIN)</tt>.
    */
   virtual IDevHistogram1D* projectionX( const AIDA::IHistogram2D & h ) = 0;
   /**
    * Create a projection from a 2D histogram parallel to the Y axis.
    * Equivalent to <tt>sliceY(UNDERFLOW_BIN,OVERFLOW_BIN)</tt>.
    */
   virtual IDevHistogram1D* projectionY( const AIDA::IHistogram2D & h ) = 0;
   /**
    * Slice parallel to the Y axis from a 2D histogram at bin indexY and one bin wide.
    * Equivalent to <tt>sliceX(indexY,indexY)</tt>.
    */
   virtual IDevHistogram1D* sliceX( const AIDA::IHistogram2D & h, int indexY ) = 0;
   /**
    * Slice parallel to the X axis from a 2D histogram at bin indexX and one bin wide.
    * Equivalent to <tt>sliceY(indexX,indexX)</tt>.
    */
   virtual IDevHistogram1D* sliceY( const AIDA::IHistogram2D & h, int indexX ) = 0;
   /**
    * Create a slice parallel to the X axis from a 2D histogram, 
    * between "indexY1" and "indexY2" (inclusive).
    * The returned IHistogram1D represents an instantaneous snapshot of the
    * histogram at the time the slice was created.
    */ 
   virtual IDevHistogram1D* sliceX( const AIDA::IHistogram2D & h, int indexY1, int indexY2 ) = 0;
   /**
    * Create a slice parallel to the Y axis from a 2D histogram, 
    * between "indexX1" and "indexX2" (inclusive).
    * The returned IHistogram1D represents an instantaneous snapshot of the
    * histogram at the time the slice was created.
    */ 
   virtual IDevHistogram1D* sliceY( const AIDA::IHistogram2D & h, int indexX1, int indexX2 ) = 0;
   /**
    * Create a projection parallel to the X axis from a 3D histogram.
    * Equivalent to <tt>sliceXY(UNDERFLOW_BIN,OVERFLOW_BIN)</tt>.
    */
   virtual IDevHistogram2D* projectionXY( const AIDA::IHistogram3D & h ) = 0;
   /**
    * Create a projection parallel to the Z axis from a 3D histogram.
    * Equivalent to <tt>sliceXZ(UNDERFLOW_BIN,OVERFLOW_BIN)</tt>.
    */
   virtual IDevHistogram2D* projectionXZ( const AIDA::IHistogram3D & h ) = 0;
   /**
    * Create a projection parallel to the Y axis from a 3D histogram.
    * Equivalent to <tt>sliceYZ(UNDERFLOW_BIN,OVERFLOW_BIN)</tt>.
    */
   virtual IDevHistogram2D* projectionYZ( const AIDA::IHistogram3D & h ) = 0;
   /**
    * Create a slice perpendicular to the Z axis from a 3D histogram,
    * between "indexZ1" and "indexZ2" (inclusive).
    * The returned IHistogram2D represents an instantaneous snapshot of the
    * histogram at the time the slice was created.
    * The X axis of the returned histogram corresponds to the X axis of this histogram.
    * The Y axis of the returned histogram corresponds to the Y axis of this histogram.
    */
   virtual IDevHistogram2D* sliceXY( const AIDA::IHistogram3D & h, int indexZ1, int indexZ2 ) = 0;
   /**
    * Create a slice perpendicular to the Y axis from a 3D histogram,
    * between "indexY1" and "indexY2" (inclusive).
    * The returned IHistogram2D represents an instantaneous snapshot of the
    * histogram at the time the slice was created.
    * The X axis of the returned histogram corresponds to the X axis of this histogram.
    * The Y axis of the returned histogram corresponds to the Z axis of this histogram.
    */
   virtual IDevHistogram2D* sliceXZ( const AIDA::IHistogram3D & h, int indexY1, int indexY2 ) = 0;
   /**
    * Create a slice perpendicular to the X axis from a 3D histogram,
    * between "indexX1" and "indexX2" (inclusive).
    * The returned IHistogram2D represents an instantaneous snapshot of the
    * histogram at the time the slice was created.
    * The X axis of the returned histogram corresponds to the Y axis of this histogram.
    * The Y axis of the returned histogram corresponds to the Z axis of this histogram.
    */
   virtual IDevHistogram2D* sliceYZ( const AIDA::IHistogram3D & h, int indexX1, int indexX2 ) = 0;

};

  }
}

#endif /* ifndef AIDA_IDEVHISTOGRAMFACTORY */
