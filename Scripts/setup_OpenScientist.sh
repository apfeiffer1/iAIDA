#define env variables to run open scientist on lxplus
#
#
. ~/setCompiler 3.2

export CMTCONFIG=rh73_gcc32
export CMTPATH=/afs/cern.ch/sw/contrib/OpenScientist/v12r0
cd /afs/cern.ch/sw/contrib/OpenScientist/v12r0/Lab/v10r0/cmt

. /afs/cern.ch/sw/contrib/OpenScientist/v12r0/Lab/v10r0/cmt/setup.sh

# set file to be used 
#export ONXFILE=/a
#cmt show uses