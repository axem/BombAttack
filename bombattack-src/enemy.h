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
#ifndef ENEMY_H
#define ENEMY_H

#include "movingobject.h"

class Enemy : public MovingObject
{
    static Animation * deathup;
    static Animation * deathdown;
    static Animation * deathleft;
    static Animation * deathright;
    static bool preloaded;
    move_type nextEnemyMove;
protected:
    virtual void modifyRect(RectFloat & r);
    virtual void onWalkDone(move_type&);
public:
    void setNextEnemyMove(move_type);
    bool wakeup;
    move_type randomMove();
    static void unloadRes();
    static void preloadRes();
    bool alive;
    Enemy();
    virtual ~Enemy();

    virtual Animation * getAnim();
};

#endif
