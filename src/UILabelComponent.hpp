#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "Components.hpp"
#include "GameManager.hpp"

class UILabelComponent : public Component {
private:
    SDL_Texture* texture;
    SDL_Rect destRect;
    TTF_Font* font;
    SDL_Color color;
    std::string text;

public:
    explicit UILabelComponent(const std::string& fontPath, int fontSize, const std::string& message, SDL_Color textColor)
        : font(nullptr), texture(nullptr), text(message), color(textColor) {
        
        font = TTF_OpenFont(fontPath.c_str(), fontSize);
        if (!font) {
            std::cerr << "Erreur : Impossible de charger la police " << fontPath << " : " << TTF_GetError() << std::endl;
        }
        setText(message);
    }

    ~UILabelComponent() {
        if (texture) SDL_DestroyTexture(texture);
        if (font) TTF_CloseFont(font);
    }

    void setText(const std::string& newText) {
        text = newText;
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
        if (!surface) {
            std::cerr << "Erreur : Impossible de créer la surface du texte : " << TTF_GetError() << std::endl;
            return;
        }
        texture = SDL_CreateTextureFromSurface(GameManager::renderer, surface);
        destRect = {100, 50, surface->w, surface->h}; // Position par défaut
        SDL_FreeSurface(surface);
    }

    void setPosition(int x, int y) {
        destRect.x = x;
        destRect.y = y;
    }

    void draw() override {
        SDL_RenderCopy(GameManager::renderer, texture, nullptr, &destRect);
    }
};
