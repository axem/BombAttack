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
#include "counter.h"

Object::Animation * Counter::digits=0;
bool Counter::loaded=false;

void Counter::loadRes()
{
    if(!loaded)
    {
        digits= new Object::Animation;
        digits->addRes(strs("0.png","1.png","2.png","3.png","4.png","5.png","6.png","7.png","8.png","9.png",0),0);
       loaded=true;
    }
}

void Counter::unloadRes()
{
    if(loaded)
    {
        digits->unload();
        delete digits;
        loaded=false;
    }
}


void Counter::setLen(unsigned _len)
{
    len=_len;
}

void Counter::setValue(int _value)
{
    value=_value;
}

int Counter::getValue()
{
    return value;
}

Counter::Counter(int _value, unsigned _len )
: marginX(normalize(10,10).first), value(_value), len(_len)
{
    loadRes();
}

Counter::~Counter()
{
}

static int pow (int base, int pow)
{
    int a=1;
    for(int i=0; i<pow; i++) a*=base;
    return a;
}

int Counter::getDigit(int i)
{
    return ( value / pow(10,i) ) % 10;
}

void Counter::draw()
{
    Tex ** label = new Tex*[len];

    float xx=x,yy=y;
    for(int i=len-1; i>=0; i--)
    { 
        label[i]=digits->get(getDigit(i));
        label[i]->draw(xx,yy);
        xx+=label[i]->getSize().first-marginX;
    }
    delete[] label;
}

void Counter::setPos(float _x, float _y)
{
    x=_x;
    y=_y;
}

void Counter::setPos(const PointFloat& f)
{
    x=f.first;
    y=f.second;
}

void AverageCounter::setValue(int _value)
{
    value=0;
    average.push_back(_value);
    if(average.size() > (size_t)average_len) average.pop_front();
    std::deque<int>::iterator it;
    it=average.begin();
    while(it != average.end())
    {
        value+=*it;
        it++;
    }
    value/=average.size();
}

AverageCounter::AverageCounter(int value, unsigned len, int _average_len )
: Counter(value,len), average_len(_average_len)
{
}

AverageCounter::~AverageCounter()
{
}


