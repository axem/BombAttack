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
#include "extended_collision_base.h"

unsigned Extended_collision_base::checkLen=3;

Extended_collision_base::Extended_collision_base()
: lastPos(0,0), field(0), collisionRect(PointFloat(0,0),PointFloat(0,0)), pos(0,0), myList(0)
{
}

Extended_collision_base::~Extended_collision_base()
{
    reset_extended_collision();
}

void Extended_collision_base::reset_extended_collision()
{
    if(myList)
    {
        myList->erase(myItr);
        if(myList->size()==0)
        {
            delete myList;
            if(field) 
            {
                int a=lastPos.first +lastPos.second * field->table.width;
                if(field->table.extended_collision[a]) field->table.extended_collision[a]=0;
            }
        }

        myList=0;
    }
}

void Extended_collision_base::makePos()
{
    if(myList)
    {
        myList->erase(myItr);
        if(myList->size()==0)
        {
            delete myList;
            if(field) 
            {
                int a=lastPos.first +lastPos.second * field->table.width;
                if(field->table.extended_collision[a]) field->table.extended_collision[a]=0;
            }
        }
        myList=0;
    }
    lastPos=pos;
    int a=pos.first + pos.second * field->table.width;
    ExCollisionList *& l=field->table.extended_collision[a];
    if(!l) l= new ExCollisionList;
    myList=l;
    l->push_back(this);
    myItr=l->end();
    --myItr;
    collisionRect.first.first=x;
    collisionRect.first.second=y;
    collisionRect.second=getSize();
    collisionRect.second.first+=x;
    collisionRect.second.second+=y;
    modifyRect(collisionRect);
}

bool Extended_collision_base::isCollisionWithRect(const RectFloat& x)
{
    return isCollisionRectRect(x,collisionRect);
}

bool Extended_collision_base::getExtendedCollision(vector<Extended_collision_base*> * v)
{
    if(!field) return false;
    bool b=false;
    if(v) v->clear();
    if(myList)
    {
        int x1=pos.first-checkLen; if(x1<0) x1=0;
        int x2=pos.first+checkLen; if(x2>field->table.width-1) x2=field->table.width-1;
        int y1=pos.second-checkLen; if(y1<0) y1=0;
        int y2=pos.second+checkLen; if(y2>field->table.height-1) y2=field->table.height-1;
        for(int x=x1; x<=x2; x++)
        for(int y=y1; y<=y2; y++)
        {
            int a=x+y*field->table.width;
            ExCollisionList * l=field->table.extended_collision[a];
            if(l)
            {
                ExCollisionList::iterator i=l->begin();
                while(i != l->end())
                {
                    if(*i != this && isCollisionWithRect((*i)->collisionRect) )
                    {
                        b=true;
                        if(!v) return b;
                        v->push_back(*i);
                    }
                    ++i;
                }
            }
        }
    }
    return b;
}


bool Extended_collision_base::isCollisionRectRect(const RectFloat& x,const RectFloat& z)
{
    const float &a=x.first.first;
    const float &b=x.first.second;
    const float &c=x.second.first;
    const float &d=x.second.second;

    const float &e=z.first.first;
    const float &f=z.first.second;
    const float &g=z.second.first;
    const float &h=z.second.second;

    bool ok=false;
    if(c-e >=0 && g-a >= 0 && h-b >= 0 && d-f >= 0)
        ok=true; else ok=false;
/*
    Thanks for puchacz 2007
*/
    return ok;
}


bool Extended_collision_base::getExtendedCollision(Field * field, const PointInt & pos, RectFloat rect, vector<Extended_collision_base*> * v)
{
    if(!field) return false;
    bool b=false;
        int x1=pos.first-checkLen; if(x1<0) x1=0;
        int x2=pos.first+checkLen; if(x2>field->table.width-1) x2=field->table.width-1;
        int y1=pos.second-checkLen; if(y1<0) y1=0;
        int y2=pos.second+checkLen; if(y2>field->table.height-1) y2=field->table.height-1;
        for(int x=x1; x<=x2; x++)
        for(int y=y1; y<=y2; y++)
        {
            int a=x+y*field->table.width;
            ExCollisionList * l=field->table.extended_collision[a];
            if(l)
            {
                ExCollisionList::iterator i=l->begin();
                while(i != l->end())
                {
                    const PointInt z = (*i)->pos;
                    if( isCollisionRectRect((*i)->collisionRect,rect ) )
                    {
                        b=true;
                        if(!v) return b;
                        v->push_back(*i);
                    }
                    ++i;
                }
            }
        }
    return b;
}

RectFloat Extended_collision_base::getRect()
{
    return collisionRect;
}

