#include "processing.h"

/***** updaterar spelarens skotts position *****/
void updateLogic(Player *p,Bullet b[])
{
    int i;
    for(i=0; i<20; i++)
    {
        if(b[i].active == 1)
        {
            b[i].x = b[i].x + (b[i].vector_unitX*300*p->deltaTimeS);
            b[i].y = b[i].y + (b[i].vector_unitY*300*p->deltaTimeS);
        }
    }
    global++;
}

/***** updaterar fienders skotts position *****/
void updateEnemyBullet(Player *man)
{
    int i,j;
    for(i=0; i<5; i++)
    {
        for(j=0; j<20; j++)
        {
            if(man->enemies[i].bullet[j].active == 1)
            {
                man->enemies[i].bullet[j].x = man->enemies[i].bullet[j].x +
                                              (man->enemies[i].bullet[j].vector_unitX*300*man->deltaTimeS);
                man->enemies[i].bullet[j].y = man->enemies[i].bullet[j].y +
                                              (man->enemies[i].bullet[j].vector_unitY*300*man->deltaTimeS);
            }
        }
    }
}

/***** Regestrerar spelarens handlingar *****/
int processEvents(Player *man,Bullet b[],int *moved,int *type,int *direct,Network *client)
{
    unsigned int spellOne, spellOne_False=0;
    int blinkX,blinkY;
    spellOne = SDL_GetTicks();
    SDL_Event event;
    int done = 0;

    man->thinkTime--;
    if(man->thinkTime<=0)
    {
        man->thinkTime = 0;
    }

    /***** Spelaren kan stänga av spelet på ett säkert sätt,
           sammlar information när spelaren klickar på musknappen *****/
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_WINDOWEVENT_CLOSE:
        {
            if(program.window)
            {
                SDL_DestroyWindow(program.window);
                program.window = NULL;
                done = 1;
                *moved = 1;
                *type = 11;
            }
        }
        break;
        case SDL_KEYDOWN:
        {
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                done = 1;
                *moved = 1;
                *type = 11;
                break;
            case SDLK_RETURN:
                done = 1;
                *moved = 1;
                *type = 11;
                break;
            }
        }
        break;

        case SDL_QUIT:
            //quit out of the game
            done = 1;
            *moved = 1;
            *type = 11;
            break;
        case SDL_MOUSEBUTTONDOWN :
            SDL_GetMouseState(&blinkX, &blinkY);
            break;

        case SDL_MOUSEBUTTONUP:
            man->justShot = 0;
            break;
        }
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if((state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) && man->alive && man->gameStarted)
    {
        man->x = man->x - (200*(man->deltaTimeS));
        *moved = 1;
        *type = 2;
        *direct += 1;
        if(man->frameX == 192)
        {
            man->frameX = 224;
        }
        else
        {
            man->frameX = 192;
        }
        if(man->x < 0)
            man->x = 0;
    }
    if((state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) && man->alive && man->gameStarted)
    {
        man->x = man->x + (200*(man->deltaTimeS));

        *moved = 1;
        *type = 2;
        *direct += 2;
        if(man->frameX == 128)
        {
            man->frameX = 160;
        }
        else
        {
            man->frameX = 128;
        }
        if(man->x > 960)
            man->x = 960;

    }
    if((state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) && man->alive && man->gameStarted)
    {
        man->y = man->y - (200*(man->deltaTimeS));
        *moved = 1;
        *type = 2;
        *direct += 4;
        if(man->frameX == 64)
        {
            man->frameX= 96;
        }
        else
        {
            man->frameX = 64;
        }
        if(man->y<0)
            man->y = 0;
    }
    if((state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]) && man->alive && man->gameStarted)
    {
        man->y = man->y + (200*(man->deltaTimeS));
        *moved = 1;
        *type = 2;
        *direct += 8;
        if(man->frameX == 0)
        {
            man->frameX = 32;
        }
        else
        {
            man->frameX = 0;
        }
        if(man->y>566)
            man->y = 566;
    }
    if (man->blinked == 1)
    {
        *moved = 1;
        *type = 2;
        *direct = -1;
        man->blinked = 0;
    }

    /***** när spelaren klickar på en musknapp för att skjuta *****/
    if((SDL_GetMouseState(NULL,NULL) &&SDL_BUTTON_LEFT) && man->alive && !man->justShot && man->gameStarted)
    {
        int bulletNo,shotX,shotY;
        checkRunningDirection(&*man, &shotX, &shotY);
        SDL_GetMouseState(&blinkX, &blinkY);
        if(global%6==0)
        {
            man->justShot = 1;
            if(((bulletNo = addBullet(shotX,shotY,5,b,blinkX,blinkY))>=0))
            {
                Mix_PlayChannel(-1,man->sounds.bulletShot,0);
                man->blinkX = blinkX;
                man->blinkY = blinkY;
                man->bulletNo = bulletNo;
                if(man->connected)
                    sendBullet(*man,*client);
            }
        }
    }
    checkCd(&*man);
    /***** när spelaren använder blink *****/
    if(state[SDL_SCANCODE_SPACE] && man->alive && man->gameStarted)
    {
        if (man->spellReady == 0 && *direct <= 0)
            *direct = -1;
        int bX,bY;

        if(spellOne > spellOne_False+1000 && man->spellReady)
        {
            man->x1 = man->x;
            man->y1 = man->y;
            spellOne_False=spellOne-1;
            SDL_GetMouseState(&bX, &bY);
            float v_length = sqrt(((bX- man->x)*(bX-man->x))+((bY-man->y)*(bY-man->y)));
            Vector2d unit_vector;
            Vector2d direction;
            direction.x = bX-man->x;
            direction.y = bY-man->y;
            unit_vector.x = (direction.x)/ v_length;
            unit_vector.y = (direction.y)/ v_length;
            man->x+=(unit_vector.x*200);
            man->y+=(unit_vector.y*200);
            *moved = 1;
            *type = 2;
            // variabler för kollisiondetect
            *direct = -1;
            man->blinkRect.w = 0;
            man->spellReady = 0;
            man->blinked = 1;
            if(LINUX)
            {
                Mix_Chunk *blink = Mix_LoadWAV("Soundeffects/blink.wav");
                Mix_PlayChannel(-1,blink,0);
            }
            else
            {
                Mix_Chunk *blink = Mix_LoadWAV("Soundeffects/blink.WAV");
                Mix_PlayChannel(-1,blink,0);
            }

        }
    }
    return done;
}

