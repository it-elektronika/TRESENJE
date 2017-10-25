#define DROWSSAP  "geslo"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "func_decl.h"
#include "func_init.h"

int adminAccess = 0;
int cycleCounter = 0;
extern int pageNumber;

char buffCC[100];
char buffMV[100];
char ebVal[42][5];

int menuValueCheck = 0; 
int focusBoxNumber = 0;

int hasFocus = 0;
char text1[10];
char text2[10];
char editText[32] = "text:";
char editBox_id[6][7];



int row = 0;
int column = 0;
int targetx;
int targety;
int padx = 50;
int pady = 50;

int editBox_id_counter = 1;
int editBox_x[6][7];
int editBox_y[6][7];

int PiControlHandle_g = -1;
int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 800;
int counter = 0;
int timestampDown = 0;
int timestampUp = 0;
int timestamp = 0;
int oldtimestamp = 0;
int textureHeight = 0;
int textureWidth = 0;
int a = 90;
int menuOpened = 0;
const char *pageTitles[7];
const char *inputLabels[14];
const char *outputLabels[14];

const char* n_num[10];
const char * var_num[8];
const char* inp_arr[14];
const char* out_arr[14];
const char* i_num[14];
const char* o_num[14];
int input[14];
int output[14];

SDL_Color textColor = {255, 255, 255, 255};
SDL_Renderer *renderer = NULL;
SDL_Event evt;
SDL_Window *window = NULL;
SDL_Texture *texture = NULL;
TTF_Font *textFont = NULL; 
SDL_Point touchLocation = {-1, -1};

struct editbox
{
  int x;
  int y;
  int w;
  int h;
  int hasFocus;
  char value[5];
};

struct editbox eb[42];

