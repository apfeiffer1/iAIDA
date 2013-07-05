
// example code as provided by Guy Barrand on the Geant-4 hypernews at:
// http://hypernews.slac.stanford.edu/HyperNews/geant4/get/analysis/391/1/1/1/1/1.html


#include <AIDA/IHistogram1D.h>
#include <AIDA/IAxis.h>
#include <vector>
#include <algorithm>

inline bool get_integral(AIDA::IHistogram1D& a_histo,
std::vector<double>& a_integral) {
   a_integral.clear();
   a_integral.push_back(0);
   int xbins = a_histo.axis().bins();
   for(int index=0;index<xbins;index++) {
      double h = a_histo.binHeight(index);
      a_integral.push_back(h+a_integral.back());
   }
 // normalize integral to 1 :
   double w = a_integral.back();
   if(w==0) return false;
   std::vector<double>::iterator it;
   for(it=a_integral.begin();it!=a_integral.end();++it){
      *it /= w;
   }
   return true;
}

template <typename T>
unsigned long binary_search(unsigned long n, const T* array,T value){
 //same logic as CERN-ROOT/TMath.h/BinarySearch.
   const T* pind = std::lower_bound(array, array + n, value);
   if ( (pind != array + n) && (*pind == value) ) {
      return (pind - array);
   } else {
      return (pind - array - 1);
   }
}

#include <cstdlib> //::rand, RAND_MAX

inline double rflat_shoot() {
 // Shoot random numbers in [0,1] according a flat distribution.
   double value  = (double) ::rand();
   value /= (double)RAND_MAX;
   return value;
}

inline double get_random(AIDA::IHistogram1D& a_histo,
const std::vector<double>& a_integral) {
 //same logic as CERN-ROOT/TH1.cxx/GetRandom().
   double r = rflat_shoot();
   int xbins = a_histo.axis().bins();
   int ibin = binary_search(xbins,a_integral.data(),r);

   double x = a_histo.axis().binLowerEdge(ibin);
   if(r>a_integral[ibin]) {
      x += a_histo.axis().binWidth(ibin)*
         (r-a_integral[ibin])/(a_integral[ibin+1]-a_integral[ibin]);
   }
   return x;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

#include <cmath>

inline double rgauss_shoot(double a_mean = 0,double a_rms = 1) {
   double v1,v2,r,fac;
   do {
      v1 = 2.0 * rflat_shoot() - 1.0;
      v2 = 2.0 * rflat_shoot() - 1.0;
      r = v1*v1 + v2*v2;
   } while ( r > 1.0 );
   fac = ::sqrt(-2.0*::log(r)/r);
   return (v2 * fac) * a_rms + a_mean;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

#include <AIDA/AIDA.h>

#include <cstdlib>

int main(int argc,char* argv[]) {

   AIDA::IAnalysisFactory* af = AIDA_createAnalysisFactory();
   if(!af) return EXIT_FAILURE;

   AIDA::ITreeFactory* trf = af->createTreeFactory();
   if(!trf) return EXIT_FAILURE;
   AIDA::ITree* tree = trf->create();
   delete trf;
   if(!tree) return EXIT_FAILURE;

   AIDA::IHistogramFactory* hf = af->createHistogramFactory(*tree);
   if(!hf) return EXIT_FAILURE;
   AIDA::IHistogram1D* h1d = hf->createHistogram1D("master 1d","master 1d",50,-3,3);
   AIDA::IHistogram1D* hr = hf->createHistogram1D("random","random",50,-3,3);
   delete hf;
   if(!h1d) return EXIT_FAILURE;
   if(!hr) return EXIT_FAILURE;

   { // fill the "master" histogram with a gaussian centered at zero, and a rms of one
      for (int i=0; i<1000000; i++) {
         h1d->fill(rgauss_shoot());
      }
   }

   { // fill the "random" histogram with values sampled from the "master"
      std::vector<double> integral;
      if(!get_integral(*h1d,integral)) return EXIT_FAILURE;
      for (int i=0; i<1000000; i++) {
         hr->fill(get_random(*h1d,integral));
      }
   }

   AIDA::IPlotterFactory* pf = af->createPlotterFactory(argc,argv);
   if(pf) {
      AIDA::IPlotter* plotter = pf->create();
      delete pf;
      if(plotter) {
         plotter->createRegions(1,2,0);
         plotter->region(0)->plot(*h1d);
         plotter->region(1)->plot(*hr);
         plotter->show();
         plotter->interact();
         delete plotter;
      }
   } else {
      // Printing some statistical values of the "master" histogram
      std::cout << "Master :" << std::endl;
      std::cout << "Title  : " << h1d->title() << std::endl;
      std::cout << "Entries: " << h1d->entries() << std::endl;
      std::cout << "Mean   : " << h1d->mean() << std::endl;
      std::cout << "RMS    : " << h1d->rms() << std::endl;      
      std::cout << "========" << std::endl;
      // ... and the same for the "randomly sampled" histogram
      std::cout << "Sampled:" << std::endl;
      std::cout << "Title  : " << hr->title() << std::endl;
      std::cout << "Entries: " << hr->entries() << std::endl;
      std::cout << "Mean   : " << hr->mean() << std::endl;
      std::cout << "RMS    : " << hr->rms() << std::endl;      
   }

   delete af;

   std::cout << "That's it !" << std::endl;

   return EXIT_SUCCESS;
}
