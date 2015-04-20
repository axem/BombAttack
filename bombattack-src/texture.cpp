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
#include "texture.h"
#include <cstring>


int Tex::used_gpu_ram=0;
int Tex::max_used_gpu_ram=0;


Tex::Textures Tex::textures;

Tex::Data::Data()
:filename(""), size(0,0), coord(0,0), size_pow(0,0), bitsperpixel(0), id(0), users(0)
{
}

Tex::Data::~Data()
{
    unload(filename);
}

Tex::Textures::Textures()
{
}

Tex::Textures::~Textures()
{
    if(debug) cout << "not unloaded textures (must be 0): " << map.size() << endl;
    Textures::map_itr it=textures.map.begin();
    while(it != textures.map.end())
    {
        if((*it).second && *(*it).second)
        {
            if(debug) cout << (*(*it).second)->filename << endl;
            delete (*it).second;
        }
        ++it;
    }
    textures.map.clear();

}


string Tex::res_prefix()
{
    return "./gfx/";
}


Tex::Tex(string _filename)
: data(0)
{
    LoadTexture(_filename,data);
}

Tex::~Tex()
{
    if(data && *data) 
    {
        unload((*data)->filename);
        data=0;
    }
}


PointInt Tex::size_pow_n(int w, int h)
{
    PointInt x(2,2);
    while(x.first<w)
        x.first*=2;

    while(x.second<h)
        x.second*=2;
    return x;
}

bool Tex::isTextureWithMipMaps(const string& s)
{

    if( s=="menu.png"          ||
        s=="button1.png"       ||
        s=="button2.png"       ||
        s=="button3.png"       ||
        s=="button4.png"       ||
        s=="gameover.png"      ||
        s=="enter.png"         ||
        s=="credits.png"       ||
        s=="instructions.png"  ||
        s=="loading.png"       ||
        s=="logo.png"          ||
        s=="0.png"             ||
        s=="1.png"             ||
        s=="2.png"             ||
        s=="3.png"             ||
        s=="4.png"             ||
        s=="5.png"             ||
        s=="6.png"             ||
        s=="7.png"             ||
        s=="8.png"             ||
        s=="9.png"             ||
        s=="theend.png"
     )
    return false; else 
    return true;
}

string Tex::texName(const string& s)
{
    return changeExt(s,".dat");
}


GLuint Tex::LoadTexture(const string &_filename, Data **& _data) 
{
        Textures::map_itr it=textures.map.find(_filename);
        if(it != textures.map.end())
        {
            if((*it).second && *(*it).second)
            {
                if(_data && *_data)
                    (*_data)->users--;
                _data = (*it).second;
                (*(*it).second)->users++;
                return (*(*it).second)->id;
            }
            else
            {
                return 0;
            }
        }
/*********************************************************************************/
        if(debug) cout << "loading: " << (res_prefix() + _filename);

        SDL_Surface *surface=0;
        GLuint textureid=0;
        GLint mode;
        surface = IMG_Load(texName(res_prefix() + _filename).c_str());
        if (!surface) return 0;

        switch(surface->format->BytesPerPixel)
        {
            case 3: mode = GL_RGB; break;
            case 4: mode = GL_RGBA; break;
            default: 
                    SDL_FreeSurface(surface);
                    return 0;
            break;
        }

        unsigned char * data = (unsigned char*)surface->pixels;
        unsigned char * pixels=0;

        PointInt p = size_pow_n(surface->w,surface->h);

        if(p.first != surface->w || p.second != surface->h)
        {
            pixels  = new  unsigned char [p.first*p.second*surface->format->BytesPerPixel];
            data=pixels;
            for(int i=0; i < surface->h; i++)
                memcpy(
                    pixels + (i * p.first * surface->format->BytesPerPixel),
                    (unsigned char*)surface->pixels + (i * surface->w * surface->format->BytesPerPixel),
                    surface->w*surface->format->BytesPerPixel
                );
        }

        glGenTextures(1, &textureid);
        glBindTexture(GL_TEXTURE_2D, textureid);

        if(isTextureWithMipMaps(_filename))
        {
            gluBuild2DMipmaps(GL_TEXTURE_2D, mode, p.first, p.second, mode, GL_UNSIGNED_BYTE, data);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, mode, p.first, p.second, 0, mode, GL_UNSIGNED_BYTE, data);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        }

        used_gpu_ram+=surface->format->BytesPerPixel*p.first*p.second;
        max_used_gpu_ram= used_gpu_ram > max_used_gpu_ram ? used_gpu_ram  : max_used_gpu_ram;

        if(pixels) delete[] pixels;
