#  <img align="center" width="400" src="https://github.com/user-attachments/assets/98177822-b80d-4bb4-838e-54ce235cdd4e"/>

<p align="center">
<img align="center" width="400" src="https://github.com/user-attachments/assets/7080b6cd-66a1-4a10-88d9-bf5364bdd3cc"/>
</p>

</br>
</br>

<p align="center">This app was made from scratch for the MiyooMini devices running OnionOS. The goal is to design a pokedex that targets generations 1 - 5.  </p>


### PLEASE READ! This is not yet stable - this is a work in progress - there are bugs - install at your own peril.

</br>

## Install on Miyoo Mini

1. Connect the Miyoo Mini device to a computer.
    - Refer to the OnionOS wiki for the various ways to do this, e.g., FTP, Webserver, Samba, etc…
2. Copy `App/Retrodex` to `/mnt/SDCARD/App` on the Miyoo Mini. Restart the device if it's turned off.
3. On the Miyoo Mini, navigate to Apps list.
    - Refresh App list by using the 'search' function - press `x`
4. Navigate to the `Retrodex` app and launch it. 

</br>
</br>


## Install locally

1. Install cmake \
   `apt-get install cmake`
2. Install SDL2 dependencies \
   `apt-get install ibsdl2-dev ibsdl2-ttf-dev libsdl2-mixer-dev libsdl2-image-dev`
3. Install sqlite amalgamation 
   - Download [here](https://www.sqlite.org/download.html). Learn about sqlite amalgamation [here](https://www.sqlite.org/amalgamation.html)
   - Place sqlite amalgamation folder in `Source/union-miyoomini-toolchain/workspace/retrodex/include/sqlite` \
       Make sure sqlite folder contains the following
   ```
       sqlite
       ├── sqlite3.h
       └── sqlite3.c
   ```
4. Run cmake 
   > **Tip:** Working dir: Source/union-miyoomini-toolchain/workspace/retrodex
   ```
       cd buil
       cmake ..
   ```
5. Binary should be in `build/bin`
    - retrodex


</br>
</br>


### Directory Structure
```
# tree
.
├── App
│   └── Retrodex
└── Source
    └── union-miyoomini-toolchain
       ├── support
       └── workspace
           └── retrodex
              ├── build
              ├── include
              ├── res -> symlink
              └── target
                   └── local
                   └── miyoo
```


### Compiling on union-miyoomini-toolchain

1. Clone, navigate to `Source/union-miyoomini-toolchain`, and start the docker container for the union-miyoomini-toolchain. 
   - Read the `README.md` located in the `Source/union-miyoomini-toolchain` directory for instructions on how to get started with union-miyoomini-toolchain, or refer to their GitHub README. [union-miyoomini-toolchain repo](https://github.com/MiyooMini/union-toolchain/tree/main)
2. Run `mksdl2.sh` script to build SDL2 dependencies.
    ```
    ./mksdl2.sh
    ```
3. Compile the Retrodex program using the provided CMakeList and Toolchain files:
    ```
    cd retrodex \
    mkdir build \
    cd build \
    cmake .. -DCMAKE_TOOLCHAIN_FILE=../Toolchain.cmake \
    make
    ```
4. The executable should be located in `workspace/retrodex/build/`
    - `retrodex`

Copy `retrodex` executable to App/Retrodex for testing. 

</br>
</br>



## Current Status

The app is currently in the early stages. At the moment, it contains:
- Game version menu
- Pokedex list
- Pokemon Views ( Basic Info, Stats, Moves, Encounters, Evolutions) 


### The primary objective is to get a stable app with pleasent UI, more features may come in the future. 

Possible Roadmap:
- [ ] Support for animated Pokemon sprites. 
- [ ] Support for Gen 6-9. Require collecting Gen 5 style sprites for Gen 6-9. 

</br>
</br>

## Contributions, Acknowledgements

Toolchain Docker Image: [union-miyoomini-toolchain](https://github.com/shauninman/union-miyoomini-toolchain).

Database: [veekun_pokedex](https://github.com/veekun/veekun-pokedex).

Pokemon Sprites: [pokemondb.net](https://pokemondb.net/).

SDL2 Library Install Script: [XK9274](https://github.com/XK9274).

Special Thanks to the folks over at #Retro Game Handhelds [Discord](https://discord.gg/retro-game-handhelds-529983248114122762)! 


</br>
</br>


## Contributing

Contributions are more than welcomed! 
If you encounter any bugs or have any questions about the app, please open an issue.

For code change suggestions, fork repo and create pr. 

</br>
</br>

Made with :heart: and C++
