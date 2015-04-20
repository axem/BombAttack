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
#include "space.h"

Space::Space()
{
    addRes(strs("space1.png","space2.png",0),500);
}

Space::~Space()
{
}

void Space::draw()
{
    Tex * t=current();
    const Tex::Data * data=t->getData();
    if(!t || !data || !data->id ) return;

    const float& xc=data->coord.first*(scrWidth/data->size.first);
    const float& yc=data->coord.second*(scrHeight/data->size.second);
    const float& xs=scrWidth;
    const float& ys=scrHeight;

    glBindTexture(GL_TEXTURE_2D, data->id );
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);     glVertex3f(0,0,0);
    glTexCoord2f(0,yc);    glVertex3f(0,ys,0);
    glTexCoord2f(xc,yc);   glVertex3f(xs,ys,0);
    glTexCoord2f(xc,0);    glVertex3f(xs,0,0);
    glEnd();
}
