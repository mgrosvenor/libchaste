#! /bin/sh


./build.sh --variant=release

#Now make the release tarball

tmp="bin/RELEASETAR/libchaste"

mkdir -p $tmp

./makeinclude.sh $tmp > $tmp/.tmp.makeincludeout
rm -rf $tmp/.tmp.makeincludeout

cp bin/libchaste.a $tmp
cd $tmp/../
tar -cjf libchaste.tar.bz2 libchaste

cd -

cp $tmp/../libchaste.tar.bz2 .

#Clean up the mess
rm -rf $tmp




