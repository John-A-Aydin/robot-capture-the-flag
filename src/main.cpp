#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <utility>
#include <math.h>
#include <numbers>
#include "sim.h"

using std::pair;

#define WIDTH 720
#define HEIGHT 720
#define SCORE_HEIGHT 120
#define FONT_SIZE 32
#define ROBOT_SIZE 8
#define TARGET_SIZE 4

pair<double, double> target, robot1, robot2;


bool running;
bool waiting_for_input;
int frameCount, timerFPS, lastFrame, fps, frame_rate;

std::string num_rounds;
int current_rounds;
int robot1_wins;
int robot2_wins;
double robot1_winrate, robot2_winrate;


std::string score;

void render();
void update();
void input();
void write_score(std::string, int, int);
int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius);
int SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius);
void render_robots(SDL_Renderer * renderer, pair<double, double> r1, pair<double, double> r2, SDL_Color r1_color, SDL_Color r2_color);
void render_game_area(SDL_Renderer * renderer, SDL_Color color);
void render_target(SDL_Renderer * renderer, pair<double, double> target, SDL_Color color);



// State of game
enum STATE {
    ENTRY,
    BEGIN,
    PLACE_TARGET,
    MOVE,
    MEASURE,
    RESULTS,
};

int main() {
    // Creating renderer and window
    SDL_Renderer* renderer;
    SDL_Window* window;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        std::cout << "Failed at SDL_Init()" << std::endl;
    if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT + SCORE_HEIGHT, 0, &window, &renderer) < 0)
        std::cout << "Failed at SDL_CreateWindowAndRenderer()" << std::endl;
    // Initializing font
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("Peepo.ttf", FONT_SIZE);
    if (font = NULL) 
        std::cout << "Failed to load font" << std::endl;
    // Setting colors
    SDL_Color main_color, r1_color, r2_color, target_color;

    main_color.r = main_color.g = main_color.b = 255;
    r1_color.r = 66;
    r1_color.g = 120;
    r1_color.b = 245;

    r2_color.r = 245;
    r2_color.g = 66;
    r2_color.b = 66;

    target_color.r = 43;
    target_color.g = 237;
    target_color.b = 46;

    // Declaring game variables
    pair<double, double> robot1 = std::make_pair(0.0, 0.0);
    pair<double, double> robot2 = std::make_pair(0.0, 0.0);
    pair<double, double> target;
    pair<double, double> target_polar;

    double robot1_distance;
    double robot2_distance;
    
    int robot1_measure_radius;
    int robot2_measure_radius;

    int robot1_pixel_distance;
    int robot2_pixel_distance;

    // Entering game loop and setting frame rate
    STATE game_state = STATE::BEGIN;
    running = true;
    static int lastTime = 0;
    frame_rate = 15;
    int frames_in_state = 0;
    while(running) {
        lastFrame = SDL_GetTicks();
        if (lastFrame >= lastTime + 1000) {
            lastTime = lastFrame;
            fps = frameCount;
            frameCount = 0;
        }
        frames_in_state++;
        render_game_area(renderer, main_color);
        render_robots(renderer, robot1, robot2, r1_color, r2_color);

        switch (game_state)
        {
        case STATE::BEGIN:
            if (frames_in_state == 1) {
                robot1 = std::make_pair(0.0, 0.0);
                robot2 = std::make_pair(0.0, 0.0);
            } else if (frames_in_state == 30) {
                game_state = STATE::PLACE_TARGET;
                frames_in_state = 0;
            }
            break;
        case STATE::PLACE_TARGET:
            if (frames_in_state == 1) {
                target = sim::generate_point();
                target_polar = sim::convert_to_polar(target);
            } else if (frames_in_state == 30) {
                game_state = STATE::MOVE;
                frames_in_state = 0;
            }
            render_target(renderer, target, target_color);
            break;
        case STATE::MOVE:
            if (frames_in_state == 1) {
                robot1 = sim::robot1_move(target_polar.second);
                robot2 = sim::robot2_move(target_polar.first);
            } else if (frames_in_state == 30) {
                game_state = STATE::MEASURE;
                frames_in_state = 0;
            }
            render_target(renderer, target, target_color);
            break;
        case STATE::MEASURE:
            if (frames_in_state == 1) {
                robot1_measure_radius = 0;
                robot2_measure_radius = 0;
                robot1_distance = sim::distance(robot1, target);
                robot2_distance = sim::distance(robot2, target);
                robot1_pixel_distance = robot1_distance*WIDTH/2;
                robot2_pixel_distance = robot2_distance*WIDTH/2;
                frame_rate = 60;
            }
            // frames_in_state--;
            if (robot1_measure_radius >= robot1_pixel_distance || robot2_measure_radius >= robot2_pixel_distance) {
                if (robot1_distance < robot2_distance) {
                    SDL_SetRenderDrawColor(renderer, target_color.r, target_color.g, target_color.b, 255);
                    SDL_RenderDrawCircle(renderer, (int)((robot1.first + 1)*WIDTH/2), (int)((robot1.second + 1)*HEIGHT/2), robot1_measure_radius);
                    SDL_SetRenderDrawColor(renderer, r2_color.r, r2_color.g, r2_color.b, 255);
                    SDL_RenderDrawCircle(renderer, (int)((robot2.first + 1)*WIDTH/2), (int)((robot2.second + 1)*HEIGHT/2), robot2_measure_radius);
                } else {
                    SDL_SetRenderDrawColor(renderer, r1_color.r, r1_color.g, r1_color.b, 255);
                    SDL_RenderDrawCircle(renderer, (int)((robot1.first + 1)*WIDTH/2), (int)((robot1.second + 1)*HEIGHT/2), robot1_measure_radius);
                    SDL_SetRenderDrawColor(renderer, target_color.r, target_color.g, target_color.b, 255);
                    SDL_RenderDrawCircle(renderer, (int)((robot2.first + 1)*WIDTH/2), (int)((robot2.second + 1)*HEIGHT/2), robot2_measure_radius);
                }
            } else {
                robot1_measure_radius++;
                robot2_measure_radius++;
                SDL_SetRenderDrawColor(renderer, r1_color.r, r1_color.g, r1_color.b, 255);
                SDL_RenderDrawCircle(renderer, (int)((robot1.first + 1)*WIDTH/2), (int)((robot1.second + 1)*HEIGHT/2), robot1_measure_radius);
                SDL_SetRenderDrawColor(renderer, r2_color.r, r2_color.g, r2_color.b, 255);
                SDL_RenderDrawCircle(renderer, (int)((robot2.first + 1)*WIDTH/2), (int)((robot2.second + 1)*HEIGHT/2), robot2_measure_radius);
            }
            render_target(renderer, target, target_color);
            if (frames_in_state == 240) {
                game_state = STATE::BEGIN;
                frames_in_state = 0;
                frame_rate = 15;
            }
            break;
        default:
            break;
        }
        frameCount++;
        timerFPS = SDL_GetTicks() - lastFrame;
        if (timerFPS < (1000/frame_rate)) {
            SDL_Delay((1000/frame_rate) - timerFPS);
        }
        SDL_RenderPresent(renderer);
        input();
    }
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// void render() {
//     SDL_RenderDrawColor(renderer, 0x00, 0x00, 0x00, 255);
//     SDL_RenderClear(renderer);
//     frameCount++;
//     timerFPS = SDL_GetTicks() - lastFrame;
//     if (timerFPS < (1000/frame_rate)) {
//         SDL_Delay((1000/frame_rate) - timerFPS);
//     }

