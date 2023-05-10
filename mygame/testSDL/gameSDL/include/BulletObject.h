#ifndef BULLET_OBJECT_H_
#define BULLET_OBJECT_H_

#include "BaseObject.h"


class BulletObject : public BaseObject  //ke thua lop base object
{
    public:
        BulletObject();
        ~BulletObject();

        enum BulletDir //cac huong dan ban
        {
            DIR_RIGHT = 20,
            DIR_LEFT = 21,
            DIR_UP = 22 ,
            DIR_UP_LEFT = 23,
            DIR_UP_RIGHT =24,
            DIR_DOWN_LEFT =25,
            DIR_DOWN_RIGHT =26,

        };

        enum BulletType  //loai dan ban
        {
            SPHERE_BULLET = 50,
            LASER_BULLET = 51,
            LIL_BULLET = 52,
        };

        void set_x_val (const int& xVal) {x_val_ = xVal;} //set x
        void set_y_val (const int& yVal) {y_val_ = yVal;} //set y
        int get_x_val() const {return x_val_;}
        int get_y_val() const {return y_val_;}

        void set_is_move (const bool isMove) {is_move_ = isMove;}
        //kiểm tra xem đạn có di chuyển không , nếu ra ngoài map thì xóa để đỡ tốn bộ nhớ
        bool get_is_move()  const {return is_move_ ;}


        void set_bullet_dir (const unsigned int& bulletDir) { bullet_dir_= bulletDir;}
        unsigned int get_bullet_dir()  const { return bullet_dir_;}

        void set_bullet_type (const unsigned int& bulletType) {bullet_type_ = bulletType;}
        unsigned int get_bullet_type () const {return bullet_type_;}

        void HandleMove (const int& x_border , const int& y_border);
        //xoa dan khi ra khoi map
        void HandleMovePlayer (const int& x_border , const int& y_border);
        bool LoadImgBullet(SDL_Renderer* des); //load bullet image

    private:
        int x_val_; //khi di chuyen
        int y_val_;
        bool is_move_; //xoa khi dan ra ngoai map
        unsigned int bullet_dir_;
        unsigned int bullet_type_;
};

#endif // BULLETOBJECT_H


