/***** cooldown för blink *****/
void checkCd(Player *man)
{
    // +62 och man->blinkRect.w+=2 bestämmer CD för blink
    man->currentTime = SDL_GetTicks();
    if (man->currentTime > man->cdTime+62 && man->blinkRect.w < 300)
    {
        man->blinkRect.w += 2;
        man->cdTime = man->currentTime;
    }
    else if (man->blinkRect.w >= 300)
    {
        man->spellReady = 1;
    }
}

/***** kollisionsdetektion när spelaren rört sig eller använt blink *****/
void collisionDetect(Player *man, int *direct)
{
    // collision när man går (wasd)
    if (*direct > 0)
    {
        int i, bpe = 0;
        // check for collision with any ledges and enemies
        for (i = 0; i < 7; i++)
        {
            // sparar brädden och höjden av karaktärerna
            int mw = 64, mh = 64;
            // sparar var karaktärerna finns på spelplanen
            int mx = man->x, my = man->y;

            // ladda ledges (kaktusarna)
            int bx = man->ledges[i].x, by = man->ledges[i].y, bw = man->ledges[i].w, bh = man->ledges[i].h;

            // kolla [i] för ledges och fiende
            while (bpe < 2)
            {
                if (my+mh > by && mx < bx+bw && mx+mw > bx && my < by+bh)
                {
                    // man moving left
                    if (*direct == 1 || *direct == 13)
                        man->x = bx+bw;
                    // man moving right
                    else if (*direct == 2 || *direct == 14)
                        man->x = bx-mw;
                    // man moving up
                    else if (*direct == 4 || *direct == 7)
                        man->y = by+bh;
                    // man moving down
                    else if (*direct == 8 || *direct == 11)
                        man->y = by-mh;

                    /**** DIAGONALT ****/
                    // man moving left up
                    else if (*direct == 5 && my < by+bh-6)
                        man->x = bx+bw;
                    else if (*direct == 5 && my > by+bh-6)
                        man->y = by+bh;
                    // man moving right up
                    else if (*direct == 6 && my < by+bh-6)
                        man->x = bx-mw;
                    else if (*direct == 6 && my > by+bh-6)
                        man->y = by+bh;
                    // man moving left down
                    else if (*direct == 9 && my+mh > by+6)
                        man->x = bx+bw;
                    else if (*direct == 9 && my+mh < by+6)
                        man->y = by-mh;
                    // man moving right down
                    else if (*direct == 10 && my+mh > by+6)
                        man->x = bx-mw;
                    else if (*direct == 10 && my+mh < by+6)
                        man->y = by-mh;
                }
                // ladda enemies istället för ledges
                if(man->enemies[i].alive)
                    bx = man->enemies[i].dstRect.x, by = man->enemies[i].dstRect.y, bw = man->enemies[i].dstRect.w, bh = man->enemies[i].dstRect.h;
                bpe++;
            }
            bpe = 0;
        }
    }
    // för blink (space)
    else if (*direct < 0)
    {
        int i, bpe = 0;
        // sparar brädden och höjden av karaktärerna
        int mw = 64, mh = 64;
        // sparar var mitten av karaktärerna finns på spelplanen
        int mx = man->x+mw/2, my = man->y+mh/2;
        // sparar var mitten av karaktärerna fanns på spelplanen innan blinken användes
        int ox = man->x1+mw/2, oy = man->y1+mh/2;

        // spelaren kan inte blinka utanför kartan
        if ((mx-mw/2) < 0)
            man->x = 0;
        if ((mx+mw/2) > 1024)
            man->x = 1024-mw;
        if ((my-mh/2) < 100)
            man->y = 100;
        if ((my+mh/2) > 630)
            man->y = 630-mh;

        // check for collision with any ledges and enemies
        for (i = 0; i < 6; i++)
        {
            // ladda ledges (mitten av ledges)
            int bw = man->ledges[i].w, bh = man->ledges[i].h;
            int bx = man->ledges[i].x+bw/2, by = man->ledges[i].y+bh/2;

            // kolla [i] för ledges och fiende
            while (bpe < 2)
            {
                if ((my-mh/2)+mh > (by-bh/2) && (mx-mw/2) < (bx-bw/2)+bw && (mx-mw/2)+mw > (bx-bw/2) && (my-mh/2) < (by-bh/2)+bh)
                {
                    // höger sida
                    if (ox >= bx)
                    {
                        // höger
                        if (abs(bx-ox) > abs(by-oy))
                            man->x = bx+bw/2;
                        // över
                        else if (abs(oy-mh/2) < abs(by-bh/2))
                            man->y = by-bw/2-mh-14;
                        // under
                        else
                            man->y = by+bh/2;
                    }
                    // vänster sida
                    else if (ox <= bx)
                    {
                        // vänster
                        if (abs(bx-ox) > abs(by-oy))
                            man->x = bx-bw/2-mw;
                        // över
                        else if (abs(oy-mh/2) < abs(by-bh/2))
                            man->y = by-bw/2-mh-14;
                        // under
                        else
                            man->y = by+bh/2;
                    }
                }
                // ladda enemies istället för ledges
                if(man->enemies[i].alive)
                {
                    bw = man->enemies[i].dstRect.w, bh = man->enemies[i].dstRect.h;
                    bx = man->enemies[i].dstRect.x+bw/2, by = man->enemies[i].dstRect.y+bh/2;
                }
                bpe++;
            }
            bpe = 0;
        }
    }
}

