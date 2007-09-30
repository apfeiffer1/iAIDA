#ifndef IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_HISTOGRAM_CONVERTER_H
#define IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_HISTOGRAM_CONVERTER_H

namespace AIDA {
  class IHistogram1D;
  class IHistogram2D;
  class IProfile1D;
  
  namespace Dev {
    class IDevHistogramFactory;
    class IDevHistogram1D;
    class IDevHistogram2D;
    class IDevProfile1D;
  }
}


namespace iAIDA {
  namespace AIDA_HBookStore {

/// Converter class for AIDA histograms to HBook

class HistogramConverter
{
public:
  static const HistogramConverter& theConverter();

protected:
  HistogramConverter() {}
  HistogramConverter( const HistogramConverter& );
  HistogramConverter& operator=( const HistogramConverter& );

public:
  ~HistogramConverter() {}

  bool convertToHBook( const AIDA::IHistogram1D& h, int id, bool storeErrors = false ) const;
  bool convertToHBook( const AIDA::IHistogram2D& h, int id, bool storeErrors = false ) const;
  bool convertToHBook( const AIDA::IProfile1D& h, int id ) const;

  AIDA::Dev::IDevHistogram1D* createHistogram1DFromHBook( AIDA::Dev::IDevHistogramFactory& factory,
							  int id ) const;
  AIDA::Dev::IDevHistogram2D* createHistogram2DFromHBook( AIDA::Dev::IDevHistogramFactory& factory,
							  int id ) const;
  AIDA::Dev::IDevProfile1D* createProfile1DFromHBook( AIDA::Dev::IDevHistogramFactory& factory,
						      int id ) const;
};

  }
}


#endif
