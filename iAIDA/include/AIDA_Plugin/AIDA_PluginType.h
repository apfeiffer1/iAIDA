#ifndef AIDA_PLUGIN_AIDAPLUGINTYPE_H
# define AIDA_PLUGIN_AIDAPLUGINTYPE_H 1


//# include "AIDA_Plugin/config.h"
# include "AIDA/AIDA.h"
# include "AIDA_Dev/IDevHistogramFactory.h"
# include "AIDA_Dev/IDevDataPointSetFactory.h"
# include "AIDA_Dev/IDevTupleFactory.h"
# include "AIDA_Dev/IDevFunctionFactory.h"
# include "AIDA_Dev/IStoreFactory.h"

# include <iostream> 


/** Base class for AIDA plugins. Provides Factories to create AIDA objects. 
 *  It contains only dummy implementations. Each concreate plugin will 
 *   re-implement the corresponding method (An HistogramPlugin will override the 
 *   createHIstogramFuctory() method.  
 **/

namespace AIDA_Plugin { 


class  AIDA_PluginType 
{
public:
    AIDA_PluginType ();
    virtual ~AIDA_PluginType (void);


    // histograms
    virtual AIDA::IHistogramFactory * createHistogramFactory(AIDA::ITree & tree) 
      {
	std::cerr << "Cannot create an Histogram Factory for tree " 
		  << tree.storeName() 
		  << " since corresponing plugin is not loaded" << std::endl; 
	return 0; 
      }
    virtual AIDA::Dev::IDevHistogramFactory * createDevHistogramFactory( ) 
      {
	std::cerr << "Cannot create a Dev Histogram Factory for tree since corresponing plugin is not loaded" << std::endl; 
	return 0; 
      }

    // datapoint sets 
    virtual AIDA::IDataPointSetFactory * createDataPointSetFactory(AIDA::ITree & tree) 
      {
	std::cerr << "Cannot create a DataPointSet Factory for tree " 
		  << tree.storeName() 
		  << " since corresponing plugin is not loaded" << std::endl; 
	return 0; 
      } 

    virtual AIDA::Dev::IDevDataPointSetFactory * createDevDataPointSetFactory( ) 
      {
	std::cerr << "Cannot create a Dev DataPointSet Factory since corresponing plugin is not loaded" << std::endl; 
	return 0; 
      } 

    // tuple 
    virtual AIDA::ITupleFactory * createTupleFactory(AIDA::ITree & tree) 
      {
	std::cerr << "Cannot create a Tuple Factory for tree " 
		  << tree.storeName() 
		  << " since corresponing plugin is not loaded" << std::endl; 
	return 0; 
      } 
    virtual AIDA::Dev::IDevTupleFactory * createDevTupleFactory( ) 
      {
	std::cerr << "Cannot create a Dev Tuple Factory since corresponing plugin is not loaded" << std::endl; 
	return 0; 
      } 


    // functions
    virtual AIDA::IFunctionFactory * createFunctionFactory(AIDA::ITree & tree) 
      {
	std::cerr << "Cannot create a Function Factory for tree " 
		  << tree.storeName() 
		  << " since corresponing plugin is not loaded" << std::endl; 
	return 0; 
      } 

    virtual AIDA::Dev::IDevFunctionFactory * createDevFunctionFactory() 
      {
	std::cerr << "Cannot create a Dev Function Factory for tree since corresponing plugin is not loaded" << std::endl; 
	return 0; 
      } 


    virtual AIDA::IFitFactory * createFitFactory( ) 
      {
	std::cerr << "Cannot create a Fit Factory since corresponing plugin is not loaded" << std::endl; 
	return 0; 
      }


    virtual AIDA::ITreeFactory * createTreeFactory( ) 
      {
	std::cerr << "Cannot create a Tree Factory since corresponing plugin is not loaded" << std::endl; 
	return 0; 
      }

    virtual AIDA::Dev::IStoreFactory * createStoreFactory( ) 
      {
	std::cerr << "Cannot create a Store Factory since corresponing plugin is not loaded" << std::endl; 
	return 0; 
      }

    virtual AIDA::IPlotterFactory * createPlotterFactory(int argc = 0, char * * args = 0, const std::string & imp = "", const std::string & lib = "" ); 

 protected: 


};



} // end namespace AIDA_Plugin 

#endif // PLUGIN_MANAGER_TEST_PLUGIN_H
