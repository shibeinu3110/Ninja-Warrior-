#include "MainObject.h"
#include <iostream>
#include "BaseObject.h"

MainObject::MainObject()
{
    //ctor
    frame_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    x_val_ = 0;
    y_val_ = 0;
    width_frame_ = 0;
    height_frame_ =0;
    input_type_.left_ =0;
    input_type_.right_ =0;
    input_type_.jump_ =0;
    input_type_.down_ =0;
    input_type_.up_ =0;
    on_ground_ = false ;
    map_x_ = 0;
    map_y_ = 0;
    come_back_time_ = 0;
    money_count = 0;

    status_ = WALK_RIGHT;
    mute_ = false;
}

MainObject::~MainObject()
{
    //dtor

}

bool MainObject :: LoadImg (std::string path , SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg (path , screen); //dùng lại hàm loadimg của base object

    if (ret == true)
    {
        width_frame_ = rect_.w / 8; //rect là chiều dài của 1 ảnh real ( 8 là số frame của ảnh)
        height_frame_ = rect_.h ;

    }
    return ret;
}

SDL_Rect MainObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;
    return rect;
}

void MainObject :: set_clips() //tăng số frame liên tục để tạo animation
{
    //dùng để cắt ảnh real ra các frame tạo animation
    if (width_frame_>0 && height_frame_ >0)
    {
        frame_clip_[0].x= 0;
        frame_clip_[0].y= 0;
        frame_clip_[0].w= width_frame_;
        frame_clip_[0].h= height_frame_;

        frame_clip_[1].x= width_frame_;
        frame_clip_[1].y= 0;
        frame_clip_[1].w= width_frame_;
        frame_clip_[1].h= height_frame_;

        frame_clip_[2].x= 2 * width_frame_;
        frame_clip_[2].y= 0 ;
        frame_clip_[2].w= width_frame_;
        frame_clip_[2].h= height_frame_;

        frame_clip_[3].x= 3 * width_frame_;
        frame_clip_[3].y= 0;
        frame_clip_[3].w= width_frame_;
        frame_clip_[3].h= height_frame_;

        frame_clip_[4].x= 4 * width_frame_;
        frame_clip_[4].y= 0;
        frame_clip_[4].w= width_frame_;
        frame_clip_[4].h= height_frame_;

        frame_clip_[5].x= 5 * width_frame_;
        frame_clip_[5].y= 0;
        frame_clip_[5].w= width_frame_;
        frame_clip_[5].h= height_frame_;

        frame_clip_[6].x= 6 * width_frame_;
        frame_clip_[6].y= 0;
        frame_clip_[6].w= width_frame_;
        frame_clip_[6].h= height_frame_;

        frame_clip_[7].x= 7 * width_frame_;
        frame_clip_[7].y= 0;
        frame_clip_[7].w= width_frame_;
        frame_clip_[7].h= height_frame_;

    }
}

void MainObject :: Show (SDL_Renderer* des)
{
    UpdateImagePlayer (des);


    if (input_type_.left_ == 1 || input_type_.right_ == 1) //khi ấn nút
    {
        frame_++;
    }
    else
    {
        frame_=0;
    }
    if (frame_ >=8) //khi frame >=8 reset lai frame =0 de chay loop nếu không sẽ mất ảnh
    {
        frame_ = 0;
    }
    if (come_back_time_ == 0) //khi roi se khong load anh nua
    {


    rect_.x = x_pos_ - map_x_; //trừ đi map để giữ nhân vật ở trung tâm
    rect_.y = y_pos_ - map_y_;

    SDL_Rect* current_clip = &frame_clip_[frame_]; //tra ra frame tuong ung

    SDL_Rect  renderQuad = {rect_.x , rect_.y , width_frame_ , height_frame_}; //tao ra kich thuoc chuan cho 1 frame

    SDL_RenderCopy (des , p_object_ , current_clip , &renderQuad );
    // copy current clip vào renderQuad
    }
}

