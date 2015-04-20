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
#include "gameover.h"
#include "menu.h"
#include "sound.h"
#include <fstream>

void GameOver::save(unsigned x)
{
    std::fstream f;
    f.open(highscorefilename.c_str(), std::fstream::out | std::fstream::trunc | std::fstream::binary );
    if(f.is_open())
    {
        f.write((char*)&x,sizeof (unsigned));
    }
}

unsigned GameOver::load()
{
    std::fstream f;
    f.open(highscorefilename.c_str(), std::fstream::in | std::fstream::binary );
    if(f.is_open())
    {
        unsigned x=0;
        f.read((char*)&x,sizeof (unsigned));
        return x;
    }
    else return 0;
}


GameOver::GameOver(const string& _tlo)
: drawScores(true), drawEnter(true), tlo(_tlo), enter("enter.png")
{
    score_cnt.setPos(normalize(340,315));
    highscore_cnt.setPos(normalize(340,394));
    score_cnt.setLen(10);
    highscore_cnt.setLen(10);
    highscore_cnt.setOneAddTime(1);

    score_cnt.setValue(my_score);
    enterTime=SDL_GetTicks();
    enterVisible=true;

    unsigned int highscore=load();
    highscore_cnt.setValue(highscore);
    if(highscore<my_score)
        highscore_cnt.addValue(my_score-highscore);
    highscore=max(highscore,my_score);
    save(highscore);
}

GameOver::~GameOver()
{
}

void GameOver::onPaint()
{
    tlo.draw(0,0);
    static PointFloat x=normalize((800-303)/2,480);
    const Uint32 enterTimeMax=750;
    if(SDL_GetTicks()-enterTime >=enterTimeMax)
    {
        enterTime=SDL_GetTicks();
        enterVisible=!enterVisible;
    }
    if(drawEnter && enterVisible)
        enter.draw(x);
    if(drawScores)
    {
        glColor4f(1,0.6,0,1);
        highscore_cnt.draw();
        glColor4f(1,1,1,1);
        score_cnt.draw();
    }
}

void GameOver::unload()
{
    tlo.unload();
    enter.unload();
    highscore_cnt.unload();
    score_cnt.unload();
}

void GameOver::onEnter(bool pressed)
{
    if(pressed)
    {
        playSND(SND_MENUENTER);
        Game::showLoading();
        unload();
        Game::setEvents(new Menu);
    }
}
