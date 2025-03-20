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
    explicit UILabelComponent(const std::string& fontId, const std::string& message, SDL_Color textColor)
    : font(nullptr), texture(nullptr), text(message), color(textColor)
    {
        font = GameManager::assets->GetFont(fontId);
        if (!font) {
            std::cerr << "Erreur : Police avec l'ID '" << fontId << "' non trouvée dans AssetManager !" << std::endl;
            return;
        }

        setText(message);
        setPosition(0, 0);
    }

    ~UILabelComponent()
    {
        if (texture) SDL_DestroyTexture(texture);
    }

    void setText(const std::string& newText)
    {
        if (text == newText) return;  // Évite une recréation inutile
        
        if (!font) {
            std::cerr << "Impossible de mettre à jour le texte, police non chargée !" << std::endl;
            return;
        }
    
        if (!GameManager::renderer) {
            std::cerr << "Erreur : Renderer non initialisé !" << std::endl;
            return;
        }

        if (texture) {
            SDL_DestroyTexture(texture);  // Évite les fuites mémoire
            texture = nullptr;
        }
    
        text = newText;
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
        if (!surface) {
            std::cerr << "Erreur : Impossible de créer la surface du texte : " << TTF_GetError() << std::endl;
            return;
        }
    
        texture = SDL_CreateTextureFromSurface(GameManager::renderer, surface);
        if (!texture) {
            std::cerr << "Erreur : Impossible de créer la texture du texte : " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            return;
        }
    
        destRect.w = surface->w;
        destRect.h = surface->h;

        SDL_FreeSurface(surface);
    }
    
    const std::string& getText() const { return text; }

    void setPosition(int x, int y) {
        destRect.x = x;
        destRect.y = y;
    }

    void draw() override {
        if (!texture) {
            std::cerr << "Warning: UILabelComponent attempted to render a null texture!" << std::endl;
            return;
        }
        SDL_RenderCopy(GameManager::renderer, texture, nullptr, &destRect);
    }
};