void MainObject :: HandleInputAction(SDL_Event events , SDL_Renderer* screen, Mix_Chunk* player_sound[10] )
{
    if (events.type == SDL_KEYDOWN)
    {
        switch (events.key.keysym.sym) //switch key
        {
        case SDLK_RIGHT: //khi an nut
            {
                status_ = WALK_RIGHT;
                input_type_.right_ = 1; //neu phim nao nhan trc thi phim con lai se bang 0
                input_type_.left_ =0;

                UpdateImagePlayer (screen);
                if (on_ground_ == true && mute_ == false)
                {
                    Mix_PlayChannel(-1, player_sound[6],0);
                    Mix_PlayChannel(-1,player_sound[7], 0);
                }
            }
            break;
        case SDLK_LEFT ://khi an nut
            {
                status_ = WALK_LEFT;
                input_type_.left_ = 1;
                input_type_.right_ =0;

                UpdateImagePlayer (screen);

                if (on_ground_ == true && mute_ == false)
                {
                    Mix_PlayChannel(-1, player_sound[6],0);
                    Mix_PlayChannel(-1,player_sound[7], 0);
                }
            }
            break;

        }
    }
    else if (events.type == SDL_KEYUP) //nhả phím
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT:
            {
                status_ = WALK_RIGHT;
                input_type_.right_ = 0;
            }
            break;
        case SDLK_LEFT:
            {
                status_ = WALK_LEFT;
                input_type_.left_ = 0;
            }
            break;

        }
    }

    //setting jump

    if ( events.type == SDL_KEYDOWN ) // khi an nut
    {

        if ( events.key.keysym.sym == SDLK_UP)
        {
            input_type_.jump_ =1; //xu ly nhay
            if (mute_ == false )
            {
                Mix_PlayChannel (-1, player_sound[4] , 0);
            }
        }
        int dem = 0;
        if (events.key.keysym.sym == SDLK_m)
        {
            mute_ = true;
        }
        if (events.key.keysym.sym == SDLK_u)
        {
            mute_ = false;
        }
    }
    //setting bullet
    if (events.type  == SDL_KEYDOWN)
    {
        if (events.key.keysym.sym == SDLK_SPACE) //xu ly ban
        {
            BulletObject* p_bullet = new BulletObject() ;
            p_bullet->set_bullet_type (BulletObject::SPHERE_BULLET);
            p_bullet->LoadImg ("img//sphere_bullet.png" , screen);
            //tạo viên đạn và nạp ảnh viên đạn vào băng

            int x_player = x_pos_ - map_x_; //vị trí nhân vật theo chiều x và y
            int y_player = y_pos_ - map_y_;

            if (status_ == WALK_LEFT)
            {
                p_bullet->set_bullet_dir (BulletObject::DIR_LEFT); //set hướng bắn
                p_bullet->SetRect (x_player , y_player + height_frame_*0.3); //set vị trsi bắn cho đạn
            }
            else if (status_== WALK_RIGHT)
            {
                p_bullet->set_bullet_dir (BulletObject::DIR_RIGHT);
                p_bullet->SetRect (x_player + width_frame_-15 , y_player + height_frame_*0.3);
            }



            p_bullet->set_x_val(15); //toc do dan
            p_bullet->set_y_val(20);

            p_bullet->set_is_move(true);

            p_bullet_list_.push_back(p_bullet);
            //nhập từng viên đạn vào băng đạn
            if (mute_ == false)
            {
                Mix_PlayChannel(-1, player_sound[8], 0); // loop bat dau tu 0
            }
        }
    }
}

void MainObject:: HandleBullet (SDL_Renderer* des)
{

    for (int i=0 ; i< p_bullet_list_.size() ; i++)
    {
        BulletObject* p_bullet = p_bullet_list_.at(i);
        if (p_bullet != NULL)
        {
            if (p_bullet->get_is_move() == true)
            {
                p_bullet->HandleMovePlayer(SCREEN_WIDTH , SCREEN_HEIGHT); //di chuyen vien dan

                p_bullet->Render (des);
            }
            else
            {
                p_bullet_list_.erase(p_bullet_list_.begin()+i);
                //hủy viên đạn khi nó ra khỏi màn hình
                if (p_bullet != NULL)
                {
                    delete p_bullet;
                    p_bullet = NULL;
                }
            }
        }
    }
}

void MainObject::RemoveBullet (const int& idx) //xóa viên đạn thứ idx
{
    int size = p_bullet_list_.size();
    if (size > 0 && idx < size) //viên đạn k đc vượt quá size
    {
        BulletObject* p_bullet = p_bullet_list_.at(idx);
        p_bullet_list_.erase(p_bullet_list_.begin() + idx); //loai bo vien dan thu idx


        if (p_bullet )
        {
            delete p_bullet; //xoa bo nho vien dan
            p_bullet = NULL;
        }

    }
}

