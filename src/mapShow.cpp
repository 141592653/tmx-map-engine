#include "tme.hpp"

using namespace std;
namespace tme
{

  void TmxME::setCameraPosition(int a_scrollX, int a_scrollY)
  {
    int widthMap = tmxMap->width * tmxMap->tile_width;
    int heightMap = tmxMap->height * tmxMap->tile_height;

    if(a_scrollX <= 0)//verify that the scrolling is correct
      scrollX = 0;
    else if(a_scrollX >= widthMap)
      scrollX = widthMap;
    else
      scrollX = a_scrollX;

    if(a_scrollY <= 0)
      scrollY = 0;
    else if(a_scrollY >= heightMap)
      scrollY = heightMap;
    else
      scrollY = a_scrollY;
  }

  void TmxME::display()
  {

    tmx_tileset* ts;
    tmx_layer* layer = tmxMap->ly_head;

    int modScrollX = scrollX%tmxMap->tile_width;
    int modScrollY = scrollY%tmxMap->tile_height;
    int i = scrollY/tmxMap->tile_height ;
    int dep_j = scrollX/tmxMap->tile_width ;
    int screenW = rectMap.w / tmxMap->tile_width + dep_j + 1;//the last tile in the screen
    int screenH = rectMap.h / tmxMap->tile_height + i + 1;

    SDL_Rect clip = {modScrollX,modScrollY,tmxMap->tile_width - modScrollX,tmxMap->tile_height - modScrollY};
    SDL_Rect pos = {rectMap.x,rectMap.y,tmxMap->tile_width - modScrollX,tmxMap->tile_height - modScrollY};

    ts = tmx_get_tileset(tmxMap, layer->content.gids[(i*tmxMap->width)+dep_j], (unsigned int*)&(clip.x), (unsigned int*)&(clip.y));
    clip.x += modScrollX;
    clip.y += modScrollY;
    
    SDL_RenderCopy(renderer,(SDL_Texture*) ts->image->resource_image,&clip,&pos);// corner up left

    pos.x += pos.w;
    pos.w = tmxMap->tile_width;
    //clip.x = 0;
    clip.w = tmxMap->tile_width;
    for(int j = dep_j + 1; j < min(screenW,(int)tmxMap->width); j++) //display of the above line
      {
        ts = tmx_get_tileset(tmxMap, layer->content.gids[(i*tmxMap->width)+j], (unsigned int*)&(clip.x), (unsigned int*)&(clip.y));
        clip.y += modScrollY;
        SDL_RenderCopy(renderer,(SDL_Texture*) ts->image->resource_image,&clip,&pos);
        pos.x += pos.w;
      }

    clip.w = modScrollX;//coin supérieur droit
    pos.w = modScrollX;
    ts = tmx_get_tileset(tmxMap, layer->content.gids[(i*tmxMap->width)+screenW], (unsigned int*)&(clip.x), (unsigned int*)&(clip.y));
    clip.y += modScrollY;
    if(screenW<tmxMap->width)//the screen is not bigger than the map
      SDL_RenderCopy(renderer,(SDL_Texture*) ts->image->resource_image,&clip,&pos);

    clip.h = tmxMap->tile_height;
    pos.y += pos.h;
    pos.h = tmxMap->tile_height;
    i++;


    for(; i<min((int)tmxMap->height,screenH); i++) //inside
      {
        ts = tmx_get_tileset(tmxMap, layer->content.gids[(i*tmxMap->width)+dep_j], (unsigned int*)&(clip.x), (unsigned int*)&(clip.y));
        clip.x += modScrollX;
        clip.w = tmxMap->tile_width - modScrollX;

        pos.x = rectMap.x; //reinitialise at each line beginning
        pos.w = clip.w;

        SDL_RenderCopy(renderer,(SDL_Texture*) ts->image->resource_image,&clip,&pos);//left border

        clip.w = tmxMap->tile_width;
        pos.x += pos.w;
        pos.w = clip.w;

        for(int j = dep_j + 1; j<min((int)tmxMap->width,screenW) ; j++)//corps de l'affichage
	  {
            ts = tmx_get_tileset(tmxMap, layer->content.gids[(i*tmxMap->width)+j], (unsigned int*)&(clip.x), (unsigned int*)&(clip.y));
            SDL_RenderCopy(renderer,(SDL_Texture*) ts->image->resource_image,&clip,&pos);
            pos.x += pos.w;
	  }

        if(screenW<tmxMap->width)
	  {
            ts = tmx_get_tileset(tmxMap, layer->content.gids[(i*tmxMap->width)+screenW], (unsigned int*)&(clip.x), (unsigned int*)&(clip.y));
            clip.w = modScrollX;
            pos.w = clip.w;
            SDL_RenderCopy(renderer,(SDL_Texture*) ts->image->resource_image,&clip,&pos);
	  }

        pos.y += pos.h;
      }



    if(i<tmxMap->height)
      {
        ts = tmx_get_tileset(tmxMap, layer->content.gids[(i*tmxMap->width)+dep_j], (unsigned int*)&(clip.x), (unsigned int*)&(clip.y));
        clip.x += modScrollX; // corner down left
        clip.w = tmxMap->tile_width - modScrollX;
        clip.h = modScrollY;
        pos.x = rectMap.x;
        pos.w = clip.w;
        pos.h = clip.h;
        SDL_RenderCopy(renderer,(SDL_Texture*) ts->image->resource_image,&clip,&pos);


        clip.w = tmxMap->tile_width;
        pos.x += pos.w;
        pos.w = clip.w;

        for(int j = dep_j + 1; j < min(screenW,(int)tmxMap->width); j++) //bottom line
	  {
            ts = tmx_get_tileset(tmxMap, layer->content.gids[(i*tmxMap->width)+j], (unsigned int*)&(clip.x), (unsigned int*)&(clip.y));
            if(SDL_RenderCopy(renderer,(SDL_Texture*) ts->image->resource_image,&clip,&pos)<0)
	      cout<<"Erreur dans l'affichage de la map (dernière ligne) : "<<SDL_GetError()<<endl;
            pos.x += pos.w;
	  }

        if(screenW < tmxMap->width)
	  ts = tmx_get_tileset(tmxMap, layer->content.gids[(i*tmxMap->width)+screenW], (unsigned int*)&(clip.x), (unsigned int*)&(clip.y));
        clip.w = modScrollX;
        pos.w = clip.w;
        SDL_RenderCopy(renderer,(SDL_Texture*) ts->image->resource_image,&clip,&pos);//bottom right corner
      }

  }
}
