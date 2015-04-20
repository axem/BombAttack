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
#include "bomb.h"

Bomb::Animation * Bomb::preloadedBomb = 0;
bool Bomb::preloaded=false;

void Bomb::loadBomb(Animation * a)
{
    if(a==0) return;
    string base="bomb0000";
    string s="";
    int bomb_ms=20; //42
    for(int i=1; i<=55; i++)
    {
        s=toString(i);
        base.replace(base.size()-s.size(),s.size(),s);
        a->addFrame(new Object::Animation::Frame(base+".png",bomb_ms));
    }
}

void Bomb::preloadRes()
{
    if(!preloaded)
    {
        preloadedBomb = new Animation();
        loadBomb(preloadedBomb);
        preloaded=true;
    }
}

void Bomb::unloadRes()
{
    if(preloaded)
    {
        delete preloadedBomb;
        preloadedBomb=0;
        preloaded=false;
    }
}

Bomb::Bomb(const PointInt &tablePos)
: startTime(SDL_GetTicks()), pos(tablePos), remoteExploded(false)
{
    loadBomb(&bomb);
    x=pos.first*blockWidth;
    y=pos.second*blockHeight;
    setAnim(bomb);
}

Bomb::~Bomb()
{
}

bool Bomb::isExploded()
{
    return(remoteExploded || SDL_GetTicks()-startTime>=3000); //2310
}

PointInt Bomb::getTablePos()
{
    return pos;
}

void Bomb::unload()
{
    Object::unload();
    bomb.unload();
}

void Bomb::setRemoteExploded()
{
    remoteExploded=true;
}

bool Bomb::getRemoteExploded()
{
    return remoteExploded;
}


