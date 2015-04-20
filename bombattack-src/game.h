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
#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include "events.h"


class Game{
    static bool running;
    static Events * events;
    static Events * new_events;
    static int max_fps;
    static int _fps;
    Game() {};
    ~Game() {};
public:
    static void setup_sdl();
    static void setup_opengl();
    static SDL_Surface * surface;
    static void setEvents(Events * _events);
    static void showLoading();
    static int fps();
#ifdef __WIN32__
    static Uint32 needredraw;
#endif
    static void run(bool windowed=false, bool nosound=false);

};

#endif
