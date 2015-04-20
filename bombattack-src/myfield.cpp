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
#include "myfield.h"
#include "menu.h"
#include "gameover.h"
#include "game.h"
#include <deque>
#include <fstream>

#include "sound.h"
#include "congratz.h"

MyField::MyField(int levelToContinue)
: explodingTime(0), leveltoLoad(-1), waitForRespawn(0), waitForGameOver(0), maxBombs(3), spawning(0), visibleMan(false), spawnAnim_pos(0,0), exitAnim(0), remoteBomb(false), altPressed(false), cameraZoomDest(1), zoomStep(0.01), zoomStep_ms(20), cameraZoomMin(0.5), cameraZoomMax(1.5), zoomLastTime(0),  bonusRunning(0), lastCheatTry(SDL_GetTicks()), cheatbuf(""), godTime(0), minus_pressed(false), plus_pressed(false), man(*this), ctrl("ctrl.png")
{
    width_zoom=1.5;
    height_zoom=1.5;
    man.setField(this);
    score.setLen(10);
    score.setPos(normalize(bestWndWidth-165-10, 10));
    lifes.setPos(normalize(96+10,10));
    lifes.setValue(3);
    PointFloat f=normalize((54-120)/2,100);
    levelCounter.setPos(scrWidth/2-f.first-normalize(20,1).first,f.second);
    Explosion::len=1;
    if(levelToContinue != -1)
    {
        level_num=levelToContinue;
        int lifes_tmp=3;
        int score_tmp=0;
        int remoteBomb_tmp=0;
        loadSavedGame(level_num, lifes_tmp, maxBombs, Explosion::len, remoteBomb_tmp);
            remoteBomb=remoteBomb_tmp;
            remoteBoom=false;
        lifes.setValue(lifes_tmp);
        score.setValue(score_tmp);
    }

    load(level_num);
    Bomb::preloadRes();
    Hint::preloadRes();

    bombsCounter.setPos(normalize(146+96+5, 10));
    bombsCounter.setValue(maxBombs);

    Enemy::preloadRes();
    /*************************************************************************/
    string spawn_base="spawn0000";
    string spawn_s="";
    int spawn_ms=25;
    for(int i=1; i<=17; i++)
    {
        spawn_s=toString(i);
        spawn_base.replace(spawn_base.size()-spawn_s.size(),spawn_s.size(),spawn_s);
        spawnAnim.addFrame(new Object::Animation::Frame(spawn_base+".png",spawn_ms));
    }
    /*************************************************************************/

    Explosion::preloadRes();

    ash.addRes(strs("deathdead.png",0),0);


    rangeCounter.setPos(normalize(388, 10));
    rangeCounter.setValue(Explosion::len);

    wall.reset();
    brick.reset();

    playGameMusic();
}


MyField::~MyField()
{
    purge(enemies);
    purge(bombs);
    purge(explosions);
    purge(ashes);
    purge(burns);
    purge(hints);
}

void MyField::onEsc(bool pressed)
{
    if(pressed)
    {
        recentre();
        Game::showLoading();
        unload();
        Game::setEvents(new Menu);
    }
}

void MyField::onArrow(arrow_type t, bool pressed)
{
    static arrow_type last=DOWN;
    if(pressed && man.alive && visibleMan && !exitAnim)
    {
        switch(t)
        {
            case(LEFT):  man.moveMe(Man::LEFT);   last=t; break;
            case(RIGHT): man.moveMe(Man::RIGHT);  last=t; break;
            case(UP):    man.moveMe(Man::UP);     last=t; break;
            case(DOWN):  man.moveMe(Man::DOWN);   last=t; break;
            default: break;
        };
    }
    if(!pressed && last==t && !exitAnim) man.moveMe(Man::NONE);
}

