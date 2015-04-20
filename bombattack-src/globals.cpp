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
#include "globals.h"

#ifdef __LINUX__
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <vector>
#include <dirent.h>
#include <cstdlib>
#endif

int wndWidth=800;
int wndHeight=600;
int bestWndWidth=800;
int bestWndHeight=600;
float scrHeight=(float)wndHeight/(float)wndWidth;
float scrWidth=1;
#ifdef __LINUX__
static const string linux_dir=string(getenv("HOME"))+"/.bombattack/";
const string lvl_prefix=linux_dir+"levels/";
#else
const string lvl_prefix="./levels/";
#endif

const float blockWidth=normalize(64,64).first;
const float blockHeight=normalize(64,64).second;
unsigned my_score=0;
bool isSound=true;
bool isFullscreen=true;
int VideoBitsPerPixel=24;
bool debug=false;
bool nosound=true;

#ifdef __LINUX__
const string savefilename=linux_dir+"save.dat";
const string highscorefilename=linux_dir+"highscore.dat";
#else
const string savefilename="./save.dat";
const string highscorefilename="./highscore.dat";
#endif


strings::strings(const char *first, ... )
: _size(0)
{
    const char* str;
    va_list vl;
    str=first;
    va_start(vl, first);
    do {
        ++_size;
        items.push_back(str);
        str=va_arg(vl,const char*);
    } while (str!=NULL);

}

std::string &strings::operator[](int index)
{
    return items[index];
}

int strings::size()
{
    return _size;
}

PointFloat normalize(int w,int h) 
{
    return PointFloat(
        (float)w / (float)wndWidth * (float)wndWidth / (float)bestWndWidth *scrWidth,
        (float)h / (float)wndHeight* (float)wndHeight / (float)bestWndHeight *scrHeight
        );
}

PointFloat normalize(PointInt x)
{
    return normalize(x.first, x.second);
}

string changeExt(string s,const string& newext)
{
    size_t pos;
    if((pos=s.find_last_of(".")  )!= string::npos)
        s.replace(s.begin()+pos,s.end(),newext);
    return s;
}
#ifdef __LINUX__
void prepare_linux()
{
    static bool prepared=false;
    if(prepared) return;
    prepared=true;

    mkdir(linux_dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(lvl_prefix.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    DIR *pDIR;
    dirent *pDirEnt;
    pDIR = opendir(lvl_prefix.c_str());
    if ( pDIR == NULL )
    {
        if(debug) cout << "error with levels dir2" << endl;
        throw;
    }
    string s="";
    bool b=false;
    pDirEnt = readdir( pDIR );
    while ( pDirEnt != NULL ) 
    {
        s=pDirEnt->d_name;
        if(s.size()>=4)
        if(s.find_last_of(".lvl")==s.size()-1) { b=true; break; }
        pDirEnt = readdir( pDIR );
    }
    closedir( pDIR );


    if(!b)
    {
        pDIR = opendir("./levels/");
        dirent *pDirEnt;
        if ( pDIR == NULL )
        {
            if(debug) cout << "error with levels dir3" << endl;
            throw;
        }
        string s="";
        std::vector<string> files;
        pDirEnt = readdir( pDIR );
        while ( pDirEnt != NULL ) 
        {
            s=pDirEnt->d_name;
            if(s.size()>=4)
            if(s.find_last_of(".lvl")==s.size()-1) files.push_back(s);
            pDirEnt = readdir( pDIR );
        }
        closedir( pDIR );
        if(files.size())
        {
            std::vector<string>::iterator itr=files.begin();
            while(itr != files.end())
            {
                s=*itr;
                std::fstream src;
                std::fstream dest;
                char * buf=0;
                unsigned len=0;
                src.open(("./levels/"+s).c_str(),std::fstream::in | std::fstream::binary);
                dest.open((lvl_prefix+s).c_str(),std::fstream::out | std::fstream::binary | std::fstream::trunc);
                src.seekg(0, std::ios::end);
                len=src.tellg();
                src.seekg(0, std::ios::beg);
                buf = new char[len];
                src.read(buf,len);
                dest.write(buf,len);
                src.close();
                dest.close();
                delete[] buf;
                ++itr;
            }

        }

    }

}
#endif


