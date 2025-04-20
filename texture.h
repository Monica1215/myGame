// Visit http://lazyfoo.net/tutorials/SDL/index.php
#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <string>

// Forward declarations
class Font;

//Texture wrapper class
class Texture {
public:

    Texture(SDL_Renderer* renderer);
    ~Texture();
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    void loadFromFile( const std::string& path );
    void loadFromRenderedText(const std::string& textureText, Font& f, SDL_Color textColor );
    void free();
    void setColor( Uint8 red, Uint8 green, Uint8 blue );
    void setBlendMode( SDL_BlendMode blending );
    void setAlpha( Uint8 alpha );
    void getColor(Uint8 &r, Uint8 &g, Uint8 &b );
    void render( int x, int y, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE );
    void renderBasic(const SDL_Rect& rect) const;

    int getWidth() const {return mWidth; }
    int getHeight() const {return mHeight;}

    SDL_Texture* getTexture() {return mTexture;}

private:
    SDL_Texture* mTexture;
    SDL_Renderer* gRenderer;

    int mWidth = 0;
    int mHeight = 0;
};



#endif // TEXTURE_H
