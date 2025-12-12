# mmorpg

## Dev install

1. get vcpkg
https://github.com/microsoft/vcpkg

2. in .bashrc:
```sh
export VCPKG_ROOT=/path/to/vcpkg
export CMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
```

3. git clone this

4. 
```sh
vcpkg install sdl3 boost-asio
```

5. copy sdl3webgpu.c, sdl3webgpu.h and CMakeLists.txt from https://github.com/eliemichel/sdl3webgpu/ to `external/sdl3webgpu`

6. 
```sh
mkdir build
cd build
cmake ..
cmake --build .
./mmorpg
```