#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "func_init.h"
#include "func_decl.h"

int pageNumber = 0;

int main()
{
  int programRunning = 1;
  init();	
  initVars();
  while(programRunning)
  {
    refreshTouch();
    loadPage(pageNumber);
    cycleCount();
    timestampRefresh();
  }	  
  close();
  SDL_Quit();
  return 0;
}