/***** spelarens skott startar från karaktärens vapen *****/
void checkRunningDirection(Player *man, int *shotX, int *shotY)
{
    // torgny
    if (man->spritePick == 1)
    {
        // ner
        if (man->frameX == 0 || man->frameX == 32)
        {
            *shotX = man->x + 54;
            *shotY = man->y + 44;
        }
        // upp
        else if (man->frameX == 64 || man->frameX == 96)
        {
            *shotX = man->x + 8;
            *shotY = man->y + 44;
        }
        // höger
        else if (man->frameX == 128 || man->frameX == 160)
        {
            *shotX = man->x + 58;
            *shotY = man->y + 40;
        }
        // vänster
        else
        {
            *shotX = man->x + 4;
            *shotY = man->y + 40;
        }
    }
    // russia
    else if (man->spritePick == 2)
    {
        if (man->frameX == 0 || man->frameX == 32)
        {
            *shotX = man->x + 36;
            *shotY = man->y + 52;
        }
        else if (man->frameX == 64 || man->frameX == 96)
        {
            *shotX = man->x + 26;
            *shotY = man->y + 52;
        }
        else if (man->frameX == 128 || man->frameX == 160)
        {
            *shotX = man->x + 48;
            *shotY = man->y + 50;
        }
        else
        {
            *shotX = man->x + 14;
            *shotY = man->y + 50;
        }
    }
    // murica
    else if (man->spritePick == 3)
    {
        if (man->frameX == 0 || man->frameX == 32)
        {
            *shotX = man->x + 44;
            *shotY = man->y + 44;
        }
        else if (man->frameX == 64 || man->frameX == 96)
        {
            *shotX = man->x + 10;
            *shotY = man->y + 44;
        }
        else if (man->frameX == 128 || man->frameX == 160)
        {
            *shotX = man->x + 56;
            *shotY = man->y + 44;
        }
        else
        {
            *shotX = man->x + 0;
            *shotY = man->y + 44;
        }
    }
    // china
    else if (man->spritePick == 4)
    {
        if (man->frameX == 0 || man->frameX == 32)
        {
            *shotX = man->x + 12;
            *shotY = man->y + 46;
        }
        else if (man->frameX == 64 || man->frameX == 96)
        {
            *shotX = man->x + 50;
            *shotY = man->y + 46;
        }
        else if (man->frameX == 128 || man->frameX == 160)
        {
            *shotX = man->x + 60;
            *shotY = man->y + 40;
        }
        else
        {
            *shotX = man->x + 0;
            *shotY = man->y + 40;
        }
    }
}