/******************************************/
        Data ** _ptr_ = new (Data *) ( new Data() );
        Data * ptr=*_ptr_;
        ptr->filename=_filename;
        ptr->size=normalize(surface->w, surface->h);

        ptr->coord.first= (float)surface->w / (float)p.first;
        ptr->coord.second= (float)surface->h / (float)p.second;

        ptr->size_pow=p;
        ptr->bitsperpixel=surface->format->BytesPerPixel;
        ptr->id=textureid;
        ptr->users=1;
        if(_data && *_data) (*_data)->users--;
        _data=_ptr_;
        textures.map.insert(std::pair<string, Data**>(_filename,_ptr_));
/******************************************/
        SDL_FreeSurface(surface);
        if(debug) cout << " with id: " << textureid << endl;
        return textureid;

}

void Tex::draw(float x, float y)
{
    if(!data || !(*data) || !(*data)->id) return;
    const float &xc=(*data)->coord.first;
    const float &yc=(*data)->coord.second;
    const float &xs=(*data)->size.first;
    const float &ys=(*data)->size.second;

    glBindTexture(GL_TEXTURE_2D, (*data)->id );
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);     glVertex3f(x,y,0);
    glTexCoord2f(0,yc);    glVertex3f(x,y+ys,0);
    glTexCoord2f(xc,yc);   glVertex3f(x+xs,y+ys,0);
    glTexCoord2f(xc,0);    glVertex3f(x+xs,y,0);
    glEnd();
}

void Tex::draw(const PointFloat& pos)
{
    draw(pos.first, pos.second);
}

PointFloat Tex::getSize() const
{
    if(data && *data)
        return (*data)->size;
    else return PointFloat(0,0);
}

void Tex::unload()
{
    if(data && *data && (*data)->id) 
    {
        unload((*data)->filename);
        data=0;
    }
}

void Tex::unload(const string &filename)
{
    Textures::map_itr it;
    it=textures.map.find(filename);
    if( it  != textures.map.end() && (*it).second)
    {
        Data *& ptr = (*(*it).second);
        if( ptr && ptr->id &&  !(--ptr->users)  )
        {
            if(debug) cout << "unloading: " << (res_prefix() + ptr->filename) << endl;
            if(ptr->id)
            {
                glDeleteTextures(1, &ptr->id);
            }
            used_gpu_ram-= ptr->bitsperpixel* ptr->size_pow.first * ptr->size_pow.second;
            max_used_gpu_ram= max( used_gpu_ram, max_used_gpu_ram );
            delete ptr; ptr=0;
            delete (*it).second; (*it).second=0;
            textures.map.erase(it);
        }
    }
}

GLuint Tex::getId() const
{
    if(data && *data)
    return (*data)->id;
    else return 0;
}

const Tex::Data * Tex::getData() const
{
    if(data)
    return *data; else return 0;
}

GLuint Tex::LoadTextureSimple(const string& _filename)
{
       SDL_Surface *surface=0;
        GLuint textureid;
        GLint mode;
        surface = IMG_Load(texName(res_prefix() + _filename).c_str());
        if (!surface) return 0;
        switch(surface->format->BytesPerPixel)
        {
            case 3: mode = GL_RGB; break;
            case 4: mode = GL_RGBA; break;
            default:
                    SDL_FreeSurface(surface);
                    return 0;
            break;
        }
        unsigned char * data = (unsigned char*)surface->pixels;
        unsigned char * pixels=0;
        PointInt p = size_pow_n(surface->w,surface->h);

        if(p.first != surface->w || p.second != surface->h)
        {
            pixels  = new  unsigned char [p.first*p.second*surface->format->BytesPerPixel];
            data=pixels;
            for(int i=0; i < surface->h; i++)
                memcpy(
                    pixels + (i * p.first * surface->format->BytesPerPixel),
                    (unsigned char*)surface->pixels + (i * surface->w * surface->format->BytesPerPixel),
                    surface->w*surface->format->BytesPerPixel
                );
        }
        glGenTextures(1, &textureid);
        glBindTexture(GL_TEXTURE_2D, textureid);

        if(isTextureWithMipMaps(_filename))
        {
            gluBuild2DMipmaps(GL_TEXTURE_2D, mode, p.first, p.second, mode, GL_UNSIGNED_BYTE, data);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, mode, p.first, p.second, 0, mode, GL_UNSIGNED_BYTE, data);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        }
    if(pixels) delete[] pixels;
    SDL_FreeSurface(surface);
    return textureid;
}

void Tex::loadAll()
{
        Textures::map_itr itr=textures.map.begin();
        while(itr != textures.map.end() && (*itr).second )
        {
            if(*(*itr).second)
            {
                glDeleteTextures(1, &(*(*itr).second)->id);
            }
            ++itr;
        }

        itr=textures.map.begin();
        while(itr != textures.map.end() && (*itr).second )
        {
            if(*(*itr).second)
            {
                ((*(*itr).second)->id)=LoadTextureSimple((*(*itr).second)->filename);
            }
            ++itr;
        }
}