//     SDL_RendererDrawCircle(renderer, 0, 0, WIDTH/2);
//     SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255); // Main Color

    
//     SDL_RenderDrawLineF(renderer, (target.first + 1)*HEIGHT/2, (1.0 - target.second + 1)*HEIGHT/2, (robot1.first + 1)*HEIGHT/2, (1 - robot1.second)*HEIGHT/2);
//     SDL_RenderDrawLineF(renderer, (target.first + 1)*HEIGHT/2, (1.0 - target.second + 1)*HEIGHT/2, (robot1.first + 1)*HEIGHT/2, (1 - robot1.second)*HEIGHT/2);

//     SDL_RenderPresent(renderer); // Draw everything to screen
// }

void render_game_area(SDL_Renderer * renderer, SDL_Color color) {
    if (!renderer)
        throw std::runtime_error("null renderer");
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 255);
    SDL_RenderClear(renderer);
    // Updating frame data
    frameCount++;
    timerFPS = SDL_GetTicks() - lastFrame;
    if (timerFPS < (1000/frame_rate)) {
        SDL_Delay((1000/frame_rate) - timerFPS);
    }
    // Drawing boarder circle
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255); // Main Color
    SDL_RenderDrawCircle(renderer, HEIGHT/2, WIDTH/2, WIDTH/2);
}

