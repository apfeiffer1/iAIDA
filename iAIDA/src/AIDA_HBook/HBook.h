// -*- C++ -*-
#ifndef IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_HBOOK_H
#define IAIDA_AIDA_HBOOKSTORE_AIDA_HBOOKSTORE_HBOOK_H

#include <string>
#include <utility>
#include <vector>

namespace iAIDA {
  namespace AIDA_HBookStore {

/**
 * Class of static functions. Simply translates
 * HBOOK function calls into less unwieldy forms
 * (no casts or pointers, for example) and uses
 * sane types and meaningful names. See the HBOOK manual at
 * <a href="http://consult.cern.ch/writeup/hbook/">http://consult.cern.ch/writeup/hbook/</a>
 * for more information.
 *
 * @brief Collection of HBOOK subroutines as a class with nicer method names
 */

class HBook
{
 protected:
  HBook(void);
  HBook(const HBook&);
  HBook& operator=(const HBook&);
 ~HBook(void);
  
 public:
  /// Maximum length of pathnames, titles etc.
  static const unsigned int MAXLEN = 256;
  /// Large number for cycle purging (and other things)
  static const int VERY_LARGE = 99999;
  /// Max length of variable names in a tuple.
  static const unsigned int MAXTAGLEN = 8; 
  /// Max length of common block name in HBNAM*
  static const unsigned int MAX_BLOCKNAME_LENGTH = 8;
  /// Max number of variables in a tuple.
  static const unsigned int MAXVAR = 256; 

  enum FileMode   { NEW, READ_ONLY, UPDATE, EXCHANGE };
  enum ObjectType { HISTO1D, HISTO2D, PROFILE, NTUPLE, DIRECTORY, UNKNOWN };

 public:
  /// calls HLIMIT.
  static void initialize();

  /// calls HROPEN. Returns false if failed.
  static bool openDirectAccessFile(const int& LUN, const std::string& symbolicName,
				   const std::string& fileName, const FileMode& mode,
				   int& recordLength);
  /// calls HREND
  static void releaseZebraDirectory(const std::string& name);

  /// calls RZOPEN, RZFILE, and RZCLOS
  static bool removeLock(const int& lun, const std::string& filename, const int& recLen = 1024);

  /// calls HCDIR(dirName)
  static void changeDirectory(const std::string& dirName);
  /// calls HCDIR('R') and returns current directory 
  static std::string getCurrentDirectory(void);
  /// calls HMDIR
  static void makeNewDirectory(const std::string& dirName);
  /// calls HDDIR
  static void deleteExistingDirectory(const std::string& dirName);

  /// calls HSCR
  static void deleteObjectFromDiskDirectory(const int& id);
  /// calls HLNEXT 
  static void getNextObjectInDiskDirectory(int& id, const ObjectType& typeToFind,
					   ObjectType& foundType, 
					   std::string& foundTitle);
  /// calls HKIND(' ')
  static ObjectType typeOfObject(const int& id);

  /// calls HDELET
  static void deleteFromMemory(const int& id);
  /// calls HRIN
  static void loadObjectIntoMemory(const int& id, const int& offset = 0,
				   const int& cycle = 999999);
  /// calls HROUT
  static void flushObjectToFile(const int& id);

  /// calls HNOENT
  static int numberOfEntries(const int& id);
  /// calls HGIVE
  static void getHistoParameters(const int& id, std::string& title, 
				 int& numberOfXBins, float& lowerEdgeX, float& upperEdgeX,
				 int& numberOfYBins, float& lowerEdgeY, float& upperEdgeY);
  /// calls internally HKIND( 'A' )
  static bool hasVariableBinSizes( const int& id );
  /// calls HSUM
  static float sumOfInRangeBinsInHisto(const int& id);

  /// calls HSTATI.
  static float histoEbe(const int& id );
  static float histoRms(const int& id );
  static float histoRmsX(const int& id );
  static float histoRmsY(const int& id );
  /// calls internally HKIND( 'A' )
  static bool areErrorsStored( const int& id );

  /// calls HI (f77 metric [1,N])
  static float binContent(const int& id, const int& binNumber);
  /// calls HIE (f77 metric [1,N])
  static float binError(const int& id, const int& binNumber);
  /// calls HIX (f77 metric [1,N])
  static float binLowerEdge(const int& id, const int& binNumber);

