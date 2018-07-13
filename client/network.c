#include "definition.h"

/***** läser in hela strängen *****/
int uncomplete_string(char tmp[])
{
    int i=0;
    while(tmp[i]!='\0')
    {
        if(tmp[i]=='\n')
            return 0;
        i++;
    }
    return 1;
}

/***** skapar förbindelse med servern *****/
int networkInit(Network *client,Player *man,char *ipaddress)
{
    int type;
    client->tcpset=SDLNet_AllocSocketSet(10);
    client->udpset=SDLNet_AllocSocketSet(10);
    IPaddress ip,tcpip;
    char tmp[1024];

    if((SDLNet_ResolveHost(&ip,man->ip,5000))==-1)
    {
        printf("Couldnt resolve udp host\n");
        return 0;
    }
    if((SDLNet_ResolveHost(&tcpip,man->ip,4000))==-1)
    {
        printf("Couldnt resolve tcp host\n");
        return 0;
    }

    if(!(client->udpsock = SDLNet_UDP_Open(0)))
    {
        printf("Couldnt open socket\n");
        return 0;
    }

    if(!(client->sendpack = SDLNet_AllocPacket(1024)))
    {
        printf("could not allocate packet\n");
        return 0;
    }

    if(!(client->rcvpack = SDLNet_AllocPacket(1024)))
    {
        printf("Could not allocate receiving packet\n");
        return 0;
    }
    client->sendpack->address.host = ip.host;
    client->sendpack->address.port = ip.port;
    client->sendpack->len = 50;

    //Försöker connecta till servern
    if(!(client->tcpsock = SDLNet_TCP_Open(&tcpip)))
    {
        return 0;
    }
    SDLNet_TCP_Recv(client->tcpsock,tmp,1024);
    sscanf(tmp,"%d %d",&type,&(man->id));
    if(type==0)
        printf("my ID: %d\n",man->id);
    else if (type == 4)
    {
        printf("Server is full.\n");
        return 0;
    }
    man->alive = 1;

    SDLNet_TCP_AddSocket(client->tcpset,client->tcpsock);
    SDLNet_UDP_AddSocket(client->udpset,client->udpsock);


    return 1;
}

/***** skickar data *****/
void send_data(Player *man,Network *client,int type)
{
    char tmp[128];
    int size,len;
    if(type == 2)
    {
        sprintf(client->sendpack->data,"%d %d %f %f %d %d",
                type,man->id,man->x,man->y, man->frameX,man->spritePick);
        SDLNet_UDP_Send(client->udpsock,-1,client->sendpack);
    }
    if(type == 3)
    {
        sprintf(tmp,"%d %d \n",type,man->id);
        size=0;
        len=strlen(tmp)+1;
        while(size<len)
        {
            size+=SDLNet_TCP_Send(client->tcpsock,tmp+size,len-size);
        }
    }
    if(type == 7)
    {
        sprintf(client->sendpack->data,"%d %d %d %d",type,man->id,man->hitid,man->bulletid);
        SDLNet_UDP_Send(client->udpsock,-1,client->sendpack);
    }
    if(type == 11)
    {
        sprintf(tmp,"%d %d \n",type,man->id);
        size=0;
        len=strlen(tmp)+1;
        while(size<len)
        {
            size+=SDLNet_TCP_Send(client->tcpsock,tmp+size,len-size);
        }
    }
    if(type == 12)
    {
        sprintf(client->sendpack->data,"%d %d",type,man->id);
        SDLNet_UDP_Send(client->udpsock,-1,client->sendpack);
    }
}

/***** skicka bullet när man skjuter *****/
void sendBullet(Player man,Network client)
{
    int type = 8;
    sprintf(client.sendpack->data,"%d %d %d %d %d %d %d",
            type,man.id,(int)man.x,(int)man.y, man.blinkX,man.blinkY,man.bulletNo);
    SDLNet_UDP_Send(client.udpsock,-1,client.sendpack);
}

