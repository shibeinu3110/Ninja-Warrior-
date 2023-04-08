#include "TextObject.h"

TextObject::TextObject()
{
    //ctor
    text_color_.r = 255;
    text_color_.g = 255;
    text_color_.b = 255;
    texture_ = NULL;
    rect.x = 0;
    rect.y = 0;
    rect.w = 0;
    rect.h = 0;

}

TextObject::~TextObject()
{
    //dtor

}


bool TextObject::LoadFromRenderText(TTF_Font* font , SDL_Renderer* screen )
{
    if (texture_ != NULL)
    {
        Free();
    }
    SDL_Surface* text_surface = TTF_RenderText_Solid(font , str_val_.c_str() , text_color_);
    if (text_surface != NULL)
    {
        texture_ = SDL_CreateTextureFromSurface(screen , text_surface);
        rect.w = text_surface->w;
        rect.h = text_surface->h;

        SDL_FreeSurface(text_surface);

    }
    return texture_!= NULL ; //tra ve true
}

void TextObject::Free()
{
    if (texture_!=NULL)
    {
        SDL_DestroyTexture(texture_);
        texture_ = NULL;
    }
}

void TextObject:: SetColor (Uint8 red , Uint8 green , Uint8 blue)
{
    text_color_.r = red;
    text_color_.g = green;
    text_color_.b = blue;
}

void TextObject:: SetColor (int type)
{
    if (type == RED_TEXT)
    {
        SDL_Color color = {255,0,0};
        text_color_ = color;
    }
    else if (type == WHITE_TEXT)
    {
        SDL_Color color = {255,255,255};
        text_color_ = color;
    }
    else if (type == BLACK_TEXT)
    {
        SDL_Color color = {0,0,0};
        text_color_ = color;
    }
}

void TextObject::RenderText(SDL_Renderer* screen /*, int xp , int yp */,
                            SDL_Rect* clip /*= NULL */, double angle /*= 0.0*/ ,
                            SDL_Point* center /*= NULL */,
                            SDL_RendererFlip flip /*= SDL_FLIP_NONE*/ )
{
    SDL_Rect renderQuad = {rect.x, rect.y , rect.w , rect.h};
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;

    }
    SDL_RenderCopyEx(screen ,texture_ , clip , &renderQuad , angle , center ,  flip );
}

BaseObject gMenu;


int Menu::ShowMenu(SDL_Renderer* des, TTF_Font* font) {
    if (gMenu.LoadImg("img//menu_img.png", des) == 0) {
        return 1;
    }

    const int MenuItemNum = 2;
    SDL_Rect posArr[MenuItemNum];
    posArr[0].x = SCREEN_WIDTH*0.5 - 170;
    posArr[0].y = 380;

    posArr[1].x = SCREEN_WIDTH*0.5 - 70;
    posArr[1].y = 450;

    TextObject textMenu[MenuItemNum];
    textMenu[0].SetText("Play Game");
    textMenu[0].SetColor(TextObject::WHITE_TEXT);
    textMenu[0].SetRect(posArr[0].x, posArr[0].y);

    textMenu[1].SetText("Exit");
    textMenu[1].SetColor(TextObject::WHITE_TEXT);
    textMenu[1].SetRect(posArr[1].x, posArr[1].y);

    bool selected[MenuItemNum] = {0, 0};
    int xm = 0;
    int ym = 0;
    SDL_Event mEvent;
    while (true) {
        gMenu.Render(des);
        for (int i = 0; i < MenuItemNum; i++) {
            textMenu[i].LoadFromRenderText(font, des);
            textMenu[i].RenderText(des);
            // textMenu[i].CreateGameText(font, des);
        }

        while (SDL_PollEvent(&mEvent)) {
            switch (mEvent.type)
            {
            case SDL_QUIT:
                return 1;
            case SDL_MOUSEMOTION:
                xm = mEvent.motion.x;
                ym = mEvent.motion.y;

                for (int i = 0; i < MenuItemNum; i++) {
                    if (SDLCommonFunc::CheckFocusWithRect(xm, ym, textMenu[i].GetRect()) == 1) {
                        if (selected[i] == false) {
                            selected[i] = true;
                            textMenu[i].SetColor(TextObject::RED_TEXT);
                        }
                    }
                    else {
                        if (selected[i] == true) {
                            selected[i] = false;
                            textMenu[i].SetColor(TextObject::BLACK_TEXT);
                        }
                    }
                }

                break;

            case SDL_MOUSEBUTTONDOWN:
                xm = mEvent.button.x;
                ym = mEvent.button.y;
                for (int i = 0; i < MenuItemNum; i++) {
                    if (SDLCommonFunc::CheckFocusWithRect(xm, ym, textMenu[i].GetRect()) == 1) {
                        return i;
                    }
                }

            case SDL_KEYDOWN:
                if (mEvent.key.keysym.sym == SDLK_ESCAPE) {
                    return 1;
                }

            default:
                break;
            }

        }
        SDL_RenderPresent(des);
    }

    return 1;
}

