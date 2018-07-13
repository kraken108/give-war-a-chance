#include "definition.h"

/***** global = en timer som hindrar spelaren från att skjuta massa skott samtidigt,
       med andra ord en pytteliten cooldown när man skjuter *****/
int global = 0;

int main(int argc, char *argv[])
{
    int pickCharacter = 0,exit = 0,ingame = 0,connected, i,done = 0,timeDiff;
    int choice,newline,moved = 0,type,direct = 0,enterIPmenu,lastPrint=300000;
    unsigned int lastTime,currentTime;
    char *tmp = (char*)malloc(100);
    int frameStart=0,frameEnd=0;
    float lastSent = 0;
    Menu menu,pick;

    Player player = {0};
    Network client;
    player.blinked = 0;
    player.spellReady = 1;
    bullet.texture=initBullet();
    Bullet ammo[20];



    int spawnTimer=4;
    srand(time(NULL));

    connected = 1;

    /** INIT SOUNDS,PICTURES,TEXTURES ETC **/
    TTF_Init();
    loadSounds(&player);
    initMenu(&menu,&player);
    initPick(&pick);
    initLedges(&player);
    initCd(&player);
    SDLNet_Init();
    intro();

    /**** MAIN MENU ****/
    while(!exit)
    {
        if(LINUX)
            Mix_PlayMusic(player.sounds.backgroundLinux, -1);
        else
            Mix_PlayMusic(player.sounds.backgroundSound, -1);

        displayMenu(menu);
        enterIPmenu = handleMenu(&exit);

        /**** SKRIV IN IP ****/
        while(enterIPmenu)
        {
            pickCharacter = enterIP(&player);
            SDL_StopTextInput();
            if(pickCharacter)
            {
                if(!(networkInit(&client,&player,tmp)))
                {
                    ingame = 0;
                    pickCharacter = 0;
                    enterIPmenu = 0;
                }
                else
                {
                    player.connected = 1;
                    player.disconnected = 0;
                }
            }
            else
                enterIPmenu = 0;

            /**** PICK CHARACTER ****/
            while(pickCharacter)
            {
                displayMenu(pick);
                ingame = handlePick(&pickCharacter,&player);
                if(ingame==1)
                {
                    player.gameStarted = 1;
                    initPlayer(&player);
                    clearCartridge(ammo);
                    send_data(&player,&client,2);
                    player.deltaTimeMs = 0;
                    player.alive = 0;
                    send_data(&player,&client,12);
                }
                recv_data(&player,&client,&done,ammo);

                /**** INGAME ****/
                while(ingame==1)
                {
                    if(player.deltaTimeMs < 1)
                    {
                        frameStart = SDL_GetTicks();
                        SDL_Delay(1);
                        frameEnd = SDL_GetTicks();
                        player.deltaTimeMs = frameEnd-frameStart;
                    }
                    frameStart = SDL_GetTicks();
                    player.deltaTimeS = (float)(player.deltaTimeMs)/ 1000;

                    direct = 0;
                    done = 0;

                    // hanterar spelarens handlingar
                    done = processEvents(&player,ammo,&moved,&type,&direct,&client);

                    updateEnemyBullet(&player);
                    updateLogic(&player,ammo);
                    if(player.alive)
                        collisionDetect(&player, &direct);
                    bulletGone(ammo,&player,&client);

                    // skickar data till servern
                    if(moved && player.alive)
                    {
                        send_data(&player,&client,type);
                        moved = 0;
                    }

                    // tar emot data från servern
                    if (done != 1)
                        recv_data(&player,&client,&done,ammo);

                    // ritar allting på spelplanen
                    doRender(&player,ammo);

                    detectHit(&player,ammo,&client);

                    /** RESPAWNING IN 4 SECONDS **/
                    if(!player.alive)
                    {
                        currentTime = SDL_GetTicks();
                        currentTime = currentTime/1000;
                        if(currentTime>lastTime)
                        {
                            spawnTimer--;
                        }
                        lastTime = currentTime;
                        if(spawnTimer<= 0)
                        {
                            respawn(&player);
                            player.alive = 1;
                            send_data(&player,&client,2);
                            spawnTimer = 4;
                        }
                    }

                    if(player.gameStarted)
                    {
                        player.currentRTime = SDL_GetTicks();
                        timeDiff = player.currentRTime - player.lastRTime;
                        player.roundTime = player.roundTime - timeDiff;
                        player.lastRTime = player.currentRTime;
                        if(player.roundTime <= (lastPrint-1000))
                        {
                            printf("Time left: %d\n",player.roundTime/1000);
                            lastPrint = player.roundTime;
                        }
                    }

                    if(done || player.disconnected)
                    {
                        ingame = 0;
                        if(LINUX)
                            Mix_PlayMusic(player.sounds.backgroundLinux, -1);
                        else
                            Mix_PlayMusic(player.sounds.backgroundSound, -1);
                    }
                    frameEnd = SDL_GetTicks();
                    player.deltaTimeMs = frameEnd - frameStart;
                }

                // om spelaren backar ur spelet till main menu
                if(ingame==-1 || player.disconnected)
                {
                    if(!player.disconnected)
                    {
                        send_data(&player,&client,3);
                        SDL_Delay(1000);
                    }
                    SDLNet_FreeSocketSet(client.udpset);
                    SDLNet_FreeSocketSet(client.tcpset);
                    SDLNet_UDP_Close(client.udpsock);
                    SDLNet_TCP_Close(client.tcpsock);
                    pickCharacter = 0;
                    enterIPmenu = 0;
                }
                exit = 0;
            }
        }
    }
    TTF_Quit();
    if(LINUX)
    {
        Mix_FreeMusic(player.sounds.backgroundLinux);
    }
    else
    {
        Mix_FreeMusic(player.sounds.backgroundSound);
    }
    Mix_CloseAudio();
    free(tmp);
    SDLNet_Quit();
    Quit();



    return 0;
}