/***** tar emot data från servern *****/
void recv_data(Player *man, Network *client,int *done,Bullet b[])
{

    int type, enemyid,enemySX,spritePick,hitid,bulletX,bulletY;

    float enemyDX,enemyDY;
    int blinkX,blinkY,bulletid;
    int kills,deaths;

    /***** kollar UDP-socket om data inkommer *****/
    while(SDLNet_CheckSockets(client->udpset,0)>0)
    {
        SDLNet_UDP_Recv(client->udpsock,client->rcvpack);
        sscanf(client->rcvpack->data,"%d %d",&type,&enemyid);

        /***** om ny fiende *****/
        if (!man->enemies[enemyid].exists && (man->id!=enemyid) && type!=12 &&type!=10)
        {
            SDL_DestroyTexture(man->enemies[enemyid].texture);
            sscanf(client->rcvpack->data,"%d %d %f %f %d %d",
                   &type,&enemyid,&enemyDX,&enemyDY,&enemySX,&spritePick);

            SDL_Surface *image;
            if(LINUX)
            {

                if(spritePick==1)
                {
                    image = IMG_Load("pictures/spriteTorg.png");
                    man->enemies[enemyid].sprite = 1;
                }
                else if(spritePick==2)
                {
                    image = IMG_Load("pictures/spriteRussia.png");
                    man->enemies[enemyid].sprite = 2;
                }
                else if(spritePick==3)
                {
                    image = IMG_Load("pictures/spriteMurica.png");
                    man->enemies[enemyid].sprite = 3;
                }
                else
                {
                    image = IMG_Load("pictures/spriteChina.png");
                    man->enemies[enemyid].sprite = 4;
                }

            }
            else
            {
                if(spritePick==1)
                {
                    image = IMG_Load("pictures/spriteTorg.PNG");
                    man->enemies[enemyid].sprite = 1;
                }
                else if(spritePick==2)
                {
                    image = IMG_Load("pictures/spriteRussia.PNG");
                    man->enemies[enemyid].sprite = 2;
                }
                else if(spritePick==3)
                {
                    image = IMG_Load("pictures/spriteMurica.PNG");
                    man->enemies[enemyid].sprite = 3;
                }
                else
                {
                    image = IMG_Load("pictures/spriteChina.PNG");
                    man->enemies[enemyid].sprite = 4;
                }
            }

            /***** skapar initiala värden för ny fiende *****/
            man->enemies[enemyid].texture = SDL_CreateTextureFromSurface(program.renderer,image);
            SDL_FreeSurface(image);
            man->enemies[enemyid].srcRect.x = enemySX;
            man->enemies[enemyid].srcRect.y = 0;
            man->enemies[enemyid].srcRect.w = 32;
            man->enemies[enemyid].srcRect.h = 32;
            man->enemies[enemyid].dstRect.x = enemyDX;
            man->enemies[enemyid].dstRect.y = enemyDY;
            man->enemies[enemyid].dstRect.w = 64;
            man->enemies[enemyid].dstRect.h = 64;
            man->enemies[enemyid].exists = 1;
            if(man->alive)
            {
                send_data(&*man,&*client,2);
                send_data(&*man,&*client,2);
            }

        }

        /***** typ 2 = fiende skickar kordinater vid förflyttning *****/
        else if (type == 2 && (SDL_GetTicks() - man->enemies[enemyid].justDied > 1000))
        {
            sscanf(client->rcvpack->data,"%d %d %f %f %d %d",
                   &type,&enemyid,&enemyDX,&enemyDY,&enemySX,&spritePick);
            man->enemies[enemyid].alive = 1;
            man->enemies[enemyid].dstRect.x = enemyDX;
            man->enemies[enemyid].dstRect.y = enemyDY;
            man->enemies[enemyid].srcRect.x = enemySX;
        }

        else if(type == 7)
        {
            sscanf(client->rcvpack->data,"%d %d %d %d",&type,&enemyid,&hitid,&bulletid);
            if(hitid == man->id)
            {
                man->alive = 0;
                man->enemies[enemyid].bullet[bulletid].active = 0;
                playerDeathSound(&*man);
            }
            else if(enemyid == man->id)
            {
                man->enemies[hitid].alive = 0;
                b[bulletid].active = 0;
                enemyDeathSound(&*man, hitid);
            }
            else
            {
                man->enemies[hitid].alive = 0;
                man->enemies[enemyid].bullet[bulletid].active = 0;
                enemyDeathSound(&*man, hitid);
            }
        }
        else if (type == 8)
        {
            sscanf(client->rcvpack->data,"%d %d %d %d %d %d %d",
                   &type,&enemyid,&bulletX,&bulletY,&blinkX,&blinkY,&bulletid);
            checkRunningEnemyDirection(&*man, &bulletX, &bulletY, enemyid);
            addEnemyBullet(bulletX,bulletY,5,man->enemies[enemyid].bullet,blinkX,blinkY,bulletid);
        }
        else if(type == 10)
        {
            sscanf(client->rcvpack->data,"%d %d %d %d",&type,&enemyid,&kills,&deaths);
            if(enemyid == man->id)
            {
                man->kills = kills;
                man->deaths = deaths;
            }
            else
            {
                man->enemies[enemyid].kills = kills;
                man->enemies[enemyid].deaths = deaths;
            }
            generateScoreboard(&*man);

        }

        /***** round time *****/
        else if (type == 12)
        {
            sscanf(client->rcvpack->data,"%d %d %d",
                   &type,&enemyid,&man->roundTime);
                   man->lastRTime = SDL_GetTicks();
                   printf("round time: %d\n",man->roundTime);
                   man->gameStarted = 1;
        }
    }

    /***** kollar TCP-socket om data inkommer *****/
    while(SDLNet_CheckSockets(client->tcpset,0)>0)
    {
        printf("incoming data on tcp socket\n");
        int offset = 0;
        int max = 0;
        char tmp[1024];
        do
        {
            offset+=SDLNet_TCP_Recv(client->tcpsock,tmp+offset,1024);
            max++;
        }
        while(uncomplete_string(tmp) && max<20);
        sscanf(tmp,"%d %d",&type,&enemyid);
        if(max>=20)
        {
            type = 6;
        }

        if(type == 3)
        {
            if(enemyid==man->id)
            {
                man->disconnected = 1;
            }
            else
            {
                man->enemies[enemyid].exists = 0;
                SDL_DestroyTexture(man->enemies[enemyid].texture);
            }

        }
        else if(type == 11)
        {
            man->enemies[enemyid].exists = 0;
            man->enemies[enemyid].alive = 0;
            SDL_DestroyTexture(man->enemies[enemyid].texture);
        }

        else if(type == 6)
        {
            printf("Server shut down!\n");
            *done = 1;
            man->disconnected = 1;
            return;
        }

    }
}
