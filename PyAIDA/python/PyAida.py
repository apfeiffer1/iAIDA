import  pylcgdict 
pylcgdict.loadDictionary("PyAIDADict")
pylcgdict.loadDictionary("SealSTLDict")
dir()

try : 
    dir()
#    AIDA = PyLCGDict.makeNamespace("AIDA")
    g = pylcgdict.Namespace("")
    pi = g.pi
    ff = pi.AFCreator()
except :
    print "cannot create AF"
    


global af,tf,cast

def AIDA_createAnalysisFactory() :
    af=ff.createAnalysisFactory()
#    print "created af"
    return af


af=AIDA_createAnalysisFactory()
tf=af.createTreeFactory()
cast=g.CastConverter()

    
