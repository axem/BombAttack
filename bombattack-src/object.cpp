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
#include "object.h"


Object::Animation::Frame::Frame(const string& filename, Uint32 _time )
: Tex(filename), time( _time ), used_time(0)
{
}

Object::Animation::Frame::~Frame()
{
}
/************************************************************************************/

Object::Animation::Animation()
: _current(0), itr(frames.begin()), started(true), lastTime( SDL_GetTicks())
{
}

void Object::Animation::addFrame(Frame * x)
{
    frames.push_back(x);
}

void Object::Animation::addRes(strings s, Uint32 _time)
{
    for(int i=0; i< s.size(); i++)
        addFrame(new Frame(s[i],_time));
    reset();
}

void Object::Animation::reset()
{
    lastTime= SDL_GetTicks();
    started=true;
    if(frames.size() > 0) { itr=frames.begin(); _current=*itr; } else itr=frames.end();
}

Tex * Object::Animation::current()
{
    if( _current && frames.size() )
    {
        if( started && _current->time )
        {
            Uint32 now = SDL_GetTicks();
            _current->used_time+=now - lastTime;
            if ( _current->used_time >= _current->time  )
            {
                itr+= _current->used_time / _current->time;
                if(itr < frames.begin()) itr=frames.end()-1;
                if(itr >= frames.end()) itr=frames.begin();
                _current->used_time=0;
                (*itr)->used_time= _current->used_time % _current->time;
                _current=*itr;
            }
            lastTime = now;
        };
    }
    return _current;
}

void Object::Animation::setCurrent(int i)
{
    itr=frames.begin();
    itr+=i;
}

Tex * Object::Animation::get(int i)
{
    return frames[i];
}

Object::Animation::~Animation()
{
    purge(frames);
}

void Object::Animation::unload()
{
    purge(frames);
}

void Object::Animation::start()
{
    //lastTime= SDL_GetTicks();
    reset();
    started=true;
}

void Object::Animation::stop()
{
    reset();
    started=false;
}

void Object::Animation::nextFrame()
{
    if(!frames.size()) return;
    if(_current) _current->used_time=0;
    itr++;
    if(itr < frames.begin()) itr=frames.end()-1;
    if(itr >= frames.end()) itr=frames.begin();
    (*itr)->used_time=0;
    _current=*itr;
}

/************************************************************************************/

Object::Object()
: x(0), y(0), anim(0)
{
}

Object::~Object()
{
}

void Object::setAnim(Animation & _anim)
{
    if(anim == &_anim && _anim.isStarted()) return;
    if(anim != &_anim ) _anim.reset();
    anim=&_anim;
}

Object::Animation * Object::getAnim()
{
    return anim;
}

void Object::draw()
{
    if(anim && anim->current())
        anim->current()->draw(x,y);
}

void Object::setPos(PointFloat p)
{
    x=p.first;
    y=p.second;
}

void Object::setPos(float _x, float _y)
{
    x=_x;
    y=_y;
}

PointFloat Object::getPos()
{
    return PointFloat(x,y);
}

PointFloat Object::getSize()
{
    if(anim && anim->current())
        return anim->current()->getSize(); else
            return PointFloat(0,0);
}

bool Object::isAnimationStarted()
{
    if(anim) return anim->isStarted();
    return false;
}



