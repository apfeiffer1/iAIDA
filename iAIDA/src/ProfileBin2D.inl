inline bool
iAIDA::AIDA_Histogram_native::ProfileBin2D::fill( double weight, double coordX, double coordY, double coordZ )
{
  m_sumOfWeightTimesCoordinate += weight * coordY;
  return iAIDA::AIDA_Histogram_native::ProfileBin1D::fill( weight, coordX, coordZ );
}
