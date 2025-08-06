#include <SDL3/SDL.h>
#include <iostream>
#include "Player.h"
#include <cmath>
#define PI 3.14159265358979323846
#define P2 PI / 2
#define P3 3 * PI / 2
#define DR 0.0174532925
#define mapX  8      //map width
#define mapY  8      //map height
#define mapS 64      //map cube size

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
bool isRunning;

Player player(100,100,32,32);

static int map[] = {
	1,1,1,1,1,1,1,1,
    1,2,2,0,0,0,0,1,
    1,2,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,2,1,
    1,0,0,0,0,2,2,1,
    1,1,1,1,1,1,1,1,
};

void DrawMap2D()
{
	for (int y = 0; y < 8; ++y) {
		for (int x = 0; x < 8; ++x) {
			if (map[y * 8 + x] == 1) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				SDL_FRect rect = { x * 64, y * 64, 63, 63 };
                SDL_RenderFillRect(renderer, &rect);
            }
            else if (map[y * 8 + x] == 2)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_FRect rect = { x * 64, y * 64, 63, 63 };
                SDL_RenderFillRect(renderer, &rect);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_FRect rect = { x * 64, y * 64, 63, 63 };
                SDL_RenderFillRect(renderer, &rect);
            }
		}
	}
}

float dist(float ax, float ay, float bx, float by, float ang)
{
    return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}


void DrawRays2D()
{
    int r, mx, my, mp, dof;
    float rx, ry, ra, xo, yo,disT;

    int hitWallType;
    bool hitVert = false;

    ra = player.pa - DR * 30;// Rays angle = Players angle
    if (ra <= 0){ra += 2 * PI;}if (ra > 2 * PI){ra -= 2 * PI;}

    for (int r = 0; r < 60; r++)
    {
        //Check horizontal lines
        dof = 0;
        float disH = 100000, hx = player.playerObject.x + 16, hy = player.playerObject.y + 16;
        float aTan;
        if (fabs(tan(ra)) < 0.0001f) aTan = 100000000; // büyük bir deðer ver, sonsuz yerine geçer
        else aTan = -1 / tan(ra);

        if (ra > PI) //Looking down
        {
            ry = ((int)(player.playerObject.y + 16) / 64) * 64 - 0.0001f;
            rx = (player.playerObject.y + 16 - ry) * aTan + player.playerObject.x + 16;
            yo = -64;
            xo = -yo * aTan;
        }
        if (ra < PI) //Looking Up
        {
            ry = ((int)(player.playerObject.y + 16) / 64) * 64 + 64;
            rx = (player.playerObject.y + 16 - ry) * aTan + player.playerObject.x + 16;
            yo = 64;
            xo = -yo * aTan;
        }
        if (ra == 0 || ra == PI)
        {
            rx = player.playerObject.x + 16; ry = player.playerObject.y + 16; dof = 8;
        }

        while (dof < 8)
        {
            mx = (int)(rx / 64);
            my = (int)(ry / 64);
            mp = my * 8 + mx; // Map position
            if (mp >= 0 && mp < 64 && map[mp] > 0) // If the ray hits a wall
            {
                hx = rx;
                hy = ry;
                disH = dist(player.playerObject.x + 16, player.playerObject.y + 16, hx, hy, ra);
                dof = 8;
            }
            else
            {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }
        //Check vertical lines
        dof = 0;
        float disV = 100000, vx = player.playerObject.x + 16, vy = player.playerObject.y + 16;
        float nTan = -tan(ra);
        if (ra > P2 && ra < P3) //Looking left
        {
            rx = ((int)(player.playerObject.x + 16) / 64) * 64 - 0.0001f;
            ry = (player.playerObject.x + 16 - rx) * nTan + player.playerObject.y + 16;
            xo = -64;
            yo = -xo * nTan;
        }
        if (ra < P2 || ra > P3) //Looking right
        {
            rx = ((int)(player.playerObject.x + 16) / 64) * 64 + 64;
            ry = (player.playerObject.x + 16 - rx) * nTan + player.playerObject.y + 16;
            xo = 64;
            yo = -xo * nTan;
        }
        if (ra == 0 || ra == PI)
        {
            rx = player.playerObject.x + 16; ry = player.playerObject.y + 16; dof = 8;
        }

        while (dof < 8)
        {
            mx = (int)(rx / 64);
            my = (int)(ry / 64);
            mp = my * 8 + mx; // Map position
            if (mp >= 0 && mp < 64 && map[mp] > 0) // If the ray hits a wall
            {
                vx = rx;
                vy = ry;
                disV = dist(player.playerObject.x + 16, player.playerObject.y + 16, vx, vy, ra);
                dof = 8;
            }
            else
            {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }

        if (disV < disH)
        {
            rx = vx; ry = vy;
            disT = disV;
            mx = (int)(rx / 64);
            my = (int)(ry / 64);
            mp = my * 8 + mx;
            if (map[mp] == 1)
            {
                SDL_SetRenderDrawColor(renderer,255 * 0.9f,0,0,255);
            }
            if (map[mp] == 2)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255 * 0.9f, 255);
            }
        }
        else
        {
            rx = hx; ry = hy;
            disT = disH;
            mx = (int)(rx / 64);
            my = (int)(ry / 64);
            mp = my * 8 + mx;
            if (map[mp] == 1)
            {
                SDL_SetRenderDrawColor(renderer, 255 * 0.7f, 0, 0, 255);
            }
            if (map[mp] == 2)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255 * 0.7f, 255);
            }
        }

        SDL_RenderLine(renderer, player.playerObject.x + 16, player.playerObject.y + 16, rx, ry);
        //Draw 3D Walls
        float ca = player.pa - ra;
        if (ca < 0)
        {
            ca += 2 * PI;
        }
        if (ca > 2 * PI)
        {
            ca -= 2 * PI;
        }
		disT = disT * cos(ca); // Fisheye görüþ açýsýndan kurtulmak için
        float lineH = (mapS * 320) / disT; if (lineH > 320) { lineH = 320; }
        float lineO = 160 - lineH / 2;
        int x = r * 8 + 530;
        SDL_FRect wallStrip = { x, lineO, 8, lineH }; // 8 px geniþliðinde dikey çizgi
        SDL_RenderFillRect(renderer, &wallStrip);

        ra += DR; if (ra <= 0) { ra += 2 * PI; }if (ra > 2 * PI) { ra -= 2 * PI; }
    }

    
}

