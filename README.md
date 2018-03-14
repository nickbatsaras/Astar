# Description
This is a minimal C implementation the A\* pathfinding algorithm.
The executable takes as input a text file that contains the "map/graph", along
with the x, y coordinates of the "source" and "destination" tile.
The text file format is really simple:
```
- 0: Non-blocking tile
- 1: Blocking tile
```
The first line contains the dimensions on your map. A typical map.txt file
would be:
```
10 10
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
1 1 1 1 0 1 0 1 1 1
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
```

# Build
By default, the output is only the time elapsed till path is found.
```
- make
```
You can also receive a "visual" of the map by building like this:
```
- make FLAG+=-DTRACE
```
Don't do it on big maps!

# Run
```
./astar map.txt srcX srcY dstX dstY
```
