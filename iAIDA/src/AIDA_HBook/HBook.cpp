// -*- C++ -*-
#include "HBook.h"


#ifdef OLDSTREAMS
# include <strstream>
# define ostringstream ostrstream
# define istringstream istrstream
#else
# include <sstream>
#endif

#include <cctype>

#ifdef __X86_64
#define HBK_FLOAT_OR_DOUBLE double
#else
#define HBK_FLOAT_OR_DOUBLE float
#endif

extern "C" {
  void hlimit_(int*);

  void hropen_(int*, char*, char*, char*, int*, int*, int, int, int);
  void hrend_(char*, int);

  void hcdir_(char*, char*, int, int);
  void hmdir_(char*, char*, int, int);
  void hrdir_(int*, char*, int*, int );
  void hddir_ (char* dir, int Ddir);

  void hscr_(int*, int*, char*, int);
  void hlnext_(int*, char*, char*, char*, int, int, int);
  void hkind_ (int* id, int* attr_array, char* option, int Doption);

  void hdelet_(int*);
  void hrout_(int*, int*, char*, int);
  void hrin_(int*, int*, int*);

  void hnoent_(int*, int*);
  void hgive_(int*, char*, int*, float*, float*, int*, float*, float*, int*, int*, int);

  HBK_FLOAT_OR_DOUBLE hsum_(int*);
  HBK_FLOAT_OR_DOUBLE hstati_(int*, int*, char*, int*, int);

  HBK_FLOAT_OR_DOUBLE hi_(int* id, int* chanx);
  HBK_FLOAT_OR_DOUBLE hie_(int* id, int* chanx);
  void hix_(int* id, int* xchan, float* xlo);

  HBK_FLOAT_OR_DOUBLE hij_(int* id, int* chanx, int* chany);
  HBK_FLOAT_OR_DOUBLE hije_(int* id, int* chanx, int* chany);
  void hijxy_(int* id, int* xchan, int* ychan, float* xlo, float* ylo);

  void hbook1_(int*, char*, int*, float*, float*, float*, int);
  void hbook2_(int*, char*, int*, float*, float*, int*, float*, float*, float*, int);
  void hbookb_(int* id, char* title, int* nbins, float* binEdges, 
	       float* vmx, int Dtitle);
  void hbprof_(int* id, char* filename, int* nb, 
	       float* xlo, float* xhi, float* ylo, float* yhi, 
	      char* options, int Dfilename, int Doptions);
  void hbarx_(int*);
  void hbar2_(int*);
  void hfill_(int*, float*, float*, float*);

  void hbookn_(int*, char*, int*, char*, int*, char*, int, int, int);
  void hbnt_(int* id, char* title, char* diskResOption, int Dtitle, int DdiskResOption);
  void hbset_(char* optionName, int* optionValue, int* errorFlag, int DoptionName);
  void hbname_(int* id, char* blockName, void* firstVariable, char* steeringString,
	       int DblockName, int DsteeringString);
  void hbnamc_(int* id, char* blockName, char* firstVariable, char* steeringString,
	       int DblockName, int DsteeringString);

  void hfn_(int*, float*);
  void hgnpar_(int*, char*, int);
  void hgnf_(int*, int*, float*, int*);

  void hfnt_(int* id);
  void hreset_(int*, char*, int);
  int  hntnew_(int*);
  void hgiven_(int*, char*, int*, char*, float*, float*, int, int);
  void hntvdef_(int* id, int* varNum, char* tagName, char* blockName, int* itype,
		int DtagName, int DblockName);
  void hgnt_( int* id, int* rowNumber, int* errorCode );
  void hgntv_( int* id, char* varList, int* nVars, int* rowNumber, int* errorCode, int DvarList );
  void hgntf_ (int* id, int* rowNumber, int* errorFlag);
}