void MyField::onPaint()
{
    space.draw();

    makeCameraZoom();
    make_zoom();
    centre(man.getPos(),man.getSize());
    makeExploding();
    Field::onPaint();
    makeEnemies();
    if(man.alive)
    {
        if(!waitForRespawn) man.make();
        if(!spawning && man.getExtendedCollision() && !exitAnim)
            onHurt();
    }
    makeAndDrawBombs();

    if(spawning)
    {
        if(spawning == 1)
        {
            spawning=SDL_GetTicks();
            spawnAnim.reset();
            spawnAnim_pos.first=man.getTablePos().first*blockWidth;
            spawnAnim_pos.second=man.getTablePos().second*blockHeight;
        }
        const Uint32 spawingMax=425;
        if(SDL_GetTicks()-spawning >= spawingMax)
        {
            spawning=0;
        }
        const Uint32 showmanTime=200;
        if(SDL_GetTicks()-spawning >= showmanTime)
        {
            visibleMan=!exitAnim;
            if(godTime==1)
            {
                godTime=SDL_GetTicks();
                man.alive=true;
                man.idleDown.start();
                man.setAnim(man.idleDown);
                bonusRunning=false;
                man.setSpeed(250);
            }

        }
    }
    checkBonuses();
    makeCheatTime();
    makeBonusRun();

    if(exitAnim)
        makeExitAnim();

    makeGod();

    if(spawning)
    {
        Tex * t= spawnAnim.current();
        if(t) t->draw(spawnAnim_pos);
    }

    drawEnemies();
    makeAshes();
    makeExplosions();
    makeBurn();

    makeHints();

    if(!godTime && !waitForRespawn && man.alive)
        makeAI();

    recentre();
    score.draw();
    lifes.draw();
    levelCounter.draw();
    bombsCounter.draw();
    rangeCounter.draw();

    if(remoteBomb)
    {
        static PointFloat ctrl_pos=normalize(433,2);
        ctrl.draw(ctrl_pos);
    }

    if(waitForRespawn)
    {
        const Uint32 respawnTime=3000;
        if(SDL_GetTicks()-waitForRespawn >= respawnTime)
        {
            if(!spawning) spawning=1;
            if(!godTime) godTime=1;
            waitForRespawn=0;
        }
    }


    if(waitForGameOver)
    {
        const Uint32 waitForGameOverMax=3000;
        if(SDL_GetTicks()-waitForGameOver >= waitForGameOverMax)
        {
            makeGameOver();
            waitForGameOver=0;
        }
    }

    if(leveltoLoad != -1) load(leveltoLoad,true);
}

void MyField::drawEnemies()
{
    std::vector<Enemy*>::iterator i=enemies.begin();
    while(i != enemies.end())
    {
        Enemy * e=*i;
        if(e && e->alive)
           if(isVisible(e->getTablePos())) e->draw();
        i++;
    }
}
void MyField::makeEnemies()
{
    std::vector<Enemy*>::iterator i=enemies.begin();
    while(i != enemies.end())
    {
        Enemy * e=*i;
        if(e)
        {
            if(e->alive && e->wakeup && explosions.size()==0)
            {
                e->wakeup=false;
                e->moveMe(e->randomMove());
            }
            if(e->alive)
            e->make();
            else
            {
                playSND(SND_ENEMYHURT);
                delete e;
                *i=0;
            }
        }
        i++;
    }
}

void MyField::makeAndDrawBombs()
{
    if(bombs.size()==0) return;
    for(vector<Bomb*>::iterator i=bombs.begin(); i<bombs.end(); ++i)
    {
        Bomb *& b=*i;
        if(b)
        if( (!remoteBomb && b->isExploded() ) || ( remoteBomb && remoteBoom ) || ( remoteBomb && b->getRemoteExploded()) )
        {
                remoteBoom=false;
                explodingTime=SDL_GetTicks();
                onBoom(b->getTablePos());

                int x=bombsCounter.getValue();
                if(x+1 <= maxBombs)
                bombsCounter.setValue(x+1);

                delete b;
                b=0;
        }
        else
            b->draw();
    }

    for(vector<Bomb*>::iterator i=bombs.end()-1; i>=bombs.begin(); --i)
        if(!(*i)) bombs.erase(i);
    if(bombs.size()==0)
    {
        remoteBoom=false;
    }
}

