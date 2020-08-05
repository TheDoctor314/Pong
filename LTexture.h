#ifndef LTEXTURE_H
#define LTEXTURE_H

#include <SDL2/SDL_ttf.h>
#include <string>
#include <stdio.h>

extern TTF_Font *gFont;
extern SDL_Renderer *gRenderer;


class LTexture
{
    private:
    SDL_Texture *pTexture;
    int textureWidth, textureHeight;

    public:

    LTexture();
    ~LTexture();
    bool loadRenderedText(std::string textureText, SDL_Color textColor);
    void free();
    void render(int x, int y);

    int getWidth();
    int getHeight();
};

#endif