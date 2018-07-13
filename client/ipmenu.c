#include "definition.h"

/***** spelaren skriver in ip till servern *****/
int enterIP(Player *man)
{
    SDL_Event e;
    int i=0;
    int x,y;
    int length = 1;
    int rectlen = 20;
    SDL_Color black = {0, 0, 0};
    SDL_Surface* surfaceMessage;
    SDL_Rect bg = {0,0,1024,768};
    SDL_Rect rect = {350,323,rectlen*length,35};

    char *text = (char*)malloc(100);

    while(strlen(text)!=0)
    {
        length = strlen(text);
        text[length-1] = '\0';
    }

    while(1)
    {
        SDL_StartTextInput();
        while(SDL_PollEvent(&e) !=0)
        {
            // gå vidare om spelaren klickar på connect
            SDL_GetMouseState(&x, &y);
            if(x>376 && x<645 && y>488 && y<587 && e.type==SDL_MOUSEBUTTONDOWN)
            {
                if(e.button.button==SDL_BUTTON_LEFT)
                {
                    man->ip = text;
                    return 1;
                }
            }

            else if(e.type==SDL_TEXTINPUT || e.type==SDL_KEYDOWN)
            {
                // låter spelaren sudda sin text
                if(e.type==SDL_KEYDOWN && e.key.keysym.sym==SDLK_BACKSPACE && i>=0)
                {
                    length = strlen(text);
                    text[length-1] = '\0';
                }
                // gå vidare om spelaren tycker på enter
                else if (e.type==SDL_KEYDOWN && e.key.keysym.sym==SDLK_RETURN)
                {
                    man->ip = text;
                    return 1;
                }
                // sparar det spelaren skriver
                else if(e.type == SDL_TEXTINPUT)
                {
                    strcat(text,e.text.text);
                }
                // spelaren kan backa med escape
                else if(e.type == SDL_KEYDOWN && e.key.keysym.sym==SDLK_ESCAPE)
                {
                    return 0;
                }
            }
        }

        // uppdaterar det spelaren skrivit på skärmen
        SDL_RenderClear(program.renderer);
        SDL_RenderCopy(program.renderer,man->ipTexture,NULL,&bg);
        surfaceMessage = TTF_RenderText_Blended(man->arial,text,black);
        man->ipAddressText = SDL_CreateTextureFromSurface(program.renderer,surfaceMessage);
        length = strlen(text);
        rect.w = rectlen*length;
        man->ipRect = rect;
        SDL_FreeSurface(surfaceMessage);
        SDL_RenderCopy(program.renderer,man->ipAddressText,NULL,&man->ipRect);
        SDL_RenderPresent(program.renderer);
        SDL_DestroyTexture(man->ipAddressText);
        SDL_Delay(40);
    }
    SDL_StopTextInput();
}
