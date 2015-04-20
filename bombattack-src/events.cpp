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
#include "events.h"
#include <GL/gl.h>
#include "counter.h"
#include "sound.h"

AverageCounter * Events::fps_counter=0;
Tex * Events::soundOn=0;
Tex * Events::soundOff=0;
bool Events::altPressed=false;
bool Events::preloaded=false;
bool Events::fps_show=false;
Uint32 Events::soundTime=0;

Events::Events()
: running(0)
{
    preloadEventsRes();
}

Events::~Events()
{
}

void Events::setRunning(bool * _running)
{
    running=_running;
}

void Events::quit()
{
    if(running) *running=false;
}

void Events::onEventsKey(Uint8 a, SDLKey key)
{
    bool b;
    if( a == SDL_KEYDOWN ) b=true;
        else b=false;
    switch (key)
    {
        case SDLK_RETURN:   if(!altPressed) { onEnter(b); }      onMyEnter(b); break;
        case SDLK_LEFT:     onArrow(LEFT,b);                     break;
        case SDLK_RIGHT:    onArrow(RIGHT,b);                    break;
        case SDLK_UP:       onArrow(UP,b);                       break;
        case SDLK_DOWN:     onArrow(DOWN,b);                     break;
        case SDLK_PLUS:     onPlus(b);                           break;
        case SDLK_MINUS:    onMinus(b);                          break;
        case SDLK_KP_PLUS:  onPlus(b);                           break;
        case SDLK_KP_MINUS: onMinus(b);                          break;
        case SDLK_SPACE:    onSpace(b);                          break;
        case SDLK_f:        onF(b);           onMyF(b);          break;
        case SDLK_s:        onS(b);           onMyS(b);          break;
        case SDLK_ESCAPE:   onEsc(b);                            break;
        case SDLK_RCTRL:    onCtrl(b);                           break;
        case SDLK_LCTRL:    onCtrl(b);                           break;
        case SDLK_RALT:     onAlt(b);         onMyAlt(b);        break;
        case SDLK_LALT:     onAlt(b);         onMyAlt(b);        break;
        default: break;
    }
    onKey(b,key);
    char c=0;
    switch (key)
    {
        case SDLK_a: c='a'; break;
        case SDLK_b: c='b'; break;
        case SDLK_c: c='c'; break;
        case SDLK_d: c='d'; break;
        case SDLK_e: c='e'; break;
        case SDLK_f: c='f'; break;
        case SDLK_g: c='g'; break;
        case SDLK_h: c='h'; break;
        case SDLK_i: c='i'; break;
        case SDLK_j: c='j'; break;
        case SDLK_k: c='k'; break;
        case SDLK_l: c='l'; break;
        case SDLK_m: c='m'; break;
        case SDLK_n: c='n'; break;
        case SDLK_o: c='o'; break;
        case SDLK_p: c='p'; break;
        case SDLK_q: c='q'; break;
        case SDLK_r: c='r'; break;
        case SDLK_s: c='s'; break;
        case SDLK_t: c='t'; break;
        case SDLK_u: c='u'; break;
        case SDLK_w: c='w'; break;
        case SDLK_x: c='x'; break;
        case SDLK_y: c='y'; break;
        case SDLK_v: c='v'; break;
        case SDLK_z: c='z'; break;

        case SDLK_0: c='0'; break;
        case SDLK_1: c='1'; break;
        case SDLK_2: c='2'; break;
        case SDLK_3: c='3'; break;
        case SDLK_4: c='4'; break;
        case SDLK_5: c='5'; break;
        case SDLK_6: c='6'; break;
        case SDLK_7: c='7'; break;
        case SDLK_8: c='8'; break;
        case SDLK_9: c='9'; break;
        default: break;
    };

    if(c) onNamedKey(b,c);
}

void Events::event(const SDL_Event &event)
{
    switch(event.type)
    {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            onEventsKey(event.type,event.key.keysym.sym);
        break;
        default: break;
    }
}

void Events::paint()
{ 
    onPaintBegin();
    onPaint();
    onPaintEnd();
}

void Events::onPaintBegin()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
}

void Events::onPaintEnd()
{
    if(fps_show && fps_counter)
    {
        fps_counter->setValue(Game::fps());
        fps_counter->draw();
    }

    if(soundTime)
    {
        const Uint32 soundTimeMax=1000;
        static PointFloat pos=normalize(bestWndWidth-52,bestWndHeight-80);
        Tex * t=0;
        if(!nosound && isSound) t=soundOn; else t=soundOff;
        if(nosound) t=soundOff;
        if(t) t->draw(pos);
        if(SDL_GetTicks()-soundTime >= soundTimeMax) soundTime=0;
    }
    //glFlush();
    SDL_GL_SwapBuffers( );
}

void Events::onMyAlt(bool pressed)
{
    altPressed=pressed;
}

void Events::onMyS(bool pressed)
{
    if(pressed)
    {
        soundTime=SDL_GetTicks();
        isSound=!isSound;
        soundMute(!isSound);
    }
}

void Events::onMyF(bool pressed)
{
    if(pressed)
        fps_show=!fps_show;
}

void Events::onMyEnter(bool pressed)
{
    if(altPressed && pressed)
    {
        isFullscreen=!isFullscreen;
#ifdef __LINUX__
        SDL_WM_ToggleFullScreen(Game::surface);
#else
        if(isFullscreen) Game::surface=SDL_SetVideoMode( wndWidth, wndHeight, VideoBitsPerPixel, SDL_OPENGL | SDL_FULLSCREEN  );
        else Game::surface=SDL_SetVideoMode( wndWidth, wndHeight, VideoBitsPerPixel, SDL_OPENGL );
        Game::setup_opengl();
        wndWidth=Game::surface->w;
        wndHeight=Game::surface->h;
        Tex::loadAll();
#endif

#ifdef __WIN32__
        Game::needredraw=SDL_GetTicks();
#endif

    }
}

void Events::preloadEventsRes()
{
    if(!preloaded)
    {
        fps_counter= new AverageCounter();
        fps_counter->setPos(normalize(0, bestWndHeight-64));
        soundOn= new Tex("soundon.png");
        soundOff= new Tex("soundoff.png");
        preloaded=true;
    }
}

void Events::unloadEventsRes()
{
    if(preloaded)
    {
        Counter::unloadRes();
        delete fps_counter;
        delete soundOn;
        delete soundOff;
        fps_counter=0;
        soundOn=0;
        soundOff=0;
        preloaded=false;
    }
}