extern "C" {
  void rzopen_(int* lun, char* tdir, char* fname, char* opt, int* reclen, int* ierr, 
	       int Dtdir, int Dfame, int Dopt);
  void rzfile_(int* lun, char* tdir, char* opt, int Dtdir, int Dopt);
  void rzclos_(char* tdir, char* opt, int Dtdir, int Dopt);
  /// subroutines to set contents  of a given HBOOK 1D histogram (O.Couet Nov. 2001)
  void  hbf1al_(int* id, float* x, int* noent, float* sw, float* sww, float* swx, 
	      float* swxx, float* e, int* iopt);
  /// subroutines to set contents of a given HBOOK 2D histogram (O.Couet Nov. 2001)
  void  hbf2al_(int* id, float* x, int* noent, float* e, int* iopt);
  
}


static const int SizeOfPawCommon  = 5000000;
static const int ValueForIquest10 = 262144; // max number of records in an hbook file (default recl 1024)
long pawc_[SizeOfPawCommon];
long quest_[100];
extern "C" {
  // the function init_ will be called at dynamic loading time!
  void init_(void) { quest_[9] = ValueForIquest10; }
}

static bool isInitialized = false;

void
pi::AIDA_HBookStore::HBook::initialize()
{
  if ( ! isInitialized ) {
    int SizeOfPawCommonT = SizeOfPawCommon;
    hlimit_( &SizeOfPawCommonT );
    isInitialized = true;
  }
}


bool
pi::AIDA_HBookStore::HBook::openDirectAccessFile(const int& LUN, const std::string& symbolicName,
						     const std::string& fileName, const FileMode& mode,
						     int& recordLength)
{
  int tlun = LUN;
  std::string ttopdir = symbolicName;
  std::string tfname  = fileName;
  char tmode[2] = " ";
  int recLen = recordLength;
  const bool returnRecordLength = (recordLength <= 0);
  switch (mode) {
  case pi::AIDA_HBookStore::HBook::NEW:       { std::strcpy(tmode, "N"); break; }
  case pi::AIDA_HBookStore::HBook::READ_ONLY: { std::strcpy(tmode, " "); break; }
  case pi::AIDA_HBookStore::HBook::UPDATE:    { std::strcpy(tmode, "U"); break; }
  case pi::AIDA_HBookStore::HBook::EXCHANGE:  { std::strcpy(tmode, "X"); break; }
  default: return false;
  }
  int failed = 0;
  hropen_(&tlun, 
	  const_cast<char*>(ttopdir.c_str()), 
	  const_cast<char*>(tfname.c_str()), 
	  tmode, &recLen, &failed,
	  ttopdir.length(), tfname.length(), 1);
  if (returnRecordLength) recordLength = recLen;
  return (failed == 0);
}


void
pi::AIDA_HBookStore::HBook::releaseZebraDirectory(const std::string& name)
{
  std::string tname = name;
  int len = tname.length();
  hrend_(const_cast<char*>(tname.c_str()), len);

  std::string zebraDir;
  if ( name[0] == '/' ) zebraDir = name;
  else zebraDir = "//" + name;
  char opt[1] = {' '};
  rzclos_( const_cast< char*>( zebraDir.c_str() ), opt, static_cast<int>( zebraDir.size() ), 1 );
}

bool
pi::AIDA_HBookStore::HBook::removeLock(const int& lun, const std::string& filename, const int& recordLength )
{
  int tlun = lun;
  std::string ttopdir = "qX6AiJb";
  std::string tfname  = filename;
  std::string omode = "1";
  std::string fmode = "1";
  std::string rmode = " ";
  int recLen = recordLength;
  int failed = 0;
  rzopen_(&tlun, 
	  const_cast<char*>(ttopdir.c_str()), 
	  const_cast<char*>(tfname.c_str()),
	  const_cast<char*>(omode.c_str()), 
	  &recLen, &failed,
	  ttopdir.length(), tfname.length(), omode.length());
  if (failed != 0) return false;
  rzfile_(&tlun,
	  const_cast<char*>(ttopdir.c_str()),
	  const_cast<char*>(fmode.c_str()), 
	  ttopdir.length(), fmode.length());
  rzclos_(const_cast<char*>(ttopdir.c_str()),
	  const_cast<char*>(rmode.c_str()),
	  ttopdir.length(), rmode.length());
  return true;
}

