#!/bin/sh
export Retrodex=/mnt/SDCARD/App/Retrodex/
export sysdir=/mnt/SDCARD/.tmp_update
export miyoodir=/mnt/SDCARD/miyoo
cd $Retrodex
export PATH=$PATH:$PWD/bin
export HOME=$Retrodex

get_curvol() {
    awk '/LineOut/ {if (!printed) {gsub(",", "", $8); print $8; printed=1}}' /proc/mi_modules/mi_ao/mi_ao0
}

is_process_running() {
  process_name="$1"
  if [ -z "$(pgrep -f "$process_name")" ]; then
    return 1
  else
    return 0
  fi
}

purge_devil() {
    if pgrep -f "/dev/l" > /dev/null; then
        echo "Process /dev/l is running. Killing it now..."
        killall -2 l
    else
        echo "Process /dev/l is not running."
    fi
}

kill_audio_servers() {
    is_process_running "audioserver" && pkill -9 -f "audioserver"
    is_process_running "audioserver.mod" && killall -q "audioserver.mod"
}

set_snd_level() {
    local target_vol="$1"
    local current_vol
    local start_time
    local elapsed_time

    start_time=$(date +%s)
    while [ ! -e /proc/mi_modules/mi_ao/mi_ao0 ]; do
        sleep 0.2
        elapsed_time=$(( $(date +%s) - start_time ))
        if [ "$elapsed_time" -ge 30 ]; then
            echo "Timed out waiting for /proc/mi_modules/mi_ao/mi_ao0"
            return 1
        fi
    done

    # start_time=$(date +%s)
    # while true; do
        echo "set_ao_volume 0 ${target_vol}dB" > /proc/mi_modules/mi_ao/mi_ao0
        echo "set_ao_volume 1 ${target_vol}dB" > /proc/mi_modules/mi_ao/mi_ao0
        # current_vol=$(get_curvol)

        # if [ "$current_vol" = "$target_vol" ]; then
            # echo "Volume set to ${current_vol}dB"
            # return 0
        # fi

        # elapsed_time=$(( $(date +%s) - start_time ))
        # if [ "$elapsed_time" -ge 360 ]; then
            # echo "Timed out trying to set volume"
            # return 1
        # fi

        # sleep 0.2
    # done
}

start_retroDex() {
    #
    export LD_LIBRARY_PATH=$Retrodex/lib:/lib:/config/lib:/mnt/SDCARD/miyoo/lib:/mnt/SDCARD/.tmp_update/lib:/mnt/SDCARD/.tmp_update/lib/parasyte:/sbin:/usr/sbin:/bin:/usr/bin
    export SDL_VIDEODRIVER=mmiyoo
    export SDL_AUDIODRIVER=mmiyoo
    export EGL_VIDEODRIVER=mmiyoo
    
    purge_devil
    kill_audio_servers
    set_snd_level "${curvol}" &
    # bin/cpuclock 1700
    # CALL YOUR APP HERE, eg:
    ./retrodex > /dev/pts/0 2>&1
}

main() {
    curvol=$(get_curvol) # grab current volume
    start_retroDex # call the function to start the app
}

main # call main
