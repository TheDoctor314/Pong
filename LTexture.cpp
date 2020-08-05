#include "LTexture.h"

LTexture::LTexture()
{
    pTexture = NULL;
    textureHeight = textureWidth = 0;
}
LTexture::~LTexture()
{
    free();
}

int LTexture::getHeight()
{
    return textureHeight;
}
int LTexture::getWidth()
{
    return textureWidth;
}

void LTexture::free()
{
    if( pTexture != NULL)
    {
        SDL_DestroyTexture(pTexture);
        pTexture = NULL;
        textureWidth = textureHeight = 0;
    }
}
bool LTexture::loadRenderedText(std::string textureText, SDL_Color textColor)
{
    free();

    SDL_Surface *textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
    if( textSurface == NULL)
    {
        printf("Unable to render text. TTF error: %s", TTF_GetError());
        return false;
    }
    
    pTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
    if( pTexture == NULL)
    {
        printf("Unable to create texture from rendered text. SDL error: %s", SDL_GetError());
        return false;
    }

    textureWidth = textSurface->w;
    textureHeight = textSurface->h;
    SDL_FreeSurface(textSurface);

    return true;
}
void LTexture::render(int x, int y)
{
    SDL_Rect RenderRect = {x, y, textureWidth, textureHeight};
    SDL_RenderCopy(gRenderer, pTexture, NULL, &RenderRect);
}