void MainObject :: DoPlayer (Map& map_data , Mix_Chunk* player_sound[10]) //xử lý di chuyển nhân vật
{
    if (come_back_time_ == 0) //neu nhu khong roi
    {
        x_val_ = 0;
        y_val_ += 0.8; //toc do roi tu do cua nhan vat;

    if (y_val_ >= MAX_FALL_SPEED)
    {
        y_val_ = MAX_FALL_SPEED;
    }
    if (input_type_.left_ == 1 ) //khi bam sang trai
    {
        x_val_ -= PLAYER_SPEED; //di chuyen lui
    }
    else if (input_type_.right_ == 1 ) //khi bam sang phai
    {
        x_val_ += PLAYER_SPEED; //di chuyen tien
    }
    // setting jump
    if (input_type_.jump_ == 1) //khi nhay len
    {
        if (on_ground_ == true)
        {
            y_val_ = -PLAYER_JUMP_VAL ;

        }
        on_ground_ = false ;
        input_type_.jump_ = 0; //khi nhay xong thi phai dung
    }

    CheckToMap (map_data, player_sound);
    CenterEntityOnMap (map_data);
    }

    if (come_back_time_ > 0)
    {

        come_back_time_ --; //dem nguoc thoi gian de respone
        if (come_back_time_ == 0)
        {
            if (x_pos_ >256)
            {
                x_pos_ -= 256;
            }
            /*else if (x_pos_ <= 256)
            {
                x_pos_ =0 ;
            }*/  //không cần thiết lắm
            y_pos_ = 0;
            x_val_ = 0;
            y_val_ = 0;
        }
    }
}

void MainObject :: CenterEntityOnMap (Map& map_data)
{
    //map_data.star_x_ là vị trí đầu tiên của map
    map_data.start_x_ = x_pos_ - (SCREEN_WIDTH/2);
    //khi nhân vật di chuyển đến giữa bản đồ thì map mới di chuyển theo
    if (map_data.start_x_ < 0) //khi kịch khung sau của map thì không cho lùi nữa
    {
        map_data.start_x_ = 0;
    }
    else if (map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_) //khi di het map
    {
        map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH;
    }

    map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT);
    if (map_data.start_y_ < 0)
    {
        map_data.start_y_ = 0;
    }
    else if (map_data.start_x_ + SCREEN_HEIGHT >= map_data.max_y_) //khi di het map
    {
        map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT ;
    }

}

