#ifndef GAME_MAP_H
#define GAME_MAP_H
//#include "CommonFunc.h"
#include "BaseObject.h"
#define MAX_TILES 20 //số ô hình ảnh tối đa là 20 ảnh
class TileMat : public BaseObject //tile mat ke thua cua BaseObject
{
    public:
        TileMat() {;}
        ~TileMat () {;}
};

class GameMap
{
public:
    GameMap() {;}
    ~GameMap () {;}

    void LoadMap (char* name); //read map from file text (notepad)
    void LoadTiles (SDL_Renderer* screen); //load image for tile map
    void DrawMap (SDL_Renderer* screen);   //print image in right position (mix loadmap & loadtiles)
    Map getMap() const {return game_map_;};
    void SetMap(Map& map_data) {game_map_ = map_data;};
private:
    Map game_map_; //lưu giữ tình trạng bản đồ (các ô và tình trạng ô)
    TileMat tile_mat [MAX_TILES] ; //lưu trữ hình ảnh


};

#endif // GAME_MAP_H









































