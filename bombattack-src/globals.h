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
#ifndef GLOBALS_H
#define GLOBALS_H

#include "game.h"
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <cstdarg>
#include <iostream>

typedef std::pair<int,int> PointInt;
typedef std::pair<float,float> PointFloat;
typedef std::pair<PointFloat,PointFloat> RectFloat;

class Game;

extern float scrHeight;
extern float scrWidth;
extern int wndWidth;
extern int wndHeight;
extern int bestWndWidth;
extern int bestWndHeight;
extern const std::string lvl_prefix;
extern const float blockWidth;
extern const float blockHeight;
extern unsigned my_score;
extern bool isSound;
extern bool isFullscreen;
extern int VideoBitsPerPixel;
extern bool debug;
extern bool nosound;
extern const std::string savefilename;
extern const std::string highscorefilename;

PointFloat normalize(int w,int h);
PointFloat normalize(PointInt x);

#ifdef __LINUX__
void prepare_linux();
#endif

template<typename T>
T abs(const T& t)
{
    if(t>=0) return t; else return -t;
}

template<typename T>
inline T max(const T& a, const T& b)
{
    if( a > b)
        return a; else return b;
}

template<typename T>
inline T min(const T& a, const T& b)
{
    if( a < b)
        return a; else return b;
}

template<typename T>
void purge(T &t)
{
    typename T::iterator i=t.begin();
    while(i != t.end())
      delete *(i++);
    t.clear();
}

class strings
{
    int _size;
    std::vector<std::string> items;
public:
    strings(const char *, ... );
    ~strings() {};
    int size();
    std::string &operator[](int index);
};

template<class TYPE> 
TYPE & stringTo(const std::string &val)
{
	std::ostringstream s(val);
	TYPE t;
	s>>t;
	return t;
}


template<class TYPE>
std::string toString(const TYPE &val)
{
	std::ostringstream oss;
	oss << val;
	return oss.str();
}

std::string changeExt(std::string s,const std::string& newext);

typedef strings strs;
using std::endl;
using std::cout;
using std::string;
using std::vector;
using std::list;
#endif
