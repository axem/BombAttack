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
#ifndef EVENTS_H
#define EVENTS_H

#include <SDL/SDL.h>

class Tex;
class AverageCounter;

class Events{
    bool * running;
    void onEventsKey(Uint8, SDLKey);
    void onMyAlt(bool pressed);
    void onMyS(bool pressed);
    void onMyF(bool pressed);
    void onMyEnter(bool pressed);

    static AverageCounter * fps_counter;
    static Tex * soundOn;
    static Tex * soundOff;
    static bool altPressed;
    static bool preloaded;
    static bool fps_show;
    static Uint32 soundTime;

public:
    static void preloadEventsRes();
    static void unloadEventsRes();
    enum arrow_type { UP, DOWN, LEFT, RIGHT };
protected:

    virtual void onPaint() {};
    virtual void onPaintBegin();
    virtual void onPaintEnd();

    virtual void onEnter(bool) {};
    virtual void onArrow(arrow_type, bool) {};

    virtual void onPlus(bool) {};
    virtual void onMinus(bool) {};
    virtual void onSpace(bool) {};

    virtual void onF(bool) {};
    virtual void onS(bool) {};
    virtual void onEsc(bool) {};

    virtual void onCtrl(bool) {};
    virtual void onAlt(bool) {};

    virtual void onKey(bool, SDLKey) {};
    virtual void onNamedKey(bool, char) {};
public:

    void event(const SDL_Event &event);
    void paint();
    void quit();
    void setRunning(bool * _running);

    virtual void unload() {};
    Events();
    virtual ~Events();

};

#endif