void MyField::onSpace(bool pressed)
{
    if(pressed && man.alive && !exitAnim && !spawning)
    {
        int x=bombsCounter.getValue();
        if(x == 0 ) return;
        bombsCounter.setValue(x-1);
        PointInt p=man.getTablePos();
        if(man.isMoving() && man.isObjectOnDestPos())
        {
            Man::move_type m=man.getCurrentMove();
            switch(m)
            {
                case Man::UP:    p.second+=1;   break;
                case Man::DOWN:  p.second-=1;   break;
                case Man::LEFT:  p.first+=1;    break;
                case Man::RIGHT: p.first-=1;    break;
                default: break;
            }
        }

        bombs.push_back(new Bomb(p));
        man.head=false;
        Man::Animation * a=man.getAnim();
        if(a == &(man.left)  || a == &(man.idleLeft)  )  man.setAnim(man.headLeft);
            else
        if(a == &(man.right) || a == &(man.idleRight) ) man.setAnim(man.headRight);
            else
        if(a == &(man.up)    || a == &(man.idleUp) )    man.setAnim(man.headUp);
            else
        if(a == &(man.down)  || a == &(man.idleDown) )  man.setAnim(man.headDown);
    }
}

void MyField::onBoom(const PointInt& tablePos)
{
    playSND(SND_BOOM);
    Explosion * e = new Explosion(this,tablePos);
    e->startTime=SDL_GetTicks();

    vector<Explosion::Part>::iterator itr=e->parts.begin();
    while(itr != e->parts.end())
    {
        for(vector<Bomb*>::iterator btr=bombs.end()-1; btr>=bombs.begin(); --btr)
        {
            Bomb* b=*btr;
            if(b)
            {
                PointInt a=b->getTablePos();
                if(a.first == (*itr).pos.first && a.second == (*itr).pos.second) b->setRemoteExploded();
            }
        }
        ++itr;
    }
    explosions.push_back(e);
}

void MyField::makeExploding()
{
    if(explodingTime)
    {
        const unsigned quakeScale=60;
        int xi=quakeScale - rand() % (quakeScale*2);
        int yi=quakeScale - rand() % (quakeScale*2);
        float xf=(scrWidth/quakeScale)*(float)xi/quakeScale;
        float yf=(scrHeight/quakeScale)*(float)yi/quakeScale;

        glTranslatef(xf,yf,0);
        const Uint32 explodingTimeMax=300;
        if(SDL_GetTicks()-explodingTime >=explodingTimeMax) explodingTime=0;
    }
}

void MyField::load(int num, bool immediately)
{
    if(immediately)
    {
        cheatbuf="";
        purge(enemies);
        purge(bombs);
        purge(explosions);
        purge(ashes);
        purge(hints);
        purge(burns);
        Field::load(num);

        waitForRespawn=0;
        waitForGameOver=0;
        godTime=0;

        width_zoom=1.5;
        height_zoom=1.5;
        cameraZoomDest=1;


        explodingTime=0;
        isCameraNear=true;
        leveltoLoad=-1;
        altPressed=false;
        man.reset_extended_collision();
        man.setTablePos(man_x,man_y);
        man.alive=true;
        man.idleDown.start();
        man.setAnim(man.idleDown);
        visibleMan=false;
        bonusRunning=false;
        man.setSpeed(250);
        bombsCounter.setValue(maxBombs);
        for(int x=0; x<table.width; x++)
        for(int y=0; y<table.height; y++)
        if(table.table[x+y*table.width] == ENEMY_SPAWN)
        {
            Enemy * e = new Enemy;
            e->setField(this);
            e->setTablePos(x,y);
            if(enemy_rand_speed >0)
            e->setSpeed(rand() % enemy_rand_speed+enemy_base_speed);
                else e->setSpeed(enemy_base_speed);
            enemies.push_back(e);
            table.table[x+y*table.width]=GRASS;
        }
        levelCounter.setValue(num+1);
        levelCounter.show();
        leveltoLoad=-1;
        if(!spawning) spawning=1;

        brick.reset();
        wall.reset();
    }
    else
    {
        leveltoLoad=num;
    }
}

