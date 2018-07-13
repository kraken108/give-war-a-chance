#include "definition.h"

/***** laddar in bilder, skapar text till scoreboard och cd *****/
void initMenu(Menu *menu,Player *man)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    // skapar spelfönstret, 1024x768
    program.window = SDL_CreateWindow("Game Window",                     // window title
                                      SDL_WINDOWPOS_UNDEFINED,           // initial x position
                                      SDL_WINDOWPOS_UNDEFINED,           // initial y position
                                      1024,                               // width, in pixels
                                      768,                               // height, in pixels
                                      0                                  // flags
                                     );
    program.renderer = SDL_CreateRenderer(program.window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface *image = IMG_Load("pictures/MainMenu.bmp");
    SDL_Texture *texture;
    texture = SDL_CreateTextureFromSurface(program.renderer,image);
    SDL_FreeSurface(image);

     SDL_Surface *ipImage;
     SDL_Texture *ipText;

    if(LINUX)
    {
    ipImage = IMG_Load("pictures/enterIP.png");
    ipText = SDL_CreateTextureFromSurface(program.renderer,ipImage);
    }
    else
    {
    ipImage = IMG_Load("pictures/enterip.PNG");
    ipText = SDL_CreateTextureFromSurface(program.renderer,ipImage);
    }


    SDL_FreeSurface(ipImage);
    menu->rect.x = 0;
    menu->rect.y = 0;
    menu->rect.w = 1024;
    menu->rect.h = 768;
    menu->texture = texture;
    man->ipTexture = ipText;
    man->arial = TTF_OpenFont("arialbd.ttf", 48);
    man->scoreText = (char*)malloc(100);

    // text för scoreboard
    SDL_Surface* surfaceMessage;
    TTF_Font* arial = TTF_OpenFont("arialbd.ttf", 48);
    SDL_Color black = {0, 0, 0};
    surfaceMessage = TTF_RenderText_Blended(arial, "Player      Kills       Deaths", black);
    man->scoreHead = SDL_CreateTextureFromSurface(program.renderer,surfaceMessage);
    SDL_FreeSurface(surfaceMessage);

    // text för cooldown av blink
    SDL_Surface* cdtext;
    cdtext = TTF_RenderText_Blended(arial, "Teleport cooldown", black);
    man->cdText = SDL_CreateTextureFromSurface(program.renderer,cdtext);
    SDL_FreeSurface(cdtext);
}

/***** skapar introt till spelet *****/
void intro()
{
    TTF_Font* Tough = TTF_OpenFont("BlackOpsOne-Regular.ttf", 24);

    SDL_Color White = {255, 255, 255};

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Tough, "Give War A Chance", White);
    SDL_Surface* subtitle = TTF_RenderText_Solid(Tough,"Press any key to continue",White);

    SDL_Texture* Message = SDL_CreateTextureFromSurface(program.renderer, surfaceMessage);
    SDL_Texture* Submessage = SDL_CreateTextureFromSurface(program.renderer,subtitle);
    int *i;
    SDL_Rect Message_rect;
    SDL_Rect mini;
    Message_rect.y = 768/2;
    Message_rect.w = 300;
    Message_rect.h = 100;
    Message_rect.x =0;


    mini.x = 400;
    mini.y = 600;
    mini.w = Message_rect.w-30;
    mini.h = Message_rect.h-30;

    Mix_Music *introMusic;
    if(LINUX)
    {
      introMusic = Mix_LoadMUS("Soundeffects/ATeam_theme5.wav");
    }
    else
    {
      introMusic = Mix_LoadMUS("Soundeffects/ATeam_theme5.MP3");
    }
    Mix_PlayMusic(introMusic, -1);

    SDL_Event event;
    bool quit = false;
    while(!quit)
    {
     while(SDL_PollEvent(&event))
     {
       if(event.type == SDL_QUIT)
       {
          quit = true;
       }
       if(event.type == SDL_KEYDOWN)
       {
          quit = true;
       }
       if(event.type == SDL_MOUSEBUTTONDOWN)
       {
          quit = true;
       }
     }
    SDL_SetRenderDrawColor(program.renderer, 0, 0, 255, 255);

    SDL_RenderClear(program.renderer);
     if(Message_rect.x != 1024/2-125)
     {
            Message_rect.x++;
     }
     SDL_SetRenderDrawColor(program.renderer, 0, 0, 255, 255);
     SDL_RenderCopy(program.renderer, Message, NULL, &Message_rect);
     SDL_RenderCopy(program.renderer,Submessage,NULL,&mini);
     SDL_RenderPresent(program.renderer);
     SDL_Delay(20);
     }
    TTF_CloseFont(Tough);
}

