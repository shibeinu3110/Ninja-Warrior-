#include "ThreatsObjects.h"


ThreatsObjects::ThreatsObjects()
{
    //ctor
    width_frame_ = 0;
    height_frame_ = 0;
    x_val_ = 0.0;
    y_val_ = 0.0;
    x_pos_ = 0.0;
    y_pos_ = 0.0;
    on_ground_ = 0; //0
    come_back_time_ = 0;
    frame_ =0;

    animation_a_ = 0;
    animation_b_ = 0;
    input_type_.left_ = 0; //luon quay sang trai
    type_move_ = STATIC_THREAT;
}

ThreatsObjects::~ThreatsObjects()
{
    //dtor
}

bool ThreatsObjects :: LoadImg (std::string path , SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg (path , screen);
    if (ret) //true
    {
        width_frame_ = rect_.w / THREAT_FRAME_NUM;
        height_frame_ = rect_.h;
    }
    return ret;
}

SDL_Rect ThreatsObjects::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;

    return rect;
}

void ThreatsObjects :: set_clips()
{
    if (width_frame_ > 0 && height_frame_ > 0)
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


void ThreatsObjects :: Show (SDL_Renderer* des)
{
    if (come_back_time_ == 0)
    {
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_ - map_y_;
        frame_++; //luon tang frame
        if (frame_ >= 8)
        {
            frame_ = 0;
        }

        SDL_Rect* currentClip = &frame_clip_[frame_];
        SDL_Rect rendQuad = {rect_.x , rect_.y ,width_frame_ , height_frame_};
        SDL_RenderCopy (des , p_object_ , currentClip , &rendQuad);
    }
}

void ThreatsObjects:: DoPlayer (Map& gMap)
{
    if (come_back_time_ == 0)
    {
        x_val_ = 0;
        y_val_ += THREAT_GARVITY_SPEED ;
        if (y_val_ >= THREAT_MAX_FALL_SPEED)
        {
            y_val_ = THREAT_MAX_FALL_SPEED;
        }

        if (input_type_.left_ ==1)
        {
            x_val_ -= THREAT_SPEED;
        }
        else if (input_type_.right_ == 1)
        {
            x_val_ += THREAT_SPEED;
        }

        CheckToMap(gMap);
    }
    else if (come_back_time_ >0)
    {
        come_back_time_ --;
        if (come_back_time_ == 0)
        {
            InitThreats();
        }
    }
}

void ThreatsObjects:: InitThreats()
{
    x_val_ =0;
    y_val_ =0;
    if (x_pos_ > 256)
    {
        x_pos_ -=256;
        animation_a_ -=256;
        animation_b_ -=256;
    }
    else
    {
        x_pos_ =0;
    }
    y_pos_ =0;
    come_back_time_ = 0;
    input_type_.left_ =1;
}

void ThreatsObjects::RemoveBullet (const int& idx)
{
    int size = bullet_list_.size();
    if (size > 0 && idx < size)
    {
        BulletObject* p_bullet = bullet_list_.at(idx);
        bullet_list_.erase(bullet_list_.begin() + idx); //loai bo vien dan thu i


        if (p_bullet )
        {
            delete p_bullet; //xoa bo nho vien dan
            p_bullet = NULL;
        }

    }
}

void ThreatsObjects :: CheckToMap (Map& gMap)
{
    int x1 =0 ;
    int x2 =0 ;

    int y1 =0 ;
    int y2 =0 ;

    // kiem tra theo chieu ngang (truc Ox)

    int height_min = height_frame_ < TILE_SIZE ? height_frame_ :TILE_SIZE;  // neu heightfram be hon thi lay chinh no
                                                                            //con khong thi lay tile size
    x1 = (x_pos_ + x_val_) / TILE_SIZE; //kiem tra dang xem nhan vat dang dung o o bao nhieu
    x2 = (x_pos_ + x_val_ + width_frame_ -1) / TILE_SIZE ; // -1 de tach nhan vat va vien cua tile map

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
    if (x1 >=0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (x_val_ > 0) //dang di chuyen sang phai
        {
            int val1 = gMap.tile[y1][x2];
            int val2 = gMap.tile[y2][x2];

            if ((val1 != BLANK_TILE && val1 !=STATE_MONEY) || (val2!= BLANK_TILE && val2!=STATE_MONEY))
            //dang o 1 o co hinh anh ( hinh anh la 1)
            {
                x_pos_ = x2*TILE_SIZE;
                x_pos_ -=(width_frame_ +1);
                // tranh va cham voi tile map
                x_val_ =0;
            }
        }
        else if (x_val_ < 0) //di lui
        {
            int val1 = gMap.tile[y1][x1];
            int val2 = gMap.tile[y2][x1];

            if ((val1 != BLANK_TILE && val1 !=STATE_MONEY) || (val2!= BLANK_TILE && val2!=STATE_MONEY))
            {
                x_pos_ = (x1+1) * TILE_SIZE;
                x_val_ =0;
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
            int val1 = gMap.tile[y2][x1];
            int val2 = gMap.tile[y2][x2];

            if ((val1 != BLANK_TILE && val1 !=STATE_MONEY) || (val2!= BLANK_TILE && val2!=STATE_MONEY))
            //dang o 1 o co hinh anh ( hinh anh la 1)
            {
                y_pos_  =y2*TILE_SIZE;
                y_pos_ -=(height_frame_ +1);
                // tranh va cham voi tile map
                y_val_ =0;
                on_ground_ =true; //dang tren mat dat
            }
        }
        else if (y_val_ < 0)
        {
            int val1 = gMap.tile[y1][x1];
            int val2 = gMap.tile[y1][x2];
            if ((val1 != BLANK_TILE && val1 !=STATE_MONEY) || (val2!= BLANK_TILE && val2!=STATE_MONEY))
            {
                y_pos_ = (y1+1) * TILE_SIZE;
                y_val_ =0;
            }
        }
    }
    x_pos_ +=x_val_;
    y_pos_ +=y_val_;

    if (x_pos_ < 0)
    {
     x_pos_ =0 ; //khong cho lui nua
    }

    else if ( x_pos_ +width_frame_ > gMap.max_x_ )
    {
        x_pos_ = gMap.max_x_ -width_frame_ -1;
    }
    if (y_pos_ >gMap.max_y_) //khi roi qua sau
    {
        come_back_time_ = 60 ; //do tre thoi gian
    }
}

void ThreatsObjects:: ImpMoveType(SDL_Renderer* screen)
{
    if (type_move_ == STATIC_THREAT)
    {
        //khong lam gi
    }
    else
    {
        if (on_ground_ == true)
        {
            if (x_pos_ > animation_b_ +100)
            {
                input_type_.left_ =1; //doi chieu
                input_type_.right_ = 0;
                LoadImg("img//threat_left.png" , screen);
            }
            else if (x_pos_ < animation_a_-100)
            {
                input_type_.left_ = 0;
                input_type_.right_ =1 ; //doi chieu sang phai
                LoadImg("img//threat_right.png" , screen);
            }
        }
        else
        {
            if (input_type_.left_ ==1 )
            {
                LoadImg("img//threat_left.png" , screen);
            }
        }
    }
}

void ThreatsObjects::InitBullet (BulletObject* p_bullet , SDL_Renderer* screen)
{
    if (p_bullet != NULL)
    {
      p_bullet->set_bullet_type(BulletObject::LASER_BULLET);
      bool ret = p_bullet->LoadImgBullet(screen);

      if (ret ==true)
      {
        int x_th = x_pos_ - map_x_;
        int y_th = y_pos_ - map_y_;

        p_bullet->set_is_move(true);
        p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
        p_bullet->SetRect(x_th + 5 , y_th + 10);
        p_bullet->set_x_val(15); //toc do dan
        bullet_list_.push_back(p_bullet);
      }
    }
}

void ThreatsObjects::MakeBullet(SDL_Renderer* screen , const int& x_limit , const int& y_limit)
{
    for (int i=0;i<bullet_list_.size() ; i++)
    {
        BulletObject* p_bullet = bullet_list_.at(i);
        if (p_bullet != NULL)
        {
            if (p_bullet->get_is_move() == true)
            {
                int bullet_distance = rect_.x - p_bullet->GetRect().x +width_frame_;
                if (bullet_distance < 300 && bullet_distance > 0) //trong khoang 300 moi ban
                {
                    p_bullet->HandleMove (x_limit , y_limit);
                    p_bullet->Render(screen);
                }
                else
                {
                    p_bullet->set_is_move(false); //k ban nua
                }
            }
            else
            {
                int x_th = x_pos_ - map_x_;
                int y_th = y_pos_ - map_y_;

                p_bullet->set_is_move (true);
                p_bullet->SetRect(x_th + 5 ,  y_th + 10);
            }
        }
    }
}

























