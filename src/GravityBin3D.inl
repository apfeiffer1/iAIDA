inline bool
iAIDA::AIDA_Histogram_native::GravityBin3D::fill( double weight, double coordX, double coordY, double coordZ )
{
  m_sumOfWeightTimesCoord += weight * coordZ;
  return iAIDA::AIDA_Histogram_native::GravityBin2D::fill( weight, coordX, coordY );
}
