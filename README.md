# Pokedex for Miyoo Mini!

This app was made from scratch using for the MiyooMini devices. The goal is to design it to target gens 1 - 5 to help you track down all your favorite Pokemon and have all their information at your finger tips :) 

# Preview

![image](https://github.com/Yorisoft/pokedex_miyoo/assets/27126548/3c8ad2e6-37c7-4f3f-a6b3-8c95c34a1c12)

## Instructions

### Compiling on Union-miyoomini-toolchain

1. Clone and start the docker container for the union-miyoomini-toolchain. 
   - For instructions on how to get started union-miyoomini-toolchain please refer to their github READ.me 
   - [union-miyoomini-toolchain](https://github.com/MiyooMini/union-toolchain/tree/main)
2. Transfer the contents `workspace` folder from the `Source/Union-miyoomini-toolchain` directory of this repo to the development environment `/workspace`. 
   - Optionally you could just replace the `workspace` folder with the one from this repo. 
3. Compile the Pokedex program using the provided CMakeList and Toolchain files:
     ```
        cd build
        cmake .. -DCMAKE_TOOLCHAIN_FILE=../Toolchain.cmake
        make
     ```
4. The binary should be located in `workspace/build/Pokedex`

### Transferring the App to the Miyoo Mini Device

1. Connect the Miyoo Mini device to your computer.
  - Refer to the OnionOS wiki for the various ways to do this, e.i: FTP, Webserver, Samba, etc..
2. Transfer the `Pokedex` folder found in the `App` directory of this repo to the device. Place the `Pokedex` folder in `/mnt/SDCARD/App` folder of the miyoo mini.
3. On the miyoo mini device, navigate to your Apps list. Refresh your App list by bringing up and exiting the 'search' function - pressing `x`
4. Navigate to the HelloMiyoo app and launch it. 

## Additional Information

### Directory Structure

The project has the following directory structure:
```
- HelloMiyoo
    - App
      - HelloMiyoo
    - Source
        - VisualStudio
            - HelloMiyoo
        - Union-miyoomini-toolchain
            - workspace
```

## Current Status

The app is currently in the early stages. At the moment, it only contains the Pokedex list with minimal stats. The next step is to work on the species dex entry.

## Data and Assets

I designed it using CPP, SDL, and SQLite.

Dataset from Veekun

Artwork from pokemondb.net.

## Installation and Building

For instructions on how to build the app and install it on the Miyoo Mini, please refer to the [HelloMiyoo repository](https://github.com/Yorisoft/HelloMiyoo).

## Contributing

Contributions are more than welcome! 

If you encounter any issues or have any questions about the Pokedex app, please open an issue in this repository.

## Acknowledgements

I thank the Miyoo Mini discord community for their support and contributing to bringing this project to life!
