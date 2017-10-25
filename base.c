#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "func_init.h"
#include "func_decl.h"
#include <time.h>

clock_t start, end;
double cpu_time_used;

int pageNumber = 0;

int main()
{
   int programRunning = 1;
  init();	
  initVars();
  while(programRunning)
  {
    start = clock();
    refreshTouch();
    loadPage(pageNumber);
    cycleCount();
    timestampRefresh();
    end = clock();
    cpu_time_used = ((double) (end - start) / CLOCKS_PER_SEC);
    /*printf("CPU TIME USED: %f\n", cpu_time_used);*/
  }
  	  
  close();
  SDL_Quit();
  return 0;
}
