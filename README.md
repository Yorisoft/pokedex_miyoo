# Pokedex for Miyoo Mini! - WIP

<p align="center">
 <img src="https://github.com/Yorisoft/pokedex_miyoo/assets/27126548/11b8ac44-8e78-4b9f-8af1-bf8f633434f1" width="80" />
</p>

<p align="center">
  <img src="https://github.com/Yorisoft/pokedex_miyoo/assets/27126548/885de49c-ab59-470c-b23e-3cd7a26d6fe7" width="400" />
  &nbsp;&nbsp;&nbsp;&nbsp;
  <img src="https://github.com/Yorisoft/pokedex_miyoo/assets/27126548/01d9a77e-0136-4e2a-ba96-3a01bc3fb4e2" width="400" /> 
</p>

</br>

This app was made from scratch for the MiyooMini devices. The goal is to design it to target gens 1 - 5 to help you track down all your favorite Pokemon and have all their information at your finger tips :) 

### PLEASE READ! This is not yet stable - this is a work in progress - there are bugs - install at your own peril.

</br>
</br>

## Installation

### Transferring the App to the Miyoo Mini Device

1. Connect the Miyoo Mini device to your computer.
  - Refer to the OnionOS wiki for the various ways to do this, e.g., FTP, Webserver, Samba, etc…
2. Transfer the `res` folder inside `App/Retrodex`
2. Transfer the `Retrodex` folder found in the `App` directory of this repo to the device.  -Place the `Retrodex` folder in `/mnt/SDCARD/App` folder of the miyoo mini.
3. On the miyoo mini device, navigate to your Apps list. Refresh your App list by bringing up and exiting the 'search' function - pressing `x`
4. Navigate to the `Retrodex` app and launch it. 

</br>
</br>

## Compiling and Building

## Dependencies 
- SDL2 
- SDL2_image 
- SDL2_ttf 
- SDL2_mixer
- SQLite

### Compiling on Windows Visual Studio

1. Open the `Source/windows-dev` directory. In there you will find a project solution for opening in Visual Studios. 
2. Configure Visual Studios with SDL libraries and include directories.
3. Compile and run the program.

</br>

### Compiling on Union-miyoomini-toolchain

1. Clone and start the docker container for the union-miyoomini-toolchain. 
   - Read the `README.md` located in the `Source/union-miyoomini-toolchain` directory for instructions on how to get started with union-miyoomini-toolchain, or refer to their GitHub README. 
   - [union-miyoomini-toolchain](https://github.com/MiyooMini/union-toolchain/tree/main)
2. Transfer the contents `workspace` folder from the `Source/union-miyoomini-toolchain` directory of this repo to the development environment `/workspace`. 
   - Optionally you could just replace the `workspace` folder with the one from this repo. 
3. Compile the Retrodex program using the provided CMakeList and Toolchain files:
     ```
        cd retrodex
        touch build
        cd build
        cmake .. -DCMAKE_TOOLCHAIN_FILE=../Toolchain.cmake
        make
     ```
4. The binary should be located in `workspace/retrodex/build/` </br>
     `retrodex`


</br>

## Data, Assets, Contributions

Dataset from [veekun_pokedex](https://github.com/veekun/veekun-pokedex).

Artwork from [pokemondb.net](https://pokemondb.net/).

SDL Library Install Scripts from [XK9274](https://github.com/XK9274).

Special Thanks to Miguel the Builder on Discord. 

</br>

## Additional Information


### Current Status

The app is currently in the early stages. At the moment, it only contains the Game version menu, Pokedex list, and a few Pokemon Views ( Basic Info, Stats, Moves, etc..) 
TODO:
- Add: ability to shift through Pokedex list while holding down button.
- Add: Gen 6-9 - collect gen 5 style pokemon sprites for gen 6-9. 

</br>

### Dependencies

```
SDL2
SDL2_image
SDL2_image
SQLite
```

</br>

### Directory Structure

The project has the following directory structure:
```
- pokedex_miyoo
    - App
      - Retrodex
    - Source
        - union-miyoomini-toolchain
            - workspace
              - retrodex
    - res
```

</br>

## Contributing

Contributions are more than welcome! 

If you encounter any issues or have any questions about the Retrodex app, please open an issue in this repository.

</br>

## Acknowledgements

I thank the Miyoo Mini discord community for their support and contributing to bringing this project to life!
