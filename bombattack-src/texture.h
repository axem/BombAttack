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
#ifndef TEXTURE_H
#define TEXTURE_H


#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <map>
#include <utility>
#include "globals.h"

class Tex {
    static bool isTextureWithMipMaps(const string&s );
public:
    struct Data
    {
        string filename;
        PointFloat  size; 
        PointFloat  coord;
        PointInt size_pow; 
        int  bitsperpixel;
        GLuint id;
        int users;
        Data();
        ~Data();
    };

    struct Textures
    {
        typedef std::map<string, Data** >::iterator map_itr;
        std::map<string, Data** > map;
        Textures();
        ~Textures();
    };
protected:
    Data ** data;
public:
    static int max_used_gpu_ram;
    static int used_gpu_ram;
    static PointInt size_pow_n(int, int);
    static GLuint LoadTexture(const string& filename, Data **& data);
    static GLuint LoadTextureSimple(const string& filename);
    static Textures textures;
    static string res_prefix();
    static void loadAll();
    static string texName(const string&);

    Tex(string filename);
    virtual ~Tex();
    virtual void draw(float x, float y );
    void draw(const PointFloat& pos );
    PointFloat getSize() const;
    GLuint getId() const;
    const Data * getData() const;
    void unload();
    static void unload(const string& filename);

};





#endif