void
pi::AIDA_HBookStore::HBook::changeDirectory(const std::string& dirName)
{
  std::string tmpDir = dirName;
  char blank[2] = " ";
  hcdir_(const_cast<char*>(tmpDir.c_str()),
	 blank, tmpDir.length(), 1);
}


std::string
pi::AIDA_HBookStore::HBook::getCurrentDirectory(void)
{
  char result[MAXLEN+1] = {'\0'};
  char read  [2]        =  "R";
  hcdir_(result, read, MAXLEN, 1);
  std::istringstream is( result );
  std::string s;
  is >> s;
  return s;
}


void
pi::AIDA_HBookStore::HBook::makeNewDirectory(const std::string& dirName)
{
  char blank[2] = " ";
  std::string tdir = dirName;
  int len = tdir.length();
  hmdir_(const_cast<char*>(tdir.c_str()), blank, len, 1);
}


void
pi::AIDA_HBookStore::HBook::deleteExistingDirectory(const std::string& dirName)
{
  std::string tdir = dirName;
  int len = tdir.length();
  hddir_(const_cast<char*>(tdir.c_str()), len);
}


void
pi::AIDA_HBookStore::HBook::deleteObjectFromDiskDirectory(const int& id)
{
  char blank[2] = " ";
  int  victim   = id;
  int  zero     = 0;
  hscr_(&victim, &zero, blank, 1);
}


void
pi::AIDA_HBookStore::HBook::getNextObjectInDiskDirectory(int& id,
							     const pi::AIDA_HBookStore::HBook::ObjectType& typeToFind,
							     pi::AIDA_HBookStore::HBook::ObjectType& foundType, 
							     std::string& foundTitle)
{
  char cFoundType [2]               = {'\0'};
  char cTypeToFind[2]               =  " ";
  char cFoundTitle[HBook::MAXLEN+2] = {'\0'};
  switch (typeToFind) {
  case pi::AIDA_HBookStore::HBook::HISTO1D:   { std::strcpy(cTypeToFind, "1"); break; }
  case pi::AIDA_HBookStore::HBook::HISTO2D:   { std::strcpy(cTypeToFind, "2"); break; }
  case pi::AIDA_HBookStore::HBook::PROFILE:   { std::strcpy(cTypeToFind, "1"); break; } // "P" not possible
  case pi::AIDA_HBookStore::HBook::NTUPLE:    { std::strcpy(cTypeToFind, "N"); break; }
  case pi::AIDA_HBookStore::HBook::DIRECTORY: { std::strcpy(cTypeToFind, "D"); break; }
  case pi::AIDA_HBookStore::HBook::UNKNOWN:   { std::strcpy(cTypeToFind, " "); break; } // look for everything
  }
  hlnext_(&id, cFoundType, cFoundTitle, cTypeToFind, 1, HBook::MAXLEN, 1);
  std::string sFoundType = cFoundType;
  if      (sFoundType == "1") foundType = pi::AIDA_HBookStore::HBook::HISTO1D; // but could be profile!
  else if (sFoundType == "2") foundType = pi::AIDA_HBookStore::HBook::HISTO2D;
  else if (sFoundType == "N") foundType = pi::AIDA_HBookStore::HBook::NTUPLE;
  else if (sFoundType == "D") foundType = pi::AIDA_HBookStore::HBook::DIRECTORY;
  else                        foundType = pi::AIDA_HBookStore::HBook::UNKNOWN;
  std::istringstream is( cFoundTitle );
  is >> foundTitle;
}


pi::AIDA_HBookStore::HBook::ObjectType
pi::AIDA_HBookStore::HBook::typeOfObject(const int& id)
{
  int tid = id;
  char opt[2] = " ";
  int kind[32]  = {0};
  hkind_(&tid, kind, opt, 1);
  const int& k = kind[0];
  switch (k) {
  case  1: return pi::AIDA_HBookStore::HBook::HISTO1D;
  case  2: return pi::AIDA_HBookStore::HBook::HISTO2D;
  case  4: return pi::AIDA_HBookStore::HBook::NTUPLE;
  case  8: return pi::AIDA_HBookStore::HBook::PROFILE;
  default: return pi::AIDA_HBookStore::HBook::UNKNOWN;
  }
}


