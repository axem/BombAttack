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
#ifndef MYFIELD_H
#define MYFIELD_H

#include "field.h"
#include "man.h"
#include "crazycounter.h"
#include "lifecounter.h"
#include "levelcounter.h"
#include "bombscounter.h"
#include "counter.h"
#include "enemy.h"
#include "bomb.h"
#include "explosion.h"
#include "space.h"
#include "hint.h"
#include "rangecounter.h"

struct Ash : public Tex
{
    Uint32 time;
    PointFloat pos;
    Ash(Uint32 _time=0, const PointFloat& _pos=PointFloat(0,0)): Tex("deathdead.png"), time(_time), pos(_pos) {};
    ~Ash(){};
};

struct Burn
{
    Uint32 time;
    PointFloat pos;
    Tex * t;
    bool withAsh;
    Burn(Uint32 _time=0, const PointFloat& _pos=PointFloat(0,0), Tex *_t=0, bool _withAsh=false): time(_time), pos(_pos), t(_t), withAsh(_withAsh) {};
    ~Burn(){};
};

class MyField : public Field
{
    vector<Enemy*> enemies;
    vector<Bomb*> bombs;
    vector<Explosion*> explosions;
    vector<Ash*> ashes;
    vector<Hint*> hints;
    vector<Burn*> burns;
    void makeAndDrawBombs();
    void makeExploding();
    Uint32 explodingTime;
    void makeEnemies();
    void drawEnemies();
    void onBoom(const PointInt& tablePos);
    void makeCameraZoom();
    bool isCameraNear;
    void onHurt();
    int leveltoLoad;
    Uint32 waitForRespawn;
    Uint32 waitForGameOver;
    void makeGameOver();
    int maxBombs;
    Uint32 spawning;
    Object::Animation spawnAnim;
    bool visibleMan;
    PointFloat spawnAnim_pos;
    void makeExplosions();

    Object::Animation ash;
    void wakeupEnemy(int x, int y);
    void checkBonuses();
    Uint32 exitAnim;
    void makeExitAnim();
    bool remoteBomb;
    bool remoteBoom;
    bool altPressed;

    float cameraZoomDest;
    const float zoomStep;
    const Uint32 zoomStep_ms;
    const float cameraZoomMin;
    const float cameraZoomMax;
    Uint32 zoomLastTime;

    void makeAshes();
    void makeHints();
    void makeBonusRun();
    Uint32 bonusRunning;

    void makeAI();

    void makeCongratz();
    static void loadSavedGame(int level_num, int &lifes, int &maxBombs, int &range, int &remote);
    void saveGame();

    Uint32 lastCheatTry;
    string cheatbuf;
    void makeCheatTime();

    Uint32 godTime;
    void makeGod();

    void makeBurn();

    bool minus_pressed;
    bool plus_pressed;

protected:
    virtual void onEsc(bool pressed);
    virtual void onPaint();
    virtual void onArrow(arrow_type, bool pressed);
    virtual void onSpace(bool pressed);
    virtual void onPlus(bool pressed);
    virtual void onMinus(bool pressed);
    virtual void onCtrl(bool pressed);
    virtual void onNamedKey(bool pressed, char x);
public:
    Man man;
    Tex ctrl;
    CrazyCounter score;
    LifeCounter lifes;
    LevelCounter levelCounter;
    BombsCounter bombsCounter;
    RangeCounter rangeCounter;
    Space space;
    virtual void set(int x, int y, field_type);
    virtual void load(int num, bool immediately=true);
    virtual void unload();
    MyField(int levelToContinue=-1);
    virtual ~MyField();

};

#endif
