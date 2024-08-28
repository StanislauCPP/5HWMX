#create test and test matrix via cmake and ctest

cd ..
btFolder=testMatrix_build
rm -rf ${btFolder}
cmake -S . -B ${btFolder} -DTESTLIB=ON ${TESTOPTION} -DCMAKE_BUILD_TYPE=DEBUG
cd ${btFolder}
cmake --build .
ctest -N
ctest -VV