/***** hjälpmenyn som visar hur man spelar *****/
void helpMenu()
{
    SDL_Event e;
    int x,y;

    SDL_Surface* image;
    SDL_Texture* helpMenu;
    SDL_Texture* mainMenu;

    image = SDL_LoadBMP("pictures/HelpMenu.bmp");
    helpMenu = SDL_CreateTextureFromSurface(program.renderer, image);
    SDL_FreeSurface(image);

    SDL_RenderCopy(program.renderer, helpMenu, NULL, NULL);
    SDL_RenderPresent(program.renderer);

    int back=0;

    while(back==0)
    {
        while(SDL_PollEvent(&e)!=0)
        {
            if(e.key.keysym.sym==SDLK_ESCAPE)
            {
                SDL_DestroyTexture(image);
                image = SDL_LoadBMP("pictures/MainMenu.bmp");
                mainMenu = SDL_CreateTextureFromSurface(program.renderer, image);
                SDL_FreeSurface(image);
                SDL_RenderCopy(program.renderer, mainMenu, NULL, NULL);
                SDL_RenderPresent(program.renderer);

                back=1;
            }
        }
    }
    return;
}

/***** låter spelaren använda menyn *****/
int handleMenu(int *exit)
{
    SDL_Event e;
    int x,y;
    int quit = 0;
    int pickCharacter = 0;
    while(!quit)
    {
        //Hantera eventer som står på kö
        while(SDL_PollEvent(&e)!=0)
        {
            SDL_GetMouseState(&x, &y);
            //Stäng fönstret & avsluta SDL om användaren klickar på X-rutan på fönstret
            if(e.type==SDL_QUIT)
            {
                quit=1;
                *exit = 1;
            }
            //Gå till menyn "Choose Character" om användaren trycker på "Play Game"
            else if(x>226 && x<728 && y>275 && y<410 && e.type==SDL_MOUSEBUTTONDOWN)
            {
                if(e.button.button==SDL_BUTTON_LEFT)
                {
                    quit = 1;
                    *exit = 0;
                    pickCharacter = 1;
                    // chooseCharacter(); //Går till funktionen för "Choose Character"
                }
            }
            //Stäng fönstret & avsluta SDL om användaren trycker på "Exit"
            else if(x>226 && x<728 && y>432 && y<558 && e.type==SDL_MOUSEBUTTONDOWN)
            {
                if(e.button.button==SDL_BUTTON_LEFT)
                {
                    *exit = 1;
                    quit=1;
                }
            }
            //Användaren trycker på "Help"
            else if(x>794 && x<995 && y>29 && y<141 && e.type==SDL_MOUSEBUTTONDOWN)
            {
                if(e.button.button==SDL_BUTTON_LEFT)
                {
                    helpMenu();
                }
            }
        }
    }
    return pickCharacter;
}

/***** visar main menu *****/
void displayMenu(Menu menu)
{
    SDL_RenderClear(program.renderer);
    SDL_RenderCopy(program.renderer,menu.texture,&menu.rect,&menu.rect);
    SDL_RenderPresent(program.renderer);
}


void initPick(Menu *pick)
{
    SDL_Surface *image = IMG_Load("pictures/ChooseCharacter.bmp");
    SDL_Texture *texture;
    texture = SDL_CreateTextureFromSurface(program.renderer,image);
    SDL_FreeSurface(image);
    pick->rect.x = 0;
    pick->rect.y = 0;
    pick->rect.w = 1024;
    pick->rect.h = 768;
    pick->texture = texture;
}

