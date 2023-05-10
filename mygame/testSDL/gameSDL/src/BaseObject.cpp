

#include "BaseObject.h"
//#include "stdafx.h"
BaseObject::BaseObject() //dinh nghia constructor
{
    p_object_ = NULL;
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = 0;
    rect_.h = 0;
}

BaseObject::~BaseObject()
{
    //dtor
    Free(); //ham huy de tiet kiem bo nho
}

bool BaseObject::LoadImg (std::string path , SDL_Renderer* screen)
{
    SDL_Texture* new_texture = NULL;

    SDL_Surface* load_surface = IMG_Load (path.c_str());
    if (load_surface != NULL) //kiem tra viec load
    {
        // xoa background thua cua 1 anh
        SDL_SetColorKey (load_surface, SDL_TRUE , SDL_MapRGB (load_surface->format, COLOR_KEY_R,COLOR_KEY_G , COLOR_KEY_B));
        new_texture = SDL_CreateTextureFromSurface (screen , load_surface); //luu giu thong tin anh
        if (new_texture !=NULL) //load thanh cong
        {

            rect_.w=load_surface ->w;
            rect_.h=load_surface ->h;

        }
        SDL_FreeSurface (load_surface);
    }
    p_object_ =new_texture ;

    return p_object_ !=NULL;
    //neu p_object khac NULL thi return 1 =>true
    //neu p_object bang NULL thi return 0 =>false
}

void BaseObject::Render (SDL_Renderer* des , const SDL_Rect* clip)
{
    SDL_Rect renderquad= {rect_.x , rect_.y ,rect_.w ,rect_.h};
    SDL_RenderCopy (des, p_object_ , clip , &renderquad);  //truyen p_object den destination voi cac thong so cua renderequad
}

void BaseObject::Free ()
{
    if (p_object_!=NULL)
    {
        SDL_DestroyTexture(p_object_);
        p_object_ = NULL;
        rect_.w = 0;
        rect_.h = 0;
    }

}






























