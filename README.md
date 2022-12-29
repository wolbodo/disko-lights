# Wolbodo Disko Lights

This is the project which runs the DiskoLights.

The DiskoLights is a panel of WS2812 programmable LEDS.
Organised in 6 \* 4 tiles, where each tile has 16 leds, all connection in series.
So a total of 6\*4\*16 = 384 leds in one chain. The LEDS are organised in a 24 \* 16 grid.

The DiskoLights are connected to a ESP32-Wroom board, and can be controlled wirelessly through
a web interface.


# Development

Sourcecode is available from [github](https://github.com/wolbodo/disko-lights).

Below:
 * `pio` is my abbreviation for the [platformio-CLI](https://docs.platformio.org/en/latest/core/installation/methods/installer-script.html#local-download-macos-linux-windows) tool.
 * `pnpm` is a node-js package manager, installed using: `sudo npm install --global pnpm`


Build and upload the controller code ( from the top-level directory ):

    pio run -t upload

Build and Upload the website code:

    cd frontend

    pnpm i          # install dependencies
    pnpm run build  # build the frontend code
    pnpm run dev    # run fruntend on your development pc

The frontend code will be in the `data` directory.

You can now upload it to the ESP32 with ( from the top-level directory ):

    pio run -t uploadfs

After updating the frontend, you will have to configure the DiskoLights wifi setup:

 * connect to `DiskoGrid\_AP` network with password: `interpol`.
 * select `configure`, then set the ssid and password for your local network.

Now the DiskoLights can be connected to via this address: `http://disko-grid.local/`.


# source code

 * `frontend`  - contains the svelte code for the frontend, which is what the user will talk to in the webinterface.
 * `lib/ESPAsyncDNSServer-1.0.0` contains the ESPAsyncDNSServer package with a patch replacing the `ESPAsyncUDP` library with `AsyncUDP`

 * `platformio.ini` configuration for platformio.
 * `src/leds.py`  python script demonstrating the x,y to tile index conversion.
 * `src/main.cpp`  The main controller code

 * `lib/Programs` the various ledgrid programs, displaying patterns.

Support libraries:

 * `lib/EventTimer/eventtimer.h`  - a bit like the `EVERY\_N\_MILLISECONDS` macro.
 * `lib/Graplib/graphlib.h`       - graphics primitives, based upon the Adafruit-GFXinterface.
 * `lib/Ledgrid/ledgrid.h`        - a (x,y) addressable grid to the led-string.
 * `lib/webserver/webserver.h`    - the json web api for controlling the DiskoLights.
 * `lib/wifi/wifisetup.h`         - handles the wireless network setup.


# Programs

 * `lib/Programs/program.h`      - the baseclass for each program.

 * `lib/Programs/Sjoerd.h`
 * `lib/Programs/Sjoerd2.h`
 * `lib/Programs/christmas.h`
 * `lib/Programs/colorsweep.h`
 * `lib/Programs/gradient.h`
 * `lib/Programs/pixelsweep.h`
 * `lib/Programs/rainbow1.h`
 * `lib/Programs/rainbow2.h`
 * `lib/Programs/scene.h`
 * `lib/Programs/spiral.h`
 * `lib/Programs/spots.h`
