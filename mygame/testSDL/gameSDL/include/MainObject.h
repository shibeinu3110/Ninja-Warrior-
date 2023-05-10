#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_

#include <vector>

#include "BaseObject.h"
#include "BulletObject.h"


#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 10
#define PLAYER_JUMP_VAL 18

class MainObject : public BaseObject //ke thua lop base object
{
    public:
        MainObject();
        ~MainObject();

        enum WalkType   //ham dinh nghia 1 bien
        {
            WALK_RIGHT = 0,
            WALK_LEFT = 1,
        };
        bool LoadImg (std::string path , SDL_Renderer* screen);
        void Show(SDL_Renderer* des); //show và tang so frame
        void HandleInputAction (SDL_Event events , SDL_Renderer* screen , Mix_Chunk* player_sound[10] ); //xu ly du lieu bam nut
        void set_clips() ; //tạo hiệu ứng clip cho frame

        void DoPlayer (Map& map_data , Mix_Chunk* player_sound[10]); //roi tu do nhan vat va di chuyen
        //truyền vào bản đồ để tìm ra va chạm
        void CheckToMap (Map& map_data , Mix_Chunk* player_sound[10]); //va cham nhan vat va ban do
        void SetMapXY (const int map_x , const int map_y) {map_x_ = map_x ; map_y_ = map_y;}
        //map_x_ và map_y_ lưu vị trí bản đồ
        void CenterEntityOnMap (Map& map_data); //luon de nhan vat o giua man hinh
        void UpdateImagePlayer (SDL_Renderer* des);
        SDL_Rect GetRectFrame(); //lay khung hinh nhan vat

        //tạo 1 băng đạn
        void set_bullet_list (std::vector<BulletObject*> bullet_list)
        {
            p_bullet_list_ = bullet_list;

        }
        //xử lý băng đạn
        std::vector<BulletObject*> get_bullet_list () const {return p_bullet_list_;}

        void HandleBullet (SDL_Renderer* des); //xử lý đạn

        void RemoveBullet (const int& idx); // xoa dan khi da ban trung
        void increase_money();
        void set_comeback_time(const int& cb_time) {come_back_time_ = cb_time;}
        int GetMoneyCount() const {return money_count;}
        int die =0 ;
        bool get_mute() const {return mute_;};
    private:
        int money_count;
        std::vector<BulletObject*> p_bullet_list_; //khoi tao bang dan
        float x_val_; //luong thay doi khi di chuyen nhan vat (tọa độ )
        float y_val_;

        float x_pos_; //vị trí hiện tại để show nhân vật
        float y_pos_;

        int width_frame_; //luu giu kich thuoc 1 frame
        int height_frame_;

        SDL_Rect frame_clip_[8]; //luu giu cac frame
        Input input_type_;       //luu trang thai trái phải lên xuống
        int frame_;              //*số frame ( tăng theo thời gian)
        int status_;             //luu huong chuyen dong sang phai hay sang trai
        bool on_ground_;

        int map_x_;
        int map_y_;
        int come_back_time_; //thời gian rơi để trở lại
        bool mute_;

};

#endif





























