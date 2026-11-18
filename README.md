# vision simulation in c
this is a simple simulation of vision systems, for example what you see in league of legends, or other top down MOBAs.

## controls
WASD to move and thats it.

## compilation and running instructions
```
git clone https://github.com/rami-droid/vision-simulation
cd vision-simulation
gcc vision.c -o vision `sdl2-config --cflags --libs` -lm
./vision
```

## TODO
**this is for me mainly** 
- walls
- Line of sight
- more vision points (like wards type shit)