void
pi::AIDA_HBookStore::HBook::deleteFromMemory(const int& id)
{
  int tid = id;
  hdelet_(&tid);
}


void
pi::AIDA_HBookStore::HBook::loadObjectIntoMemory(const int& id, const int& offset, const int& cycle)
{
  int tid = id;
  int toffset = offset;
  int tcycle = cycle;
  hrin_(&tid, &tcycle, &toffset);
}


void
pi::AIDA_HBookStore::HBook::flushObjectToFile(const int& id)
{
  int tid       = id;
  int icycle    = 0;
  char blank[2] = " ";
  hrout_(&tid, &icycle, blank, 1);
}


int
pi::AIDA_HBookStore::HBook::numberOfEntries(const int& id)
{
  int n = 0;
  int tid = id;
  hnoent_(&tid, &n);
  return n;
}


void
pi::AIDA_HBookStore::HBook::getHistoParameters(const int& id, std::string& title, 
						   int& numberOfXBins, float& lowerEdgeX, float& upperEdgeX,
						   int& numberOfYBins, float& lowerEdgeY, float& upperEdgeY)
{
  int  zebraAddress  = 0;
  int  nCharTitle    = 0;
  const int space    = pi::AIDA_HBookStore::HBook::MAXLEN;
  char cTitle[space] = {'\0'};
  int  tid           = id;
  hgive_(&tid, cTitle, 
	 &numberOfXBins, &lowerEdgeX, &upperEdgeX, 
	 &numberOfYBins, &lowerEdgeY, &upperEdgeY, 
	 &nCharTitle, &zebraAddress, space-1);//ND was space-1 in bininfo and space+2 in axisinfo!!!
  title = std::string( cTitle );
  int last = title.length() - 1;
  if (last < 0) return;
  while (! isgraph( title[last] ) && last >=0  ) title.erase(last--,1);
}


bool
pi::AIDA_HBookStore::HBook::hasVariableBinSizes( const int& id )
{
  int tid = id;
  char opt[2] = "A";
  int kind[32];
  hkind_(&tid, kind, opt, 1);
  if ( kind[5] == 0 ) return false;
  else return true;
}


float
pi::AIDA_HBookStore::HBook::sumOfInRangeBinsInHisto(const int& id)
{
  int tid = id;
  return hsum_(&tid);
}


float
pi::AIDA_HBookStore::HBook::histoEbe(const int& id )
{
  int tid = id;
  int icase = 3;
  char choice[5] = "HIST";
  int num = 0;
  return hstati_( &tid, &icase, choice, &num, 4 );
}

float
pi::AIDA_HBookStore::HBook::histoRms(const int& id )
{
  int tid = id;
  int icase = 2;
  char choice[5] = "HIST";
  int num = 0;
  return hstati_( &tid, &icase, choice, &num, 4 );
}

float
pi::AIDA_HBookStore::HBook::histoRmsX(const int& id )
{
  int tid = id;
  int icase = 2;
  char choice[5] = "PROX";
  int num = 1;
  return hstati_( &tid, &icase, choice, &num, 4 );
}

float
pi::AIDA_HBookStore::HBook::histoRmsY(const int& id )
{
  int tid = id;
  int icase = 2;
  char choice[5] = "PROY";
  int num = 1;
  return hstati_( &tid, &icase, choice, &num, 4 );
}

bool
pi::AIDA_HBookStore::HBook::areErrorsStored( const int& id )
{
  int tid = id;
  char opt[2] = "A";
  int kind[32];
  hkind_(&tid, kind, opt, 1);

  if ( kind[7] != 0 ) return true;
  else if ( kind[0] != 0 ) {
    if ( kind[8] != 0 ) return true;
    else return false;
  }
  else return false;
}


float
pi::AIDA_HBookStore::HBook::binContent(const int& id, const int& binNumber)
{
  int tid = id;
  int tbn = binNumber;
  return hi_(&tid, &tbn);
}