void Input()
{
	const bool* keys = SDL_GetKeyboardState(nullptr);
    if (keys[SDL_SCANCODE_A]) { player.pa -= 0.1f; if (player.pa < 0) { player.pa += PI * 2; } player.pdx = cos(player.pa) * 5; player.pdy = sin(player.pa) * 5; }
    if (keys[SDL_SCANCODE_D]) { player.pa += 0.1f; if (player.pa >= 2 * PI) { player.pa -= PI * 2; } player.pdx = cos(player.pa) * 5; player.pdy = sin(player.pa) * 5; }
    if (keys[SDL_SCANCODE_W]) { player.playerObject.x += player.pdx; player.playerObject.y += player.pdy;}
    if (keys[SDL_SCANCODE_S]) { player.playerObject.x -= player.pdx; player.playerObject.y -= player.pdy; }
}

void Update()
{
    Input();
}

void Render() 
{
    SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
    SDL_RenderClear(renderer);

    DrawMap2D();

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &player.playerObject);

    SDL_RenderLine(
        renderer,
        player.playerObject.x + 16,
        player.playerObject.y + 16,
        player.playerObject.x + 16 + player.pdx * 10,
		player.playerObject.y + 16 + player.pdy * 10
    );

    DrawRays2D();

    SDL_RenderPresent(renderer);
    SDL_Delay(16);
}

int main()
{
    player.pdx = cos(player.pa) * 5; player.pdy = sin(player.pa) * 5;
    if (SDL_Init(SDL_INIT_VIDEO) != 1) {
        std::cout << "SDL baþlatýlamadý! Hata: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!SDL_CreateWindowAndRenderer("Raytracer", 1024, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE,&window,&renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }
    isRunning = true;
    SDL_Event event;
    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
            }
        }
        Update();
        Render();
    }
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}