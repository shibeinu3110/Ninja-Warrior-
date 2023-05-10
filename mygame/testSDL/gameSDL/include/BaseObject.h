#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_
#include "CommonFunc.h"

class BaseObject
{
public:
    BaseObject();
    ~BaseObject();
    void SetRect (const int& x , const int& y) {rect_.x = x , rect_.y = y; }
    SDL_Rect GetRect() const {return rect_;} //dinh nghia 1 hcn
    SDL_Texture* GetObject() const {return p_object_;} //lay ra object

    virtual bool LoadImg (std::string path , SDL_Renderer* screen);              //load anh
    void Render (SDL_Renderer* des ,  const SDL_Rect* clip = NULL);      //tao anh
    void Free ();
protected:
    SDL_Texture* p_object_;
    SDL_Rect rect_;         //luu kich thuoc

};

#endif // BASEOBJECT_H
