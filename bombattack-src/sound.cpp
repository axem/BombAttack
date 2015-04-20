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
#include "sound.h"
#include <SDL/SDL_mixer.h>
#include <sys/types.h>
#include <dirent.h>

struct Playlist{
    Playlist();
    ~Playlist(){};
    static vector<string> music;
    static int current;
    static Mix_Music * current_music;
    static void musicFinished();
};



vector<string> Playlist::music;
int Playlist::current=-1;
static Playlist playlist;
static string sndPrefix="./sfx/";
Mix_Music* Playlist::current_music=0;
Mix_Music* playMP3(const string& s );
Mix_Chunk* playWAV(const string& s );

class CWav
{
public:
    Mix_Chunk *chunk;
    CWav(const string& s) { chunk=playWAV(changeExt(sndPrefix+s,".dat"));  };
    void play() { Mix_PlayChannel(-1,chunk,0); };
    ~CWav() { Mix_FreeChunk(chunk); chunk=0; };
};

Playlist::Playlist()
{
    Mix_HookMusicFinished(musicFinished);
}


void Playlist::musicFinished()
{
    if(!nosound)
    {
        if(current+1<static_cast<int>(music.size())) ++current;
        else
        if((current+1)==static_cast<int>(music.size())) current=0;
    
        if(current>=0 && current<static_cast<int>(music.size()) )
        {
            current_music=playMP3(sndPrefix+music[current]);
        }
    }
}

Mix_Chunk* playWAV(const string& s )
{
    if(nosound) return 0;
    Mix_Chunk *music;
    //Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
    //music = Mix_LoadWAV((sndPrefix+s).c_str());
    music = Mix_LoadWAV(s.c_str());
    //Mix_PlayChannel(-1,music,0);
    return music;

}


Mix_Music* playMP3(const string& s )
{
    if(nosound) return 0;
    Mix_Music *music;
    //Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
    //music = Mix_LoadMUS((sndPrefix+s).c_str());
    //music = Mix_LoadMUS(changeExt(sndPrefix+s,".dat").c_str());
    music = Mix_LoadMUS((s).c_str());
    Mix_PlayMusic(music,1);
    return music;
}

void soundMute(bool a)
{
    if(nosound) return;
    if(a)
    {
        Mix_VolumeMusic(0);
        Mix_Volume(-1,0);
    }
    else 
    {
        Mix_Volume(-1,51);
        Mix_VolumeMusic(40);
    }

}

void playMenuMusic()
{
    if(nosound) return;
    playlist.music.clear();
    playlist.music.push_back("menu.ogg");

    playlist.current_music=playMP3(sndPrefix+playlist.music[playlist.current=0]);
}

bool nameOk(const string& s)
{
    bool b=false;
    if(s.size()>=4)
        if(s.find_last_of(".dat")!=s.size()-1) 
        if(s != "menu.ogg")  b=true;
            //if(s != "menu.dat") b=true;
    return b;
}

void playGameMusic()
{
    if(nosound) return;
    playlist.music.clear();
    DIR *pDIR;
    dirent *pDirEnt;
    pDIR = opendir(sndPrefix.c_str());
    if ( pDIR == NULL )
    {
        if(debug) cout << "error with music dir" << endl;
        throw;
    }

    pDirEnt = readdir( pDIR );
    while ( pDirEnt != NULL ) 
    {
        if(nameOk(pDirEnt->d_name)) 
            playlist.music.push_back(pDirEnt->d_name);
        pDirEnt = readdir( pDIR );

    }
    sort(playlist.music.begin(),playlist.music.end());
    playlist.current_music=playMP3(sndPrefix+playlist.music[playlist.current=0]);
    closedir(pDIR);
}

void playSND(SND_type s)
{
    if(nosound) return;
    static CWav bomberhurt("bomberhurt.wav");
    static CWav boom("boom.wav");
    static CWav enemyhurt("enemyhurt.wav");
    static CWav menuclick("menuclick.wav");
    static CWav menuenter("menuenter.wav");
    static CWav bonus("bonus.wav");
    static CWav brick("brick.wav");

    switch(s)
    {
        case SND_BOMBERHURT: bomberhurt.play();  break;
        case SND_BOOM:       boom.play();        break;
        case SND_ENEMYHURT:  enemyhurt.play();   break;
        case SND_MENUCLICK:  menuclick.play();   break;
        case SND_MENUENTER:  menuenter.play();   break;
        case SND_BONUS:      bonus.play();       break;
        case SND_BRICK:      brick.play();       break;
        default: break;
    };


}


void freeSND()
{
    if(nosound) return;
    Mix_HaltMusic();
    Mix_FreeMusic(playlist.current_music);
}



