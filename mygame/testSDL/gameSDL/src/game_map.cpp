#include "game_map.h"
#include <iostream>
void GameMap::LoadMap (char* name) //đọc file
{
    FILE* fp = NULL;
    fopen_s(&fp , name , "rb"); //rb là chế độ đọc của file
    if (fp == NULL) //nếu không có file
    {
        return ;
    }

    game_map_.max_x_=0; //mặc định ô hiện tại  theo x là 0
    game_map_.max_y_=0; //mặc định ô hiện tại theo y là 0
    for (int i=0 ; i<MAX_MAP_Y ; i++)
    {
        for (int j=0 ; j<MAX_MAP_X ; j++)
        {
            //lấy 4000 phần tử tiles trong mảng 2 chiều theo các dòng , sau đó theo các cột
            fscanf (fp , "%d" , &game_map_.tile[i][j]); //lay tung phan tu trong map
            int val = game_map_.tile[i][j];
            if (val > 0 )
            {
                if (j > game_map_.max_x_)
                {
                    game_map_.max_x_=j; //gán ô hiện tại theo j (chiều x)
                }
                if (i > game_map_.max_y_)
                {
                    game_map_.max_y_=i; //gán ô hiện tại theo i (chiều y)
                }
            }
        }
    }


    game_map_.max_x_= (game_map_.max_x_+1)*TILE_SIZE; //từ ô ở bên trên tính ra tọa độ hiện tại
    //phải cộng thêm 1 tránh tình trạng thiếu map
    game_map_.max_y_= (game_map_.max_y_+1)*TILE_SIZE;
    //sau khi tính xong mapx , mapy , tọa độ sẽ nằm ở điểm dưới cùng bên phải của map

    game_map_.start_x_ = 0;
    game_map_.start_y_ = 0;

    game_map_.file_name_ = name;
    fclose (fp); //đóng file

}

void GameMap :: LoadTiles (SDL_Renderer* screen) //load img cho cac tam anh
{
    char file_img[30];
    FILE* fp = NULL;

    for (int i=0;i<MAX_TILES ; i++)
    {
        sprintf_s (file_img , "map/%d.png" , i); //truyền img ở folder map thứ i vào trong tiles

        fopen_s(&fp , file_img , "rb"); //kiem tra mo file anh
        if (fp == NULL) //khong co anh
        {
            continue; //bo qua
        }
        fclose(fp);

        tile_mat[i].LoadImg (file_img , screen); //load tile thứ i vào screen
    }
}

void GameMap :: DrawMap (SDL_Renderer* screen) //print image vao cac tiles
{
    int x1=0;
    int x2=0;

    int y1=0;
    int y2=0;

    int map_x = 0;
    int map_y = 0;

    map_x = game_map_.start_x_ / TILE_SIZE;
    x1= (game_map_.start_x_ % TILE_SIZE) * (-1); //print lui lai 1 tile
    x2=x1 + SCREEN_WIDTH + (x1==0 ? 0: TILE_SIZE); //neu x1=0 thi khong + con x1 khac 0 thi cong them 1 tile để mở rộng ra
    //khi màn hình di chuyển phải lùi lại 1 ô và mở rộng thêm 1 ô để map không bị lặp
    map_y = game_map_.start_y_ / TILE_SIZE;
    y1= (game_map_.start_y_ % TILE_SIZE) * (-1);
    y2=y1 + SCREEN_HEIGHT + (y1==0 ? 0: TILE_SIZE);

    //fill tile theo 2 tọa độ x và y
    for (int i = y1 ; i < y2 ; i+=TILE_SIZE)
    {
        map_x = game_map_.start_x_ / TILE_SIZE;
        for (int j = x1 ; j < x2 ; j+=TILE_SIZE)
        {
            int val = game_map_.tile [map_y] [map_x];
            if (val > 0 )
            {
                tile_mat[val].SetRect( j , i); //set rect theo tạo độ i j
                tile_mat[val].Render (screen);
            }
            map_x++; //tăng chiều ngang lên 1 ô
        }
        map_y++; //tăng chiều dọc lên 1 ô
    }
}



















