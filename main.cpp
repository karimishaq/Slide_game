#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int data[9]={8, 7, 6, 5, 4, 3, 2, 1, 0};
int blank=4;
int utama(SDL_Event *evt);
SDL_Surface* screenSurface = NULL;

struct status{
    int lpetak=0;
    int tpetak=0;
    SDL_Surface* gambar = NULL;
    SDL_Surface* menang = NULL;
}stat;

int tukar(int *a, int i, int j){
int tmp;
    tmp=data[i];
    a[i]=a[j];
    a[j]=tmp;
    return 0;
}

int cocok(int *a){
int i, total=0;
    for(i=0;i<9;i++){
        if(i==a[i]){
            total++;
        }
    }
    return total;
}

int tampilkan(SDL_Surface *latar, SDL_Surface *gambar){
int i, lebar, tinggi;
SDL_Rect rct, rcd;
    lebar=gambar->w/3;
    tinggi=gambar->h/3;
    for(i=0;i<9;i++){
        rcd.x=(i%3)*(gambar->w/3);
        rcd.y=(i/3)*(gambar->h/3);
        rcd.w=lebar;
        rcd.h=tinggi;
        if(i!=blank){
            rct.x=(data[i]%3)*(gambar->w/3);
            rct.y=(data[i]/3)*(gambar->h/3);
            rct.w=lebar;
            rct.h=tinggi;

            SDL_BlitSurface(gambar, &rct, latar, &rcd);
        }else{
            SDL_FillRect( screenSurface, &rcd, SDL_MapRGB( screenSurface->format, 0xFF, 0x00, 0xFF ) );
        }
    }
    return 0;
}

int main( int argc, char* args[] ){
	SDL_Window *window = NULL;
	SDL_Surface *gambar1=NULL;
	SDL_Surface *gambar2=NULL;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 && IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) <0){
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}else{
		window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

		if( window != NULL ){
            SDL_Event sevent;

			//Get window surface
			screenSurface = SDL_GetWindowSurface( window );
            SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0x00, 0xFF ) );

            gambar1=IMG_Load("slider.jpg");
            gambar2=IMG_Load("menang.png");
            if(gambar1!=NULL && gambar2!=NULL){
                stat.gambar=SDL_ConvertSurface(gambar1, screenSurface->format, 0);
                stat.lpetak=gambar1->w/3;
                stat.tpetak=gambar1->h/3;
                SDL_FreeSurface(gambar1);

                stat.menang=SDL_ConvertSurface(gambar2, screenSurface->format, 0);
                SDL_FreeSurface(gambar2);

                tampilkan(screenSurface, stat.gambar);
                while(sevent.type!=SDL_QUIT){
                    while(SDL_PollEvent(&sevent)){
                        utama(&sevent);
                        SDL_UpdateWindowSurface(window);
                    }
                }

                SDL_FreeSurface(stat.gambar);
                SDL_FreeSurface(stat.menang);
            }
            SDL_DestroyWindow( window );
		}
	}

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}

int utama(SDL_Event *evt){
int x1, y1, x2, y2;
    switch(evt->type){
        case SDL_MOUSEBUTTONUP:
            x1=evt->button.x/stat.lpetak;
            y1=evt->button.y/stat.tpetak;
            x2=blank%3;
            y2=blank/3;
            if(x1<3 && y2<3){
                if((y1==y2 && (x2-x1==1 || x1-x2==1)) || (x1==x2 && (y2-y1==1 || y1-y2==1))){
                    tukar(data, blank, y1*3+x1);
                    blank=y1*3+x1;
                }
            }
            if(cocok(data)==9){
                SDL_BlitSurface(stat.menang, 0, screenSurface, 0);
            }else{
                tampilkan(screenSurface, stat.gambar);
            }
            break;
        case SDL_KEYDOWN:
            switch(evt->key.keysym.sym){
                case SDLK_DOWN:
                    if(blank>2){
                        tukar(data, blank, blank-3);
                        blank-=3;
                    }
                    break;
                case SDLK_UP:
                    if(blank<6){
                        tukar(data, blank, blank+3);
                        blank+=3;
                    }
                    break;
                case SDLK_RIGHT:
                    if((blank%3)>0){
                        tukar(data, blank, blank-1);
                        blank--;
                    }
                    break;
                case SDLK_LEFT:
                    if((blank%3)<2){
                        tukar(data, blank, blank+1);
                        blank++;
                    }
                    break;
                default:
                    break;
            }

            SDL_FillRect(screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0x00, 0xFF ) );
            if(cocok(data)==9){
                tampilkan(screenSurface, stat.menang);
            }else{
                tampilkan(screenSurface, stat.gambar);
            }
            break;
        default:
            break;
    }
    return 0;
}