void MyField::onHurt()
{
    if(exitAnim) return;
    if(!man.alive) return;
    if(godTime) return;
    playSND(SND_BOMBERHURT);
    remoteBomb=false;

    man.moveMe(Man::NONE);
    man.setAnim(man.dead);
    man.alive=false;

    if(lifes.getValue()==0)
    {
        waitForGameOver=SDL_GetTicks();
    }
    else
    {
        waitForRespawn=SDL_GetTicks();
        lifes.setValue(lifes.getValue()-1);
        if(Explosion::len>1) Explosion::len-=1;
        rangeCounter.setValue(Explosion::len);
        if(maxBombs>3) maxBombs-=1;
        if(bombsCounter.getValue()>3) bombsCounter.setValue(bombsCounter.getValue()-1);
    }
}

void MyField::unload()
{
    Field::unload();
    purge(enemies);
    purge(ashes);
    purge(burns);
    purge(hints);
    man.unload();
    score.unload();
    lifes.unload();
    levelCounter.unload();
    purge(bombs);
    purge(explosions);
    Bomb::unloadRes();
    Enemy::unloadRes();
    bombsCounter.unload();
    spawnAnim.unload();
    Explosion::unloadRes();
    ash.unload();
    ctrl.unload();
    space.unload();
    Hint::unloadRes();
    rangeCounter.unload();
}

void MyField::makeGameOver()
{
    recentre();
    my_score=score.getValue();
    unload();
    Game::setEvents(new GameOver);
}

void MyField::makeExplosions()
{
    if(explosions.size()==0) return;
    for(vector<Explosion*>::iterator i=explosions.end()-1; i>=explosions.begin(); i--)
    {
        bool b=false;
        if(*i)
        {
            b=true;
            const Uint32 explodingTimeMax=300;
            if(SDL_GetTicks()-(*i)->startTime >=explodingTimeMax)
            {
                delete (*i);
                explosions.erase(i);
                b=false;
            } else
            (*i)->draw();
        }
        if(b)
        {
            vector<Extended_collision_base*>  v;
            if((*i)->getExtendedCollision(&v))
            {
                vector<Extended_collision_base*>::iterator itr=v.begin();
                while(itr != v.end())
                {
                    Man * m = dynamic_cast<Man*>(*itr);
                    if(m && m->alive)
                    {
                        if(!godTime)
                        {
                            Tex * t=0;
                            if(man.getAnim()) t=man.getAnim()->current();

                            if(t)
                                burns.push_back(new Burn(SDL_GetTicks(),man.getPos(),t,false));

                            onHurt();
                            m->setAnim(ash);
                        }
                    }
                    Enemy * e = dynamic_cast<Enemy*>(*itr);
                    if(e && e->alive /* && !e->wakeup*/)
                    {
                        e->alive=false;
                        playSND(SND_ENEMYHURT);
                        const PointFloat & f=e->getPos();
                        //ashes.push_back(new Ash(SDL_GetTicks(),f));
                        Tex * t=0;
                        if(e->getAnim()) t=e->getAnim()->current();
                        if(t)
                            burns.push_back(new Burn(SDL_GetTicks(),f,t,true));
                        hints.push_back(new Hint(f,Hint::V100));
                        score.addValue(100);
                    }
                    ++itr;
                }
            }
        }
    }
}

void MyField::wakeupEnemy(int x, int y)
{
    if(x<0) x=0; if(x>=table.width) x=table.width-1;
    if(y<0) y=0; if(y>=table.height) y=table.height-1;
    std::list<Extended_collision_base*> * list = table.extended_collision[x+y*table.width];
    if(!list) return;
    std::list<Extended_collision_base*>::iterator itr=list->begin();
    while(itr != list->end())
    {
        Enemy * e = dynamic_cast<Enemy*>(*itr);
        if(e && !e->isMoving())
        {
            e->wakeup=true;
        }
        ++itr;
    }
}

