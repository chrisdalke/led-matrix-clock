# led-matrix-clock
An RGB LED Matrix clock. Tells the time and weather information.

# Design notes

Top left: Time, date
Top right: Current temperature
current weather

bottom right:
graph of temperature for whole day
low/high temperature

bottom left: sunset, sunrise time

gradient bg: current weather animation

- clouds
- night


# Dependencies
-raylib

```
sudo apt install libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev

```

```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
```

-rpi-rgb-led-matrix
https://github.com/hzeller/rpi-rgb-led-matrix