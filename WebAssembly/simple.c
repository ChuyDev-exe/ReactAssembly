#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <emscripten.h>
#include <stdbool.h>

// Screen dimensions
#define SCREEN_WIDTH 300
#define SCREEN_HEIGHT 300

// Colors
SDL_Color COLOR_BG = {30, 30, 46, 255};        // Dark background
SDL_Color COLOR_TEXT = {137, 180, 250, 255};   // Blue text
SDL_Color COLOR_BUTTON_ON = {166, 227, 161, 255};  // Green
SDL_Color COLOR_BUTTON_OFF = {243, 139, 168, 255}; // Red
SDL_Color COLOR_BUTTON_TEXT = {30, 30, 46, 255};   // Dark text

// Global variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;
bool isToggled = false;

// Button rectangle
SDL_Rect buttonRect = {75, 200, 150, 50};

// Function to check if point is inside rectangle
bool pointInRect(int x, int y, SDL_Rect* rect) {
    return (x >= rect->x && x <= rect->x + rect->w &&
            y >= rect->y && y <= rect->y + rect->h);
}

// Function to render text
void renderText(const char* text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture) {
            SDL_Rect destRect = {x, y, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, NULL, &destRect);
            SDL_DestroyTexture(texture);
        }
        SDL_FreeSurface(surface);
    }
}

// Function to render centered text
void renderTextCentered(const char* text, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture) {
            int x = (SCREEN_WIDTH - surface->w) / 2;
            SDL_Rect destRect = {x, y, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, NULL, &destRect);
            SDL_DestroyTexture(texture);
        }
        SDL_FreeSurface(surface);
    }
}

// Main loop function
void mainLoop() {
    SDL_Event event;
    
    // Handle events
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX = event.button.x;
            int mouseY = event.button.y;
            
            // Check if button was clicked
            if (pointInRect(mouseX, mouseY, &buttonRect)) {
                isToggled = !isToggled;
            }
        }
    }
    
    // Clear screen
    SDL_SetRenderDrawColor(renderer, COLOR_BG.r, COLOR_BG.g, COLOR_BG.b, COLOR_BG.a);
    SDL_RenderClear(renderer);
    
    // Render title
    renderTextCentered("SDL2 Simple Demo", 30, COLOR_TEXT);
    
    // Render status text
    const char* statusText = isToggled ? "Status: ON" : "Status: OFF";
    renderTextCentered(statusText, 80, COLOR_TEXT);
    
    // Render button
    SDL_Color buttonColor = isToggled ? COLOR_BUTTON_ON : COLOR_BUTTON_OFF;
    SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    SDL_RenderFillRect(renderer, &buttonRect);
    
    // Render button border
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &buttonRect);
    
    // Render button text
    const char* buttonText = isToggled ? "Turn OFF" : "Turn ON";
    SDL_Surface* buttonSurface = TTF_RenderText_Blended(font, buttonText, COLOR_BUTTON_TEXT);
    if (buttonSurface) {
        SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(renderer, buttonSurface);
        if (buttonTexture) {
            int textX = buttonRect.x + (buttonRect.w - buttonSurface->w) / 2;
            int textY = buttonRect.y + (buttonRect.h - buttonSurface->h) / 2;
            SDL_Rect textRect = {textX, textY, buttonSurface->w, buttonSurface->h};
            SDL_RenderCopy(renderer, buttonTexture, NULL, &textRect);
            SDL_DestroyTexture(buttonTexture);
        }
        SDL_FreeSurface(buttonSurface);
    }
    
    // Render instructions
    renderTextCentered("Click the button to toggle", 140, COLOR_TEXT);
    
    // Present renderer
    SDL_RenderPresent(renderer);
}

// Initialize SDL
int init() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
        return 0;
    }
    
    // Create window
    window = SDL_CreateWindow("SDL2 Simple Demo",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             SCREEN_WIDTH,
                             SCREEN_HEIGHT,
                             SDL_WINDOW_SHOWN);
    
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    
    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    
    // Load font
    font = TTF_OpenFont("/fuente.ttf", 20);
    if (font == NULL) {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        return 0;
    }
    
    return 1;
}

// Cleanup
void cleanup() {
    if (font) TTF_CloseFont(font);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

// Main function
int main(int argc, char* args[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
        return 1;
    }
    
    printf("SDL2 Simple Demo initialized successfully!\n");
    
    // Set the main loop
    emscripten_set_main_loop(mainLoop, 0, 1);
    
    // Cleanup (won't be reached in Emscripten, but good practice)
    cleanup();
    
    return 0;
}
