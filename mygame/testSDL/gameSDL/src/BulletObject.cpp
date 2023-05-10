#include "BulletObject.h"

BulletObject::BulletObject()
{
    //ctor
    x_val_ = 0;
    y_val_ = 0;
    is_move_ = false;
    bullet_type_ = SPHERE_BULLET;
}

BulletObject::~BulletObject()
{
    //dtor
}

//load 2 loại đạn
bool BulletObject:: LoadImgBullet (SDL_Renderer* des)
{
    bool ret = false;
    if (bullet_type_ == LASER_BULLET)
    {
        ret = LoadImg("img//laser_bullet.png" , des );
    }
    else if (bullet_type_ == SPHERE_BULLET)
    {
        ret = LoadImg("img//sphere_bullet.png" , des );
    }
    else if (bullet_type_ == LIL_BULLET)
    {
        ret = LoadImg("img//lil_bullet.png" , des );
    }
    return ret;
}

void BulletObject :: HandleMove (const int& x_border , const int& y_border)
{

    if (bullet_dir_ == DIR_RIGHT) //đạn bắn sang phải
    {
        rect_.x += x_val_ ; //dan di sang phai

        if (rect_.x > x_border ) //khi di ra khoi map
        {
            is_move_ = false;
        }
    }
    else if (bullet_dir_ == DIR_LEFT) //đạn bắn sang trái
    {
        rect_.x -= x_val_;
        if (rect_.x < 0)
        {
            is_move_ =false;
        }
    }
}


void BulletObject :: HandleMovePlayer (const int& x_border , const int& y_border)
{

    if (bullet_dir_ == DIR_RIGHT) //đạn bắn sang phải
    {
        rect_.x += x_val_ ; //dan di sang phai

        if (rect_.x > x_border*0.5+250 ) //khi di ra khoi map
        {
            is_move_ = false;
        }
    }
    else if (bullet_dir_ == DIR_LEFT) //đạn bắn sang trái
    {
        rect_.x -= x_val_;
        if (rect_.x < 0)
        {
            is_move_ =false;
        }
    }
}



