void MyField::set(int x, int y, field_type e)
{
    field_type last=get(x,y);
    Field::set(x,y,e);
    wakeupEnemy(x-1,y); wakeupEnemy(x+1,y);
    wakeupEnemy(x,y-1); wakeupEnemy(x,y+1);
    Hint::Value v=Hint::V50;
    int v2=50;
    bool ok=true;
    switch(last)
    {
        case BONUSBOMB_HIDDEN:
        case BONUSRUN_HIDDEN:
        case BONUSLIFE_HIDDEN:
        case BONUSRANGE_HIDDEN:
        case BONUSREMOTE_HIDDEN:
        case DOOR_HIDDEN:
        case BRICK:
            v=Hint::V50;
            v2=50;
            playSND(SND_BRICK);
            if(brick.current())
                burns.push_back(new Burn(SDL_GetTicks(),PointFloat((float)x*blockWidth,(float)y*blockHeight),brick.current(),false));
        break;

        case BONUSBOMB:
        case BONUSRUN:
        case BONUSLIFE:
        case BONUSRANGE:
        case BONUSREMOTE: v=Hint::V250; v2=250; break;

        case DOOR: v=Hint::V1000; v2=1000; break;

        case DOOR_OPEN:
            ok=false;
        break;
        default: break;
    }

    if(ok)
    {
        hints.push_back(new Hint(PointFloat(x*blockWidth,y*blockHeight),v));
        score.addValue(v2);
    }
}

void MyField::checkBonuses()
{
    int x=man.getTablePos().first;
    int y=man.getTablePos().second;
    field_type e=table.table[x+y*table.width];
    if(e == DOOR || e == BONUSBOMB || e == BONUSLIFE || e == BONUSRANGE || e == BONUSRUN || e == BONUSREMOTE)
    if(Extended_collision_base::isCollisionRectRect(man.getRect(),
        RectFloat(
            PointFloat(x*blockWidth,y*blockHeight),
            PointFloat((x+1)*blockWidth,(y+1)*blockHeight)
    )))
    {
    playSND(SND_BONUS);
    switch(e)
    {
        case DOOR:
            set(x,y,DOOR_OPEN);
            man.moveMe(Man::NONE);
            exitAnim=1;
            bonusRunning=0;
        break;

        case BONUSBOMB:
            set(x,y,GRASS);
            if(maxBombs<99)
            maxBombs+=1;
            bombsCounter.setValue(bombsCounter.getValue()+1);
        break;

        case BONUSLIFE:
            set(x,y,GRASS);
            if(lifes.getValue()<99)
            lifes.setValue(lifes.getValue()+1);
        break;

        case BONUSRANGE:
            set(x,y,GRASS);
            if(Explosion::len<99)
            Explosion::len+=1;
            rangeCounter.setValue(Explosion::len);
        break;

        case BONUSRUN:
            set(x,y,GRASS);
            bonusRunning=SDL_GetTicks();
        break;

        case BONUSREMOTE:
            set(x,y,GRASS);
            remoteBomb=true;
            remoteBoom=false;
        break;

        default: break;
    }
    }
}

void MyField::makeExitAnim()
{
    if(!man.isMoving())
    {
        if(exitAnim==1) exitAnim=SDL_GetTicks();
        if(!spawning)
        {
            visibleMan=false;
            spawning=1;
        }
        man.getAnim()->stop();
        const Uint32 exitAnimMax=500;
        if(SDL_GetTicks()-exitAnim >= exitAnimMax)
        {
            spawning=0;
            LevelLoader &x=LevelLoader::get();
            if(level_num+1<x.count())
            {
                ++level_num;
                saveGame();
                load(level_num);
            }
            else
                {
                    makeCongratz();
                }
            exitAnim=0;
        }
    }
}

void MyField::onCtrl(bool pressed)
{
    if(man.alive && remoteBomb && pressed && bombs.size() != 0) remoteBoom=true;
}

void MyField::makeAshes()
{
    if(ashes.size()==0) return;
    vector<Ash*>::iterator itr=ashes.end()-1;
    while(itr >= ashes.begin())
    {
        const Uint32 ashTimeMax=1000;
        Ash *& a=*itr;
        if(a)
        {
            if(SDL_GetTicks()-a->time >=ashTimeMax)
            {
                delete a;
                a=0;
                ashes.erase(itr);
            }
            else
                a->draw(a->pos);
        }
        --itr;
    }
}

void MyField::makeHints()
{
    if(hints.size()==0) return;
    vector<Hint*>::iterator itr=hints.end()-1;
    while(itr >= hints.begin())
    {
        Hint *& h=*itr;
        if(h)
        {
            if(!h->ended())
                h->draw();
            else
               {
                    delete h;
                    h=0;
                    hints.erase(itr);
               }
        }
        --itr;
    }
}

