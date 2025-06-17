#include <SDL2/SDL.h>
#include "../PixelForge/PixelForge/include/PixelForge/core/fb_context.hpp"
#include "../PixelForge/PixelForge/include/PixelForge/primitives/canvas.hpp"
#include "../PixelForge/PixelForge/include/PixelForge/primitives/color.hpp"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 800;

int main() {
    // SDL init
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }
    // Creating SDL Window
    SDL_Window* window = SDL_CreateWindow(
        "PixelForge SDL Test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_SOFTWARE
    );

    if (!renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    // Creating "framebuffer"
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(
        0,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        32,
        SDL_PIXELFORMAT_ARGB8888
    );

    if (!surface) {
        SDL_Log("SDL_CreateRGBSurfaceWithFormat failed: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Creating FrameBuffer context ans Canvas
    PixelForge::FBContext fb;
    fb.initFromBuffer(
        surface->pixels,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        surface->pitch,
        32
    );

    PixelForge::Canvas canvas(fb);

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        // Clearing screen
        SDL_FillRect(surface, nullptr, SDL_MapRGBA(surface->format, 0, 0, 0, 255));

        // Painting
        canvas.fillRect(100, 100, 300, 200, 0xFFFF0000);
        canvas.fillCircle(400, 300, 80, 0xFF00FF00);

        canvas.drawLine(50, 50, 750, 550, 0xFF0000FF);

        std::vector<std::pair<int, int>> triangle = {
            {600, 100},
            {700, 200},
            {500, 200}
        };
        canvas.drawPolygon(triangle, 0xFFFFFFFF);
        canvas.drawPolygon(triangle, 0xFFFFFFFF);

        // Screen update
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(texture);
    }

    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
