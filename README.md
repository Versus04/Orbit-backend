mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/Users/abind/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build .
.\Debug\orbit.exe