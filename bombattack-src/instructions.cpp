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
#include "instructions.h"
#include "menu.h"
#include "sound.h"

void Instructions::onEnter(bool pressed)
{
    if(pressed)
    {
        playSND(SND_MENUENTER);
        Game::showLoading();
        unload();
        Game::setEvents(new Menu);
    }
}

void Instructions::onPaint()
{
    tex.draw(0,0);
    static PointFloat x=normalize(249,540);
    const Uint32 enterTimeMax=750;
    if(SDL_GetTicks()-enterTime >=enterTimeMax)
    {
        enterTime=SDL_GetTicks();
        enterVisible=!enterVisible;
    }
    if(enterVisible)
        enter.draw(x);
}

Instructions::Instructions()
: tex("instructions.png"), enter("enter.png"), enterTime(SDL_GetTicks()), enterVisible(true)
{
}

Instructions::~Instructions()
{
}

void Instructions::unload()
{
    tex.unload();
    enter.unload();
}
