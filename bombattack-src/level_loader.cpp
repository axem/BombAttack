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
#include "level_loader.h"
#include <sys/types.h>
#include <dirent.h>
#include <algorithm>
#include <fstream>
#include <cstring>

bool LevelLoader::nameOk(const string& s)
{
    bool b=false;
    if(s.size()>=4)
        if(s.find_last_of(".lvl")==s.size()-1) b=true;
    return b;
}

LevelLoader::LevelLoader()
{
#ifdef __LINUX__
    prepare_linux();
#endif
    const int version=2;
    const char header[]="LVL";

    DIR *pDIR;
    dirent *pDirEnt;
    pDIR = opendir(lvl_prefix.c_str());
    vector<string> files;
    if ( pDIR == NULL )
    {
        if(debug) cout << "error with levels dir" << endl;
        throw;
    }

    pDirEnt = readdir( pDIR );
    while ( pDirEnt != NULL ) 
    {
        if(nameOk(pDirEnt->d_name)) files.push_back(pDirEnt->d_name);
        pDirEnt = readdir( pDIR );
    }
    if(files.size()==0)
    {
        if(debug) cout << "error with levels dir" << endl;
        throw;
    }
    sort(files.begin(),files.end());
    vector<string>::iterator i=files.begin();
    while(i != files.end())
    {
        std::fstream f;
        f.open((lvl_prefix+*i).c_str(), std::ios_base::in );
        f.seekg (0, std::ios::end);
        unsigned size = f.tellg();
        f.seekg (0, std::ios::beg);
        bool ok=false;
        if(size >= sizeof header+3*(sizeof (int))+9*(sizeof (int)))
        {
            char * data = new char[size];
            f.read(data,size);
            f.close();
            if(strcmp(data,header) == 0)
            {
                int pos=0;
                pos=+sizeof header;
                if(*(int*)((data+pos))==version)
                {
                    ok=true;
                    pos+=sizeof(int);
                    Level * L = new Level(); 
                    L->man_x=*(int*)(data+pos); pos+=sizeof (int);
                    L->man_y=*(int*)(data+pos); pos+=sizeof (int);
                    L->enemy_base_speed=*(int*)(data+pos); pos+=sizeof (int);
                    L->enemy_rand_speed=*(int*)(data+pos); pos+=sizeof (int);
                    L->width=*(int*)(data+pos); pos+=sizeof (int);
                    L->height=*(int*)(data+pos); pos+=sizeof (int);
                    L->table.resize(L->width * L->height);
                    for(int x=0; x<L->width; x++)
                    for(int y=0; y<L->height; y++)
                    {
                        L->table[x+y*L->width]=*(char*)(data+pos); pos+=sizeof (char);
                    }
                    lvls.push_back(L);
                }
            }
            delete[] data;
            if(!ok)
            {
                if(debug) cout << "broken level" << endl;
                throw;
            }
            i++;
        }
    }
    closedir( pDIR );
}

LevelLoader::~LevelLoader()
{
    purge(lvls);
}

LevelLoader & LevelLoader::get()
{
    static LevelLoader x;
    return x;
}

LevelLoader::Level * LevelLoader::operator[](int i)
{
    return lvls[i];
}

int LevelLoader::count() const
{
    return lvls.size();
}
