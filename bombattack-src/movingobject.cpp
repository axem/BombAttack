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
#include "movingobject.h"

MovingObject::MovingObject(Field * _field, int _speed)
: speed(_speed), next(NONE), current_move(NONE), skip_rate_x(0), skip_rate_y(0), blockPosition_x(0), blockPosition_y(0), time_tmp(0)
{
    field=_field;
}

MovingObject::~MovingObject()
{
}

Object::Animation & MovingObject::moveAnim(move_type m)
{
    switch(m)
    {
        case NONE:  return down;  break;
        case UP:    return up;    break;
        case DOWN:  return down;  break;
        case LEFT:  return left;  break;
        case RIGHT: return right; break;
        default: return down; break;
    }
}

bool MovingObject::moveMe(move_type m)
{
    if(current_move !=NONE && m==NONE ) {  next=m;  }

    switch(current_move)
    {
        case NONE:
            switch(m)
            {
                case NONE: 
                    if(getAnim())
                    {
                        bool b=true;
                        onMoveAnimStop(b);
                        if(b) getAnim()->stop();
                    }
                    current_move=NONE;
                break;
                case UP:
                case DOWN:
                case LEFT:
                case RIGHT:
                    if(moveMe2(m)) current_move=m;
                    if(m != NONE) setAnim(moveAnim(m)); 
                    next=m;
                break;
                default: break;
            }
        break;

        case UP:
        case DOWN:
        case LEFT:
        case RIGHT:
            if(current_move != m && m !=NONE)
            {
                if(opposite(m)==current_move)
                {
                    blockPosition_x=blockWidth-blockPosition_x;
                    blockPosition_y=blockHeight-blockPosition_y;

                    if(moveMe2(m)) current_move=m;
                    if(m != NONE) setAnim(moveAnim(m));
                    next=m;
                }
                next=m;
            }
            if(current_move == m)
            {
                next=m;
            }
        break;
        default: break;
    }

    if(current_move != NONE) return true;
        else return false;
}

bool MovingObject::moveMe3(int diff_x, int diff_y, move_type &m, bool apply)
{
    bool b=false;
    if(!field) return true;
    if(field->isCollision(pos.first+diff_x,pos.second+diff_y))
    {
        onCollision(b,m);
        return b;
    }

    if(!field->isCollision(pos.first+diff_x,pos.second+diff_y) || b)
    {
        skip_rate_x=diff_x;
        skip_rate_y=diff_y;
        if(apply)
        {
            pos.first+=diff_x;
            pos.second+=diff_y;
        }
        return true;
    }
    return false;
}

bool MovingObject::moveMe2(move_type &m, bool apply)
{
    if(!field)  return true; 
    if(m==NONE) return true;
    switch(m)
    {
        case(UP):    return moveMe3( 0,-1, m, apply);  break;
        case(DOWN):  return moveMe3( 0,+1, m, apply);  break;
        case(LEFT):  return moveMe3(-1, 0, m, apply);  break;
        case(RIGHT): return moveMe3(+1, 0, m, apply);  break;
        default: break;
    }
    return false;
}

int MovingObject::getSpeed() const
{
    return (int)speed;
}

void MovingObject::setSpeed(int _speed)
{
    speed=_speed;
}

bool MovingObject::skipMe()
{
    if(!time_tmp) time_tmp=SDL_GetTicks();
    Uint32 elapsed=SDL_GetTicks()-time_tmp;
    time_tmp=SDL_GetTicks();

    float skip_x = (float)elapsed/speed * blockWidth  * skip_rate_x;
    float skip_y = (float)elapsed/speed * blockHeight * skip_rate_y;

    blockPosition_x+= abs(skip_x);
    blockPosition_y+= abs(skip_y);

    if( skip_rate_x &&  blockWidth  - blockPosition_x > 0) 
        x+=(blockPosition_x-blockWidth)* skip_rate_x;

    if( skip_rate_y && blockHeight - blockPosition_y > 0)
        y+=(blockPosition_y-blockHeight)*skip_rate_y;

    if( ( blockPosition_x >= blockWidth  || skip_rate_x == 0 ) && 
        ( blockPosition_y >= blockHeight || skip_rate_y == 0 ) )
    {
        blockPosition_x=0;
        blockPosition_y=0;
        time_tmp=0;
        current_move=NONE;
        return true;
    }
    else 
    {
        return false;
    }
}

void MovingObject::draw()
{
    Object::draw();
}

void MovingObject::setField(Field * _field)
{
    field=_field;
}

void MovingObject::setTablePos(int _pos_x, int _pos_y)
{
    pos=PointInt(_pos_x,_pos_y);
}

MovingObject::move_type MovingObject::opposite(move_type m)
{
    switch(m)
    {
        case NONE:  return NONE;  break;
        case UP:    return DOWN;  break;
        case DOWN:  return UP;    break;
        case LEFT:  return RIGHT; break;
        case RIGHT: return LEFT;  break;
        default:    return NONE;  break;
    }
}

void MovingObject::setTablePos(PointInt p)
{
    setPos(p.first,p.second);
}

bool MovingObject::goodMove(move_type m)
{
    return moveMe2(m,false);
}

void MovingObject::make()
{
    x=(float)pos.first*blockWidth;
    y=(float)pos.second*blockHeight;
    if(current_move != NONE && skipMe()) 
    {
        onWalkDone(next);
        moveMe(next);
        make();
    }
    makePos();
}

PointInt MovingObject::getTablePos()
{
    return pos;
}


void MovingObject::unload()
{
    Object::unload();
    up.unload();
    down.unload();
    left.unload();
    right.unload();
}

bool MovingObject::isMoving()
{
    return (current_move != NONE);
}

MovingObject::move_type MovingObject::getCurrentMove()
{
    return current_move;
}

bool MovingObject::isObjectOnDestPos()
{
    return(
      ((current_move==LEFT || current_move==RIGHT) && blockPosition_x < blockWidth/2) ||
      ((current_move==UP   || current_move==DOWN ) && blockPosition_y < blockHeight/2)
    );
}

void MovingObject::revers()
{
    moveMe(opposite(current_move));
}
