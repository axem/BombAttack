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
#ifndef COUNTER_H
#define COUNTER_H

#include <deque>
#include "object.h"


class Counter
{
    const float marginX;
    static bool loaded;
protected:
    float x;
    float y;
    int value;
    int len;
    static Object::Animation * digits;
    inline int getDigit(int);
public:
    static void loadRes();
    static void unloadRes();
    void setPos(float x, float y);
    void setPos(const PointFloat&);
    void setLen(unsigned len);
    virtual void setValue(int _value);
    virtual int getValue();
    void draw();
    Counter(int value=0, unsigned len=3 );
    virtual ~Counter();
    virtual void unload() {};
};

class AverageCounter : public Counter
{
protected:
    std::deque<int> average;
    int average_len;
public:
    virtual void setValue(int _value);
    AverageCounter(int value=0, unsigned len=3, int average_len=5 );
    virtual ~AverageCounter();
};
#endif