BaseObject gRestart;
int Menu::ShowRestart(SDL_Renderer* des, TTF_Font* font) {
    if (gRestart.LoadImg("img//restart_img.png", des) == 0) {
        return 1;
    }

    const int RestartItemNum = 2;
    SDL_Rect posArr[RestartItemNum];
    posArr[0].x = SCREEN_WIDTH*0.5 - 170;
    posArr[0].y = 200;

    posArr[1].x = SCREEN_WIDTH*0.5 - 70;
    posArr[1].y = 250;

    TextObject textMenu[RestartItemNum];
    textMenu[0].SetText("PLAY AGAIN");
    textMenu[0].SetColor(TextObject::WHITE_TEXT);
    textMenu[0].SetRect(posArr[0].x, posArr[0].y);

    textMenu[1].SetText("EXIT");
    textMenu[1].SetColor(TextObject::WHITE_TEXT);
    textMenu[1].SetRect(posArr[1].x, posArr[1].y);

    bool selected[RestartItemNum] = {0, 0};
    int xm = 0;
    int ym = 0;
    SDL_Event mEvent;
    while (true) {
        gRestart.Render(des);
        for (int i = 0; i < RestartItemNum; i++) {
            textMenu[i].LoadFromRenderText(font, des);
            textMenu[i].RenderText(des);
            // textMenu[i].CreateGameText(font, des);
        }

        while (SDL_PollEvent(&mEvent)) {
            switch (mEvent.type)
            {
            case SDL_QUIT:
                return 1;
            case SDL_MOUSEMOTION:
                xm = mEvent.motion.x;
                ym = mEvent.motion.y;

                for (int i = 0; i < RestartItemNum; i++) {
                    if (SDLCommonFunc::CheckFocusWithRect(xm, ym, textMenu[i].GetRect()) == 1) {
                        if (selected[i] == false) {
                            selected[i] = true;
                            textMenu[i].SetColor(TextObject::RED_TEXT);
                        }
                    }
                    else {
                        if (selected[i] == true) {
                            selected[i] = false;
                            textMenu[i].SetColor(TextObject::BLACK_TEXT);
                        }
                    }
                }

                break;

            case SDL_MOUSEBUTTONDOWN:
                xm = mEvent.button.x;
                ym = mEvent.button.y;
                for (int i = 0; i < RestartItemNum; i++) {
                    if (SDLCommonFunc::CheckFocusWithRect(xm, ym, textMenu[i].GetRect()) == 1) {
                        return i;
                    }
                }

            case SDL_KEYDOWN:
                if (mEvent.key.keysym.sym == SDLK_ESCAPE) {
                    return 1;
                }

            default:
                break;
            }

        }
        SDL_RenderPresent(des);
    }

    return 1;
}



























