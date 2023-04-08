#ifndef TEXT_OBJECT_H_
#define TEXT_OBJECT_H_

#include "BaseObject.h"

class TextObject
{
    public:
        TextObject();
         ~TextObject();
        enum TextColor
        {
            RED_TEXT = 0,
            WHITE_TEXT = 1,
            BLACK_TEXT = 2,
        };

        bool LoadFromRenderText (TTF_Font* font , SDL_Renderer* screen ); //font
        void Free();
        void SetColor (Uint8 red , Uint8 green , Uint8 blue);

        void SetColor(int type);

        void RenderText(SDL_Renderer* screen /*, int xp , int yp*/ ,
                        SDL_Rect* clip = NULL , double angle = 0.0 ,
                        SDL_Point* center = NULL ,
                        SDL_RendererFlip flip = SDL_FLIP_NONE );
        int GetWidth() const {return /*width_*/rect.w; }
        int GetHeight() const {return /*height_*/rect.h ;}

        void SetText (const std::string& text ) {str_val_ = text ;}
        std::string GetText() const {return str_val_;}
        void SetRect(const int& xp, const int& yp) { rect.x = xp, rect.y = yp; }
        SDL_Rect GetRect() const { return rect; }

    private:
        std::string str_val_;
        SDL_Color text_color_; //mau chu
        SDL_Texture* texture_; //quan ly text
        /*int width_;
        int height_;*/
        SDL_Rect rect;
};
namespace Menu {
    // handle menu
    int ShowMenu(SDL_Renderer* des, TTF_Font* font);
    int ShowRestart(SDL_Renderer* des, TTF_Font* font);
}



#endif // TEXTOBJECT_H







































