wget -q -O qt5.zip https://github.com/francescmm/ci-utils/releases/download/qt/qt5.zip &&
unzip -qq qt5.zip &&
export QTDIR=$PWD/qt5
export PATH=$QTDIR/bin:$PATH
export QT_PLUGIN_PATH=$PWD/qt5/plugins
mkdir build &&
cd build &&
cmake -DCMAKE_PREFIX_PATH=../qt5 .. &&
cmake --build . --target all