void MyField::makeBonusRun()
{
    typedef std::pair<Tex*, PointFloat> P;
    static std::deque<P> ts;
    static Uint32 lastElapsedMS=0;
    if(bonusRunning)
    {
        if(man.getAnim()==0) return;
        Uint32 elapsed=SDL_GetTicks()-bonusRunning;
        const Uint32 interval=15;
        const Uint32 count=10; //20
        if(lastElapsedMS != elapsed/interval)
            ts.push_back(P(man.getAnim()->current(),man.getPos()));
        if(ts.size() > count) ts.pop_front();
        man.setSpeed(100);
        const Uint32 bonusRunningMax=30000;

        std::deque<P>::iterator itr=ts.begin();

        const float color4start = 0.7;
        Uint32 itr_i=0;

        if(godTime)
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        while(itr != ts.end())
        {
            if(itr_i < count ) itr_i++;
            float x=color4start*((float)itr_i/count);
            glColor4f(1,1,1, x );
            Tex * t=(*itr).first;
            if(t)
                t->draw((*itr).second);
            ++itr;
        }

        if(godTime)
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(1,1,1,1);
        if(elapsed >= bonusRunningMax)
        {
            man.setSpeed(250);
            bonusRunning=0;
            lastElapsedMS=0;
            ts.clear();
        }
        lastElapsedMS=elapsed/interval;
    }

    if(bonusRunning==0 &&  ts.size() != 0)
    {
        man.setSpeed(250);
        lastElapsedMS=0;
        ts.clear();
    }
}

void MyField::makeCameraZoom()
{
    if(cameraZoomDest != width_zoom)
    {
        if(zoomLastTime==0)
        {
            zoomLastTime=SDL_GetTicks();
        }
        int elapsed=(SDL_GetTicks()-zoomLastTime);
        zoomLastTime=SDL_GetTicks();
        float step=(float)elapsed / (float)zoomStep_ms *  zoomStep;
        if(cameraZoomDest > width_zoom)
            width_zoom+=step;

        if(cameraZoomDest < width_zoom)
            width_zoom-=step;

        if(width_zoom >= cameraZoomMax)  width_zoom=cameraZoomMax;
        if(width_zoom <= cameraZoomMin) width_zoom=cameraZoomMin;
        height_zoom=width_zoom;
        if(abs(width_zoom-cameraZoomDest) < zoomStep)
        {
            zoomLastTime=0;
            if(plus_pressed)
            {
                cameraZoomDest+=zoomStep*10;
                if(cameraZoomDest > cameraZoomMax) cameraZoomDest=cameraZoomMax;
            }
            if(minus_pressed)
            {
                cameraZoomDest-=zoomStep*10;
                if(cameraZoomDest < cameraZoomMin) cameraZoomDest=cameraZoomMin;
            }
        }
    }
}

void MyField::onPlus(bool pressed)
{
    if(pressed)
    {
        cameraZoomDest+=zoomStep*10;
        if(cameraZoomDest > cameraZoomMax) cameraZoomDest=cameraZoomMax;
    }
    plus_pressed=pressed;
}

void MyField::onMinus(bool pressed)
{
    if(pressed)
    {
        cameraZoomDest-=zoomStep*10;
        if(cameraZoomDest < cameraZoomMin) cameraZoomDest=cameraZoomMin;
    }
    minus_pressed=pressed;
}

void MyField::makeAI()
{
    const PointInt& pos = man.getTablePos();
    const int checkLen = 3;
    int x1=pos.first-checkLen; if(x1<0) x1=0;
    int x2=pos.first+checkLen; if(x2>table.width-1) x2=table.width-1;
    int y1=pos.second-checkLen; if(y1<0) y1=0;
    int y2=pos.second+checkLen; if(y2>table.height-1) y2=table.height-1;

    for(int x=x1; x<=x2; x++)
    for(int y=y1; y<=y2; y++)
    {
            int a=x+y*table.width;
            ExCollisionList * l=table.extended_collision[a];
            if(l)
            {
                ExCollisionList::iterator i=l->begin();
                while(i != l->end())
                {
                    Enemy * e = dynamic_cast<Enemy*>(*i);
                    if(e)
                    {
                        const PointInt& ePos=e->getTablePos();
                        Enemy::move_type x=Enemy::NONE;
                        if(ePos.first < pos.first) x=Enemy::RIGHT;
                        if(ePos.first > pos.first) x=Enemy::LEFT;
                        if(ePos.second < pos.second) x=Enemy::DOWN;
                        if(ePos.second > pos.second) x=Enemy::UP;
                        e->setNextEnemyMove(x);

                    }
                    ++i;
                }
            }
    }
}

