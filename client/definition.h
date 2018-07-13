#ifdef __linux__
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#define LINUX 1
#elif _WIN32
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_net.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#define LINUX 0
#endif // _WIN32
#include <time.h>
#include <stdbool.h>

typedef struct
{
    SDL_Texture *texture;
    int x, y, w, h;
} Ledge;

typedef struct
{
    SDL_Texture *cdTimer;
    int x, y, w, h;
} Timer;

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;

} Program;

typedef struct
{
    float x, y;
    float dx;
    float directionX,directionY;
    float vector_unitX,vector_unitY;
    float v_length;
    SDL_Texture *texture;
    int active;
} Bullet;

typedef struct Vector2d
{
    float x, y;
} Vector2d;

typedef struct
{
    TCPsocket tcpsock;
    UDPsocket udpsock;
    SDLNet_SocketSet tcpset,udpset;
    UDPpacket *sendpack,*rcvpack;
    int type;

} Network;

typedef struct
{
    float x,y;
    int frameX,frameY;
    SDL_Texture *texture,*score;
    int exists, sprite;
    SDL_Rect srcRect,dstRect,scoreRect;
    Bullet bullet[20];
    int kills,deaths;
    int alive;
    int justDied;
} Enemy;

typedef struct
{
    Mix_Music *backgroundSound;
    Mix_Music *backgroundLinux;
    Mix_Music *backstreet;
    Mix_Music *cykablyat;
    Mix_Music *america;
    Mix_Music *china;
    Mix_Chunk *bulletShot;
    Mix_Chunk *deathsound1;
    Mix_Chunk *deathsound2;
    Mix_Chunk *deathsound3;
    Mix_Chunk *deathsound4;

    Mix_Chunk *Ldeathsound1;
    Mix_Chunk *Ldeathsound2;
    Mix_Chunk *Ldeathsound3;
    Mix_Chunk *Ldeathsound4;

} Sound;

typedef struct
{
    Enemy enemies[10];
    Ledge ledges[7];
    float x, y;
    int frameX,frameY;
    int x1, y1;
    int hitid,alive,kills,deaths;
    int currentTime, cdTime, spellReady, blinked;
    SDL_Texture *texture,*background,*scoreBackground,*bullet,*cdTimer,*cdText,*score,*scoreHead,*ipTexture,*ipAddressText;
    int thinkTime,disconnected;
    int shot,bulletid,blinkX,blinkY,bulletNo,connected;
    int gameStarted;
    TTF_Font* arial;
    char *scoreText;
    int id;
    int justShot;
    int deltaTimeMs,roundTime,currentRTime,lastRTime;
    float deltaTimeS;
    int spritePick;
    SDL_Rect blinkRect,scoreRect,ipRect;
    char *ip;
    Sound sounds;
} Player;




typedef struct
{
    SDL_Texture *texture;
    SDL_Rect rect;
} Menu;

Bullet bullet;
Program program;

