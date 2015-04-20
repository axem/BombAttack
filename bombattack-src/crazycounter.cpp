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
#include "crazycounter.h"

CrazyCounter::CrazyCounter(Uint32 _oneAddTime)
: lastTime(0), oneAddTime(_oneAddTime), destValue(0)
{
}

CrazyCounter::~CrazyCounter()
{
}

void CrazyCounter::addValue(int i)
{
    destValue+=+i;
}

void CrazyCounter::setOneAddTime(Uint32 i)
{
    oneAddTime=i;
}

void CrazyCounter::draw()
{
    bool b=false;
    if(destValue > value )
    {
        if(lastTime == 0 ) lastTime=SDL_GetTicks();
        int elapsed=(SDL_GetTicks()-lastTime);
        value+=elapsed / oneAddTime;
        lastTime=SDL_GetTicks();
        if(destValue < value) b=true;
    }
    if(destValue < value || b )
    {
        value = destValue;
        lastTime=0;
    }

    Counter::draw();
}

void CrazyCounter::setValue(int _value)
{
    destValue=_value;
    value=_value;
}

