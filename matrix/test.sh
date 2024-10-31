#create test and test matrix via cmake and ctest
#for unit testing - export TESTOPTION=-DUNITTEST=ON; for testing determinant computing - export TESTOPTION=-DDET=ON

btFolder=testMatrix_build
rm -rf ${btFolder}
cmake -S . -B ${btFolder} -DTESTLIB=ON ${TESTOPTION} -DCMAKE_BUILD_TYPE=DEBUG
cd ${btFolder}
cmake --build .
ctest -N
ctest -VV
