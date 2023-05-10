//#include "CommonFunc.h"
#include "BaseObject.h"
#include <bits/stdc++.h>
#include <windows.h>
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatsObjects.h"
#include "ExplosionObject.h"
#include "TextObject.h"
#include "PlayerPower.h"
#include "Geometric.h"


BaseObject g_background ;
TTF_Font* font_time = NULL;
TTF_Font* font_menu = NULL;
TTF_Font* font_tuto = NULL;
using namespace std;

bool InitData ()
{

    bool success = true;
    int ret = SDL_Init (SDL_INIT_VIDEO);
    if (ret < 0)
        return false;

    SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "1");

    //tao cua so
    g_window = SDL_CreateWindow ("Game cua Shiba Inu " , SDL_WINDOWPOS_UNDEFINED ,SDL_WINDOWPOS_UNDEFINED ,
                                                        SCREEN_WIDTH , SCREEN_HEIGHT , SDL_WINDOW_SHOWN );

    if (g_window == NULL)
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer (g_window , -1 , SDL_RENDERER_ACCELERATED);
        if ( g_screen == NULL)
            success = false;
        else
        {
            SDL_SetRenderDrawColor (g_screen , RENDER_DRAW_COLOR , RENDER_DRAW_COLOR ,
                                    RENDER_DRAW_COLOR , RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                success = false;
        }
        if (TTF_Init() == -1)
        {
            success = false ;
        }

        font_time = TTF_OpenFont("font//dlxfont_.ttf" , 15 );
        font_menu = TTF_OpenFont("font//Haverbrooke.otf" , 40 );
        font_tuto = TTF_OpenFont("font//Keylock_Fighter.ttf" , 40 );

        if (font_time == NULL || font_menu == NULL)
        {
            success = false;
        }
    }
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        return false;
    }
    else
    {
        // read file wav audio
        g_sound_player[8] = Mix_LoadWAV("music//sound_gunShot.wav");

        g_sound_player[0] = Mix_LoadWAV("music//player_bullet_load.wav");
        g_sound_player[2] = Mix_LoadWAV("music//player_coin_eating.wav");

        g_sound_player[3] = Mix_LoadWAV("music//player_hurt.wav");
        g_sound_player[4] = Mix_LoadWAV("music//player_jump.wav");
        g_sound_player[5] = Mix_LoadWAV("music//player_landing.wav");
        g_sound_player[6] = Mix_LoadWAV("music//step1.wav");
        g_sound_player[7] = Mix_LoadWAV("music//step2.wav");

        g_sound_player[9] = Mix_LoadWAV("music//failed.wav");

        g_general_music[0] = Mix_LoadWAV("music//snd_danger.wav");


        g_sound_exp[0] = Mix_LoadWAV("music//exp.wav");
    }
    return success;
}

bool LoadBackground ()
{
    bool ret = g_background.LoadImg ("img//background.png" , g_screen);
    if (ret == false)
        return false;

    return true;

}

void close ()
{
    g_background.Free();

    SDL_DestroyRenderer (g_screen);
    g_screen = NULL;

    SDL_DestroyWindow (g_window);
    g_window = NULL;

    IMG_Quit ();
    SDL_Quit ();
}

std::vector<ThreatsObjects*> MakeThreadList()
{
    std::vector<ThreatsObjects*> list_threats;

    ThreatsObjects* dynamic_threats = new ThreatsObjects[20]; //quái di chuyển
    for (int i=0;i<20;i++)
    {
        ThreatsObjects* p_threat = (dynamic_threats + i);
        if (p_threat != NULL)
        {
            p_threat->LoadImg("img//threat_left.png", g_screen);
            p_threat->set_clips();
            p_threat->set_type_move(ThreatsObjects::MOVE_IN_SPACE_THREAT);
            p_threat->set_x_pos (500+ i*500);
            p_threat->set_y_pos (200);
            int pos1 = p_threat->get_x_pos() - 60;
            int pos2 = p_threat->get_x_pos() - 60;
            p_threat->SetAnimationPos(pos1 , pos2);
            p_threat->set_input_left(1);
            BulletObject* p_bullet = new BulletObject();

            p_bullet->set_bullet_type(BulletObject::LIL_BULLET);

            p_threat->InitBullet(p_bullet,g_screen);
            list_threats.push_back(p_threat);
        }
    }


    ThreatsObjects* threats_objs = new ThreatsObjects[20]; //quái đứng yên

    for (int i=0;i<20;i++)
    {
        ThreatsObjects* p_threat = (threats_objs + i); //chạy từng con quái
        if (p_threat!= NULL)
        {
            p_threat ->LoadImg ("img//threat_level.png" , g_screen);
            p_threat ->set_clips();
            p_threat ->set_x_pos(700 + i*1200); //random
            p_threat ->set_y_pos(250);
            p_threat ->set_type_move(ThreatsObjects :: STATIC_THREAT);

            BulletObject* p_bullet = new BulletObject();

            p_bullet->set_bullet_type(BulletObject::LASER_BULLET);

            p_threat->InitBullet(p_bullet,g_screen);

            list_threats.push_back(p_threat);
        }
    }
    return list_threats;
}

