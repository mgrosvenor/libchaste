#! /bin/sh
##################################
#Build the source in release mode#
##################################

./build.sh --variant=release

##################################
#Now make the release tarball    #
##################################

#Set up a working directory
tmp="bin/RELEASETAR/libchaste"
mkdir -p $tmp

#Build up an include directory in it
./makeinclude.sh $tmp > $tmp/.tmp.makeincludeout
rm -rf $tmp/.tmp.makeincludeout

#Copy specific files into place
cp bin/libchaste.a $tmp
cp LICENSE $tmp
cp README $tmp

#Make the tarball
cd $tmp/../
tar -cjf libchaste.tar.bz2 libchaste
cd -

#Copy out the tarball 
cp $tmp/../libchaste.tar.bz2 .

#Clean up the mess
rm -rf $tmp



