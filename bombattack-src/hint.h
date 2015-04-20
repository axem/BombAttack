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
#ifndef HINT_H
#define HINT_H
#include "texture.h"

class Hint
{
    static const Uint32 timeMax;
    static Tex * v50;
    static Tex * v100;
    static Tex * v250;
    static Tex * v1000;
    static bool preloaded;
    PointFloat startPos;
    Tex * t;
    Uint32 time;
public:
    enum Value { V50, V100, V250, V1000 };
    static void preloadRes();
    static void unloadRes();
    Hint(const PointFloat & startPos, Value);
    ~Hint();
    void draw();
    bool ended();
};

#endif