int main(int argc, char* argv[])
{
    ImpTimer fps_timer ; //set fps

    if (InitData() == false)
        return -1;

    if (LoadBackground() == false)
        return -1;

    GameMap game_map;
    game_map.LoadMap("map/map01.dat"); //truyen file luu trang thai cua 1 o
    game_map.LoadTiles (g_screen);



    MainObject p_player;
    p_player.LoadImg ("img//player_right.png" , g_screen); //load ảnh đầu tiên quay sang phải
    p_player.set_clips();

    PlayerPower player_power;
    player_power.Init(g_screen);

    PlayerMoney player_money;
    player_money.Init(g_screen);

    PlayerMusic player_music;
    player_music.InitUnmute(g_screen);

    std::vector<ThreatsObjects*> threats_list = MakeThreadList();

    //vu no quai vat
    ExplosionObject exp_threat ;
    bool tRet = exp_threat.LoadImg ("img//exp3.png" , g_screen);
    if (tRet == false)
    {
        return -1;
    }
    exp_threat.set_clip();

    //nhan vat no
    ExplosionObject exp_main ;
    bool mRet = exp_main.LoadImg ("img//exp3.png" , g_screen);
    exp_main.set_clip();
    if (mRet == false)
    {
        return -1;
    }

    int num_die = 0;

    //Time text
    TextObject time_game;
    time_game.SetColor(TextObject::WHITE_TEXT);

    TextObject mark_game;
    mark_game.SetColor(TextObject::WHITE_TEXT);

    UINT mark_value = 0 ;


    TextObject money_game;
    money_game.SetColor(TextObject::WHITE_TEXT);
    int t = 1;

    bool is_quit = false;

    //menu display
    int retMenu = Menu::ShowMenu(g_screen, font_menu);
    if (retMenu == 1)
    {
        is_quit = true;
    }
    else if (retMenu == 2)
    {
        while (t == 1)
            {
                int retTuto = Menu::ShowTutorial(g_screen, font_tuto);
                if(retTuto == 0)
                {
                    int retMenu1 = Menu::ShowMenu(g_screen, font_menu);
                    if (retMenu1 == 1)
                        {
                            is_quit = true;
                            t = 2;
                        }
                    else if(retMenu1 == 2)
                        {
                            t = 1;
                        }
                    else t = 2; // ấn play game r thì thoát vòng lặp
            }
        }
    }



    while (!is_quit)
    {
        fps_timer.start();

        while (SDL_PollEvent (&g_event) != 0) //hàng chờ bấm nút
        {
            if (g_event.type == SDL_QUIT)
            {
                is_quit = true;
            }

            p_player.HandleInputAction(g_event , g_screen, g_sound_player );
        }
        SDL_SetRenderDrawColor (g_screen , RENDER_DRAW_COLOR , RENDER_DRAW_COLOR ,
                                    RENDER_DRAW_COLOR , RENDER_DRAW_COLOR);
        SDL_RenderClear (g_screen);

        g_background.Render(g_screen , NULL); //render background
        //game_map.DrawMap (g_screen); // ve cac o
        Map map_data = game_map.getMap();

        p_player.HandleBullet (g_screen);
        p_player.SetMapXY (map_data.start_x_ , map_data.start_y_);
        p_player.DoPlayer(map_data , g_sound_player);

        p_player.Show (g_screen);

        //vẽ lại map sau khi nhân vật di chuyển thì map phải di chuyển theo
        game_map.SetMap (map_data); //set lai map
        game_map.DrawMap(g_screen); //ve lai map


        //draw geometric
        GeometricFormat rectangle_size ( 0 , 0 , SCREEN_WIDTH , 40) ;
        ColorData color_data (36,36,36);
        Geometric::RenderRectangle(rectangle_size , color_data , g_screen);

        GeometricFormat outlineSize (1,1,SCREEN_WIDTH-1 , 40);
        ColorData color_data2 (255,255,255);

        Geometric::RenderOutLine(outlineSize , color_data2 , g_screen );


        player_power.Show(g_screen);
        player_money.Show(g_screen);
        if (p_player.get_mute() == false)
        {
            player_music.InitUnmute(g_screen);
            player_music.Show(g_screen);
        }

        if (p_player.get_mute() == true)
        {
            player_music.InitMute(g_screen);
            player_music.Show(g_screen);
        }
        for (int i=0 ; i<threats_list.size() ; i++)
        {
            ThreatsObjects* p_threat = threats_list.at(i);
            if (p_threat != NULL)
            {
                p_threat->SetMapXY(map_data.start_x_ ,map_data.start_y_);
                p_threat->ImpMoveType (g_screen , map_data);
                p_threat->DoPlayer (map_data);
                p_threat->MakeBullet(g_screen , SCREEN_WIDTH ,SCREEN_HEIGHT);
                p_threat->Show(g_screen);

                SDL_Rect rect_player = p_player.GetRectFrame();
                bool bCol1 = false;
                std::vector <BulletObject*> tBullet_list = p_threat->get_bullet_list();
                for (int ii =0 ; ii < tBullet_list.size() ; ii++)
                {
                    BulletObject* pt_bullet = tBullet_list.at(ii);
                    if (pt_bullet)
                    {
                        //kiem tra va cham nhan vat va dan quái vật
                        bCol1 = SDLCommonFunc::CheckCollision(pt_bullet ->GetRect() , rect_player);
                        //dùng get rect khi chỉ có 1 frame
                        //dùng set clip khi có nhiều frame để tạo animation
                        if (bCol1) //va cham
                        {
                            if (p_player.get_mute() == false)
                            {
                                Mix_PlayChannel(-1, g_sound_player[3],0);
                            }
                            if (num_die > 3) //dan ban den luc chet
                            {
                                p_threat->RemoveBullet(ii); //xoa dan
                                break;
                            }
                        }
                    }
                }
                //kiem tra va chma main va threat
                SDL_Rect rect_threat = p_threat->GetRectFrame();
                bool bCol2 = SDLCommonFunc::CheckCollision(rect_player , rect_threat);
                if (bCol2 == true || bCol1 ==true || p_player.die != 0 )
                {


                    if (p_player.get_mute() == false)
                    {
                        Mix_PlayChannel(-1, g_sound_player[3],0);
                    }

                    int width_exp_frame = exp_main.get_frame_width();
                    int height_exp_frame = exp_main.get_frame_height();
                    for (int ex =0 ; ex < 8 ; ex++)
                            {
                                int x_pos2 = p_player.GetRect().x + exp_main.get_frame_width()*0.5 - width_exp_frame*0.5;
                                int y_pos2 = p_player.GetRect().y + exp_main.get_frame_height()*0.5 - height_exp_frame*0.5;

                                exp_main.set_frame(ex);
                                exp_main.SetRect(x_pos2 , y_pos2);
                                exp_main.Show(g_screen);
                                SDL_RenderPresent(g_screen);

                            }
                    if (p_player.die != 0)
                    {
                        num_die += p_player.die;
                        p_player.die = 0;
                    }
                    else
                    {
                        num_die++;
                    }

                    if (  num_die < 3)
                    {
                        p_player.SetRect( 0 , 0);
                        p_player.set_comeback_time(60);
                        SDL_Delay(800);
                        player_power.Decrease();
                        player_power.Render(g_screen);
                        continue;
                    }
                    else
                    {
                        //hien thong bao you die
                        //menu display
                        int retRestart = Menu::ShowRestart(g_screen, font_menu);
                        if (retRestart == 1)
                        {
                            is_quit = true;
                        }
                        else
                        {

                        }
                    }
                }
            }
        }
        int frame_exp_width = exp_threat.get_frame_width();
        int frame_exp_height = exp_threat.get_frame_height();

        std::vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
        for (int r=0; r < bullet_arr.size() ; r++)
        {
            BulletObject* p_bullet = bullet_arr.at(r);
            if (p_bullet!=NULL)
            {
                for (int t = 0 ; t < threats_list.size();t++)
                {
                    ThreatsObjects* obj_threat = threats_list.at(t);
                    if (obj_threat != NULL)
                    {
                        SDL_Rect tRect;
                        tRect.x = obj_threat->GetRect().x;
                        tRect.y = obj_threat->GetRect().y;
                        tRect.w = obj_threat->get_width_frame();
                        //do su dung frame
                        //neu k dung frame thi su dung setrect
                        tRect.h = obj_threat->get_height_frame();

                        SDL_Rect bRect = p_bullet->GetRect();

                        bool bCol = SDLCommonFunc::CheckCollision (bRect , tRect); //bullet and threat
                        //kiem tra va cham
                        if (bCol == true) //dan ban trung dich
                        {


                            if (p_player.get_mute() == false)
                        {
                            Mix_PlayChannel(-1, g_sound_exp[0],0);
                        }

                            mark_value += 10 ; //tăng điểm
                            for (int ex =0 ; ex<NUM_FRAME_EXP ; ex++) //chạy frame vụ nổ
                            {
                                int x_pos = p_bullet->GetRect().x - frame_exp_width*0.5;
                                int y_pos = p_bullet->GetRect().y - frame_exp_height*0.5;

                                exp_threat.set_frame(ex);
                                exp_threat.SetRect(x_pos , y_pos);
                                exp_threat.Show(g_screen);
                            }

                            p_player.RemoveBullet(r); //xóa đạn
                            obj_threat->Free(); //loai bo threats
                            threats_list.erase(threats_list.begin()+ t);
                        }
                    }
                }
            }
        }

        //Show game time

        std::string str_time = "TIME : ";
        Uint32 time_val = SDL_GetTicks() / 1000;
        Uint32 val_time = 300 - time_val;
        if (val_time <= 0)
        {
            if (MessageBox(NULL , "OVERTIME" , "Info" , MB_OK | MB_ICONSTOP) == IDOK )
            {
                is_quit = true ;
                break;
            }
        }
        else
        {
            std::string str_val = std::to_string(val_time);
            str_time += str_val;

            time_game.SetText(str_time);
            time_game.LoadFromRenderText (font_time , g_screen);
            time_game.SetRect(SCREEN_WIDTH - 200 , 15);
            time_game.RenderText(g_screen);
        }



        std::string val_str_mark = std::to_string (mark_value);
        std::string strMark("YOUR MARK: ");
        strMark += val_str_mark;

        mark_game.SetText(strMark);
        mark_game.LoadFromRenderText(font_time , g_screen);
        mark_game.SetRect(SCREEN_WIDTH*0.5-50 , 15);
        mark_game.RenderText(g_screen );



        int money_count = p_player.GetMoneyCount();

        std::string money_str = std::to_string (money_count);

        money_game.SetText(money_str);
        money_game.LoadFromRenderText(font_time , g_screen);
        money_game.SetRect(SCREEN_WIDTH*0.5-250 , 15);
        money_game.RenderText(g_screen);

        SDL_RenderPresent (g_screen);

        int real_imp_time =fps_timer.get_ticks(); //thoi gian thuc da troi qua
        int time_one_frame = 1000/FRAME_PER_SECOND; //1000ms; //thoi gian 1 frame

        if (real_imp_time < time_one_frame ) //thoi gian qua cham && di chuyen qua nhanh
        {
            int delay_time = time_one_frame - real_imp_time ;
            if (delay_time >=0)
            {
                SDL_Delay(delay_time); //tao do tre
                //delaytime lon thi ctrinh cang cham
                //delaytime to khi framepersec cang nho

            }
        }
    }

    for (int i=0;i<threats_list.size() ; i++)
    {
        ThreatsObjects* p_threat = threats_list.at(i);
        if (p_threat !=NULL)
        {
            p_threat->Free();
            p_threat = NULL;
        }
    }
    threats_list.clear();

    close();
    return 0;
}





























