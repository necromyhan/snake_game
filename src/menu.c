#include <stdbool.h>
#include <stdlib.h>

#include "game.h"
#include "menu.h"


MENU*
CreateMenu(
   int            Count,
   const char**   Strings)
{
   MENU* menu = NULL;

   menu = SDL_malloc(sizeof(MENU) + sizeof(MENU_ITEM) * Count);
   if (NULL == menu)
   {
      goto exit;
   }

   menu->Count = Count;
   for (int i = 0; i < Count; ++i)
   {
      menu->Items[i].State = ItemStateIdle;
      menu->Items[i].Text = Strings[i];
      menu->Items[i].Type = i;
   }

   menu->Items[0].State = ItemStateHover;
   menu->ActiveType = 0;

exit:
   return menu;
}

void
DestroyMenu(MENU* Menu)
{
   if (Menu)
   {
      SDL_free(Menu);
   }
}

extern GAME_STATE gCurrentScene;

int
StartMenuHandleEvents(
   GAME*             Game,
   const SDL_Event*  Event)
{
   int status = 0;
   if (NULL == Event || NULL == Game)
   {
      status = -1;
      goto exit;
   }

   if (Event->type == SDL_EVENT_KEY_DOWN)
   {
      switch (Event->key.keysym.sym)
      {
         case SDLK_UP:
         {
            if (Game->StartMenu->ActiveType > StartMenuStart)
            {
               Game->StartMenu->Items[Game->StartMenu->ActiveType--].State = ItemStateIdle;
               Game->StartMenu->Items[Game->StartMenu->ActiveType].State = ItemStateHover;
            }
         }
         break;
         case SDLK_DOWN:
         {
            if (Game->StartMenu->ActiveType < StartMenuMax - 1)
            {
               Game->StartMenu->Items[Game->StartMenu->ActiveType++].State = ItemStateIdle;
               Game->StartMenu->Items[Game->StartMenu->ActiveType].State = ItemStateHover;
            }
         }
         break;
         case SDLK_RETURN:
         {
            if (Game->StartMenu->ActiveType == StartMenuStart)
            {
               gCurrentScene = StateGameplay;
            }
         }
      }
   }

   exit:
   return status;
}

int
StartMenuUpdate(GAME* Game)
{
   return 0;
}

int
StartMenuRender(GAME* Game)
{
   int status = 0;

   if (NULL == Game)
   {
      status = -1;
      goto exit;
   }

   status = SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 0);
   if (status) { goto exit; }
   
   status = SDL_RenderFillRect(Game->Renderer, NULL);
   if (status) { goto exit; }

   int posX = Game->Field.CellSize;
   int posY = Game->Field.CellSize;
   for (int i = 0; i < Game->StartMenu->Count; ++i)
   {
      status = PrintFontToRenderer(
            Game->Font,
            Game->Renderer,
            Game->StartMenu->Items[i].Text,
            (Game->StartMenu->Items[i].State) ? 
            (SDL_Color){.b = 200, .g = 0, .r = 0, .a = 0} : 
            (SDL_Color){.b = 255, .g = 255, .r = 255, .a = 0},
            (SDL_Point){.x = posX, .y = posY});
      if (status)
      {
         goto exit;
      }

      // TODO: get screen width from window
      posY += Game->Field.CellSize;
   }
   
   status = SDL_RenderPresent(Game->Renderer);

exit:
   return status;
}

SCENE gMenuScene = 
{
   .HandleEvents  = StartMenuHandleEvents,
   .Render        = StartMenuRender,
   .Update        = StartMenuUpdate    
};

int
GameOverMenuHandleEvents(
   GAME*             Game,
   const SDL_Event*  Event)
{
   int status = 0;
   if (NULL == Event || NULL == Game)
   {
      status = -1;
      goto exit;
   }

   if (Event->type == SDL_EVENT_KEY_DOWN)
   {
      switch (Event->key.keysym.sym)
      {
         case SDLK_UP:
         {
            SDL_Log("UP!");
            if (Game->GameOverMenu->ActiveType > GameOverMenuRetry)
            {
               Game->GameOverMenu->Items[Game->GameOverMenu->ActiveType--].State = ItemStateIdle;
               Game->GameOverMenu->Items[Game->GameOverMenu->ActiveType].State = ItemStateHover;
            }
         }
         break;
         case SDLK_DOWN:
         {
            if (Game->GameOverMenu->ActiveType < GameOverMenuMax - 1)
            {
               Game->GameOverMenu->Items[Game->GameOverMenu->ActiveType++].State = ItemStateIdle;
               Game->GameOverMenu->Items[Game->GameOverMenu->ActiveType].State = ItemStateHover;
            }
         }
         break;
         case SDLK_RETURN:
         {
            if (Game->GameOverMenu->ActiveType == GameOverMenuExit)
            {
               gCurrentScene = StateMenu;
            }
            else if (Game->GameOverMenu->ActiveType == GameOverMenuRetry)
            {
               gCurrentScene = StateGameplay;
            }
         }
      }
   }   

exit:
   return status;
}

int
GameOverMenuUpdate(GAME* Game)
{
   return 0;
}

int
GameOverMenuRender(GAME* Game)
{
   int status = 0;

   if (NULL == Game)
   {
      status = -1;
      goto exit;
   }

   status = SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 0);
   if (status) { goto exit; }
   
   status = SDL_RenderFillRect(Game->Renderer, NULL);
   if (status) { goto exit; }

   int posX = Game->Field.CellSize;
   int posY = Game->Field.CellSize;
   for (int i = 0; i < Game->GameOverMenu->Count; ++i)
   {
      status = PrintFontToRenderer(
            Game->Font,
            Game->Renderer,
            Game->GameOverMenu->Items[i].Text,
            (Game->GameOverMenu->Items[i].State) ? 
            (SDL_Color){.b = 200, .g = 0, .r = 0, .a = 0} : 
            (SDL_Color){.b = 255, .g = 255, .r = 255, .a = 0},
            (SDL_Point){.x = posX, .y = posY});
      if (status)
      {
         goto exit;
      }

      // TODO: get screen width from window
      posY += Game->Field.CellSize;
   }
   
   status = SDL_RenderPresent(Game->Renderer);

exit:
   return status;
}

SCENE gGameOverScene = 
{
   .HandleEvents  = GameOverMenuHandleEvents,
   .Render        = GameOverMenuRender,
   .Update        = GameOverMenuUpdate
};