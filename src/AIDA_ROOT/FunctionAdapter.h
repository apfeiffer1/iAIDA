
// functional adapters from AIDA to function pointer understood by ROOT

 class FuncAdapter { 

 public: 

  FuncAdapter(const AIDA::IFunction * f) : m_func(f) {} 
   
   double operator()(double * x, double *p); 

 private: 

   AIDA::IFunction * m_func; 

 };