/***** fienders skott startar från karaktärernas vapen *****/
void checkRunningEnemyDirection(Player *man, int *bulletX, int *bulletY, int id)
{
    // torgny
    if (man->enemies[id].sprite == 1)
    {
        // ner
        if (man->enemies[id].srcRect.x == 0 || man->enemies[id].srcRect.x == 32)
        {
            *bulletX = *bulletX + 54;
            *bulletY = *bulletY + 44;
        }
        // upp
        else if (man->enemies[id].srcRect.x == 64 || man->enemies[id].srcRect.x == 96)
        {
            *bulletX = *bulletX + 8;
            *bulletY = *bulletY + 44;
        }
        // höger
        else if (man->enemies[id].srcRect.x == 128 || man->enemies[id].srcRect.x == 160)
        {
            *bulletX = *bulletX + 58;
            *bulletY = *bulletY + 40;
        }
        // vänster
        else
        {
            *bulletX = *bulletX + 4;
            *bulletY = *bulletY + 40;
        }
    }
    // russia
    else if (man->enemies[id].sprite == 2)
    {
        if (man->enemies[id].srcRect.x == 0 || man->enemies[id].srcRect.x == 32)
        {
            *bulletX = *bulletX + 36;
            *bulletY = *bulletY + 52;
        }
        else if (man->enemies[id].srcRect.x == 64 || man->enemies[id].srcRect.x == 96)
        {
            *bulletX = *bulletX + 26;
            *bulletY = *bulletY + 52;
        }
        else if (man->enemies[id].srcRect.x == 128 || man->enemies[id].srcRect.x == 160)
        {
            *bulletX = *bulletX + 48;
            *bulletY = *bulletY + 50;
        }
        else
        {
            *bulletX = *bulletX + 14;
            *bulletY = *bulletY + 50;
        }
    }
    // murica
    else if (man->enemies[id].sprite == 3)
    {
        if (man->enemies[id].srcRect.x == 0 || man->enemies[id].srcRect.x == 32)
        {
            *bulletX = *bulletX + 44;
            *bulletY = *bulletY + 44;
        }
        else if (man->enemies[id].srcRect.x == 64 || man->enemies[id].srcRect.x == 96)
        {
            *bulletX = *bulletX + 10;
            *bulletY = *bulletY + 44;
        }
        else if (man->enemies[id].srcRect.x == 128 || man->enemies[id].srcRect.x == 160)
        {
            *bulletX = *bulletX + 56;
            *bulletY = *bulletY + 44;
        }
        else
        {
            *bulletX = *bulletX + 0;
            *bulletY = *bulletY + 44;
        }
    }
    // china
    else if (man->enemies[id].sprite == 4)
    {
        if (man->enemies[id].srcRect.x == 0 || man->enemies[id].srcRect.x == 32)
        {
            *bulletX = *bulletX + 12;
            *bulletY = *bulletY + 46;
        }
        else if (man->enemies[id].srcRect.x == 64 || man->enemies[id].srcRect.x == 96)
        {
            *bulletX = *bulletX + 50;
            *bulletY = *bulletY + 46;
        }
        else if (man->enemies[id].srcRect.x == 128 || man->enemies[id].srcRect.x == 160)
        {
            *bulletX = *bulletX + 60;
            *bulletY = *bulletY + 40;
        }
        else
        {
            *bulletX = *bulletX + 0;
            *bulletY = *bulletY + 40;
        }
    }
}