void MyField::makeCongratz()
{
    recentre();
    my_score=score.getValue();
    unload();
    Game::setEvents(new Congratz);
}

void MyField::loadSavedGame(int level_num, int &lifes, int &maxBombs, int &range, int &remote)
{
    std::fstream f;
    bool won=false;
    f.open(savefilename.c_str(), std::fstream::in | std::fstream::binary );
    if(f.is_open())
    {
        f.seekg (0, std::ios::end);
        size_t len=0;
        len=f.tellg();
        f.seekg (0, std::ios::beg);
        if(len >= sizeof(int))
        {
            int x=0;
            f.read(reinterpret_cast<char*>(&x),sizeof(int));
            if(x==-1) won=true;
        }

        if(!won && len >= sizeof(int)*5)
        {
            f.seekg (sizeof(int)*(level_num)*4+sizeof(int), std::ios::beg);
            f.read(reinterpret_cast<char*>(&lifes),sizeof(int));
            f.read(reinterpret_cast<char*>(&maxBombs),sizeof(int));
            f.read(reinterpret_cast<char*>(&range),sizeof(int));
            f.read(reinterpret_cast<char*>(&remote),sizeof(int));
        }
        else
        {
            lifes=3;
            maxBombs=3;
            range=1;
            remote=0;
        }
        f.close();
    }
    else
    {
        lifes=3;
        maxBombs=3;
        range=1;
        remote=0;
    }

    if(won)
    {
        lifes=20;
        maxBombs=20;
        range=20;
        remote=1;
    }
}

void MyField::saveGame()
{
    size_t sload_len=0;
    char * sload_data=0;
    bool won=false;
    {
        std::fstream sload;
        sload.open(savefilename.c_str(), std::fstream::in | std::fstream::binary );
        if(sload.is_open())
        {
            sload.seekg(0, std::ios::end);
            size_t len=sload.tellg();
            sload.seekg(0, std::ios::beg);
            if(len >=sizeof(int))
            {
                int x = 0;
                sload.read(reinterpret_cast<char*>(&x),sizeof(int));
                if(x==-1) won=true;
            }
            sload.seekg(0, std::ios::beg);
            if(!won && (len >=(sizeof(int)*5)))
            {
                sload_data = new char[len];
                sload.read(sload_data,len);
                sload_len=len;
            }
        }
        sload.close();
    }
    if(!won)
    {
        size_t pos = sizeof(int)*(level_num)*4+sizeof(int);
        std::fstream ssave;
        ssave.open(savefilename.c_str(), std::fstream::out | std::fstream::binary | std::fstream::trunc);
        if(ssave.is_open())
        {
            if(sload_data)
            {
                ssave.write(sload_data,sload_len);
                ssave.seekp(0, std::ios::beg);
                int x=max(level_num,static_cast<int>(*sload_data));
                ssave.write(reinterpret_cast<char*>(&x),sizeof(int));
                ssave.seekp(sload_len, std::ios::beg);
            } else
                ssave.write(reinterpret_cast<char*>(&level_num),sizeof(int));
            if(sload_len < pos)
            {
                int z[4]= { 3 , 3 , 1 , 0 };
                for(size_t i=0; i<(pos-sload_len) / (sizeof(int)*4); i++)
                ssave.write(reinterpret_cast<char*>(z),sizeof(int)*4);
            }
            int lifes_tmp=lifes.getValue();
            ssave.seekp(sizeof(int)*(level_num)*4+sizeof(int), std::ios::beg);
            ssave.write(reinterpret_cast<char*>(&lifes_tmp),sizeof(int));
            ssave.write(reinterpret_cast<char*>(&maxBombs),sizeof(int));
            ssave.write(reinterpret_cast<char*>(&Explosion::len),sizeof(int));
            int remoteBomb_tmp=remoteBomb;
            ssave.write(reinterpret_cast<char*>(&remoteBomb_tmp),sizeof(int));
        }
        ssave.close();
    }
    delete[] sload_data;

}


