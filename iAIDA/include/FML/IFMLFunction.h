# ifndef FML_IFMLFunction_h__included__
# define FML_IFMLFunction_h__included__

//////////////////////////////////////////
// Fitting and Minimization Library
//
// Component: FML/C++
// Author   : Jakub.Moscicki@cern.ch
//
// CERN/IT, Geneva, 2001
//
//////////////////////////////////////////

# include <vector>
# include <string>
# include <memory>

namespace fml{

  // namespace FML {

    class FunctionCatalogue;
    class RangeSet; 

/** 
 * @interface IFMLFunction
 *
 * FML model function interface. It contains advanced mechanisms required for
 * model functions.
 *
 * The standard convention of referring to variables by name:
 * "x[0]" corresponds to variable number 0 etc.
 *
 */
class IFMLFunction {
 
public:  

    /**
     * Scalar value of the function.
     */
    virtual double value(const std::vector<double>& x) const = 0;

    /**
     * Dimensionality of the domain space.
     */
    virtual int dimension() const = 0;

    /**
     * Compare if functions are the same.  
     * PENDING: define exactly what it means ;)
     */
    virtual bool isEqual(const IFMLFunction * f) const = 0;

    /**
     * Gradient of the function.
     * The result is valid only if providesGradient()==true. 
     * Otherwise the result is some default (empty) vector.
     */
    virtual const std::vector < double > & gradient(const std::vector < double > & x)  const = 0;

    /**
     * Determine whether function is able to compute gradient.
     */
    virtual bool providesGradient() const = 0;

    /**
     * Get the symbolic name of i-th variable.
     */
    virtual std::string variableName(int i) const = 0;

    /**
     * Get all the names in a vector.
     */
    virtual const std::vector<std::string>& variableNames() const = 0;

    /**
      * Set new vector of parameters.
      */
    virtual bool setParameters(const std::vector<double>& params) = 0;

    /**
     * Retrieve current vector of parameters.
     */
    virtual const std::vector<double>& parameters() const = 0;

    /**
     * Number of parameters.
     */
    virtual int numberOfParameters() const = 0;

    /**
     * Names of paramaters.
     * Names should be unique. Case-insensitive for comparison. 
     * Case-sensitive for storage and output.
     * All or several names may be left blank "" if the implementation
     * does not want to provide them.
     */
    virtual const std::vector<std::string>& parameterNames() const = 0;
  
  /**
   * Set parameter by name.
   */
  virtual bool setParameter(std::string name, double x) = 0;

  /**
   * Set parameter by index.
   */
  virtual bool setParameter(int ix, double x) = 0;

  virtual double parameter(std::string name) const = 0;

  /**
   * Return -1 if parameter not found (name undefined).
   * Leave it because there are use cases - some users prefer to use numbers.
   */
  virtual int indexOfParameter(std::string name) const = 0;

  /**
   * String describing the metatype and implementation of the function.
   *
   * Generic format of the string:
   *
   * codelet:TYPE:LOCATION
   *
   * TYPE part contains only a 'typename' string
   * which is a metatype identifier (used in plugins and catalogues)
   *
   * LOCATION may be: 'file', 'verbatim', or 'catalogue'
   *
   * Detailed description:
   *
   * codelet:typename - no location specified (using 'catalogue')
   *
   * codelet:typename:file:/filename.cpp    -  a c++ source file
   * codelet:typename:file:/filename.py     -  a python source file
   * codelet:typename:file:/filename.so     -  a shared library
   *
   * codelet:typename:catalogue        - entry in a catalogue repository
   *
   * codelet:typename:verbatim:cpp \n  - c++ source file included verbatim 
   *                                     after \n
   *
   * codelet:typename:verbatim:py  \n  - python source file included 
   *                                     verbatim aftern \n
   *
   *
   * Comments:
   *
   * file:/filename is a relative path to $AIDA_FUNCTION_DEFAULT_DIR
   * file://filename is an absolute path
   *
   * Examples:
   *
   * codelet:G:catalogue
   * codelet:userSinus:verbatim:cpp \n { return sin(x); }
   */
  virtual std::string codeletString() const = 0;

