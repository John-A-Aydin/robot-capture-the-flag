# WIP

Just messing around with SDL2 and CUDA to simulate [Jane Street's puzzle from April 2024](https://www.janestreet.com/puzzles/robot-capture-the-flag-index/)


[robot-game.webm](https://github.com/John-A-Aydin/jane-st-robot-game/assets/132794075/2d0b105d-0d4d-4bb9-99b4-852330623975)


# How to Run

For the animated simulator you need to download [SLD2](https://www.libsdl.org/).

On Debian Linux, use
```console
sudo apt-get install libsdl2-dev libsdl2-ttf-dev
```
On macOS, use
```console
brew install sdl2 sdl2_ttf
```

To run the animated simulator go to the root directory of the project and use
```console
make && ./build/robot-game
```

To run the an optimized version of the simulator without the animation use
```console
make sim && ./build/sim
```

To run the optimized simulator with higher accuracy use
```console
make sim && ./build/sim -a <ACCURACY>
```

The default accuracy is 10 which creates a grid of $2^{(10\times 2)}$ points that are checked.

There is a further optimized version of the simulation, but you'll need the CUDA Toolkit and an Nvidia GPU to run it.

```console
mkdir build
nvcc src/sim_gpu.cu -o build/sim_gpu
./build/sim_gpu
```








# Game Explanation

The full explanation can be found [here](https://www.janestreet.com/puzzles/robot-capture-the-flag-index/).

Two robots are placed at the center of a unit circle. A flag is placed somewhere inside the circle, at a location chosen uniformly at random. Once the flag is placed, Robot 1 (blue) is able to deduce its direction to the flag, and Robot 2 (red) is able to deduce its distance to the flag. (Equivalently: if (r, θ) are the polar coordinates of the flag’s location, Robot 1 is told r and Robot 2 is told θ.)

Both robots can choose to make a single move after the flag is placed, but they do not know what the other robot is doing. Whichever robot is closest to the flag after moving, wins the round.

Robot 1 is programmed to play a fixed distance along the detected angle θ, but otherwise both robots play optimally.


# Solution

The first step in solving this problem is recognizing the distribution of points where the flag can be placed. Since the flag is placed uniformly at random over the unit circle, the probability density function (PDF) of the flag being placed at any point $(x, y)$ is a constant.

$$
f(x,y) = C
$$

Knowing that the volume under this PDF is 1 and the area of the unit circle is $\pi$, the value for $C$ is $\frac{1}{\pi}$

### Note:

Here's a [cool video](https://www.youtube.com/watch?v=4y_nmpv-9lI) I found about generating random points in terms of polar coordinates that discusses a unique problem I encountered.
-----



Next, the optimal strategies for Robot 1 and 2 need to be found. Since Robot 2 knows the radius of the flag, $R_f$, and will know Robot 1's fixed radius, $R_{1}$, it will always know Robot 1's distance to the flag, $d_{1}$.


The positions of the flag that would give Robot 2 the win can be defined by the disk: $r\lt d_1$ centered at whatever point Robot 2 moves to. Using this information, Robot 2 should choose a point where the circle $r=R_f$ is maximally covered by the disk of winning points for Robot 2.

If $R_f$ is less than $\large\frac{R_1}{2}$, Robot 2 can simply stay at the origin, knowing that the circle $r = R_f$ is completely within Robot 2's disk of winning points.

If $R_f$ is greater than $\large\frac{R_1}{2}$, this point happens to be the center of a chord of the circle $r=R_f$ with chord length $2d_1$. This chord is shown in the simulation as a dotted red line passing through Robot 2.

Robot 2's strategy can be expressed by the piecewise function:

$$
R_2 = \begin{cases}
   0 &\text{if } \quad 0 \le R_f \le \frac{R_1}{2}\\
   R_1(2R_f-R_1) &\text{if } \quad\frac{R_1}{2} \lt R_f \le 1\\
\end{cases} \tag{1}
$$

With this strategy, we can construct another piecewise function that tells us Robot 2's chance of winning for any given value of $R_f$. This function returns the amount of the circle covered by Robot 2's winning disk over the total length of the circle.
$$
P(\text{Robot 2}) = \begin{cases}
   1 &\text{if } \quad 0 \le R_f \le \frac{R_1}{2}\\
   {\large\frac{1}{\pi}}\arcsin\Big(\frac{|R_f - R_1|}{R_f}\Big) &\text{if } \quad\frac{R_1}{2} \lt R_f \le 1\\
\end{cases} \tag{2}
$$

Next we integrate over (2) with respect to $R_f$ while accounting for the PDF of $R_f$ to find the total probability of Robot 2 winning.

$$
{\large\int_{0}^{2\pi}\int_{0}^{\frac{R_1}{2}}} \frac{r}{\pi}\quad  dR_fd\theta \quad + \quad 
{\large\int_{\frac{R_1}{2}}^{1}} \frac{2}{\pi}\arcsin\Bigg(\frac{|R_f - R_1|}{R_f}\Bigg)R_f\quad dR_f \tag{3}
$$





-------

Robot 2 then needs to maximize the angle on the circle given by $r = R_f$ where $d_2$, Robot 2's distance to the flag, is less than $d_1$.

To achieve this Robot 2 needs to move to the center of a chord of length $2d_1$
The ideal 
