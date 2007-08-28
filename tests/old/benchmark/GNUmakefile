
# EXPAT_INSTALL_DIR = /afs/cern.ch/project/asddat/lhcxx/kuba/redhat61/usr/local
EXPAT_INSTALL_DIR = $(LHCXXTOP)/specific/${PLATF}/PublicDomainPackages


C_FLAGS = -g -I $(EXPAT_INSTALL_DIR)/include -I$(LHCXX_REL_DIR)/include 
C_LIBS = -L$(LHCXX_REL_DIR)/lib -lDataXML -L $(EXPAT_INSTALL_DIR)/lib -lexpat  

all :  readTest.exec gen.exec

clean :
	rm -f *.o *.a *.exec

.PHONY : all clean

readTest.exec: readTest.cpp
	g++ $(C_FLAGS) readTest.cpp -o readTest.exec $(C_LIBS)

gen.exec: gen.cpp
	g++ gen.cpp -o gen.exec