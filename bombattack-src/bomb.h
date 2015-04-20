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
#ifndef BOMB_H
#define BOMB_H
#include "object.h"

class Bomb : public Object
{
    bool exploding;
    Uint32 startTime;
    Bomb();
    PointInt pos;
    static bool preloaded;
    static Animation * preloadedBomb;
    static void loadBomb(Animation *);
    bool remoteExploded;
public:
    Animation bomb;
    static void preloadRes();
    static void unloadRes();
    bool isExploded();
    void setRemoteExploded();
    bool getRemoteExploded();
    Bomb(const PointInt &tablePos);
    PointInt getTablePos();
    ~Bomb();
    virtual void unload();
};
#endif

