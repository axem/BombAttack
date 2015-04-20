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
#include "menu.h"
#include "myfield.h"
#include "instructions.h"
#include "credits.h"
#include "sound.h"
#include "level_loader.h"
#include <fstream>


bool Menu::last_is_instructions=false;

Menu::Menu(bool playmus)
: tlo("menu.png"), button1("button1.png"), button2("button2.png"), button3("button3.png"), button4("button4.png"), selected(1)
{
    if(!last_is_instructions && playmus)
    playMenuMusic();
        else last_is_instructions=false;
    cnt.setPos(normalize(220+284,+350+76));

    max_lvl=min(LevelLoader::get().count(),getSaved_max_lvl());
    cnt.setLen(2);
    cnt.setValue(max_lvl);
}

int Menu::getSaved_max_lvl()
{
    std::fstream sload;
    size_t len=0;
    int x=0;
    sload.open(savefilename.c_str(), std::fstream::in | std::fstream::binary );
    if(sload.is_open())
    {
        sload.seekg(0, std::ios::end);
        len=sload.tellg();
        sload.seekg(0, std::ios::beg);
        if(len >= sizeof(int))
        sload.read(reinterpret_cast<char*>(&x),sizeof(int));
        if(x==-1) return LevelLoader::get().count();
        return x+1;
    }
    sload.close();
    return 1;
}

Menu::~Menu()
{
}

void Menu::onSpace(bool pressed)
{
    onEnter(pressed);
}

void Menu::onEnter(bool pressed)
{
    if(pressed)
    {
        playSND(SND_MENUENTER);
        switch(selected)
        {
            case 1: Game::showLoading(); unload(); Game::setEvents(new MyField); break;
            case 2: Game::showLoading(); unload(); Game::setEvents(new MyField(cnt.getValue()-1)); break;
            case 3: last_is_instructions=true; unload(); Game::setEvents(new Instructions); break;
            case 4: unload(); Game::setEvents(new Credits); break;
            default: break;
        };
    }
}

void Menu::onPaint()
{
    tlo.draw(0,0);
    switch(selected){
        case 1: button1.draw(normalize(220,350)); break;
        case 2: button2.draw(normalize(220,350)); break;
        case 3: button3.draw(normalize(220,350)); break;
        case 4: button4.draw(normalize(220,350)); break;
        default: break;
    };
    cnt.draw();
}

void Menu::onArrow(arrow_type t,bool pressed)
{
    if(pressed)
    switch(t)
    {
        case(UP):    selectionChanged(-1); break;
        case(DOWN):  selectionChanged(+1); break;
        default: break;
    };

    if(pressed && selected==2)
    {
        int offset=0;
        switch(t)
        {
            case(LEFT):  offset=-1; break;
            case(RIGHT): offset=+1; break;
            default: break;
        }
        int x= cnt.getValue()+offset;
        if(x >=1 && x <=max_lvl) cnt.setValue(x);
    }
}


void Menu::selectionChanged(int progress)
{
    playSND(SND_MENUCLICK);
    selected+=progress;
    selected%=4;
    if(selected <=0) selected=4;
}

void Menu::unload()
{

    cnt.unload();
    tlo.unload();
    button1.unload();
    button2.unload();
    button3.unload();
    button4.unload();

}

void Menu::onEsc(bool pressed)
{
    if(pressed)
    {
	if(selected == 4)
		onEnter(pressed);
	else
	{
	        playSND(SND_MENUCLICK);
	        selected=4;
	}
    }
}
