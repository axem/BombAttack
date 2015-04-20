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
#include "hint.h"


Tex * Hint::v50=0;
Tex * Hint::v100=0;
Tex * Hint::v250=0;
Tex * Hint::v1000=0;
bool Hint::preloaded=false;
const Uint32 Hint::timeMax=1000;

void Hint::preloadRes()
{
    if(!preloaded)
    {
        v50=new Tex("50.png");
        v100=new Tex("100.png");
        v250=new Tex("250.png");
        v1000=new Tex("1000.png");
        preloaded=true;
    }
}

void Hint::unloadRes()
{
    if(preloaded)
    {
        delete v50;
        delete v100;
        delete v250;
        delete v1000;
        v50=0;
        v100=0;
        v250=0;
        v1000=0;
        preloaded=false;
    }
}

Hint::Hint(const PointFloat &_startPos, Value v)
: startPos(_startPos), t(0)
{
    preloadRes();
    time=SDL_GetTicks();
    switch(v)
    {
        case V50:   t=v50;   break;
        case V100:  t=v100;  break;
        case V250:  t=v250;  break;
        case V1000: t=v1000; break;
        default: break;
    };
}

Hint::~Hint()
{
}

void Hint::draw()
{
    static float z=normalize(1,35).second;
    Uint32 elapsed=SDL_GetTicks()-time;
    float offset=(float)(elapsed)/ (float)timeMax * z;
    if(t)
    {
        float x=1;
        if(elapsed > timeMax/2)
        {
            x= 1- ( (float)(elapsed-timeMax/2)/timeMax *2 );
        }
        glColor4f(1,1,1,x);
        t->draw(startPos.first,startPos.second-offset);
        glColor4f(1,1,1,1);
    }
}

bool Hint::ended()
{
    return ( SDL_GetTicks() - time >= timeMax);
}
