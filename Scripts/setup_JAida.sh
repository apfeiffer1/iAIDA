#define env variables to run open scientist on lxplus
#
#
if [ -z $JDK_HOME ] ; then 
  export JDK_HOME=/afs/cern.ch/sw/java/i386_redhat73/jdk/sun-1.4.2
fi 

if [ -z $AIDAJNI_HOME ] ; then 
  export AIDAJNI_HOME=/afs/cern.ch/sw/lcg/contrib/moneta/AIDAJNI-3.2.1
fi 

if [ -z $JAIDA_HOME ] ; then 
  export JAIDA_HOME=/afs/cern.ch/sw/lcg/contrib/moneta/JAIDA-3.2.1
fi 

# for JAIDA
. $JAIDA_HOME/bin/aida-setup.sh
#for AIDAJNI
. $AIDAJNI_HOME/bin/Linux-g++/aidajni-setup.sh

