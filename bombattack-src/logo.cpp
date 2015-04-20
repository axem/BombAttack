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
#include "logo.h"
#include "menu.h"

void Logo::onKey(bool pressed, SDLKey)
{
    if(pressed)
    {
        Game::showLoading();
        unload();
        Game::setEvents(new Menu);
    }
}

void Logo::onPaint()
{
    tex.draw(pos);
    if(SDL_GetTicks()-startTime > 5000) onKey(true,SDLK_RETURN);
}

Logo::Logo()
: tex("logo.png"), startTime(SDL_GetTicks())
{
    pos.first=(scrWidth-tex.getSize().first)/2;
    pos.second=(scrHeight-tex.getSize().second)/2;
}

Logo::~Logo()
{
}

void Logo::unload()
{
    tex.unload();
}