int handlePick(int *pickCharacter,Player *man)
{
    int x, y;
    SDL_Event e;
    int ingame = 0;

    //Medan fönstret är öppet
    //Hantera eventer som står på kö
    while(SDL_PollEvent(&e)!=0)
    {
        //Positionen på musen i x-led och y-led
        SDL_GetMouseState(&x, &y);
        //printf("x=%d \ny=%d\n", x, y);
        //Stäng fönstret om användaren klickar på X-rutan på fönstret
        if(e.type==SDL_QUIT)
        {
            ingame=-1;
            *pickCharacter = 0;
        }
        //Stäng fönstret om användaren trycker på ESC-knappen på tangentbordet
       // else if(e.key.keysym.sym==SDLK_ESCAPE)
       // {
       //     ingame=-1;
       //     *pickCharacter = 0;

     //   }
        //Återgå till huvudmenyn om användaren trycker back-pilen genom ett vänsterklick
        else if(x>400 && x<606 && y>626 && y<725 && e.type==SDL_MOUSEBUTTONDOWN)
        {
            if(e.button.button==SDL_BUTTON_LEFT)
            {
                //Laddar upp huvudmenyn på fönstret och programräknaren återgår till main-loopen
                *pickCharacter = 0;
                ingame = -1;
            }
        }
        //Användaren väljer första gubben
        else if(x>131 && x<242 && y>357 && y<496 && e.type==SDL_MOUSEBUTTONDOWN)
        {
            if(e.button.button==SDL_BUTTON_LEFT)
            {
                ingame = 1;
                man->spritePick = 1;
                //character(1);
            }
        }
        //Användaren väljer andra gubben
        else if(x>344 && x<437 && y>357 && y<496 && e.type==SDL_MOUSEBUTTONDOWN)
        {
            if(e.button.button==SDL_BUTTON_LEFT)
            {
                ingame = 1;
                man->spritePick = 2;
                //character(2);
            }
        }
        //Användaren väljer tredje gubben
        else if(x>549 && x<669 && y>357 && y<496 && e.type==SDL_MOUSEBUTTONDOWN)
        {
            if(e.button.button==SDL_BUTTON_LEFT)
            {
                ingame = 1;
                man->spritePick = 3;
                //character(3);
            }
        }
        //Användaren väljer fjärde gubben
        else if(x>771 && x<870 && y>357 && y<496 && e.type==SDL_MOUSEBUTTONDOWN)
        {
            if(e.button.button==SDL_BUTTON_LEFT)
            {
                ingame = 1;
                man->spritePick = 4;
                //character(4);
            }
        }
    }

    return ingame;
}

void generateScoreboard(Player *man)
{
    int i,scoreposition=657;
    SDL_Surface* surfaceMessage;
    SDL_Color black = {0, 0, 0};


    /** FOR PLAYER **/
    if(man->spritePick == 1)
    {
        sprintf(man->scoreText,"Torgny     %d      %d",man->kills,man->deaths);
    }
    else if(man->spritePick == 2)
    {
        sprintf(man->scoreText,"Russia     %d      %d",man->kills,man->deaths);
    }
    else if(man->spritePick == 3)
    {
        sprintf(man->scoreText,"Murica    %d      %d",man->kills,man->deaths);
    }
    else
    {
        sprintf(man->scoreText,"China      %d      %d",man->kills,man->deaths);
    }
    surfaceMessage = TTF_RenderText_Blended(man->arial,man->scoreText,black);
    SDL_DestroyTexture(man->score);
    man->score = SDL_CreateTextureFromSurface(program.renderer,surfaceMessage);
    SDL_Rect rect = {164,scoreposition,250,20};
    man->scoreRect = rect;
    SDL_FreeSurface(surfaceMessage);
    scoreposition = scoreposition+20;

    /** FOR ENEMIES **/
    for(i=0; i<4; i++)
    {
        if(man->enemies[i].exists)
        {
            printf("printing enemy %d\n",i);
            if(man->enemies[i].sprite == 1)
            {
                sprintf(man->scoreText,"Torgny     %d      %d",man->enemies[i].kills,man->enemies[i].deaths);
            }
            else if(man->enemies[i].sprite == 2)
            {
                sprintf(man->scoreText,"Russia     %d      %d",man->enemies[i].kills,man->enemies[i].deaths);
            }
            else if(man->enemies[i].sprite == 3)
            {
                sprintf(man->scoreText,"Murica     %d      %d",man->enemies[i].kills,man->enemies[i].deaths);
            }
            else
            {
                sprintf(man->scoreText,"China      %d      %d",man->enemies[i].kills,man->enemies[i].deaths);
            }

            surfaceMessage = TTF_RenderText_Blended(man->arial,man->scoreText,black);
            SDL_DestroyTexture(man->enemies[i].score);
            man->enemies[i].score = SDL_CreateTextureFromSurface(program.renderer,surfaceMessage);
            SDL_Rect rect = {164,scoreposition,250,20};
            man->enemies[i].scoreRect = rect;
            SDL_FreeSurface(surfaceMessage);
            scoreposition = scoreposition+20;
        }
    }
}
