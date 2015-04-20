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
#ifndef MENU_H
#define MENU_H

#include "events.h"
#include "texture.h"
#include "counter.h"

class Menu : public Events{

    static bool last_is_instructions;
    Tex tlo;
    Tex button1;
    Tex button2;
    Tex button3;
    Tex button4;
    Counter cnt;
    int max_lvl;
    static int getSaved_max_lvl();
protected:
    virtual void onEnter(bool pressed);
    virtual void onPaint();
    virtual void onEsc(bool pressed);
    virtual void onArrow(arrow_type,bool pressed);
    virtual void onSpace(bool pressed);

    void selectionChanged(int);
    int selected;
public:
    virtual void unload();
    Menu(bool playmus=true);
    virtual ~Menu();
};


#endif
