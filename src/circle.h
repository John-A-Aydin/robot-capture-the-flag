#include <SDL2/SDL.h>


int     SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius);
int     SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius);
int     SDL_RenderDrawDottedCircle(SDL_Renderer * renderer, int x, int y, int radius, int spacing);
int     SDL_RenderDrawDashedCircle(SDL_Renderer * renderer, int x, int y, int radius, int spacing);
int     SDL_RenderDrawDashedCircle2(SDL_Renderer * renderer, int x, int y, int radius, int dashes, float dash_to_blank);
