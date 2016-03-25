# get all dependecy tarballs
#wget https://github.com/jonls/qsopt-ex/archive/v2.5.10.3.tar.gz
wget http://zlib.net/zlib-1.2.8.tar.gz
wget https://sites.google.com/site/jayantapteshomepage/home/qsopt.tar.gz
wget https://sites.google.com/site/jayantapteshomepage/home/bzip2.tar.gz
wget https://gmplib.org/download/gmp/gmp-6.1.0.tar.xz
tar -xf gmp-6.1.0.tar.xz
tar -xf qsopt.tar.gz
tar -xf bzip2.tar.gz
tar -xf zlib-1.2.8.tar.gz
if ( test ! -d gmp_install ) ; then mkdir -p gmp_install ; fi
if ( test ! -d qsopt_install ) ; then mkdir -p qsopt_install ; fi
if ( test ! -d bzip2_install ) ; then mkdir -p bzip2_install ; fi
if ( test ! -d zlib_install ) ; then mkdir -p zlib_install ; fi
if ( test ! -d include ) ; then mkdir -p include ; fi
if ( test ! -d lib ) ; then mkdir -p lib ; fi
# gmp install
cd gmp-6.1.0
./configure --prefix=$PWD/../gmp_install
make -j12
make install
cd ../
#zlib install
cd zlib-1.2.8
./configure --prefix=$PWD/../zlib_install
make -j12
make install
cd ../
#bzip2 install
cd bzip2-1.0.6
make clean
make -f Makefile-libbz2_so
make
make install
if ( test ! -d $PWD/../bzip2_install/lib ) ; then mkdir -p  $PWD/../bzip2_install/lib ; fi
if ( test ! -d $PWD/../bzip2_install/include ) ; then mkdir -p  $PWD/../bzip2_install/include ; fi
cp libbz2.so.1.0.6 $PWD/../bzip2_install/lib/
cp *.h $PWD/../bzip2_install/include/
cd ../ 
cd qsopt-ex-2.5.10.3
mkdir build && cd build
../configure --prefix=$PWD/../../qsopt_install LDFLAGS="-L$PWD/../../gmp_install/lib -L$PWD/../../zlib_install/lib -L$PWD/../../bzip2_install/lib"
make -j12
make install
cd ../../
cp gmp_install/lib/* lib/
cp qsopt_install/lib/* lib/
cp zlib_install/lib/* lib/
cp bzip2_install/lib/* lib/
cp gmp_install/include/* include/
cp qsopt_install/include/qsopt_ex/* include/
cp zlib_install/include/* include/
cp bzip2_install/include/* include/
export LD_LIBRARY_PATH=$PWD/lib:$LD_LIBRARY_PATH
C_INCLUDE_PATH=$PWD/include
