/***************************************************************************
 *   Copyright                                                             *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "field.h"
#include "level_loader.h"
#include <cstdlib>

Field::Field()
:  lastScrWidth(scrWidth),  lastScrHeight(scrHeight), camera_pos_x(0), camera_pos_y(0), displayArea(scrWidth,scrHeight),width_zoom(1), height_zoom(1), man_x(1), man_y(1), bonuslife("bonuslife.png"), bonusbomb("bonusbomb.png"), bonusrun("bonusrun.png"), door("exit1.png"), bonusrange("bonusrange.png"), bonusremote("bonusremote.png"), door_open("exit2.png"), level_num(0)
{
    grass.addRes(strs("grass.png"/*,"grass2.png"*/,0),0);

    int wall_ms=42;
    wall.addFrame(new Object::Animation::Frame("wall.png",wall_ms));
    wall.addFrame(new Object::Animation::Frame("wall2.png",wall_ms));
    wall.addFrame(new Object::Animation::Frame("wall3.png",wall_ms));
    wall.addFrame(new Object::Animation::Frame("wall4.png",wall_ms));
    wall.addFrame(new Object::Animation::Frame("wall5.png",wall_ms));
    wall.addFrame(new Object::Animation::Frame("wall6.png",wall_ms));
    wall.addFrame(new Object::Animation::Frame("wall7.png",wall_ms));
    wall.addFrame(new Object::Animation::Frame("wall8.png",wall_ms));
    wall.addFrame(new Object::Animation::Frame("wall9.png",wall_ms));
    wall.addFrame(new Object::Animation::Frame("wall.png",4000));

    int brick_ms=42;
    brick.addFrame(new Object::Animation::Frame("brick.png",brick_ms));
    brick.addFrame(new Object::Animation::Frame("brick2.png",brick_ms));
    brick.addFrame(new Object::Animation::Frame("brick3.png",brick_ms));
    brick.addFrame(new Object::Animation::Frame("brick4.png",brick_ms));
    brick.addFrame(new Object::Animation::Frame("brick5.png",brick_ms));
    brick.addFrame(new Object::Animation::Frame("brick6.png",brick_ms));
    brick.addFrame(new Object::Animation::Frame("brick7.png",brick_ms));
    brick.addFrame(new Object::Animation::Frame("brick8.png",brick_ms));
    brick.addFrame(new Object::Animation::Frame("brick9.png",brick_ms));
    brick.addFrame(new Object::Animation::Frame("brick.png",4000));
    wall.reset();
    brick.reset();
}

Field::~Field()
{
   // purge(table.extended_collision);
}

void Field::make_zoom()
{
    scrWidth/=width_zoom;
    scrHeight/=width_zoom;
    glScalef(width_zoom,height_zoom,1);
}

bool Field::isVisible(const PointInt & p)
{
    int a=int(camera_pos_x / displayArea.first * table.width)-2; if(a<0) a=0;
    int b=int(camera_pos_y / displayArea.second * table.height)-2; if(b<0) b=0;
    int c=int((camera_pos_x+scrWidth) / displayArea.first * table.width)+2; if(c>table.width) c=table.width;
    int d=int((camera_pos_y+scrHeight) / displayArea.second * table.height)+2; if(d>table.height) d=table.height;
    if(a <= p.first &&  p.first <= c && b <= p.second  &&   p.second  <= d )
    return true; else return false;
}

void Field::onPaint()
{
    int a=int(camera_pos_x / displayArea.first * table.width); if(a<0) a=0;
    int b=int(camera_pos_y / displayArea.second * table.height); if(b<0) b=0;
    int c=int((camera_pos_x+scrWidth) / displayArea.first * table.width)+1; if(c>table.width) c=table.width;
    int d=int((camera_pos_y+scrHeight) / displayArea.second * table.height)+1; if(d>table.height) d=table.height;
    if(!table.table.size()) return;

    for(int x=a; x < c; x++)
    for(int y=b; y < d; y++)
    {
        field_type e=table.table[x+y*table.width];
        if(e != SPACE)
        {
            Tex * v=grass.current();
            if(v) v->draw((float)x*blockWidth,(float)y*blockHeight);
        }
        Tex * t=getTex(e);
        if(t) t->draw((float)x*blockWidth,(float)y*blockHeight);
    }
}

