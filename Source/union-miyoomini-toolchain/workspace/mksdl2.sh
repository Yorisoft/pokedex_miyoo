unset urls
unset files
unset log_files
unset missing_tools

check_dev_tools() {  # // check which dev packages we have and if they work, if not install them
  declare -A tool_to_package_map
  tool_to_package_map["pkg-config"]="pkg-config"
  tool_to_package_map["autoconf"]="autoconf"
  tool_to_package_map["autoupdate"]="autoconf"
  tool_to_package_map["libtoolize"]="libtool"
  tool_to_package_map["m4"]="m4"
  tool_to_package_map["automake"]="automake"
  tool_to_package_map["autoreconf"]="autoconf"

  missing_tools=()
  missing_packages=()

  for tool in "${!tool_to_package_map[@]}"; do
    if command -v "$tool" > /dev/null 2>&1; then
      echo "$tool exists."
    else
      echo "$tool does not exist."
      package=${tool_to_package_map[$tool]}
      missing_tools+=("$tool")
      if ! [[ " ${missing_packages[@]} " =~ " ${package} " ]]; then
        missing_packages+=("$package")
      fi
    fi
  done

  if [ ${#missing_tools[@]} -ne 0 ]; then
    echo "Missing tools: ${missing_tools[*]}"
    echo "Corresponding missing packages: ${missing_packages[*]}"
    echo "Running 'apt-get update' and then 'apt-get install' for each missing package."
    apt-get update
    apt-get install -y "${missing_packages[@]}"
  else
    echo "All tools are available and functional."
  fi
}

check_dev_tools

# setup some env, not required in some toolchains but is in mine
export FIN_BIN_DIR="/root/workspace/build"
export CROSS_COMPILE="arm-linux-gnueabihf"
export AR=${CROSS_COMPILE}-ar
export AS=${CROSS_COMPILE}-as
export LD=${CROSS_COMPILE}-ld
export RANLIB=${CROSS_COMPILE}-ranlib
export CC=${CROSS_COMPILE}-gcc
export NM=${CROSS_COMPILE}-nm
export HOST=arm-linux-gnueabihf
export BUILD=x86_64-linux-gnu
export CFLAGS="-Wno-undef -Os -marm -mtune=cortex-a7 -mfpu=neon-vfpv4 -march=armv7ve+simd -mfloat-abi=hard -ffunction-sections -fdata-sections -I/usr"
export CXXFLAGS="-s -O3 -fPIC -pthread"
export PATH="$PATH:$FIN_BIN_DIR/bin"
export LDFLAGS="-L/opt/miyoomini-toolchain/arm-linux-gnueabihf/libc/lib -L/usr/lib/arm-linux-gnueabihf/"

export LOGFILE=./logs/buildtracker.txt # set a full log file
mkdir $ROOTDIR/logs

echo -e "-Starting shortly - a full logfile with be in: \033[32m"$LOGFILE "\033[0m"
echo -e "\n"

for i in {3..1}; do
    echo -ne "Starting in $i\r"
    sleep 1
done


cd ~/workspace/

check_dev_tools	

#Download everything, but check if it already exists.

urls=(
    "https://github.com/libsdl-org/SDL/releases/download/release-2.26.5/SDL2-2.26.5.tar.gz"
	"https://github.com/libsdl-org/SDL_image/releases/download/release-2.6.3/SDL2_image-2.6.3.tar.gz"
	"https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.20.2/SDL2_ttf-2.20.2.tar.gz"
	"https://github.com/libsdl-org/SDL_net/releases/download/release-2.2.0/SDL2_net-2.2.0.tar.gz"
	"https://github.com/libsdl-org/SDL_mixer/releases/download/release-2.6.3/SDL2_mixer-2.6.3.tar.gz"
)

# Parallel download and wait until finished.
pids=()
for url in "${urls[@]}"; do
  file_name=$(basename "$url")
  if [ ! -f "$file_name" ]; then
    echo "Downloading $file_name..."
    wget -q "$url" &
    pids+=($!)
  else
    echo "$file_name already exists, skipping download..."
  fi
done

for pid in "${pids[@]}"; do
  wait $pid
done

echo -e "\n\n\033[32mAll downloads finished, now building..\033[0m\n\n"

basename_from_url() {
  echo "${1##*/}"
}

missing_files=0

for url in "${urls[@]}"; do
  file_name=$(basename_from_url "$url")
  if [ ! -f "$file_name" ]; then
    echo "Missing file: $file_name"
    missing_files=$((missing_files + 1))
  fi
done

if [ $missing_files -eq 0 ]; then
  echo "All files are present."
else
  echo "$missing_files files are missing."
fi

missing_files=()
for file in "${files[@]}"; do
    if [ ! -f "$file" ]; then
        missing_files+=("$file")
    fi
done

if [ ${#missing_files[@]} -eq 0 ]; then
    echo -e "\033[32mAll files exist...\033[0m\n\n"
    sleep 1
else #check if any of the downloads failed, if they did try to redownload, if they still fail prompt for a new url with the filename..
    echo "Missing files: ${missing_files[@]}"
    echo "Trying to download again...."
    for file in "${missing_files[@]}"; do
        for url in "${urls[@]}"; do
            if [[ "$url" == *"$file"* ]]; then
                wget -q "$url"
                if [ $? -ne 0 ]; then
                    echo "Error downloading $file from $url"
                    read -p "Enter a new WORKING URL for $file: " new_url
                    wget -q "$new_url"
                fi
            fi
        done
    done
fi

# Start compiling..

echo "******************************************"
echo "*                                        *"
echo "*             BUILDING SDL2              *"
echo "*                                        *"
echo "*                                        *"
echo "******************************************"

cd ~/workspace
mkdir $FIN_BIN_DIR
mkdir logs
export PKG_CONFIG_PATH="$FIN_BIN_DIR/lib/pkgconfig:$PKG_CONFIG_PATH:/root/workspace/SDL2-2.26.5/"
export SDL2_CONFIG

# CONFIGURE YOUR SDL REQUIREMENTS BELOW
# SDL2
echo -e "-Compiling \033[32mSDL2\033[0m"
tar -xf SDL2-2.26.5.tar.gz &
wait $!
cd SDL2-2.26.5
./autogen.sh
./configure CC=$CC --host=$HOST --build=$BUILD --prefix=$FIN_BIN_DIR  --disable-joystick-virtual --disable-sensor --disable-power --disable-alsa --disable-diskaudio --disable-video-x11 --disable-video-wayland --disable-video --disable-video-vulkan --disable-dbus --disable-ime --disable-fcitx --disable-hidapi --disable-pulseaudio --disable-sndio --disable-libudev --disable-jack --disable-video-opengl --disable-video-opengles --disable-video-opengles2 --disable-oss --disable-dummyaudio --disable-video-dummy & 
wait $!
make clean && make -j$(( $(nproc) - 2 )) && make install -j$(( $(nproc) - 2 )) > ../logs/SDL2-2.26.5.txt 2>&1 &
wait $!
cd ..

# SDL2TTF
echo -e "-Compiling \033[32mSDL2TTF\033[0m"
tar -xf SDL2_ttf-2.20.2.tar.gz &
wait $!
cd SDL2_ttf-2.20.2
./autogen.sh
./configure CC=$CC --host=$HOST --build=$BUILD --prefix=$FIN_BIN_DIR &
wait $!
make clean && make -j$(( $(nproc) - 2 )) && make install -j$(( $(nproc) - 2 )) > ../logs/SDL2_ttf-2.20.2.txt 2>&1 &
wait $!
cd ..

# SDL2IMAGE
echo -e "-Compiling \033[32mSDL2Image\033[0m"
tar -xf SDL2_image-2.6.3.tar.gz &
wait $!
cd SDL2_image-2.6.3
./autogen.sh
./configure CC=$CC --host=$HOST --build=$BUILD --prefix=$FIN_BIN_DIR &
wait $!
make clean && make -j$(( $(nproc) - 2 )) && make install -j$(( $(nproc) - 2 )) > ../logs/SDL2_image-2.2.0.txt 2>&1 &
wait $!
cd ..