float
pi::AIDA_HBookStore::HBook::binError(const int& id, const int& binNumber)
{
  int tid = id;
  int tbn = binNumber;
  return hie_(&tid, &tbn);
}


float
pi::AIDA_HBookStore::HBook::binLowerEdge(const int& id, const int& binNumber)
{
  float x   = 0.0;
  int   tid = id;
  int   i   = binNumber;
  hix_(&tid, &i, &x);
  return x;
}


float
pi::AIDA_HBookStore::HBook::binContent(const int& id, const int& binNumberX, const int& binNumberY)
{
  int tid = id;
  int tbnx = binNumberX;
  int tbny = binNumberY;
   HBK_FLOAT_OR_DOUBLE val = hij_(&tid, &tbnx, &tbny);;
  return val;
}


float
pi::AIDA_HBookStore::HBook::binError(const int& id, const int& binNumberX, const int& binNumberY)
{
  int tid = id;
  int tbnx = binNumberX;
  int tbny = binNumberY;
  
  HBK_FLOAT_OR_DOUBLE val = hije_(&tid, &tbnx, &tbny);
  return val;
}



std::pair<float,float>
pi::AIDA_HBookStore::HBook::binLowerEdges( const int& id, 
					       const int& binNumberX, 
					       const int& binNumberY)
{
  float x   = 0.0;
  float y   = 0.0;
  int   tid = id;
  int   i   = binNumberX;
  int   j   = binNumberY;
  hijxy_(&tid, &i, &j, &x, &y);
  return std::make_pair(x,y);
}


void
pi::AIDA_HBookStore::HBook::bookFixedBin1DHisto(const int& id, const std::string title,
						    const int& nb, const float& lowX, const float& highX)
{
  int   tid          = id;
  std::string ttitle = title;
  int   n            = nb;
  float lx           = lowX;
  float hx           = highX;
  float words        = 0;
  hbook1_(&tid, const_cast<char*>(ttitle.c_str()), 
	  &n, &lx, &hx, &words, ttitle.length());
}


void
pi::AIDA_HBookStore::HBook::bookFixedBinProfileHisto(const int& id, const std::string title,
							 const int& nb, 
							 const float& lowX, const float& highX,
							 const float& lowY, const float& highY )
{
  int   tid          = id;
  std::string ttitle = title;
  int   len          = ttitle.length();
  int   n            = nb;
  float lx           = lowX;
  float hx           = highX;
  float ly           = lowY;
  float hy           = highY;
  std::string spr    = "S";
  hbprof_(&tid, 
	  const_cast<char*>(ttitle.c_str()),
	  &n, &lx, &hx, &ly, &hy, 
	  const_cast<char*>(spr.c_str()),
	  len, 1);
}


void
pi::AIDA_HBookStore::HBook::bookVariableBin1DHisto(const int& id, const std::string title,
						       const std::vector<float>& binEdges)
{
  const int nedges       = binEdges.size();
  int       nbins        = nedges - 1;
  if (nbins < 1) return;
  int       tid          = id;
  std::string ttitle     = title;
  float     words        = 0;
  float*    arr          = new float[nedges];
  for (int i = 0; i < nedges; ++i) arr[i] = binEdges[i];
  hbookb_(&tid, const_cast<char*>(ttitle.c_str()),
	  &nbins, arr, &words, ttitle.length());
  delete[] arr;
}


void
pi::AIDA_HBookStore::HBook::bookFixedBin2DHisto(const int& id, const std::string title,
						    const int& nbx, const float& lowX, const float& highX,
						    const int& nby, const float& lowY, const float& highY)
{
  int   tid          = id;
  std::string ttitle = title;
  int   nx           = nbx;
  int   ny           = nby;
  float lx           = lowX;
  float hx           = highX;
  float ly           = lowY;
  float hy           = highY;
  float words        = 0;
  hbook2_(&tid, const_cast<char*>(ttitle.c_str()), 
	  &nx, &lx, &hx, &ny, &ly, &hy, &words, ttitle.length());
}


void
pi::AIDA_HBookStore::HBook::useErrorsForHisto1D( const int& id )
{
  int tid = id;
  hbarx_( &tid );
}



