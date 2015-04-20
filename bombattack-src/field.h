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
#ifndef FIELD_H
#define FIELD_H

#include <list>

#include "events.h"
#include "object.h"
#include "level_loader.h"
#include "extended_collision_base.h"

class Extended_collision_base;

class Field : public Events
{
    friend class Extended_collision_base;
private:
    float lastScrWidth;
    float lastScrHeight;
    float camera_pos_x;
    float camera_pos_y;
public:
    PointFloat displayArea;
    enum field_type{ GRASS, BRICK, WALL, DOOR, DOOR_HIDDEN, BONUSBOMB, BONUSBOMB_HIDDEN, BONUSRUN, BONUSRUN_HIDDEN, BONUSLIFE, BONUSLIFE_HIDDEN, BONUSRANGE, BONUSRANGE_HIDDEN, BONUSREMOTE, BONUSREMOTE_HIDDEN , ENEMY_SPAWN, DOOR_OPEN, SPACE };
    struct Table
    {
        int width;
        int height;
        vector<field_type> table;
        vector< std::list<Extended_collision_base*> * > extended_collision;
        Table(): width(0), height(0) {};
        ~Table() {};
    };
protected:
    float width_zoom;
    float height_zoom;
    int man_x;
    int man_y;
    int enemy_base_speed;
    int enemy_rand_speed;
    Table table;
    virtual void onPaint();
    void centre(PointFloat pos,PointFloat size=PointFloat(0,0));
    void recentre();
    void make_zoom();
public:
    Object::Animation grass;
    Object::Animation brick;
    Object::Animation wall;
    Tex bonuslife;
    Tex bonusbomb;
    Tex bonusrun;
    Tex door;
    Tex bonusrange;
    Tex bonusremote;
    Tex door_open;
    int level_num;
    virtual void load(int num);
    virtual void set(int x, int y, field_type);
    field_type get(int x, int y);
    bool isCollision(int x, int y);
    bool isVisible(const PointInt &pos);
    Tex * getTex(field_type);
    PointInt randomPos();
    virtual void unload();
    Field();
    virtual ~Field();

};

#endif