void MainObject :: CheckToMap (Map& map_data , Mix_Chunk* player_sound[10]) //kiem tra va cham
{
    //kiểm tra từ a đến b theo chiều x
    int x1 =0 ;
    int x2 =0 ;

    //kiểm tra từ a đến b theo chiều y
    int y1 =0 ;
    int y2 =0 ;

    // kiem tra theo chieu ngang (truc Ox)

    int height_min = height_frame_ < TILE_SIZE ? height_frame_ :TILE_SIZE;  // neu heightfram be hon thi lay chinh no
                                                                            //con khong thi lay tile size

    //x_pos là vị trí hiện tại
    //x_val là cộng thêm sau mỗi lần di chuyển
    x1 = (x_pos_ + x_val_) / TILE_SIZE; //kiem tra dang xem nhan vat dang dung o o bao nhieu
    x2 = (x_pos_ + x_val_ + width_frame_ -1) / TILE_SIZE ; // -1 de tach nhan vat va vien cua tile map
    // ta trừ 1 để bớt đi viền của nhân vật từ đó check va chạm nhân vật và tile sẽ chuẩn hơn ( không trừ cũng đc)
    y1 = (y_pos_) /TILE_SIZE ;
    y2 = (y_pos_ + height_min -1 ) /TILE_SIZE ;
    /*
    x1y1.........x2y1
    .................
    .................
    .................
    x1y2.........x2y2
    */
    //kiem tra nhan vat co trong banr do hay khong
    if (x1 >=0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) //đang ở trong bản đồ
    {
        if (x_val_ > 0) //dang di chuyen sang phai
        {
            int val1 = map_data.tile[y1][x2];
            int val2 = map_data.tile[y2][x2];
            if (val1 == STATE_MONEY || val2 == STATE_MONEY) //la hinh coin
            {
                //bien mat khi an
                map_data.tile[y1][x2] = 0;
                map_data.tile[y2][x2] = 0;
                if (mute_ == false)
                {
                    Mix_PlayChannel(-1, player_sound[2],0);
                }
                increase_money();
            }
            else
            {
                if (val1 != BLANK_TILE || val2!= BLANK_TILE)
                //dang o 1 o co hinh anh ( hinh anh la 1)
                {
                    x_pos_ = x2*TILE_SIZE;
                    x_pos_ -=(width_frame_ +1);

                    // tranh va cham voi tile map
                    x_val_ =0; //không cho di chuyển nữa
                }
            }
        }
        else if (x_val_ < 0) //di lui
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];
            if (val1 == STATE_MONEY || val2 == STATE_MONEY) //la hinh coin
            {
                //bien mat khi an
                map_data.tile[y1][x1] = 0;
                map_data.tile[y2][x1] = 0;
                increase_money();
                if (mute_ == false)
                {
                    Mix_PlayChannel(-1, player_sound[2],0);
                }
            }
            else
            {
               if (val1 != BLANK_TILE || val2!= BLANK_TILE)
                {
                    x_pos_ = (x1+1) * TILE_SIZE;
                    x_val_ =0;
                }
            }
        }
    }


    // kiem tra theo chieu doc (truc Oy)
    int width_min = width_frame_ < TILE_SIZE ? width_frame_ :TILE_SIZE;
    x1 = (x_pos_) / TILE_SIZE;
    x2 = (x_pos_ + width_min ) / TILE_SIZE ;

    y1 = (y_pos_ + y_val_) /TILE_SIZE;
    y2 = (y_pos_ + height_frame_ -1 + y_val_ ) /TILE_SIZE ;

    if (x1 >=0 && x2 <MAX_MAP_X && y1 >=0 && y2< MAX_MAP_Y)
    {
        if (y_val_ > 0)
        {
            int val1 = map_data.tile[y2][x1];
            int val2 = map_data.tile[y2][x2];
            if (val1 == STATE_MONEY || val2 == STATE_MONEY) //la hinh coin
            {
                //bien mat khi an
                map_data.tile[y2][x1] = 0;
                map_data.tile[y2][x2] = 0;
                increase_money();
                if (mute_ == false)
                {
                    Mix_PlayChannel(-1, player_sound[2],0);
                }
            }
            else
            {
                if (val1 != BLANK_TILE || val2!= BLANK_TILE)
            //dang o 1 o co hinh anh ( hinh anh la 1)
                {
                    y_pos_  =y2*TILE_SIZE;
                    y_pos_ -=(height_frame_ +1);
                    // tranh va cham voi tile map
                    y_val_ =0;
                    on_ground_ =true; //dang tren mat dat
                }
            }
        }
        else if (y_val_ < 0)
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];
            if (val1 == STATE_MONEY || val2 == STATE_MONEY) //la hinh coin
            {
                //bien mat khi an
                map_data.tile[y1][x1] = 0;
                map_data.tile[y1][x2] = 0;
                increase_money();
                if (mute_ == false)
                {
                    Mix_PlayChannel(-1, player_sound[2],0);
                }
            }
            else
            {
                if (val1 != BLANK_TILE || val2!= BLANK_TILE)
                {
                    y_pos_ = (y1+1) * TILE_SIZE;
                    y_val_ =0;
                }
            }
        }
    }
    x_pos_ +=x_val_;
    y_pos_ +=y_val_;

    if (x_pos_ < 0)
    {
     x_pos_ =0 ; //khong cho lui nua
    }

    else if ( x_pos_ +width_frame_ > map_data.max_x_ )
    {
        x_pos_ = map_data.max_x_ -width_frame_ -1;
    }
    if (y_pos_ >map_data.max_y_) //khi roi qua sau
    {
        come_back_time_ = 60 ; //ms
        die++; //do tre thoi gian
    }
}

void MainObject::increase_money ()
{
    money_count++;
}



void MainObject :: UpdateImagePlayer (SDL_Renderer* des) //giam viec load anh
{
    Free() ; //xoa baseobject de giai phong bo nho
    if (on_ground_ == true )
    {
        if (status_ == WALK_LEFT ) //dang di sang trai
        {
            LoadImg("img//player_left.png" , des);
        }

        else
        {
            LoadImg("img//player_right.png" , des);
        }
    }
    else
    {
        if (status_ == WALK_LEFT)
        {
            LoadImg("img//jump_left.png" , des);
        }
        else
        {
            LoadImg("img//jump_right.png" , des);
        }
    }
}
























