void render_robots(SDL_Renderer * renderer, pair<double, double> r1, pair<double, double> r2, SDL_Color r1_color, SDL_Color r2_color) {
    if (!renderer)
        throw std::runtime_error("null renderer");
    int r1_x = (r1.first + 1)*WIDTH/2;
    int r1_y = (r1.second + 1)*HEIGHT/2;
    int r2_x = (r2.first + 1)*WIDTH/2;
    int r2_y = (r2.second + 1)*HEIGHT/2;
    // Creating robot circles
    SDL_SetRenderDrawColor(renderer, r1_color.r, r1_color.g, r1_color.b, 128);
    SDL_RenderFillCircle(renderer, r1_x, r1_y, ROBOT_SIZE);
    SDL_SetRenderDrawColor(renderer, r2_color.r, r2_color.g, r2_color.b, 128);
    SDL_RenderFillCircle(renderer, r2_x, r2_y, ROBOT_SIZE);
}

void render_target(SDL_Renderer * renderer, pair<double, double> target, SDL_Color color) {
    if (!renderer)
        throw std::runtime_error("null renderer");
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillCircle(renderer, (int)((target.first + 1)*WIDTH/2), (int)((target.second + 1)*HEIGHT/2), TARGET_SIZE);
}




// Updates score
// void write_score(std::string text, int x, int y) {

// }

// Updates user input
// void write_input(std::string input, int x, int y) {

// }

void input() {
    SDL_Event e;
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            running = false;
    }
    if (keystates[SDL_SCANCODE_ESCAPE])
        running = false;
    // if (game_state == STATE::ENTRY || game_state == STATE::RESULTS) {
    //     if (keystates[SDL_SCANCODE_KP_ENTER])
    //         game_state = STATE::SIMULATING;
    //     if (keystates[SDL_SCANCODE_BACKSPACE])
    //         num_rounds.pop_back();
    //     // Number inputs
    //     if (keystates[SDL_SCANCODE_0] && num_rounds.length != 0)
    //         num_rounds.push_back('0');
    //     if (keystates[SDL_SCANCODE_1])
    //         num_rounds.push_back('1');
    //     if (keystates[SDL_SCANCODE_2])
    //         num_rounds.push_back('2');
    //     if (keystates[SDL_SCANCODE_3])
    //         num_rounds.push_back('3');
    //     if (keystates[SDL_SCANCODE_4])
    //         num_rounds.push_back('4');
    //     if (keystates[SDL_SCANCODE_5])
    //         num_rounds.push_back('5');
    //     if (keystates[SDL_SCANCODE_6])
    //         num_rounds.push_back('6');
    //     if (keystates[SDL_SCANCODE_7])
    //         num_rounds.push_back('7');
    //     if (keystates[SDL_SCANCODE_8])
    //         num_rounds.push_back('8');
    //     if (keystates[SDL_SCANCODE_9])
    //         num_rounds.push_back('9');
    // } 
}

int SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius) {
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius) {
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}