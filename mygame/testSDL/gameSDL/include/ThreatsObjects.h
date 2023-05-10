#ifndef THREATS_OBJECTS_H_
#define THREATS_OBJECTS_H_

#include"BaseObject.h"
#include"BulletObject.h"
#include<bits/stdc++.h>

#define THREAT_FRAME_NUM 8
#define THREAT_GARVITY_SPEED 0.8
#define THREAT_MAX_FALL_SPEED 10
#define THREAT_SPEED 3

class ThreatsObjects : public BaseObject
{
    public:
        ThreatsObjects();
        ~ThreatsObjects();
        enum TypeMove
        {
            STATIC_THREAT = 0 , //đứng yên
            MOVE_IN_SPACE_THREAT = 1 , //đi trong khoảng a b
        };
        enum WalkType   //ham dinh nghia 1 bien
        {
            WALK_RIGHT = 0,
            WALK_LEFT = 1,
        };

        void set_x_val(const float& xVal) {x_val_ = xVal;}
        void set_y_val(const float& yVal) {y_val_ = yVal;}

        void set_x_pos(const float& xp) {x_pos_ = xp;}
        void set_y_pos(const float& yp) {y_pos_ = yp;}

        float get_x_pos() const {return x_pos_;}
        float get_y_pos() const {return y_pos_;}

        void SetMapXY (const int& mp_x , const int& mp_y )
        {
            map_x_ = mp_x;
            map_y_ = mp_y;
        }
        void set_clips();
        bool LoadImg (std::string path , SDL_Renderer* screen);
        void Show (SDL_Renderer* des);

        int get_width_frame () const {return width_frame_;}
        int get_height_frame () const {return height_frame_;}

        void DoPlayer(Map& gMap);
        void CheckToMap (Map& gMap);

        void InitThreats(); //khởi tạo threat

        void RemoveBullet (const int& idx); // xoa dan khi da ban trung

        void set_type_move (const int& typeMove) {type_move_ = typeMove;}

        void SetAnimationPos (const int& pos_a , const int& pos_b)
        {animation_a_ = pos_a ;animation_b_ = pos_b;}
        //set pos ,move
        void set_input_left (const int& ipLeft) {input_type_.left_ = ipLeft;}

        void ImpMoveType (SDL_Renderer* screen, Map& gMap); //xu ly di chuyen trong 1 khoang
        SDL_Rect GetRectFrame();

        std::vector<BulletObject*> get_bullet_list() const {return bullet_list_;}
        //tạo băng đạn
        void set_bullet_list(const std::vector<BulletObject*>& bl_list)
        {
            bullet_list_ = bl_list;
        }
        //khoi tao thong so bullet
        void InitBullet(BulletObject* p_bullet , SDL_Renderer* screen);
        void MakeBullet(SDL_Renderer* screen , const int& x_limit , const int& y_limit);

    private:
        int frame_;
        int width_frame_;
        int height_frame_;
        SDL_Rect frame_clip_[THREAT_FRAME_NUM];
        bool on_ground_;
        int come_back_time_;
        float x_pos_;
        float y_pos_;
        float x_val_;
        float y_val_;
        int map_x_;
        int map_y_;
        int type_move_;
        int animation_a_; //left
        int animation_b_; //right
        Input input_type_; //static hay move
        int dir_threats_;
        std::vector<BulletObject*> bullet_list_;

};

#endif // THREATSOBJECTS_H






















