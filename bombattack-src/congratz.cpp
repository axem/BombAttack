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
#include "congratz.h"
#include <fstream>

void Congratz::onPaint()
{
    Uint32 elapsed=SDL_GetTicks()-gzTime;
    const Uint32 gz1TimeMax=3000;
    const Uint32 gz1TimeStopMax=1000;
    const Uint32 gz2TimeMax=3000;
    const Uint32 gz2TimeStopMax=3000;
    const Uint32 gzScoreTime=3000;
    switch(gzAnim)
    {
        case gz1Up:
            if(elapsed < gz1TimeMax)
            {
                gz1p.second = srcY- (float)elapsed/gz1TimeMax*offsetY;
            }
            else
            {
                gzTime=SDL_GetTicks();
                if(srcY == scrHeight)
                {
                    gzAnim=gz1Stop;
                    srcY-=offsetY;
                }
                else
                {
                    gzAnim=gz2Up;
                    srcY=scrHeight;
                    offsetY=scrHeight/2+gz2.getSize().second/2;
                }
            }
        break;

        case gz1Stop:
            if(elapsed < gz1TimeStopMax)
            {
            }
            else
            {
                gzAnim=gz1Up;
                gzTime=SDL_GetTicks();
                offsetY+=gz1.getSize().second;
            }
        break;

        case gz2Up:
            if(elapsed < gz2TimeMax)
            {
                gz2p.second = srcY- (float)elapsed/gz2TimeMax*offsetY;
            }
            else
            {
                gzTime=SDL_GetTicks();
                if(srcY == scrHeight)
                {
                    gzAnim=gz2Stop;
                    srcY-=offsetY;
                }
                else
                {
                    gzAnim=gzScore;
                    srcY=scrHeight;
                    offsetY=scrHeight/2;
                }
            }
        break;

        case gz2Stop:
            if(elapsed < gz2TimeStopMax)
            {
            }
            else
            {
                gzAnim=gz2Up;
                gzTime=SDL_GetTicks();
                offsetY+=gz2.getSize().second;
            }
        break;

        case gzScore:
            if(elapsed < gzScoreTime)
            {
                drawScores=true;
                float tmp=normalize((800-160)/2,315).first;
                float tmp2=normalize(1,79).second;
                score_cnt.setPos(tmp,
                    srcY- (float)elapsed/gzScoreTime*offsetY);
                highscore_cnt.setPos(tmp,
                    srcY- (float)elapsed/gzScoreTime*offsetY
                    +tmp2);
            }
            else
            {
                drawEnter=true;
                drawScores=true;
                gzAnim=gzScoreStop;
            }
        break;

        case gzScoreStop:
        break;
        default: break;
    };
    GameOver::onPaint();
    gz1.draw(gz1p);
    gz2.draw(gz2p);
}

Congratz::Congratz()
: GameOver("theend.png"), offsetY(0), srcY(0), gzAnim(gz1Up), gzTime(SDL_GetTicks()), gz1("congratulations1.png"), gz2("congratulations2.png"), gz1p(0,scrHeight), gz2p(0,scrHeight)
{
    gz1p.first=scrWidth/2-gz1.getSize().first/2;
    gz2p.first=scrWidth/2-gz2.getSize().first/2;
    offsetY=scrHeight/2+gz1.getSize().second/2;
    srcY=scrHeight;
    drawScores=false;
    drawEnter=false;
    score_cnt.setPos(normalize(340,315).first,scrHeight);
    highscore_cnt.setPos(normalize(340,315).first,scrHeight);

    std::fstream f;
    f.open(savefilename.c_str(),std::fstream::out | std::fstream::binary | std::fstream::trunc);
    if(f.is_open())
    {
        int x=-1;
        f.write(reinterpret_cast<char*>(&x),sizeof(int));
    }
    f.close();
}

Congratz::~Congratz()
{
}

void Congratz::unload()
{
    GameOver::unload();
    gz1.unload();
    gz2.unload();
}