  /// calls HIJ (f77 metric [1,N])
  static float binContent(const int& id, const int& binNumberX, const int& binNumberY);
  /// calls HIJE (f77 metric [1,N])
  static float binError(const int& id, const int& binNumberX, const int& binNumberY);
  /// calls HIJXY (f77 metric [1,N]). First number in pair is x edge, second is y.
  static std::pair<float,float> binLowerEdges(const int& id, 
					      const int& binNumberX, const int& binNumberY);

  /// calls HBOOK1
  static void bookFixedBin1DHisto(const int& id, const std::string title,
				  const int& nbx, const float& lowX, const float& highX);
  /// calls HBPROF.
  static void bookFixedBinProfileHisto( const int& id, const std::string title,
				        const int& nb, 
				        const float& lowX, const float& highX,
				        const float& lowY, const float& highY );
  /// calls HBOOKB
  static void bookVariableBin1DHisto(const int& id, const std::string title,
				     const std::vector<float>& binEdges);
  /// calls HBOOK2
  static void bookFixedBin2DHisto(const int& id, const std::string title,
				  const int& nbx, const float& lowX, const float& highX,
				  const int& nby, const float& lowY, const float& highY);

  /// calls HBARX
  static void useErrorsForHisto1D( const int& id );
  /// calls HBAR2
  static void useErrorsForHisto2D( const int& id );
  /// calls HFILL
  static void fillHisto( const int& id, const float& x, const float& y, const float& w );

  /// calls HBOOKN
  static bool bookRWNtuple(const int& id, const std::string& title, 
			   const std::vector<std::string>& columnNames,
			   const std::string& zebraDirectory, const int& bufferSize);
  /// calls HBNT
  static void declareCWNTuple(const int& id, const std::string& title);
  /// calls HBNAME. Returns false if strings not well formed.
  static bool describeCWNvariables(const int& id, const std::string& blockName,
                                   void* firstVariableInCommonBlock, 
                                   const std::string& steeringString);
  /// calls HBNAMC. Returns false if strings not well formed.
  static bool describeCWNvariablesChar(const int& id, const std::string& blockName,
                                       char* firstVariableInCommonBlock, 
                                       const std::string& steeringString);
  /// calls HFN.
  static void fillRWNtuple( const int& id, const std::vector<float>& columns );

  /// calls HGNPAR
  static void refreshRWNtuplePointers(const int& id);
  ///@brief calls HGNF (f77 metric [1,N]). Returns false if failed. You <b>must</b> call
  /// refreshNtuplePointers() before calling this for the first time.
  static bool fastFillArrayFromRWNtupleRow(const int& id, const int& rowNumber, 
					   float* destinationArray);

  /// calls HFNT.
  static void fillCWNtuple(const int& id);

  /// calls HRESET
  static void resetTuple(const int& id);

  /// calls HNTNEW
  static bool isCWNtuple( const int& id );
  /// calls HGIVEN
  static void getTupleParameters(const int& id, std::string& title, 
				 std::vector<std::string>& columnNames,
				 std::vector<std::pair<float,float> >& columnMinAndMax);
  /// calls HNTVDEF
  static std::pair< std::string, std::string > getCWNtupleVariableDescription( const int& id, const int& iVar );

  /// calls HGNT. Returns false if row not found.
  static bool readCWNtupleRow(const int& id, const int& rowNumber );

  /// calls HGNTV. Returns false if row not found.
  static bool fillBuffersFromCWNtupleRow(const int& id, const int& rowNumber, const std::vector<std::string>& variables );

  /// calls HGNTF. Returns false if row not found.
  static bool fastFillBuffersFromCWNtupleRow(const int& id, const int& rowNumber);

  /// calls Olivier hack to set contents for Histogram 1D
  static void setHisto1DContents(const int & id, const int & entries, 
				 const float & eqBinEntries,
				 const float & mean, const float & rms, 
				 std::vector<float> & binHeights, 
				 std::vector<float> & binErrors, 
				 bool setErrors = false); 

  /// calls Olivier hack to set contents foe Histogram 2D (and profile ? ) 
    static void setHisto2DContents(const int & id, const int & entries, 
				   std::vector<float> & binHeights, 
				   std::vector<float> & binErrors, 
				   bool setErrors = false); 

};

  }
}


#endif
