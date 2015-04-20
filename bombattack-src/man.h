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
#ifndef MAN_H
#define MAN_H

#include "movingobject.h"

class MyField;

class Man : public MovingObject
{
    Man();
    MyField& field;
    void do_it();
protected:
    virtual void modifyRect(RectFloat &);
    virtual void onWalkDone(move_type &next);
    virtual void onCollision(bool &change_move, move_type &pending);
    virtual void onMoveAnimStop(bool &canStop);
public:
    bool alive;
    bool head;
    Animation headUp;
    Animation headDown;
    Animation headRight;
    Animation headLeft;
    Animation dead;
    Animation idleUp;
    Animation idleDown;
    Animation idleLeft;
    Animation idleRight;

    virtual void unload();
    Man(MyField&);
    virtual ~Man();
};

#endif