  /**
   * Full name of the function.
   * Examples: "G", "P5", "E+P3"
   */
  virtual std::string functionName() const = 0;


  /// Return false if function does not provide the normalization capability.
  virtual  bool providesNormalization() const = 0;

  /**
   * Normalize by internally adjusting some of the parameters.
   * When normalization is on then integral of the function in 
   * the normalization range equals to 1.
   */
  virtual  void normalize(bool on) = 0;
  
  /**
   * Return true if normalization is currently switched on.
   */
  virtual  bool isNormalized() const = 0;
  
  /**
   * Compute gradient with respect to parameters.
   */
  virtual const std::vector<double>& parameterGradient(const std::vector<double>& x) const = 0;
  
  /** 
   * Return true if provides parameter gradient.
   */
  virtual bool providesParameterGradient() const = 0;
  
  /**
   * Set the normalization range from -inf to +inf in the i-th 
   * axis (dimension).
   *
   * Full range is the default if not set otherwise.
   * It is still possible to evaluate function outside of the 
   * normalization range.
   * 
   *  LM: modified to use RangeSet
   */
  virtual RangeSet *  normalizationRange(int iAxis) const = 0;

  /**
   * Set full range in all axes.
   */
  virtual void includeNormalizationAll() = 0;

  /**
   * Set empty range in all axes.
   */
  virtual void excludeNormalizationAll() = 0;

  /** 
   *  set normalization range
   */ 
  virtual bool setNormalizationRange(int ix, const RangeSet & rs) = 0;
  /**
   * Make an exact copy of an object.
   */
  virtual std::auto_ptr<IFMLFunction> clone() const = 0;

  /**
   * Return false if the state of the function may not be completely 
   * set with the setters below and would require special actions 
   * which the function does not support, i.e. function is not streamable
   *
   * Example: FitMethod is a function which needs an external data object.
   * Its state may not be stored to disk because that would require 
   * to store also the data object and regenerate the reference automatically.
   * While such implementation is possible, right now it is not needed
   * so function is NOT streamable.
   */

  virtual bool isStreamable() const = 0;

  virtual ~IFMLFunction() {}

protected:

  virtual bool setDimension(int dimx) = 0;
  virtual bool setNumberOfParameters(int parnum) = 0;
  virtual bool setProvidesGradient(bool provides_gradient) = 0;
  virtual bool setParName(int ix, std::string name) = 0;
  virtual bool setVarName(int ix, std::string name) = 0;
  virtual bool setCodeletString(std::string cds) = 0;
  virtual bool setFunctionName(std::string fname) = 0;
  virtual bool setProvidesParameterGradient(bool yes) = 0;
  virtual bool setNormalization(bool provides, bool is_normalized) = 0;


  /** @name dynamic object reconstruction.
   * required if object has to take any active action when streaming
   * in order to reconstruct its state. example:
   * compound function needs to create member functions.
   *
   * interaction pattern for dynamic reconstruction:
       - create f with DefaultConstructor()
       - f._constructThis(functionName) // P5 (for example)
   *
   * interaction pattern for streaming in:
       - create f with DefaultConstructor()
       - f._constructThis(functionName)
       - read the state of the object from the stream (stream-in)
       - f._preStreamThis(codelet) // codelet contains P (for example)
       - f.setDimension() ... and all the other setter methods
       - f._postStreamThis(codelet)
   *  
   */

  //@{
  /// construct this object from a codelet string.
  /// the streamer calls this method _after_ it called all setters.
  //  virtual bool _postStreamThis(std::string codelet_string, const FunctionCatalogue* cat = 0) = 0; 

  /// construct this object from a codelet string.
  /// the streamer calls this method on an empty object (default constructed)
  /// _before_ calling any setters.
  //  virtual bool _preStreamThis(std::string codelet_string, const FunctionCatalogue* cat = 0) = 0;

  virtual bool _constructThis(std::string functionName, const FunctionCatalogue* cat = 0) = 0;
  //@}

  /// Friendly setter used for striming and other internal purposes.
  friend class FunctionSetterHelper;
  
};

// }

}

#endif 