void
pi::AIDA_HBookStore::HBook::useErrorsForHisto2D( const int& id )
{
  int tid = id;
  hbar2_( &tid );
}


void
pi::AIDA_HBookStore::HBook::fillHisto( const int& id, const float& x, const float& y, const float& w )
{
  int tid = id;
  float tx = x;
  float ty = y;
  float tw = w;
  hfill_( &tid, &tx, &ty, &tw );
}


bool
pi::AIDA_HBookStore::HBook::bookRWNtuple( const int& id, const std::string& title, 
					      const std::vector<std::string>& columnNames,
					      const std::string& zebraDirectory, const int& bufferSize)
{
  if ( columnNames.size() > pi::AIDA_HBookStore::HBook::MAXVAR ) return false;
  int tid = id;
  std::string ttitle = title;
  int ntags = static_cast<int>( columnNames.size() );
  std::string dir = zebraDirectory;
  int bufsize = bufferSize;
  std::ostringstream os;
  for ( std::vector<std::string>::const_iterator iVar = columnNames.begin();
	iVar != columnNames.end(); ++iVar ) {
    if ( iVar->size() > pi::AIDA_HBookStore::HBook::MAXTAGLEN ) return false;
    os << *iVar;
    for ( unsigned int i = pi::AIDA_HBookStore::HBook::MAXTAGLEN; i > iVar->size(); --i ) os << " ";
  }
  os << std::ends;
  const std::string columnString = os.str();
  char* columnCstring = const_cast< char * >( columnString.c_str() );
  hbookn_(&tid, 
          const_cast<char*>(ttitle.c_str()), 
          &ntags,
          const_cast<char*>(dir.c_str()), 
          &bufsize, 
          columnCstring, 
          ttitle.length(), 
          dir.length(), 
          MAXTAGLEN);
  return true;
}


void
pi::AIDA_HBookStore::HBook::declareCWNTuple(const int& id, const std::string& title)
{
  int tid = id;
  std::string ttitle = title;
  std::string diskRes = "D";
  hbnt_(&tid, 
        const_cast<char*>(title.c_str()), 
        const_cast<char*>(diskRes.c_str()), 
        title.length(), diskRes.length());
}


bool
pi::AIDA_HBookStore::HBook::describeCWNvariables(const int& id, const std::string& blockName,
						     void* firstVariableInCommonBlock, 
						     const std::string& steeringString)
{
  if (blockName.length() > pi::AIDA_HBookStore::HBook::MAX_BLOCKNAME_LENGTH) return false;
  if (0 == firstVariableInCommonBlock) return false;
  int tid = id;
  std::string tblockName = blockName;
  std::string tsteeringString = steeringString;
  if ( tsteeringString != " " ) {
    int emptySpacesToAdd = ( steeringString.size() )%4;
    for ( int i = 0; i < emptySpacesToAdd; ++i ) tsteeringString += " ";
  }
  hbname_(&tid, 
	  const_cast<char*>(tblockName.c_str()),
	  firstVariableInCommonBlock, 
	  const_cast<char*>(tsteeringString.c_str()),
	  tblockName.length(),
	  tsteeringString.length() );
  return true;
}


bool
pi::AIDA_HBookStore::HBook::describeCWNvariablesChar(const int& id, const std::string& blockName,
							 char* firstVariableInCommonBlock, 
							 const std::string& steeringString )
{
  if (blockName.length() > pi::AIDA_HBookStore::HBook::MAX_BLOCKNAME_LENGTH) return false;
  if (0 == firstVariableInCommonBlock) return false;
  int tid = id;
  std::string tblockName = blockName;
  std::string tsteeringString = steeringString;
  hbnamc_(&tid, 
	  const_cast<char*>(tblockName.c_str()),
	  firstVariableInCommonBlock, 
	  const_cast<char*>(tsteeringString.c_str()),
	  tblockName.length(),
	  tsteeringString.length() );
  return true;
}


void
pi::AIDA_HBookStore::HBook::fillRWNtuple( const int& id, const std::vector<float>& columns )
{
  int tid       = id;
  float* addr = const_cast<float*>( &( columns[0] ) );
  hfn_( &tid, addr );
}


