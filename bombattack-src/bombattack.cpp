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
#include "game.h"
#include <string>
#include <iostream>

#ifdef __WIN32__
#undef main
#endif


using std::string;
using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    bool b=false;
    bool s=false;
    if(argc>1)
    {
        for(int i=1; i<argc; i++)
        if(string(argv[i]) == "--help") 
        {
            cout << "Usage: " << argv[0] << " [--help|--windowed|--nosound]\n";
            cout << "\t--nosound\t\trun without sound\n";
            cout << "\t--windowed\trun in window\n";
            cout << "\t--help\t\tshow this help\n";
            cout << "maxem software 2007" << endl;
            return 0;
        }

        for(int i=1; i<argc; i++)
        {
            if(string(argv[i]) == "--windowed") b=true;
            if(string(argv[i]) == "--nosound") s=true;
        }
    }
    Game::run(b,s);
    return 0;
}
