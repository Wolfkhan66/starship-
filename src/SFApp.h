#ifndef SFAPP_H
#define SFAPP_H

#include <memory>   // Pull in std::shared_ptr
#include <iostream> // Pull in std::cerr, std::endl
#include <list>     // Pull in list
#include <sstream>
#include <string>
#include <unistd.h>

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
  void    GameOver(int Callonce);
  void    Pause();
  void    GameReset(int Resetonce);
  void    ClearAssets(int Clearonce);

  void    SpawnExplosion(Point2 pos, int explosionN);
  void	  PlayerHealth(int PlayerHP);
  void	  SpawnAlien(int AlienNo);
  void	  SpawnRanger(int RangerNo);
  void	  SpawnScout(int ScoutNo);
  void	  SpawnCoin(int CoinNo);
  void	  SpawnHealthPack(int HealthPackNo);
  void	  SpawnPickUp(int PickUpNo);
  void	  SpawnLives(int lifeNo);
  void	  SpawnScore(int Points);
  void    SpawnGameOver(int GONo);
	void		SpawnCloudsOnTop(int CloudNo);
	void		SpawnCloudsOnBottom(int CloudNo);
	void		AlienFire(Point2 pos, int fireN);
	void		SpawnAttackers(int AttackerID,int AttackerNO);

  void    SpawnStartWave(int x);
	void		SpawnWave2(int x);
	void		SpawnWave3(int x);
	void		SpawnWave4(int x);
	void		SpawnBoss1(int x);
	void		SpawnBonusWave(int x);


private:
  SDL_Surface           * surface;
  bool                    is_running;
  bool 			  is_paused;
  bool 			  restart;
  bool 				attackerdirection;
	bool				Boss1Dead;
	bool				GameEnded;

  shared_ptr<SFWindow>       sf_window;

  shared_ptr<SFAsset>        player;
  shared_ptr<SFAsset>        hphud;
  shared_ptr<SFAsset>        scorehud;
  shared_ptr<SFBoundingBox>  app_box;

  list<shared_ptr<SFAsset> > alienfires;
  list<shared_ptr<SFAsset> > attackers;
	list<shared_ptr<SFAsset> > interfaces;
  list<shared_ptr<SFAsset> > projectiles;
  list<shared_ptr<SFAsset> > explosions;
  list<shared_ptr<SFAsset> > aliens;
  list<shared_ptr<SFAsset> > coins;
  list<shared_ptr<SFAsset> > rangers;
  list<shared_ptr<SFAsset> > scouts;
  list<shared_ptr<SFAsset> > pickups;
  list<shared_ptr<SFAsset> > healthpacks;
  list<shared_ptr<SFAsset> > healthbars;
  list<shared_ptr<SFAsset> > clouds;
  list<shared_ptr<SFAsset> > clouds2;
  list<shared_ptr<SFAsset> > points;
  list<shared_ptr<SFAsset> > lives;

  int numberOfLives = 3;
  int fire = 0;
  int fireN = 1;

  int alienfire = 0;
	int alienfireN = 1;
  int PlayerHP = 100;
  int Points = 0;
  int HealthPackSeed = 0;
	int PickUpsCollected= 0;
  int PickUpSeed = 0;
  int EnemiesDead = 0;
	int CoinsCollected = 0;
  int Timer = 0;
  int currentSecond = 0;
  int powertime = 0;
  int attime = 0;
  int Wave = 1;
  int firePower = 1;

  SFError OnInit();
};

#endif
