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
#ifndef EXPLOSION
#define EXPLOSION

#include "object.h"
#include "field.h"
class Explosion 
{
    static bool preloaded;
    static Object::Animation * left_pre;
    static Object::Animation * right_pre;
    static Object::Animation * pion_pre;
    static Object::Animation * poziom_pre;
    static Object::Animation * up_pre;
    static Object::Animation * down_pre;
    static Object::Animation * srodek_pre;
    Explosion();
    Field * field;
    PointInt tablePos;
public:
    struct Part
    {
        enum Type { LEFT, RIGHT, PION, POZIOM, UP, DOWN, SRODEK };
        Type type;
        PointInt pos;
        RectFloat rect;
        Part(Type type, const  PointInt &pos, const RectFloat &rect );
        ~Part();
    };
    Object::Animation left;
    Object::Animation right;
    Object::Animation pion;
    Object::Animation poziom;
    Object::Animation up;
    Object::Animation down;
    Object::Animation srodek;
    vector<Part> parts;
    static void unloadRes();
    static void preloadRes();

    Explosion(Field * field, const PointInt& tablePos );
    ~Explosion();
    void draw();
    static int len;
    Uint32 startTime;
    bool collisionFound(const PointInt& pos);
    bool getExtendedCollision(vector<Extended_collision_base*> * = 0);
};

#endif
