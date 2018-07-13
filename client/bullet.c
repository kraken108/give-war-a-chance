#include "bullet.h"

SDL_Texture *getBulletSprite()
{
    return bullet.texture;
}

/***** skapar ett skott när man skjutit *****/
int loadAmmo(Bullet b[])
{
    int i;
    for(i=0; i< 20 ; i++)
    {
        if(b[i].active == 0)
        {
            memset(&b[i], 0, sizeof(Bullet));
            b[i].active = 1;
            return i;
        }
    }
    return -1;
}

void shotgun(Player *p)
{
    if(p->shot == 1)
    {
        if(p->thinkTime <= 0)
        {
            printf("it!\n");
            p->thinkTime = 20;
        }
    }
}

/***** tar bort spelarens skott *****/
void clearCartridge(Bullet a[])
{
    int i;
    for(i=0; i<20; i++)
    {
        a[i].active = 0;
        memset(&a[i],0,sizeof(Bullet));
    }
}

/***** funktionen tar spelarens pos. samt muspekarens pos. skapar längdvektor
       och sen bedömmer i vilken riktning ska projektilen färdas *****/
int addBullet(int x, int y,int dx, Bullet b[],int b1,int b2)
{
    int i = loadAmmo(b);
    if(i== -1)
    {
        printf("couldn't get a free slot for a bullet!\n");
        return i;
    }
    b[i].x = x;
    b[i].y = y;
    b[i].v_length = sqrt(((b1-b[i].x)*(b1-b[i].x)) + ((b2 - b[i].y)*(b2 - b[i].y)));

    b[i].directionX = b1 - b[i].x;
    b[i].directionY = b2 - b[i].y;

    b[i].vector_unitX = (b[i].directionX) / b[i].v_length;
    b[i].vector_unitY = (b[i].directionY) / b[i].v_length;
    return i;
}

/***** skapar ett skott när en fiende har skjutit och
       sen bedömmer i vilken riktning ska projektilen färdas *****/
void addEnemyBullet(int x,int y,int dx,Bullet b[],int b1,int b2,int i)
{
    b[i].x = x;
    b[i].y = y;
    b[i].v_length = sqrt(((b1-b[i].x)*(b1-b[i].x)) + ((b2 - b[i].y)*(b2 - b[i].y)));

    b[i].directionX = b1 - b[i].x;
    b[i].directionY = b2 - b[i].y;

    b[i].vector_unitX = (b[i].directionX) / b[i].v_length;
    b[i].vector_unitY = (b[i].directionY) / b[i].v_length;
    b[i].active = 1;
}

/***** kollisionsdetektion för när fiender träffas av skott *****/
void detectHit(Player *man,Bullet b[],Network *client)
{
    int i,j;
    for(i=0; i<5; i++)
    {
        if(man->enemies[i].exists && man->enemies[i].alive)
            for(j=0; j<20; j++)
            {
                if(b[j].active)
                {
                    if(b[j].x > (man->enemies[i].dstRect.x) && b[j].x <= (man->enemies[i].dstRect.x+50)

                            && b[j].y >= (man->enemies[i].dstRect.y) && b[j].y <= (man->enemies[i].dstRect.y+64))
                    {
                        b[j].active = 0;
                        man->bulletid = j;
                        man->hitid = i;

                        man->enemies[i].alive = 0;
                        man->enemies[i].justDied = SDL_GetTicks();
                        send_data(&*man,&*client,7);
                        enemyDeathSound(&*man, i);
                    }
                }
            }
    }
}

/***** raderar kulor när de träffar kaktusar *****/
void bulletGone(Bullet b[],Player *man,Network *client)
{
    int i, j;

    for(i=0; i<20; i++) /**PLAYERS BULLETS **/
    {
        if(b[i].active)
        {
            /***** går igenom alla kaktusar *****/
            if(b[i].x > 361 && b[i].x < 401 && b[i].y > 181 && b[i].y < 247)
            {
                b[i].active = 0;
            }
            if(b[i].x > 207 && b[i].x < 247 && b[i].y > 266 && b[i].y < 332)
            {
                b[i].active = 0;
            }
            if(b[i].x > 666 && b[i].x < 706 && b[i].y > 225 && b[i].y < 291)
            {
                b[i].active = 0;
            }
            if(b[i].x > 207 && b[i].x < 247 && b[i].y > 478 && b[i].y < 544)
            {
                b[i].active = 0;
            }
            if(b[i].x > 463 && b[i].x < 503 && b[i].y > 436 && b[i].y < 502)
            {
                b[i].active = 0;
            }
            if(b[i].x > 666 && b[i].x < 706 && b[i].y > 476 && b[i].y < 542)
            {
                b[i].active = 0;
            }
            if(b[i].x < (0-8) || b[i].x > 1032 || b[i].y < 100 || b[i].y > 638)
            {
                b[i].active = 0;
            }
        }
    }

    for(i=0; i<5; i++) /**ENEMIES BULLETS **/
    {
        if(man->enemies[i].exists)
        {
            for(j=0; j<20; j++)
            {
                if(man->enemies[i].bullet[j].active)
                {
                    /***** går igenom alla kaktusar *****/
                    if(man->enemies[i].bullet[j].x > 361 && man->enemies[i].bullet[j].x < 401
                       && man->enemies[i].bullet[j].y > 181 && man->enemies[i].bullet[j].y < 247)
                    {
                        man->enemies[i].bullet[j].active = 0;
                    }
                    if(man->enemies[i].bullet[j].x > 207 && man->enemies[i].bullet[j].x < 247
                       && man->enemies[i].bullet[j].y > 266 && man->enemies[i].bullet[j].y < 332)
                    {
                        man->enemies[i].bullet[j].active = 0;
                    }
                    if(man->enemies[i].bullet[j].x > 666 && man->enemies[i].bullet[j].x < 706
                       && man->enemies[i].bullet[j].y > 225 && man->enemies[i].bullet[j].y < 291)
                    {
                        man->enemies[i].bullet[j].active = 0;
                    }
                    if(man->enemies[i].bullet[j].x > 207 && man->enemies[i].bullet[j].x < 247
                       && man->enemies[i].bullet[j].y > 478 && man->enemies[i].bullet[j].y < 544)
                    {
                        man->enemies[i].bullet[j].active = 0;
                    }
                    if(man->enemies[i].bullet[j].x > 463 && man->enemies[i].bullet[j].x < 503
                       && man->enemies[i].bullet[j].y > 436 && man->enemies[i].bullet[j].y < 502)
                    {
                        man->enemies[i].bullet[j].active = 0;
                    }
                    if(man->enemies[i].bullet[j].x > 666 && man->enemies[i].bullet[j].x < 706
                       && man->enemies[i].bullet[j].y > 476 && man->enemies[i].bullet[j].y < 542)
                    {
                        man->enemies[i].bullet[j].active = 0;
                    }

                    if(man->enemies[i].bullet[j].x < (0-8) || man->enemies[i].bullet[j].x > 1032
                            || man->enemies[i].bullet[j].y < 100 || man->enemies[i].bullet[j].y > 638)
                    {
                        man->enemies[i].bullet[j].active = 0;
                    }

                }
            }
        }
    }
}



