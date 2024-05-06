# WIP
--------------
Just messing around with SDL2 to simulate [Jane Street's puzzle from April 2024](https://www.janestreet.com/puzzles/robot-capture-the-flag-index/)


# Game Explanation
------

The full explanation can be found [here](https://www.janestreet.com/puzzles/robot-capture-the-flag-index/).

Two robots are placed at the center of a unit circle. A flag is placed somewhere inside the circle, at a location chosen uniformly at random. Once the flag is placed, Robot 1 (blue) is able to deduce its distance to the flag, and Robot 2 (red) is able to deduce its direction to the flag. (Equivalently: if (r, θ) are the polar coordinates of the flag’s location, Robot 1 is told r and Robot 2 is told θ.)

Both robots can choose to make a single move after the flag is placed, but they do not know what the other robot is doing. Whichever robot is closest to the flag after moving, wins the round.

Robot 1 is programmed to play a fixed distance along the detected angle θ, but otherwise both robots play optimally.


# Solution
----