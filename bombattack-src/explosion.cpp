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
#include "explosion.h"

bool Explosion::preloaded=false;
Object::Animation * Explosion::left_pre=0;
Object::Animation * Explosion::right_pre=0;
Object::Animation * Explosion::pion_pre=0;
Object::Animation * Explosion::poziom_pre=0;
Object::Animation * Explosion::up_pre=0;
Object::Animation * Explosion::down_pre=0;
Object::Animation * Explosion::srodek_pre=0;

int Explosion::len=1;

void Explosion::preloadRes()
{
    if(!preloaded)
    {
        left_pre = new Object::Animation();
        right_pre = new Object::Animation();
        pion_pre = new Object::Animation();
        poziom_pre = new Object::Animation();
        up_pre = new Object::Animation();
        down_pre = new Object::Animation();
        srodek_pre = new Object::Animation();

        int ms=42;
        left_pre->addRes(strs("xplo_left1.png","xplo_left2.png",0),ms);
        right_pre->addRes(strs("xplo_right1.png","xplo_right2.png",0),ms);
        pion_pre->addRes(strs("xplo_pion1.png","xplo_pion2.png",0),ms);
        poziom_pre->addRes(strs("xplo_poziom1.png","xplo_poziom2.png",0),ms);
        up_pre->addRes(strs("xplo_up1.png","xplo_up2.png",0),ms);
        down_pre->addRes(strs("xplo_down1.png","xplo_down2.png",0),ms);
        srodek_pre->addRes(strs("xplo_srodek1.png","xplo_srodek2.png",0),ms);

        preloaded=true;
    }
}

void Explosion::unloadRes()
{
    if(preloaded)
    {
        delete left_pre;
        delete right_pre;
        delete pion_pre;
        delete poziom_pre;
        delete up_pre;
        delete down_pre;
        delete srodek_pre;
        preloaded=false;
    }
}

Explosion::Explosion(Field * _field, const PointInt& _tablePos)
: field(_field), tablePos(_tablePos), startTime(0)
{
    int ms=42;
    left.addRes(strs("xplo_left1.png","xplo_left2.png",0),ms);
    right.addRes(strs("xplo_right1.png","xplo_right2.png",0),ms);
    pion.addRes(strs("xplo_pion1.png","xplo_pion2.png",0),ms);
    poziom.addRes(strs("xplo_poziom1.png","xplo_poziom2.png",0),ms);
    up.addRes(strs("xplo_up1.png","xplo_up2.png",0),ms);
    down.addRes(strs("xplo_down1.png","xplo_down2.png",0),ms);
    srodek.addRes(strs("xplo_srodek1.png","xplo_srodek2.png",0),ms);


    PointFloat f((float)_tablePos.first*blockWidth,(float)_tablePos.second*blockHeight);
    parts.push_back(Part(Part::SRODEK,_tablePos,RectFloat(f,PointFloat(f.first+blockWidth,f.second+blockHeight))));

    PointInt _pos;
    RectFloat _rect;
    float offset_x=normalize(5,5).first;
    float offset_y=normalize(5,5).second;

    for(int i=1; i<=len; i++)
    {
        _pos.first=_tablePos.first+i;
        _pos.second=_tablePos.second;
        if(field->isCollision(_pos.first,_pos.second)) { if(collisionFound(_pos)) i=len; else break; }
        Part::Type t=Part::POZIOM;
        if(i==len) t=Part::RIGHT;
        _rect.first=PointFloat((float)_pos.first*blockWidth,(float)_pos.second*blockHeight+offset_y);
        _rect.second=PointFloat(_rect.first.first+blockWidth,_rect.first.second+blockHeight-2*offset_y);
        parts.push_back(Part(t,_pos,_rect));
    }

    for(int i=1; i<=len; i++)
    {
        _pos.first=_tablePos.first-i;
        _pos.second=_tablePos.second;
        if(field->isCollision(_pos.first,_pos.second)) { if(collisionFound(_pos)) i=len; else break; }
        Part::Type t=Part::POZIOM;
        if(i==len) t=Part::LEFT;
        _rect.first=PointFloat((float)_pos.first*blockWidth,(float)_pos.second*blockHeight+offset_y);
        _rect.second=PointFloat(_rect.first.first+blockWidth,_rect.first.second+blockHeight-2*offset_y);
        parts.push_back(Part(t,_pos,_rect));
    }

    for(int i=1; i<=len; i++)
    {
        _pos.first=_tablePos.first;
        _pos.second=_tablePos.second+i;
        if(field->isCollision(_pos.first,_pos.second)) { if(collisionFound(_pos)) i=len; else break; }
        Part::Type t=Part::PION;
        if(i==len) t=Part::DOWN;
        _rect.first=PointFloat((float)_pos.first*blockWidth+offset_x,(float)_pos.second*blockHeight);
        _rect.second=PointFloat(_rect.first.first+blockWidth-2*offset_x,_rect.first.second+blockHeight);
        parts.push_back(Part(t,_pos,_rect));
    }

    for(int i=1; i<=len; i++)
    {
        _pos.first=_tablePos.first;
        _pos.second=_tablePos.second-i;
        if(field->isCollision(_pos.first,_pos.second)) { if(collisionFound(_pos)) i=len; else break; }
        Part::Type t=Part::PION;
        if(i==len) t=Part::UP;
        _rect.first=PointFloat((float)_pos.first*blockWidth+offset_x,(float)_pos.second*blockHeight);
        _rect.second=PointFloat(_rect.first.first+blockWidth-2*offset_x,_rect.first.second+blockHeight);
        parts.push_back(Part(t,_pos,_rect));
    }


}

