#include "definition.h"

/***** laddar menymusiken och spritens egna musik *****/
void loadSounds(Player *man)
{
    // om linux
    if(LINUX)
    {
        Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);
        man->sounds.backgroundLinux = Mix_LoadMUS("Soundeffects/Fall.wav");
    }
    // annars (windows)
    else
    {
        Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);
        Mix_VolumeMusic(20);
        man->sounds.backgroundSound = Mix_LoadMUS("Soundeffects/Fall.MP3");
    }

    if(LINUX)
    {
        man->sounds.backstreet = Mix_LoadMUS("Soundeffects/Torgny.wav");
        man->sounds.cykablyat = Mix_LoadMUS("Soundeffects/cykablyat.wav");
        man->sounds.america = Mix_LoadMUS("Soundeffects/America.wav");
        man->sounds.china = Mix_LoadMUS("Soundeffects/Chinese.wav");
        man->sounds.Ldeathsound1 = Mix_LoadWAV("Soundeffects/deathtorgny.wav");
        man->sounds.Ldeathsound2 = Mix_LoadWAV("Soundeffects/death4.wav");
        man->sounds.Ldeathsound3 = Mix_LoadWAV("Soundeffects/death15.wav");
        man->sounds.Ldeathsound4 = Mix_LoadWAV("Soundeffects/death8.wav");
        man->sounds.bulletShot = Mix_LoadWAV("Soundeffects/bulletPop.wav");
    }
    else
    {
        man->sounds.bulletShot = Mix_LoadWAV("Soundeffects/bulletPop.WAV");
        man->sounds.backstreet = Mix_LoadMUS("Soundeffects/Torgny.MP3");
        man->sounds.cykablyat = Mix_LoadMUS("Soundeffects/cykablyat.MP3");
        man->sounds.america = Mix_LoadMUS("Soundeffects/America.MP3");
        man->sounds.china = Mix_LoadMUS("Soundeffects/Chinese.MP3");
        man->sounds.deathsound1 = Mix_LoadWAV("Soundeffects/deathtorgny.WAV");
        man->sounds.deathsound2 = Mix_LoadWAV("Soundeffects/death4.WAV");
        man->sounds.deathsound3 = Mix_LoadWAV("Soundeffects/death15.WAV");
        man->sounds.deathsound4 = Mix_LoadWAV("Soundeffects/death8.WAV");
    }
}

/***** spelar upp fiendens dödsljud beroende på vilken sprite fiendespelaren valt *****/
void enemyDeathSound(Player *man, int hitid)
{
    // om linux
    if(LINUX)
    {
        // torgny
        if(man->enemies[hitid].sprite==1)
        {

            Mix_PlayChannel(-1,man->sounds.Ldeathsound1,0);
        }
        // russia
        else if(man->enemies[hitid].sprite==2)
        {

            Mix_PlayChannel(-1,man->sounds.Ldeathsound2,0);
        }
        // murica
        else if(man->enemies[hitid].sprite==3)
        {

            Mix_PlayChannel(-1,man->sounds.Ldeathsound3,0);
        }
        // china
        else if(man->enemies[hitid].sprite==4)
        {

            Mix_PlayChannel(-1,man->sounds.Ldeathsound4,0);
        }

    }
    // annars (windows)
    else
    {
        if(man->enemies[hitid].sprite==1)
        {

            Mix_PlayChannel(-1,man->sounds.deathsound1,0);
        }
        else if(man->enemies[hitid].sprite==2)
        {

            Mix_PlayChannel(-1,man->sounds.deathsound2,0);
        }
        else if(man->enemies[hitid].sprite==3)
        {

            Mix_PlayChannel(-1,man->sounds.deathsound3,0);
        }
        else if(man->enemies[hitid].sprite==4)
        {

            Mix_PlayChannel(-1,man->sounds.deathsound4,0);
        }
    }
}

/***** spelar upp spelarens egna dödsljud när denne dör beroende på vilken sprite spelaren valt *****/
void playerDeathSound(Player *man)
{
    // om linux
    if(LINUX)
    {
        // torgny
        if(man->spritePick==1)
        {
            Mix_PlayChannel(-1,man->sounds.Ldeathsound1,0);
        }
        // russia
        else if(man->spritePick==2)
        {
            Mix_PlayChannel(-1,man->sounds.Ldeathsound2,0);
        }
        // murica
        else if(man->spritePick==3)
        {
            Mix_PlayChannel(-1,man->sounds.Ldeathsound3,0);
        }
        // china
        else if(man->spritePick==4)
        {
            Mix_PlayChannel(-1,man->sounds.Ldeathsound4,0);
        }
    }
    // annars (windows)
    else
    {
        if(man->spritePick==1)
        {
            Mix_PlayChannel(-1,man->sounds.deathsound1,0);
        }
        else if(man->spritePick==2)
        {
            Mix_PlayChannel(-1,man->sounds.deathsound2,0);
        }
        else if(man->spritePick==3)
        {
            Mix_PlayChannel(-1,man->sounds.deathsound3,0);
        }
        else if(man->spritePick==4)
        {
            Mix_PlayChannel(-1,man->sounds.deathsound4,0);
        }
    }
}