void
pi::AIDA_HBookStore::HBook::refreshRWNtuplePointers( const int& id )
{
  int  tid      = id;
  char caller[] = "pi";
  int  len      = std::strlen(caller);
  hgnpar_(&tid, caller, len);
}


bool
pi::AIDA_HBookStore::HBook::fastFillArrayFromRWNtupleRow( const int& id, const int& rowNumber, 
							      float* destinationArray)
{
  int tid       = id;
  int rn        = rowNumber;
  int errorFlag = 0;
  hgnf_(&tid, &rn, destinationArray, &errorFlag);
  return (errorFlag == 0);
}

void
pi::AIDA_HBookStore::HBook::HBook::fillCWNtuple(const int& id)
{
  int tid = id;
  hfnt_(&tid);
}


void
pi::AIDA_HBookStore::HBook::resetTuple(const int& id )
{
  int tid = id;
  std::string ttitle = " ";
  hreset_(&tid, const_cast<char*>(ttitle.c_str()), ttitle.length());
}


bool
pi::AIDA_HBookStore::HBook::isCWNtuple( const int& id )
{
  int tid = id;
  return ( hntnew_(&tid) != 0 );
}


void
pi::AIDA_HBookStore::HBook::getTupleParameters( const int& id, std::string& title, 
						    std::vector<std::string>& columnNames,
						    std::vector<std::pair<float,float> >& columnMinAndMax)
{
  const int tagLength         = pi::AIDA_HBookStore::HBook::MAXTAGLEN;
  const int tagArrayLength    = pi::AIDA_HBookStore::HBook::MAXVAR*tagLength;
  const int titleArrayLength  = pi::AIDA_HBookStore::HBook::MAXLEN;
  const int minMaxArrayLength = pi::AIDA_HBookStore::HBook::MAXVAR;
  columnNames.clear();
  columnMinAndMax.clear();
  char*  titleArray = new char[titleArrayLength];
  char*  tagArray   = new char[tagArrayLength];
  for (int i = 0; i < titleArrayLength;  ++i) titleArray[i] = ' ';
  for (int i = 0; i < tagArrayLength;    ++i) tagArray[i]   = ' ';

  std::vector< float > maxValues( minMaxArrayLength, 0.0 );
  std::vector< float > minValues( minMaxArrayLength, 0.0 );

  int tid  = id;
  int ncol = minMaxArrayLength; 

  hgiven_(&tid, titleArray, &ncol, tagArray, &( minValues[0] ), &( maxValues[0] ),
	  titleArrayLength, tagLength );

  columnNames.clear();
  if ( ncol > 0 ) columnNames.reserve( ncol );
  for ( int i = 0; i < ncol; ++i ) {
    std::string sTagName = &( tagArray[ i*MAXTAGLEN ] );
    std::istringstream is( sTagName.substr( 0, MAXTAGLEN ).c_str() );
    std::string tagName;
    is >> tagName >> std::ws;
    columnNames.push_back( tagName );
  }

  title = std::string( titleArray );
  int nwhiteSpaces = 0;
  const int maxWhiteSpaces = 10; // patch to get rid of funny characters !!!
  for ( unsigned int i = 0; i < title.size(); ++i ) {
    char c = title[i];
    if ( c == '\n' || c == '\0' || c == '\r' || c == '\t' ) {
      title = title.substr( 0, i );
      break;
    }
    if ( c == ' ' || c == '\t' ) ++nwhiteSpaces;
    if ( nwhiteSpaces == maxWhiteSpaces )  {
      title = title.substr( 0, i );
      break;
    }
  }
  int last = title.length() - 1;
  if (last < 0) return;
  // LM: add check last >=0
  while (! isgraph(title[last]) && last>=0 ) title.erase(last--,1);

  delete [] titleArray;
  delete [] tagArray;

  for (int i = 0; i < ncol; ++i)
    columnMinAndMax.push_back(std::make_pair(minValues[i],maxValues[i]));
}


