#include <SDL2/SDL.h>
#include <cstdio>
#include <cstdlib>

#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480
#define NUM_STARS       200

SDL_Window* myWindow = NULL;
SDL_Renderer* myRenderer = NULL;
SDL_Surface* mySurface = NULL;
SDL_Texture* myTexture = NULL;

typedef struct Star {
  int x, y, speed;
} Star;

Star stars[ NUM_STARS ];

int getStarColor( int );

void resetStars() {

  for( int i = 0; i < NUM_STARS; ++i ) {
    stars[i].x = rand() % SCREEN_WIDTH;
    stars[i].y = rand() % SCREEN_HEIGHT;
    stars[i].speed = 1 + ( rand() % 12 );
  }

}

void doStars() {
  for( int i = 0; i < NUM_STARS; ++i ) {
    stars[i].x -= stars[i].speed;

    if( stars[i].x < 0 ) {
      stars[i].x = SCREEN_WIDTH + rand() % 20;
      stars[i].y = rand() % SCREEN_HEIGHT;
      stars[i].speed = 1 + ( rand() % 12 );
    }

  }
}

void updateStars() {
  SDL_Rect rect;

  for( int i = 0; i < NUM_STARS; ++i ) {
    if( stars[i].x < SCREEN_WIDTH ) {
      rect.x = stars[i].x;
      rect.y = stars[i].y;
      rect.w = 1;
      rect.h = 1;

      SDL_FillRect( mySurface, & rect, getStarColor( stars[i].speed ) );
    }
  }

}

int getStarColor( int speed ) {

  SDL_Color color;

  switch( speed ) {

    case 1:
    case 2:
    case 3:
      color.r = 159;
      color.g = 159;
      color.b = 159;
      break;
    case 4:
    case 5:
    case 6:
      color.r = 191;
      color.g = 191;
      color.b = 191;
      break;
    default:
      color.r = 255;
      color.g = 255;
      color.b = 255;
      break;
    }

    return SDL_MapRGB( mySurface->format, color.r, color.g, color.b );

}

void delay( unsigned int frameLimit ) {
  unsigned int ticks = SDL_GetTicks();

  if( frameLimit < ticks ) {
    return;
  }

  if( frameLimit > ticks + 16 ) {
    SDL_Delay( 16 );
  }
  else {
    SDL_Delay( frameLimit - ticks );
  }
}

int main( int argc, char* argv[] ) {

  SDL_Event event;
  unsigned int Limit = SDL_GetTicks() + 16;

  SDL_Init( SDL_INIT_EVERYTHING );

  myWindow = SDL_CreateWindow( "Stars",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH,
                                SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN );

  if( myWindow == NULL ) {
    printf( "Could not create window: %s\n", SDL_GetError() );
    return 1;
  }

  myRenderer = SDL_CreateRenderer( myWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

  if( myRenderer == NULL ) {
    printf( "Could not create renderer: %s\n", SDL_GetError() );
    return 1;
  }



  mySurface = SDL_CreateRGBSurface( 0, 640, 480, 32,
                                    0,
                                    0,
                                    0,
                                    0 );

  myTexture = SDL_CreateTexture( myRenderer,
                                  SDL_PIXELFORMAT_ARGB8888,
                                  SDL_TEXTUREACCESS_STREAMING,
                                  SCREEN_WIDTH, SCREEN_HEIGHT );


  bool running = true;

  resetStars();

  while( running ) {

    if( SDL_PollEvent( &event ) ) {
      if( event.type == SDL_QUIT ) {
        running = false;
      }

      if( event.type == SDL_KEYDOWN ) {
        switch( event.key.keysym.sym ) {
          case SDLK_ESCAPE: running = false;
          default: break;
        }
      }
    }


    doStars();
    SDL_FillRect( mySurface, NULL, 0 );
    updateStars();
    SDL_UpdateTexture( myTexture, NULL, mySurface->pixels, mySurface->pitch );
    SDL_RenderCopy( myRenderer, myTexture, NULL, NULL );
    SDL_RenderPresent( myRenderer );
    SDL_Delay( 1 );
    delay( Limit );
    Limit = SDL_GetTicks() + 16;
}

  SDL_DestroyWindow( myWindow );
  SDL_DestroyRenderer( myRenderer );
  myWindow = NULL;
  myRenderer = NULL;
  SDL_Quit();
  return 0;

}
