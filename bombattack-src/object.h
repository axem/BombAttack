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
#ifndef OBJECT_H
#define OBJECT_H


#include <vector>
#include <SDL/SDL.h>
#include "globals.h"
#include "texture.h"

class Object{
public:
    class Animation;
protected:
    float x;
    float y;
    Animation * anim;
public:
    class Animation
    {
        public:
            class Frame;
        private:
            std::vector<Frame*> frames;
            Frame* _current;
            std::vector<Frame*>::iterator itr;
            bool started;
        public:
            class Frame : public Tex
            {
                public:
                    Uint32 time;
                    Uint32 used_time;
                    Frame();
                    Frame(const string& filename, Uint32 _time = 0);
                    virtual ~Frame();
            };

            void addFrame(Frame *);
            void addRes(strings s, Uint32 _time=0);
            void setCurrent(int);
            void nextFrame();
            Tex * get(int);
            void reset();
            Tex * current();
            Uint32 lastTime;

            void start();
            void stop();
            bool isStarted() { return started; };
            void unload();
            Animation();
            ~Animation();
    };

    Object();
    virtual ~Object();

    void setAnim(Animation & anim);
    virtual Animation * getAnim();
    virtual void draw();
    void setPos(float _x, float _y);
    void setPos(PointFloat);
    bool isAnimationStarted();
    PointFloat getSize();
    PointFloat getPos();
    virtual void unload() {};
};

#endif
