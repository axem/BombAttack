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
#ifndef MOVINGOBJECT_H
#define MOVINGOBJECT_H

#include "extended_collision_base.h"

class MovingObject : public Extended_collision_base
{
public:
    enum move_type{ NONE, UP, DOWN, LEFT, RIGHT };
private:
    float speed;

    move_type next;
    move_type current_move;

    float skip_rate_x; //-1 0 1
    float skip_rate_y; //-1 0 1

    float blockPosition_x;
    float blockPosition_y;

    bool moveMe2(move_type&, bool apply=true);
    bool moveMe3(int diff_x, int diff_y, move_type&, bool apply=true);

    Uint32 time_tmp;

    bool skipMe();
    Animation & moveAnim(move_type);

    friend class Field;
protected:
    virtual void onWalkDone(move_type& /*next*/) {};
    virtual void onCollision(bool& /*change_move*/, move_type& /*pending*/) {};
    virtual void onMoveAnimStop(bool& /*canStop*/) {};
public:
    move_type opposite(move_type);
    Animation up;
    Animation down;
    Animation left;
    Animation right;

    bool moveMe(move_type);

    void setSpeed(int);
    int getSpeed() const;

    void setField(Field *);
    bool goodMove(move_type);
    void setTablePos(int pos_x, int pos_y);
    void setTablePos(PointInt);
    PointInt getTablePos();

    virtual void unload();

    virtual void draw();
    virtual void make();

    bool isMoving();
    move_type getCurrentMove();
    bool isObjectOnDestPos();

    void revers();

    MovingObject(Field * field=0, int speed=500);
    virtual ~MovingObject();

};

#endif
