#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WIDTH 720
#define HEIGHT 720
#define FONT_SIZE 32
#define FRAME_RATE 10 

SDL_Renderer* renderer;
SDL_Window* window;
TTF_Font* font;
SDL_Color color;

bool running;
bool waiting_for_input;
int frameCount, timerFPS, lastFrame, fps;

std::string num_rounds;
int current_rounds;
int robot1_wins;
int robot2_wins;
double robot1_winrate, robot2_winrate;


std::string_score;

void render();
void update();
void input();
void write_score(std::string, int, int);

// State of game
enum STATE {
    ENTRY,
    SIMULATING,
    RESULTS,
} game_state;

int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        std::cout << "Failed at SDL_Init()" << std::endl;
    if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer) < 0)
        std::cout << "Failed at SDL_CreateWindowAndRenderer()" << std::endl;
    TTF_Init();
    font = TTF_OpenFont("Peepo.ttf", FONT_SIZE);
    if (font = NULL) 
        std::cout << "Failed to load font" << std::endl;
    running = true
    static int lastTime = 0;

    STATE game_state = STATE::ENTRY;
    while(running) {
        lastFrame = SDL_GetTicks();
        if (lastFrame >= lastTime + 1000) {
            lastTime = lastFrame;
            fps = frameCount;
            frameCount = 0;
        }
        while (game_state == STATE::ENTRY) {

        }


    }
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Updates score
void write_score(std::string text, int x, int y) {

}

// Updates user input
void write_input(std::string input, int x, int y) {

}

void input() {
    SDL_Event e;
    cont Uint8 *keystates = SDL_GetKeyboardState(NULL);
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            running = false;
    }
    if (keystates[SDL_SCANCODE_ESCAPE])
        running = false;
    if (game_state == STATE::ENTRY || game_state == STATE::RESULTS) {
        if (keystates[SDL_SCANCODE_KP_ENTER])
            game_state = STATE::SIMULATING;
        if (keystates[SDL_SCANCODE_BACKSPACE])
            num_rounds.pop_back();
        // Number inputs
        if (keystates[SDL_SCANCODE_0] && num_rounds.length != 0)
            num_rounds.push_back('0');
        if (keystates[SDL_SCANCODE_1])
            num_rounds.push_back('1');
        if (keystates[SDL_SCANCODE_2])
            num_rounds.push_back('2');
        if (keystates[SDL_SCANCODE_3])
            num_rounds.push_back('3');
        if (keystates[SDL_SCANCODE_4])
            num_rounds.push_back('4');
        if (keystates[SDL_SCANCODE_5])
            num_rounds.push_back('5');
        if (keystates[SDL_SCANCODE_6])
            num_rounds.push_back('6');
        if (keystates[SDL_SCANCODE_7])
            num_rounds.push_back('7');
        if (keystates[SDL_SCANCODE_8])
            num_rounds.push_back('8');
        if (keystates[SDL_SCANCODE_9])
            num_rounds.push_back('9');
    }
    


    

    
}