/***** ritar allting på spelplanen *****/
void doRender(Player *man,Bullet b[])
{
    int i,j;
    // set the drawing color to blue
    SDL_SetRenderDrawColor(program.renderer, 0, 0, 255, 255);
    // Clear the screen (to blue)

    SDL_Rect bg = {0,0,1024,768};

    SDL_RenderCopy(program.renderer,man->background,NULL,&bg);

    // ritar spelarens skott
    for(i=0; i<20; i++)
    {
        if(b[i].active == 1)
        {
            SDL_Rect object = {b[i].x , b[i].y,8,8};
            SDL_RenderCopy(program.renderer,man->bullet,NULL,&object);
        }
    }

    // ritar fienders skott
    for(i=0; i<4; i++)
    {
        for(j=0; j<20; j++)
        {
            if(man->enemies[i].bullet[j].active == 1)
            {
                SDL_Rect enemybullet = {man->enemies[i].bullet[j].x,
                                        man->enemies[i].bullet[j].y,8,8
                                       };

                SDL_RenderCopy(program.renderer,man->bullet,NULL,&enemybullet);
            }
        }
    }

    // ritar spelare
    if(man->alive)
    {
        SDL_Rect rect = { man->x, man->y, 64, 64 };
        SDL_Rect src = {man->frameX,0,30,32};

        SDL_RenderCopy(program.renderer,man->texture,&src,&rect);
    }

    // ritar fiender
    for(i=0; i<4; i++)
    {
        if (man->enemies[i].exists && man->enemies[i].alive)
        {
            man->enemies[i].srcRect.w = 30;
            SDL_RenderCopyEx(program.renderer,man->enemies[i].texture, &man->enemies[i].srcRect, &man->enemies[i].dstRect, 0, NULL, 0);
        }
    }
    // ritar scoreboard
    SDL_Rect scoreBg = {0,630,1024,138};
    SDL_RenderCopy(program.renderer,man->scoreBackground,NULL,&scoreBg);
    SDL_Rect scoreHeadRect = {164,637,300,20};
    SDL_RenderCopy(program.renderer,man->scoreHead,NULL,&scoreHeadRect);
    SDL_RenderCopy(program.renderer,man->score,NULL,&man->scoreRect);

    // ritar cd bar för blink
    SDL_Rect blinkRec = {man->blinkRect.x, man->blinkRect.y, man->blinkRect.w, man->blinkRect.h};
    SDL_RenderCopy(program.renderer,man->cdTimer,NULL,&blinkRec);

    SDL_Rect cdRect = {500,637,300,20};
    SDL_RenderCopy(program.renderer,man->cdText,NULL,&cdRect);

    // ritar score
    for(i=0; i<4; i++)
    {
        if(man->enemies[i].exists)
        {
            SDL_RenderCopy(program.renderer,man->enemies[i].score,NULL,&man->enemies[i].scoreRect);
        }
    }

    SDL_RenderPresent(program.renderer);
}

/***** spelaren spawnar på en slupmässig plats på spelplanen *****/
void respawn(Player *man)
{
    int spawn = rand()%9;

    if(spawn == 0)
    {
        man->x = 70;
        man->y = 212;
    }
    else if(spawn == 1)
    {
        man->x = 942;
        man->y = 212;
    }
    else if(spawn == 2)
    {
        man->x = 918;
        man->y = 550;
    }
    else if(spawn == 3)
    {
        man->x = 120;
        man->y = 528;
    }
    else if(spawn == 4)
    {
        man->x = 502;
        man->y = 300;
    }
    else if(spawn == 5)
    {
        man->x = 286;
        man->y = 288;
    }
    else if(spawn == 6)
    {
        man->x = 511;
        man->y = 121;
    }
    else if(spawn == 7)
    {
        man->x = 736;
        man->y = 286;
    }
    else
    {
        man->x = 513;
        man->y = 415;
    }
}