Tex * Field::getTex(field_type e)
{
    switch(e)
    {
        case GRASS:              return 0;                break;
        case BRICK:              return brick.current();  break;
        case WALL:               return wall.current();   break;
        case DOOR:               return &door;            break;
        case DOOR_HIDDEN:        return brick.current();  break;
        case BONUSBOMB:          return &bonusbomb;       break;
        case BONUSBOMB_HIDDEN:   return brick.current();  break;
        case BONUSRUN:           return &bonusrun;        break;
        case BONUSRUN_HIDDEN:    return brick.current();  break;
        case BONUSLIFE:          return &bonuslife;       break;
        case BONUSLIFE_HIDDEN:   return brick.current();  break;
        case BONUSRANGE:         return &bonusrange;      break;
        case BONUSRANGE_HIDDEN:  return brick.current();  break;
        case BONUSREMOTE:        return &bonusremote;     break;
        case BONUSREMOTE_HIDDEN: return brick.current();  break;
        case DOOR_OPEN:          return &door_open;       break;
        case SPACE:              return 0;                break;
        default: return 0; break;
    }
}

Field::field_type Field::get(int x, int y)
{
    return table.table[x+y*table.width];
}

bool Field::isCollision(int x, int y)
{
    field_type e=get(x,y);
    switch(e)
    {
        case BRICK:              return true;  break;
        case WALL:               return true;  break;
        case BONUSBOMB_HIDDEN:   return true;  break;
        case BONUSRUN_HIDDEN:    return true;  break;
        case BONUSLIFE_HIDDEN:   return true;  break;
        case DOOR_HIDDEN:        return true;  break;
        case BONUSRANGE_HIDDEN:  return true;  break;
        case BONUSREMOTE_HIDDEN: return true;  break;
        case SPACE:              return true;  break;
        default:    return false; break;
    };
}


void Field::centre(PointFloat p, PointFloat s)
{
    float x=p.first - (scrWidth-s.first)/2 ;
    float y=p.second - (scrHeight-s.second)/2 ;
    glTranslatef(-x,-y,0);
    camera_pos_x=x;
    camera_pos_y=y;
}

void Field::recentre()
{
    glLoadIdentity();
    scrWidth=lastScrWidth;
    scrHeight=lastScrHeight;
    camera_pos_x=0;
    camera_pos_y=0;
}

PointInt Field::randomPos()
{
    int x=0;
    int y=0;
    while(isCollision(x,y))
    {
        x=rand() % table.width;
        y=rand() % table.height;
    }
    return PointInt(x,y);
}

void Field::set(int x, int y, field_type e)
{
    table.table[x+y*table.width]=e;
}

void Field::load(int num)
{
    LevelLoader::Level * level= LevelLoader::get()[num];
    table.width=level->width;
    table.height=level->height;
    man_x=level->man_x;
    man_y=level->man_y;
    enemy_base_speed=level->enemy_base_speed;
    enemy_rand_speed=level->enemy_rand_speed;
    //purge(table.extended_collision);
    wall.reset();
    brick.reset();
    table.table.resize(table.width*table.height);
    table.extended_collision.resize(table.width*table.height);
    for(int x=0; x<table.width; x++)
    for(int y=0; y<table.height; y++)
    {
        int a=x+y*table.width;
        field_type e=GRASS;
        switch(level->table[a])
        {
            case 0:    e = GRASS;              break;
            case 1:    e = WALL;               break;
            case 2:    e = BRICK;              break;
            case 3:    e = ENEMY_SPAWN;        break;
            case 4:    e = DOOR_HIDDEN;        break;
            case 5:    e = BONUSBOMB_HIDDEN;   break;
            case 6:    e = BONUSRUN_HIDDEN;    break;
            case 7:    e = BONUSLIFE_HIDDEN;   break;
            case 8:    e = BONUSRANGE_HIDDEN;  break;
            case 9:    e = BONUSREMOTE_HIDDEN; break;
            case 10:   e = SPACE;              break;
            default: break;
        }
        table.table[a]=e;
        table.extended_collision[a]=0;
    }
    displayArea.first=(float)table.width*blockWidth;
    displayArea.second=(float)table.height*blockHeight;
}

void Field::unload()
{
    grass.unload();
    brick.unload();
    wall.unload();
    bonuslife.unload();
    bonusbomb.unload();
    bonusrun.unload();
    bonusrange.unload();
    bonusremote.unload();
    door.unload();
    door_open.unload();
}



