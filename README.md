# GPS Tracking Algorithm
A very simple point tracking algorithm using only the distance from a point to 3 (or more) fixed pillars
This was just a test to see what a possible algorithm would be for localizing something with a distance (or signal range) from multiple fixed signal sources.

## The Algorithm
The algorithm of this tracking algorithm is quite straightforward. 
It first makes 3 circles for the 3 pillars with a radius of the distance to the tracked point.
Then it finds the intersection points between each circle (maximum of 6 points in this case) and takes the average value of the three closest points.

Basically it finds the closest point between all three circles, as that is most likely where the tracked point is.

## Accounting for bad cases
This algorithm also works for cases where there are no intersecting points between the circles.
As the whole point is to find the closest point between the circles, it's actually easier when they don't intersect because than means there is only one point, 
that being the middle between the two radii of the circles directly between the two pillar points.

## The program
This program uses my custom text-based graphics library which includes mouse input as well.
Purposeful inaccuracies were added to the distances to the pillars to make it a bit more realistic.
Although it can sometimes be accurate in some locations more than others when there are inaccuracies.

The controls consist of the following:
Mouse: Current point to be tracked (green)
Left Mouse (hold): Show the distances from the position to the pillars (with inaccuracies)
Right Mouse (hold): Show the calculated position from the pillar distances (yellow)

## Compiling
Since this uses a Windows text-based graphics library, it will only compile on Windows (unless you switched to a different graphics library).

All that is needed is the BirdfulGraphics.h graphics library in the 'include' folder.