int init()
{
  
  int flags=IMG_INIT_JPG|IMG_INIT_PNG;
  int initted=IMG_Init(flags);	
  pageNumber = 0;

  if((SDL_Init(SDL_INIT_VIDEO||SDL_INIT_AUDIO||SDL_INIT_TIMER)) != 0)
  {
    SDL_Log("Unable to initialize SDL:%s ", SDL_GetError());
    return 1;                                                                               
  }  

  window = SDL_CreateWindow("IT-Elektronika", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);
  
  if (window == NULL)
  {
    return 1;
  }		  

  renderer = SDL_CreateRenderer(window, - 1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(renderer == NULL)
  {
    printf("RENDERER IS NULL");
  }
		    
  if((initted&flags) != flags)               
  {
    printf("IMG_Init: Failed to init required jpg and png support!\n");
    printf("IMG_Init: %s\n", IMG_GetError());
  }
  if(TTF_Init() == -1)
  {
    printf("SDL Could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
  }
  textFont = TTF_OpenFont("helvetica.ttf", 30);

  if(!textFont)
  {
    printf("TTF_OpenFont: %s\n", TTF_GetError());
  }
    
  return 15;
}

void freeTexture(void)
{
  if(texture != NULL)
  {
    SDL_DestroyTexture(texture);	  
    texture = NULL;
    textureWidth = 0;
    textureHeight = 0;
  }
}

int writeText(const char *text, SDL_Color textColor)
{
  SDL_Surface* textSurface = TTF_RenderText_Solid(textFont, text, textColor);

  freeTexture();

  if(textSurface == NULL)
  {
    printf("Unable to render text surface! SDL Error: %s\n", SDL_GetError());
  }
  else
  {
    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if(texture == NULL)
    {
      printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
    }
    else
    {
      textureWidth = textSurface -> w;
      textureHeight = textSurface -> h;
    }
    SDL_FreeSurface(textSurface);
  }
  return texture != NULL;
}

int loadIOLights(int value)
{
  SDL_Surface *imageSurface;

  freeTexture();
  if(value == 1)
  {
    imageSurface = IMG_Load("/home/pi/tresenje_sdl/on.png");
  }	  
  else if(value == 0)
  {
  imageSurface = IMG_Load("/home/pi/tresenje_sdl/off.png");
  }
    if(imageSurface == NULL)
  {
    printf("unable to render image surface! SDL_image Error: %s\n", IMG_GetError());
  }
  else
  {
    texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    if(texture == NULL)
    {
      printf("Unable to create texture from rendered image! SDL_image Error: %s\n", SDL_GetError());
    }
    else
    {
      textureWidth = imageSurface -> w;
      textureHeight = imageSurface -> h;
    }
    SDL_FreeSurface(imageSurface);
  }
  return texture != NULL;
}

void draw(void)
{
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
  SDL_RenderClear(renderer);
}

void drawMenu(void)
{
  SDL_Rect fillRect = {1205, 0, 75, 75};
  SDL_SetRenderDrawColor(renderer, 0x50, 0x50, 0xFF, 0xFF);
  SDL_RenderFillRect(renderer, &fillRect); 
}

void close()
{
  freeTexture();	
  TTF_CloseFont(textFont);  
  textFont = NULL; 	
  
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  IMG_Quit();  
  TTF_Quit();
  SDL_Quit();
}

void initVars()
{
  int x = 50;
  int y = 100;
  int w = 125;
  int h = 100;
  editBox_id_counter = 1;


  for(row = 0; row < 6; ++row)
  {
    for(column = 0; column < 7; ++column)
    {
      eb[editBox_id_counter].x = x;
      eb[editBox_id_counter].y = y;
      eb[editBox_id_counter].w = w;
      eb[editBox_id_counter].h = h;
      eb[editBox_id_counter].hasFocus = 0;
      strcpy(eb[editBox_id_counter].value, "0\0");
      /*printf("ebval[%d]: %s\n", editBox_id_counter, ebVal[editBox_id_counter]); */
      
      editBox_id_counter++;
      x = x + w;
    }
    x = 50;
    y = y + h;  
  }
  x = 50;
  y = 100;


  n_num[0] = "1";
  n_num[1] = "2";
  n_num[2] = "3";
  n_num[3] = "4";
  n_num[4] = "5";
  n_num[5] = "6";
  n_num[6] = "7";
  n_num[7] = "8";
  n_num[8] = "9";
  n_num[9] = "10";
  
  var_num[0] = "N";
  var_num[1] = "ANG.1";
  var_num[2] = "ANG.2";
  var_num[3] = "VEL.";
  var_num[4] = "ACC.";
  var_num[5] = "DEC.";
  var_num[6] = "DLY";
  var_num[7] = "RPT.";

  i_num[0] = "I_1";  
  i_num[1] = "I_2"; 
  i_num[2] = "I_3"; 
  i_num[3] = "I_4"; 
  i_num[4] = "I_5";  
  i_num[5] = "I_6"; 
  i_num[6] = "I_7"; 
  i_num[7] = "I_8"; 
  i_num[8] = "I_9"; 
  i_num[9] = "I_10"; 
  i_num[10] = "I_11"; 
  i_num[11] = "I_12"; 
  i_num[12] = "I_13"; 
  i_num[13] = "I_14";
 
  o_num[0] = "O_1";  
  o_num[1] = "O_2"; 
  o_num[2] = "O_3"; 
  o_num[3] = "O_4"; 
  o_num[4] = "O_5";  
  o_num[5] = "O_6"; 
  o_num[6] = "O_7"; 
  o_num[7] = "O_8"; 
  o_num[8] = "O_9"; 
  o_num[9] = "O_10"; 
  o_num[10] = "O_11"; 
  o_num[11] = "O_12"; 
  o_num[12] = "O_13"; 
  o_num[13] = "O_14"; 

  inp_arr[0] = "INPUT 1:";
  inp_arr[1] = "INPUT 2:";
  inp_arr[2] = "INPUT 3:";
  inp_arr[3] = "INPUT 4:";
  inp_arr[4] = "INPUT 5:";
  inp_arr[5] = "INPUT 6:";
  inp_arr[6] = "INPUT 7:";
  inp_arr[7] = "INPUT 8:";
  inp_arr[8] = "INPUT 9:";
  inp_arr[9] = "INPUT 10:";
  inp_arr[10] = "INPUT 11:";
  inp_arr[11] = "INPUT 12:";
  inp_arr[12] = "INPUT 13:";
  inp_arr[13] = "INPUT 14:";
  out_arr[0] = "OUTPUT 1:";
  out_arr[1] = "OUTPUT 2:";
  out_arr[2] = "OUTPUT 3:";
  out_arr[3] = "OUTPUT 4:";
  out_arr[4] = "OUTPUT 5:";
  out_arr[5] = "OUTPUT 6:";
  out_arr[6] = "OUTPUT 7:";
  out_arr[7] = "OUTPUT 8:";
  out_arr[8] = "OUTPUT 9:";
  out_arr[9] = "OUTPUT 10:";
  out_arr[10] = "OUTPUT 11:";
  out_arr[11] = "OUTPUT 12:";
  out_arr[12] = "OUTPUT 13:";
  out_arr[13] = "OUTPUT 14:";
}

void drawIOLights()
{ 
  int a = 95;
  int i;
  for(i = 0; i < 14; ++i)
  {
    loadIOLights(input[i]);
    render(225, a, NULL, 0.0, NULL, SDL_FLIP_NONE); 
    a = a + 50; 
  }
  a = 95;

  for(i = 0; i < 14; ++i)
  {
    loadIOLights(output[i]);
    render(625, a, NULL, 0.0, NULL, SDL_FLIP_NONE); 
    a = a + 50; 
  }   
}

void toggleOutput(const char *io_num, int io_n, int x1, int x2, int y1, int y2)
{
  if(touchLocation.x >= x1 && touchLocation.x <= x2 && touchLocation.y >= y1 && touchLocation.y <= y2
     && oldtimestamp != timestamp)
  {
    if(output[io_n-1] == 0)
    {
      writeVariableValue(io_num, 1);
    }
    else if(output[io_n-1] == 1)
    {
      writeVariableValue(io_num, 0);
    }
  }	  
}

void render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
  
  SDL_Rect renderQuad;
  renderQuad.x = x;
  renderQuad.y = y;
  renderQuad.w = textureWidth;
  renderQuad.h = textureHeight;

  if(clip != NULL)
  {
    renderQuad.w = clip -> w;
    renderQuad.h = clip -> h;
  }
  SDL_RenderCopyEx(renderer, texture, clip, &renderQuad, angle, center, flip);
}

void readVariables()
{
  int i;
  for(i = 0; i < 14; ++i)
  {
    input[i] = readVariableValue(i_num[i]);
    output[i] = readVariableValue(o_num[i]);
  }
}

void drawLabels(void)
{
  int i;
  for(i = 0; i < 14; ++i)
  {
    inputLabels[i] = inp_arr[i];
    outputLabels[i] = out_arr[i];
  }
  
  a = 90;
  for(i = 0; i < 14; ++i)
  {
    writeText(inputLabels[i], textColor);
    render(50, a, NULL, 0.0, NULL, SDL_FLIP_NONE);
    a = a + 50;
  }
  a = 90;

  for(i = 0; i < 14; ++i)
  {
    writeText(outputLabels[i], textColor);
    render(400, a, NULL, 0.0, NULL, SDL_FLIP_NONE);
    a = a + 50;
  }

}

void writeVariables()
{
  int y1 = 95;
  int y2 = 145;
  int i;
  for(i = 0; i < 14; ++i)
  {
    toggleOutput(o_num[i], (i+1), 625, 675, y1, y2);
    y1 = y1 + 50;
    y2 = y2 + 50;
  }
}

void drawPageTitle(int pageNumber)
{
  SDL_SetRenderDrawColor(renderer, 0x50, 0x50, 0xFF, 0xFF);
  SDL_RenderDrawLine(renderer, 0, 75, 1280, 75);

  pageTitles[0] = "MAIN";
  pageTitles[1] = "ADMIN";
  pageTitles[2] = "EDITOR";
  pageTitles[3] = "LOADER";
  pageTitles[4] = "SETTINGS";
  pageTitles[5] = "I / O STATUS";
  
  writeText(pageTitles[pageNumber], textColor);
  render(50, 25, NULL, 0.0, NULL, SDL_FLIP_NONE);
  
}

void openMenu()
{
  if(touchLocation.x >= 1205 && touchLocation.y <= 75 && timestamp > oldtimestamp && menuOpened == 0) 
  {
    menuOpened = 1;
    menuValueCheck = cycleCounter;
  }
  if(menuOpened == 1)
  {
    SDL_Rect closeMenu = { 1205, 0, 75, 75};
    SDL_Rect menuBox = {900, 0, 500, 525};
    SDL_SetRenderDrawColor(renderer, 0x50, 0x50, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &menuBox);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderFillRect(renderer, &closeMenu);
    SDL_RenderDrawLine(renderer, 900, 75, 1280, 75);
    SDL_RenderDrawLine(renderer, 900, 150, 1280, 150);
    SDL_RenderDrawLine(renderer, 900, 225, 1280, 225);
    SDL_RenderDrawLine(renderer, 900, 300, 1280, 300);
    SDL_RenderDrawLine(renderer, 900, 375, 1280, 375);
    SDL_RenderDrawLine(renderer, 900, 450, 1280, 450);
    SDL_RenderDrawLine(renderer, 900, 525, 1280, 525);
    if(touchLocation.x >= 900 && 75 < touchLocation.y && touchLocation.y <= 150 && timestamp > oldtimestamp && cycleCounter != menuValueCheck)
    {
      pageNumber = 0;
      menuOpened = 0;
    }
    
    if(touchLocation.x >= 900 &&  150 < touchLocation.y && touchLocation.y <= 225 && timestamp > oldtimestamp && cycleCounter != menuValueCheck)      
    {
      pageNumber = 1;
      menuOpened = 0;
    }
    if(touchLocation.x >= 900 && 225 < touchLocation.y && touchLocation.y <= 300 && timestamp > oldtimestamp && cycleCounter != menuValueCheck)
    {
      pageNumber = 2;
      menuOpened = 0;
    }
    if(touchLocation.x >= 900 && 300 < touchLocation.y  && touchLocation.y <= 375 && timestamp > oldtimestamp && cycleCounter != menuValueCheck)
    {
      pageNumber = 3;
      menuOpened = 0;
    }
    if(touchLocation.x >= 900 && 375 < touchLocation.y && touchLocation.y <= 450 && timestamp > oldtimestamp && cycleCounter != menuValueCheck)
    { 
      pageNumber = 4;
      menuOpened = 0;
    }
    if(touchLocation.x >= 900 && 450 < touchLocation.y && touchLocation.y <= 525 && timestamp > oldtimestamp && cycleCounter != menuValueCheck)
    { 
      pageNumber = 5;
      menuOpened = 0;
    }
    
    drawMenuItems();
  }

}

void closeMenu(void)
{
  if(touchLocation.x >= 1205 && touchLocation.y <= 75 && timestamp > oldtimestamp && menuOpened == 1 && cycleCounter != menuValueCheck) 
  { 
    menuOpened = 0;
  }
}

void timestampRefresh(void)
{
  oldtimestamp = timestamp;
}

void refreshTouch(void)
{
  while(SDL_PollEvent(&evt) != 0 )
  {
    if(evt.type == SDL_FINGERDOWN)
    { 
      timestamp = evt.tfinger.timestamp;
      timestampDown = evt.tfinger.timestamp;
      touchLocation.x = evt.tfinger.x;
      touchLocation.y = evt.tfinger.y;
    }
    if(evt.type == SDL_FINGERUP)
    {
      timestampUp = evt.tfinger.timestamp; 
      touchLocation.x = evt.tfinger.x;
      touchLocation.y = evt.tfinger.y;
      timestampUp = 0;
    }
    
    if(evt.type == SDL_TEXTINPUT && pageNumber == 1)
    {
      strcat(text1, evt.text.text);
      strcat(text2, "*");
    }
    
    if(evt.type == SDL_KEYDOWN && pageNumber == 1 )
    {
      if(evt.key.keysym.sym == SDLK_BACKSPACE && strlen(text1) > 0)
      {
        text1[strlen(text1) - 1] = 0;
        text2[strlen(text2) - 1] = 0;
      }
    }
  
    if(evt.type == SDL_QUIT)
    {
      close(); 
      SDL_Quit(); 
    }
  }
}

void cycleCount()
{
  cycleCounter++;
}

void drawMenuItems(void)
{
  int i;
  int x = 905;
  int y = 100; 
  writeText("MENU", textColor);
  render(905, 25, NULL, 0.0, NULL, SDL_FLIP_NONE);

  
  for(i = 0; i < 6; ++i)
  {
    writeText(pageTitles[i], textColor);
    render(x, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
    y = y + 75;
  }
  y = 100;
}


void gotoAdmin(void)
{
  if(touchLocation.x >= 1205 && touchLocation.y <= 75 && timestamp > oldtimestamp) 
  {
    pageNumber = 1;
  }
}

void page_main(void)
{
  refreshTouch();
  draw();
  drawMenu();
  drawPageTitle(pageNumber); 
  if(adminAccess == 1)
  {
    openMenu();
    closeMenu();
  }
  else
  {
    gotoAdmin();
  } 
}

void page_admin(void)
{
  refreshTouch();
  draw();
  drawPageTitle(pageNumber);
  writeText("ENTER PASSWORD:", textColor);
  render(500, 300, NULL, 0.0, NULL, SDL_FLIP_NONE);
  
  writeText(text2, textColor);
  render(500, 400, NULL, 0.0, NULL, SDL_FLIP_NONE);
  passCheck();
}

void page_statusIO(void)
{
  if(menuOpened == 0)
  {
    readVariables();
    writeVariables();
    draw();
    drawMenu();  
    drawPageTitle(pageNumber);    
    drawLabels();
    drawIOLights();
  }
   if(adminAccess == 1)
  {
    openMenu();
    closeMenu();
  } 
}

void page_editor(void)
{
  draw();
  drawMenu();
  drawPageTitle(pageNumber); 
  drawGrid();
 
  sprintf(buffCC, "CCount: %d\n", cycleCounter);  
  writeText(buffCC, textColor);
  render(930, 200, NULL, 0.0, NULL, SDL_FLIP_NONE);

  sprintf(buffMV, "MVCheck: %d\n", menuValueCheck);
  writeText(buffMV, textColor);
  render(930, 300, NULL, 0.0, NULL, SDL_FLIP_NONE);


  if(adminAccess == 1)
  {
    openMenu();
    closeMenu();
  }
}

void page_loader(void)
{
  checkFocus();
  eventUpdate(focusBoxNumber);
  draw();
  drawMenu();
  drawPageTitle(pageNumber); 
  drawEbGrid();

  

  if(adminAccess == 1)
  {
    openMenu();
    closeMenu();
  }
}

void page_settings(void)
{
  draw();
  drawMenu();
  drawPageTitle(pageNumber); 
  if(adminAccess == 1)
  {
    openMenu();
    closeMenu();
  }
}

void loadPage(int pageNumber)
{
  switch(pageNumber)
  {
    case 0:
      page_main();
      break;
    case 1:
      page_admin();
      break;
    case 2:
      page_editor();
      break;
    case 3:
      page_loader();
      break;
    case 4:
      page_settings();
      break;
    case 5: 
      page_statusIO();
      break;   
  }
  SDL_RenderPresent(renderer); 
}

void passCheck(void)
{
  int ret;
  ret = strcmp(DROWSSAP, text1);
  if(ret == 0)
  {  
    pageNumber = 0;
    text1[0] = 0;
    text2[0] = 0;
    adminAccess = 1;
  }
}

void drawGrid()
{ 
  int x = 50;
  int y = 100;
  int w = 125;
  int h = 100;
  editBox_id_counter = 1;

  for(row = 0; row < 6; ++row)
  {
    for(column = 0; column < 7; ++column)
    {
      drawTextBox(row, x, y, w, h);      
      editBox_id[row][column] = editBox_id_counter;
      editBox_x[row][column] = x;
      editBox_y[row][column] = y;
      editBox_id_counter++;
      x = x + w;
    }
    x = 50;
    y = y + h;  
  }
  x = 50;
  y = 100;
}

void drawEbGrid(void)
{
  int i;
  
  
  for(i = 1; i < 43; ++i)
  {
    drawTextBox(i, eb[i].x, eb[i].y, eb[i].w, eb[i].h);
  }
}

void drawVarBar(void)
{
  int y = 300;
  int x = 25;
  int i;
  for(i = 0; i < 8; ++i)
  {
    writeText(var_num[i], textColor);
    render(x, 200, NULL, 0.0, NULL, SDL_FLIP_NONE);
    x = x + 150;
  }

  for(i = 0; i < 10; ++i)
  {
    writeText(n_num[i], textColor);
    render(25, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
   y = y + 75;
  }
  y = 300;
  x = 25;
}

void drawTextBox(int i, int x, int y, int w, int h)
{
  if(eb[i].hasFocus == 0)
  {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawLine(renderer, x, y, (x+w), y);
    SDL_RenderDrawLine(renderer, (x+w), y, (x+w), (y+h)); 
    SDL_RenderDrawLine(renderer, (x+w), (y+h), x, (y+h));
    SDL_RenderDrawLine(renderer, x, (y+h), x, y);
    writeText(eb[i].value, textColor);
    render(eb[i].x, eb[i].y, NULL, 0.0, NULL, SDL_FLIP_NONE);
    if (touchLocation.x >= x &&  touchLocation.x < (x + w) && touchLocation.y >= y && touchLocation.y < (y + h) 
        && timestamp > oldtimestamp && cycleCounter != menuValueCheck) 
    {
      menuValueCheck = cycleCounter;
      clearFocus();
      eb[i].hasFocus = 1;

      targetx = x / 125;
      targety = (y / 100) - 1; 
    
   
      printf("editbox_id[%d][%d]: %d  TOUCHX: %d  TOUCHY: %d DRAWX: %d  DRAWY:  %d\n",targetx, targety,editBox_id[targety][targetx], 
           touchLocation.x, touchLocation.y, editBox_x[targety][targetx], editBox_y[targety][targetx]);
    
    }
  }
  else if(eb[i].hasFocus == 1)
  {
    SDL_SetRenderDrawColor(renderer, 70, 191, 63, 255);
    SDL_RenderDrawLine(renderer, x, y, (x+w), y);
    SDL_RenderDrawLine(renderer, (x+w), y, (x+w), (y+h)); 
    SDL_RenderDrawLine(renderer, (x+w), (y+h), x, (y+h));
    SDL_RenderDrawLine(renderer, x, (y+h), x, y);
    
    writeText(eb[i].value, textColor);
    render(eb[i].x, eb[i].y, NULL, 0.0, NULL, SDL_FLIP_NONE);
  }
}


void clearFocus()
{
  int i;
  for(i = 1; i < 43; ++i)
  {
    eb[i].hasFocus = 0;
  }
}

void eventUpdate(int i)
{
  while(SDL_PollEvent(&evt) != 0 )
  {
    if(evt.type == SDL_FINGERDOWN)
    { 
      timestamp = evt.tfinger.timestamp;
      timestampDown = evt.tfinger.timestamp;
      touchLocation.x = evt.tfinger.x;
      touchLocation.y = evt.tfinger.y;
    }
    if(evt.type == SDL_FINGERUP)
    {
      timestampUp = evt.tfinger.timestamp; 
      touchLocation.x = evt.tfinger.x;
      touchLocation.y = evt.tfinger.y;
      timestampUp = 0;
    }
    if(evt.type == SDL_TEXTINPUT)
    { 
      for(i = 1; i < 43; ++i)
      {
        if(eb[i].hasFocus == 1)
        {
          strcat(eb[i].value, evt.text.text);
          printf("Typing\n");
        }
      }
     
    }
    if(evt.type == SDL_KEYDOWN)
    { 
      for(i = 1; i < 43; ++i)
      {
        if(evt.key.keysym.sym == SDLK_BACKSPACE && strlen(eb[i].value) > 0 && eb[i].hasFocus == 1)
        {
          eb[i].value[strlen(eb[i].value) - 1] = 0;
        }
      }
    } 
  }
}

void checkFocus(void)
{
  int i;
  for(i = 1; i < 43; ++i)
  {
    if(eb[i].hasFocus == 1)
    {
      focusBoxNumber = i;
    }
  }
  /*printf("FOCUS NUMBER: %d\n", focusBoxNumber);*/
}


