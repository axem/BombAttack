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
#include "man.h"

Man::Man(MyField& _field)
: field(_field), alive(true), head(true)
{
    int walk_ms=100;

    up.addRes(strs("back.png","up1.png","back.png","up2.png",0),walk_ms);
    down.addRes(strs("face.png","down1.png","face.png","down2.png",0),walk_ms);
    right.addRes(strs("right.png","right1.png","right.png","right2.png",0),walk_ms);
    left.addRes(strs("left.png","left1.png","left.png","left2.png",0),walk_ms);

    headUp.addRes(strs("headup.png",0),0);
    headDown.addRes(strs("headdown.png",0),0);
    headLeft.addRes(strs("headleft.png",0),0);
    headRight.addRes(strs("headright.png",0),0);
    dead.addRes(strs("bomberdead.png",0),0);

    int idle_ms_a=300;
    int idle_ms_b=300;

    idleUp.addFrame(new Animation::Frame("idleup.png",idle_ms_a));
    idleUp.addFrame(new Animation::Frame("back.png",idle_ms_b));

    idleDown.addFrame(new Animation::Frame("idledown.png",idle_ms_a));
    idleDown.addFrame(new Animation::Frame("face.png",idle_ms_b));

    idleLeft.addFrame(new Animation::Frame("idleleft.png",idle_ms_a));
    idleLeft.addFrame(new Animation::Frame("left.png",idle_ms_b));

    idleRight.addFrame(new Animation::Frame("idleright.png",idle_ms_a));
    idleRight.addFrame(new Animation::Frame("right.png",idle_ms_b));

    setAnim(idleDown);
}

Man::~Man()
{
}

void Man::modifyRect(RectFloat & r)
{
    static PointFloat collisionOffset(normalize(15,5));
    r.first.first+=collisionOffset.first;
    r.first.second+=collisionOffset.second;
    r.second.first-=collisionOffset.first;
    r.second.second-=collisionOffset.second;
}

void Man::onWalkDone(move_type & e)
{
    if(e == NONE)
    {
        do_it();
    }
}

void Man::onCollision(bool&, move_type & e)
{
    e=NONE;
    do_it();
}


void Man::do_it()
{
    if(anim == &up) setAnim(idleUp); else
    if(anim == &down) setAnim(idleDown); else
    if(anim == &left) setAnim(idleLeft); else
    if(anim == &right) setAnim(idleRight);
    anim->start();
}

void Man::onMoveAnimStop(bool &canStop)
{
    canStop=false;
}

void Man::unload()
{
    MovingObject::unload();
    headUp.unload();
    headDown.unload();
    headRight.unload();
    headLeft.unload();
    dead.unload();
    idleUp.unload();
    idleDown.unload();
    idleLeft.unload();
    idleRight.unload();
}
