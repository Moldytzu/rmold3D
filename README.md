# Rewritten mold 3D
### Rewritten mold 3D is a game engine written using C++ and modern OpenGL. It is a redesign of the old mold3D 

# Features
    - Basic event system
    - Basic logger
    - Basic game objects management with component system
    - Texture loading and mapping
    - 2 3D figures
    - Skybox
    - Fog
    
# Building the example and the engine

### On Linux based operating systems
```
make -fplatform/Linux game -j$(nproc)
```

### Cross-compiling for Windows64 from Debian/Ubuntu using mingw-w64
```
sudo apt install mingw-w64* # install cross-compiler
sudo cp -r /usr/include/GLFW /usr/x86_64-w64-mingw32/include/ # make sure that glfw's headers are ready
make -fplatform/Win64 game -j$(nproc)
```

### Cross-compiling for Windows32 from Debian/Ubuntu using mingw-w64
```
sudo apt install mingw-w64* # install cross-compiler
sudo cp -r /usr/include/GLFW /usr/i686-w64-mingw32/include/ # make sure that glfw's headers are ready
make -fplatform/Win32 game -j$(nproc)
```