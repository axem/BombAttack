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
#include "game.h"
#include "globals.h"
#include "logo.h"
#include <SDL/SDL_mixer.h>
#include <cstdlib>
#include "sound.h"

#ifdef __WIN32__
#include <windows.h>
#endif

Events * Game::new_events =0;
int Game::max_fps(99);
int Game::_fps(max_fps);
Events * Game::events(0);
bool Game::running(false);
SDL_Surface * Game::surface=0;

#ifdef __WIN32__
Uint32 Game::needredraw=0;
#endif

void Game::run(bool windowed, bool _nosound)
{
#ifdef __LINUX__
    prepare_linux();
#endif
    nosound=_nosound;
    isFullscreen=!windowed;
    srand(time(NULL));
    setup_sdl();
    setup_opengl();
    //showLoading();
    SDL_Event event;

    events=new Logo;
    events->setRunning(&running);
    running=true;
    while(running)
    {
//////////
        static int lastTime=SDL_GetTicks();
        int elapsed=SDL_GetTicks()-lastTime;
        int max_ms=1000/max_fps;

        if(elapsed < max_ms)
        {
            int x=max_ms - elapsed;
            if(!x) x=1;
            if(x<1) x=1;
            if(x>999) x=999;
            if(x>1)
            {
                 SDL_Delay( x );
            }
        }

        elapsed=SDL_GetTicks()-lastTime;
        if(elapsed > 1 && elapsed < 999)
            _fps=1000/elapsed;
        else
            if(elapsed > 1) _fps=1; else _fps=999;
        lastTime=SDL_GetTicks();
////////
        while( SDL_PollEvent( &event ) )
        {
            if(events) events->event(event);
            if(event.type == SDL_QUIT) running=false;
        }
        if(new_events) { delete events; events=new_events; events->setRunning(&running); new_events=0; }
        if(events) events->paint();

#ifdef __WIN32__
        if(needredraw && SDL_GetTicks()-needredraw > 1000)
        {
            RedrawWindow(GetDesktopWindow(),0,0,RDW_FRAME|RDW_UPDATENOW|RDW_ALLCHILDREN|RDW_INVALIDATE);
            needredraw=0;
        }
#endif

    }
    delete events;
    Events::unloadEventsRes();

    SDL_FreeSurface(surface);

    if(debug)
    {
        int x=Tex::max_used_gpu_ram / (1024*1024);
        if( Tex::max_used_gpu_ram % (1024*1024) > 0 ) ++x;
        cout << "---------------------------------------------" << endl;
        cout << "Max gpu ram usage: " << x <<"MB, "<< Tex::max_used_gpu_ram <<" bytes" << endl;
        cout << "cleaning up..." << endl;
    }
    if(!nosound)
    {
        freeSND();
        Mix_CloseAudio();
    }
}

void Game::setup_sdl() 
{
    if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 ) {
        fprintf(stderr,
                "Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    atexit(SDL_Quit);


    if( !SDL_GetVideoInfo() ) {
        fprintf(stderr,
                "Couldn't get video information: %s\n", SDL_GetError());
        exit(1);
    }

    /* Set the minimum requirements for the OpenGL window */
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    VideoBitsPerPixel=SDL_GetVideoInfo()->vfmt->BitsPerPixel;
    if(isFullscreen)
    surface=SDL_SetVideoMode( wndWidth, wndHeight, VideoBitsPerPixel, SDL_OPENGL | SDL_FULLSCREEN  );
    else
    surface=SDL_SetVideoMode( wndWidth, wndHeight, VideoBitsPerPixel, SDL_OPENGL);

    if(surface == 0) {
        fprintf(stderr,
                "Couldn't set video mode: %s\n", SDL_GetError());
        exit(1);
    }
    wndWidth=surface->w;
    wndHeight=surface->h;
    SDL_WM_SetCaption("Bomb Attack!",0);
    SDL_ShowCursor(false);

    if(!nosound)
    {
        if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 1024) < 0)
        {
            printf("Error initializing SDL_mixer: %s\n", Mix_GetError());
            nosound=true;
        }
        else
        {
            Mix_VolumeMusic(40);
            Mix_Volume(-1,51);
        }
    }

}

void Game::setup_opengl()
{
    glViewport(0,0,wndWidth,wndHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,scrWidth,scrHeight,0,0,1);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Game::setEvents(Events * _events)
{
    new_events=_events;
}

void Game::showLoading()
{
    static Tex loading("loading.png");
    glClear(GL_COLOR_BUFFER_BIT);
    PointFloat f=loading.getSize();
    loading.draw(scrWidth/2-f.first/2,scrHeight/2-f.second/2);
    SDL_GL_SwapBuffers( );
}

int Game::fps()
{
    return _fps;
}
