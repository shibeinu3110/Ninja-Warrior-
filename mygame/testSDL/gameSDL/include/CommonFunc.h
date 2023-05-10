#ifndef COMMON-FUNCTION_H_
#define COMMON_FUNCTION_H_

#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

static Mix_Chunk* g_sound_player[10];
static Mix_Chunk* g_sound_exp[5];
static Mix_Chunk* g_theme_music[5];
static Mix_Chunk* g_general_music[5];

static SDL_Window* g_window = NULL ; //khoi tao bien const window
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event; //bat su kien ban phim
static SDL_Renderer* g_img_menu = NULL;

//Screen : thiet lap man hinh
const int SCREEN_WIDTH =1280;
const int SCREEN_HEIGHT =640;
const int SCREEN_BPP =32; //pixel

//Setting color
const int COLOR_KEY_R=167;      //red
const int COLOR_KEY_G=175;      //green
const int COLOR_KEY_B=180;      //blue

const int RENDER_DRAW_COLOR  = 0Xff;

const int FRAME_PER_SECOND = 37; //fps

#define TILE_SIZE 64   // 1 mắt lưới của ảnh

#define MAX_MAP_X 400   //chieu rong (400 cột)
#define MAX_MAP_Y 10    //chieu dai (10 dòng)

#define BLANK_TILE 0
#define STATE_MONEY 4 //trong file dat thi coin la 4

typedef struct Input //trạng thái đầu vào của nhân vật
{
  int left_;
  int right_;
  int up_;
  int down_;
  int jump_;
};

typedef struct Map
{
    int start_x_; //tọa độ x
    int start_y_; //tọa độ y

    int max_x_; //đang ở ô bao nhiêu tính theo chiều x  (max_x_ = start_x_ / TILE_SIZE)
    int max_y_; //đang ở ô bao nhiêu tính theo chiều y  (max_y_ = start_y_ / TILE_SIZE)

    int tile[MAX_MAP_Y] [MAX_MAP_X]; //mang 2 chieu luu giu trang thai cua map
    char* file_name_;
};

namespace SDLCommonFunc
{
    bool CheckCollision (const SDL_Rect& object1 , const SDL_Rect& object2); //kiem tra va cham 2 object
    bool CheckFocusWithRect(const int& x, const int& y, const SDL_Rect& rect);
    int  ShowMenu (SDL_Renderer* des , TTF_Font* font);

}

#endif // COMMON_FUNCTION_H_








