void MyField::onNamedKey(bool pressed, char x)
{
    if(pressed)
    {
        lastCheatTry=SDL_GetTicks();
        static size_t buf_len=20;
        cheatbuf+=x;
        if(cheatbuf.size() && cheatbuf.size() > buf_len ) cheatbuf.erase(cheatbuf.begin());
        if(cheatbuf.find("next") != string::npos)
        {
            cheatbuf="";
            man.moveMe(Man::NONE);
            exitAnim=1;
            bonusRunning=0;
        }
        else
        if(cheatbuf.find("kill") != string::npos)
        {
            cheatbuf="";
            onHurt();
        }
        else
        if(cheatbuf.find("bonusrun") != string::npos)
        {
            cheatbuf="";
            bonusRunning=SDL_GetTicks();
        }
        else
        if(cheatbuf.find("bonusremote") != string::npos)
        {
            cheatbuf="";
            remoteBomb=true;
            remoteBoom=false;
        }
        else
        if(cheatbuf.find("bonuslife") != string::npos)
        {
            cheatbuf="";
            if(lifes.getValue()<99)
            lifes.setValue(lifes.getValue()+1);
        }
        else
        if(cheatbuf.find("bonusrange") != string::npos)
        {
            cheatbuf="";
            if(Explosion::len<99)
            Explosion::len+=1;
            rangeCounter.setValue(Explosion::len);
        }
        else
        if(cheatbuf.find("bonusbomb") != string::npos)
        {
            cheatbuf="";
            if(maxBombs<99)
            maxBombs+=1;
            bombsCounter.setValue(bombsCounter.getValue()+1);
        }
        else
        if(cheatbuf.find("giveall") != string::npos)
        {
            cheatbuf="";
            remoteBomb=true;
            remoteBoom=false;
            rangeCounter.setValue(Explosion::len=20);
            lifes.setValue(20);
            bombsCounter.setValue(maxBombs=20);
            bonusRunning=SDL_GetTicks();
        }
        else
        if(cheatbuf.find("god") != string::npos)
        {
            cheatbuf="";
            godTime=SDL_GetTicks();
        }
    }
}

void MyField::makeCheatTime()
{
    const Uint32 cheatTimeMax=5000;
    if(SDL_GetTicks()-lastCheatTry>=cheatTimeMax)
            cheatbuf="";
}

void MyField::makeGod()
{
    if(godTime != 1 && godTime)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        //glColor4f(0.937,0.514,0.094, 0.5 );
    if(visibleMan)
        man.draw();
    if(godTime != 1 && godTime)
        //glColor4f(1,1,1,1);
       glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const Uint32 godTimeMax=5000;
    if(godTime != 1 && SDL_GetTicks()-godTime>=godTimeMax) 
        godTime=0;

}

void MyField::makeBurn()
{
    if(burns.size()==0) return;
    vector<Burn*>::iterator itr=burns.end()-1;
    while(itr >= burns.begin())
    {
        const Uint32 burnTimeMax=400;
        Burn *& a=*itr;
        if(a)
        {
            Uint32 elapsed=SDL_GetTicks()-a->time;
            if(elapsed >=burnTimeMax)
            {
                if(a->withAsh)
                    ashes.push_back(new Ash(SDL_GetTicks(),a->pos));
                delete a;
                a=0;
                burns.erase(itr);
            }
            else
            if(a->t)
            {
                const float startR=1;
                const float startG=1;
                const float startB=1;
                const float startA=0.8;

                const float endR=1;
                const float endG=0;
                const float endB=0;
                const float endA=0.3;

                float tmp=(float)elapsed/burnTimeMax;
                glColor4f(
                    startR*(1-tmp)+tmp*endR,
                    startG*(1-tmp)+tmp*endG,
                    startB*(1-tmp)+tmp*endB,
                    startA*(1-tmp)+tmp*endA
                );

                a->t->draw(a->pos);
                glColor4f(1,1,1,1);
            }
        }
        --itr;
    }
}


