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
  init();	// CREATES WINDOW AND RENDERER
  initVars();
  int counter = 0;
  int programRunning = 1;
  while(programRunning)
  {
    refreshTouch();
    loadPage(pageNumber);
    cycleCount();
    timestampRefresh();
    printf("PAGE NUMBER: %d\n", pageNumber);
  }	  
  close();
  SDL_Quit();
  return 0;
}
