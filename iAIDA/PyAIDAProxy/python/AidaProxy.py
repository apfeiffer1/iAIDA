import  pylcgdict
pylcgdict.loadDictionary("PyAIDAProxyDict")
pylcgdict.loadDictionary("SealSTLDict")


# redefining classes in namespace


# make a namespace otherwise it is not know to Python 
# need to use globals g
g = pylcgdict.Namespace("")
pi_aida = g.pi_aida


Profile2D = pi_aida.Profile2D
DataPointSet = pi_aida.DataPointSet
Profile1D = pi_aida.Profile1D
Histogram3D = pi_aida.Histogram3D
Histogram2D = pi_aida.Histogram2D
Histogram1D = pi_aida.Histogram1D
Proxy_Selector = pi_aida.Proxy_Selector
FitData = pi_aida.FitData
Function = pi_aida.Function
Fitter = pi_aida.Fitter
Tuple = pi_aida.Tuple
Filter = pi_aida.Filter
Evaluator = pi_aida.Evaluator
Cloud3D = pi_aida.Cloud3D
Cloud2D = pi_aida.Cloud2D
Cloud1D = pi_aida.Cloud1D
Proxy_Store = pi_aida.Proxy_Store
Plotter = pi_aida.Plotter

#now for std
std=pylcgdict.Namespace("std")



# load user funcitons function 
pylcgdict.loadDictionary("UserFunctionsDict")
# use deprecated method here


UserFunction =  pi_aida.UserFunction



# add user convinient methods for functions (no need of std.vectors)
#
#  Function::setParameters( double, double, double,....)
#  Function::eval(double, double, double,....)


def xxx():
    def setParams(self, *args):
        pars = std.vector('double')()
        for arg in args:
            pars.push_back(arg)
        return self.setParameters(pars)
    Function.setParams = setParams

    def eval(self, *args):
        vx = std.vector('double')()
        for arg in args:
            vx.push_back(arg)
        return self.value(vx)
    Function.eval = eval

# for pylcgdict 1 do for user func
# no needed for 2

    def setParams(self, *args):
        pars = std.vector('double')()
        for arg in args:
            pars.push_back(arg)
        return self.setParameters(pars)
    UserFunction.setParams = setParams

    def eval(self, *args):
        vx = std.vector('double')()
        for arg in args:
            vx.push_back(arg)
        return self.value(vx)
    UserFunction.eval = eval


xxx()
