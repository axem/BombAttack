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
#ifndef EXTENDED_COLLISION_BASE
#define EXTENDED_COLLISION_BASE

#include <list>
#include "globals.h"
#include "field.h"
#include "object.h"

class Extended_collision_base;
typedef std::list<Extended_collision_base*> ExCollisionList;
typedef std::list<Extended_collision_base*>::iterator ExCollisionList_iterator;
class Field;

class Extended_collision_base : public Object
{
    PointInt lastPos;
protected:
    Field * field;
    RectFloat collisionRect;
    PointInt pos;
    void makePos();
    ExCollisionList * myList;
    ExCollisionList_iterator myItr;
    virtual void modifyRect(RectFloat &){};
public:
    Extended_collision_base();
    virtual ~Extended_collision_base();
    bool isCollisionWithRect(const RectFloat&);
    bool getExtendedCollision(vector<Extended_collision_base*> * = 0);
    void reset_extended_collision();

    static bool isCollisionRectRect(const RectFloat& x,const RectFloat& z);
    static bool getExtendedCollision(Field * field, const PointInt & pos, RectFloat rect, vector<Extended_collision_base*> * v=0);
    static unsigned checkLen;
    RectFloat getRect();
};



#endif