Explosion::Part::Part(Type _type, const  PointInt &_pos, const RectFloat &_rect )
: type(_type), pos(_pos), rect(_rect)
{
}

Explosion::Part::~Part()
{
}

Explosion::~Explosion()
{
}

void Explosion::draw()
{
    vector<Part>::iterator i=parts.begin();
    while(i != parts.end())
    {
        Object::Animation * a=0;
        switch((*i).type)
        {
            case Part::LEFT:    a=&left;    break;
            case Part::RIGHT:   a=&right;   break;
            case Part::PION:    a=&pion;    break;
            case Part::POZIOM:  a=&poziom;  break;
            case Part::UP:      a=&up;      break;
            case Part::DOWN:    a=&down;    break;
            case Part::SRODEK:  a=&srodek;  break;
            default: break;
        };

        if(a) 
        {
            Tex * t=a->current();
            if(t) t->draw((float)(*i).pos.first*blockWidth,(float)(*i).pos.second*blockHeight);
        }
        i++;
    }
}

bool Explosion::collisionFound(const PointInt& pos)
{
    if(!field) return false;
    Field::field_type f=field->get(pos.first,pos.second);
    Field::field_type g=f;
    switch(f)
    {
        case Field::BRICK:               f=Field::GRASS;       break;
        case Field::DOOR_HIDDEN:         f=Field::DOOR;        break;
        case Field::BONUSBOMB_HIDDEN:    f=Field::BONUSBOMB;   break;
        case Field::BONUSRUN_HIDDEN:     f=Field::BONUSRUN;    break;
        case Field::BONUSLIFE_HIDDEN:    f=Field::BONUSLIFE;   break;
        case Field::BONUSRANGE_HIDDEN:   f=Field::BONUSRANGE;  break;
        case Field::BONUSREMOTE_HIDDEN:  f=Field::BONUSREMOTE; break;
        default: break;
    };

    if(f != g) 
    {
        field->set(pos.first,pos.second,f);
        return true; 
    }
    else return false;
}

bool Explosion::getExtendedCollision(vector<Extended_collision_base*> * v)
{
    bool b=false;
    vector<Part>::iterator i=parts.begin();
    while(i != parts.end())
    {
        if(Extended_collision_base::getExtendedCollision(field,(*i).pos,(*i).rect,v))
            b=true;
        i++;
    }
    return b;
}
