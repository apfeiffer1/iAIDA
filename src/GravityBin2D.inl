inline bool
iAIDA::AIDA_Histogram_native::GravityBin2D::fill( double weight, double coordX, double coordY )
{
  m_sumOfWeightTimesCoord += weight * coordY;
  return iAIDA::AIDA_Histogram_native::GravityBin1D::fill( weight, coordX );
}
