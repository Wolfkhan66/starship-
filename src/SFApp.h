#ifndef SFAPP_H
#define SFAPP_H

#include <memory>   // Pull in std::shared_ptr
#include <iostream> // Pull in std::cerr, std::endl
#include <list>     // Pull in list
#include <sstream>

using namespace std;

#include "SFCommon.h"
#include "SFEvent.h"
#include "SFAsset.h"

/**
 * Represents the StarshipFontana application.  It has responsibilities for
 * * Creating and destroying the app window
 * * Processing game events
 */
class SFApp {

public:
  SFApp(std::shared_ptr<SFWindow>);
  virtual ~SFApp();
  void    OnEvent(SFEvent &);
  int     OnExecute();
  void    OnUpdateWorld();
  void    OnRender();
  void    FireProjectile();
  void    GameOver();
  void    Pause();


private:
  SDL_Surface           * surface;
  bool                    is_running;
  bool 			  is_paused;
  bool 			  restart;

  shared_ptr<SFWindow>       sf_window;

  shared_ptr<SFAsset>        player;
  shared_ptr<SFAsset>        gameover;
  shared_ptr<SFBoundingBox>  app_box;

  list<shared_ptr<SFAsset> > projectiles;
  list<shared_ptr<SFAsset> > aliens;
  list<shared_ptr<SFAsset> > coins;
  list<shared_ptr<SFAsset> > rangers;
  list<shared_ptr<SFAsset> > scouts;
  list<shared_ptr<SFAsset> > pickups;
  list<shared_ptr<SFAsset> > healthpacks;
  list<shared_ptr<SFAsset> > healthbars;
  list<shared_ptr<SFAsset> > clouds;
  list<shared_ptr<SFAsset> > clouds2;


  int fire = 0;
  int fireN = 1;
  int PlayerHP = 100;
  int Points = 0;
  int HealthPackSeed = 0;
  int PickUpSeed = 0;
  int EnemiesDead = 0;
  int Timer = 0;
  int currentSecond = 0;
  int powertime = 0;

  SFError OnInit();
};

#endif
