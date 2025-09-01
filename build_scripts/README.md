# Build scripts

This folder contains build helper scripts.

- build_msvc2019_x64.bat: Windows MSVC x64 build using CMake + NMake and CPack.
- build_linux.sh: Linux build with Qt autodetection. Uses /tmp/tmp_build as a temporary build directory.

The previous folder name was `build`. Wrapper scripts remain in `build/` to preserve backward compatibility.