std::pair< std::string, std::string >
pi::AIDA_HBookStore::HBook::getCWNtupleVariableDescription( const int& id, const int& iVar )
{
  int tupleId = id;
  int var = iVar;
  char block[10] = {'\0'};
  char chtag[1000] = {'\0'};
  int itype = 0;
  hntvdef_( &tupleId, &var, chtag, block, &itype, 1000, 10 );
  std::istringstream ischtag( chtag );
  std::string schtag;
  ischtag >> schtag;
  std::istringstream isblock( block );
  std::string sblock;
  isblock >> sblock;
  return std::make_pair( sblock, schtag );
}

bool
pi::AIDA_HBookStore::HBook::readCWNtupleRow(const int& id, const int& rowNumber )
{
  int tid = id;
  int row = rowNumber;
  int errorCode = 0;
  hgnt_( &tid, &row, &errorCode );
  return (errorCode == 0);
}

bool
pi::AIDA_HBookStore::HBook::fillBuffersFromCWNtupleRow(const int& id, const int& rowNumber,
							   const std::vector< std::string >& variableList )
{
  if ( variableList.empty() ) return false;
  int tid = id;
  int row = rowNumber;
  int nVars = static_cast< int >( variableList.size() );
  int varListSize = static_cast<int>(pi::AIDA_HBookStore::HBook::MAXTAGLEN * variableList.size());
  char * varList = new char[ varListSize ];
  for ( int i = 0; i < varListSize; ++i ) varList[i] = ' ';
  for ( unsigned int i = 0; i < variableList.size(); ++i ) {
    unsigned int firstChar = i * pi::AIDA_HBookStore::HBook::MAXTAGLEN;
    const std::string& variableName = variableList[i];
    for ( unsigned int j = 0; j < variableName.size() && j < pi::AIDA_HBookStore::HBook::MAXTAGLEN; ++j ) {
      varList[firstChar + j] = variableName[j];
    }
  }
  int errorCode;
  hgntv_( &tid, varList, &nVars, &row, &errorCode, varListSize );
  delete [] varList;
  return (errorCode == 0);
}


bool
pi::AIDA_HBookStore::HBook::fastFillBuffersFromCWNtupleRow(const int& id, const int& rowNumber)
{
  int tid = id;
  int trowNumber = rowNumber;
  int errorFlag = 0;
  hgntf_(&tid, &trowNumber, &errorFlag);
  return (errorFlag == 0);
}

/// calls Olivier hack to set contents for Histogram 1D
void 
pi::AIDA_HBookStore::HBook::setHisto1DContents(const int & id, const int & entries, const float & eqBinEntries,  
						   const float & mean, const float & rms, 
						   std::vector<float> & binHeights, 
						   std::vector<float> & binErrors, 
						   bool setErrors)
{
  // compute sumW and sumW*2
  int nb = binHeights.size(); 
  float sumW = 0; 
  float sumWW = 0; 
  for (int i = 0; i < nb; ++i) {
    sumW += binHeights[i];
  }
  if (eqBinEntries != 0) 
    sumWW = (sumW*sumW)/eqBinEntries; 
	
  float sumWX = mean * sumW;
  float sumWXX = ( mean * mean + rms * rms) * sumW;
  int tid = id; 
  int nev = entries; 
  int iopt = 0; 
  if (setErrors)  iopt = 1; // if we set  errors paw will plot them with errors 
  hbf1al_(&tid,&binHeights.front(), &nev, &sumW, &sumWW, &sumWX, &sumWXX, &binErrors.front(), &iopt);

} 

/// calls Olivier hack to set contents foe Histogram 2D (and profile ? ) 
void 
pi::AIDA_HBookStore::HBook::setHisto2DContents(const int & id, const int & entries, 
						   std::vector<float> & binHeights, 
						   std::vector<float> & binErrors, 
						   bool setErrors)
{

  int tid = id; 
  int nev = entries; 
  int iopt = 0;
  if (setErrors)  iopt = 1; // if we set  errors paw will plot them with errors 
  hbf2al_(&tid,&binHeights.front(), &nev, &binErrors.front(), &iopt);

} 
