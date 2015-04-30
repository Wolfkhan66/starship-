
#include "SFApp.h"




	SFApp::SFApp(std::shared_ptr<SFWindow> window) :GameEnded(false),	is_running(true),is_paused(false),Boss1Dead(false),WaveCriteriaMet(false), sf_window(window) {
  int canvas_w, canvas_h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);
  app_box = make_shared<SFBoundingBox>(Vector2(canvas_w, canvas_h), 	canvas_w, canvas_h);

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// 
		SpawnStartWave(1);

//	Spawn Player  
	player  = make_shared<SFAsset>(SFASSET_PLAYER, sf_window);
  		auto player_pos = Point2(320, 88.0f);
  			player->SetPosition(player_pos);

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////


//	Spawn the GUI
	hud  = make_shared<SFAsset>(SFASSET_HUD, sf_window);
  		auto hud_pos = Point2(320, 720);
  			hud->SetPosition(hud_pos);	

}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//***************************************************************************************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
SFApp::~SFApp() {
}

/**
 * Handle all events that come from SDL.
 * These are timer or keyboard events.
 */
void SFApp::OnEvent(SFEvent& event) {
SFEVENT the_event = event.GetCode();


	switch(the_event) {
		case SFEVENT_UPDATE: {
			if(!is_paused){
				OnUpdateWorld();
			}
				OnRender();
			break;
		}
		case SFEVENT_QUIT: {
			is_running = false;
			restart = false;
			break;
		}
		case SFEVENT_PAUSE:{
			Pause();
			break;
		}
		case SFEVENT_RESTART:{
			GameReset(1);
			is_paused = false;
			break;
		}
			break;
	}
}

int SFApp::OnExecute() {
  // Execute the app
  SDL_Event event;
  while (SDL_WaitEvent(&event) && is_running) {
    // if this is an update event, then handle it in SFApp,
    // otherwise punt it to the SFEventDispacher.
    SFEvent sfevent((const SDL_Event) event);
    OnEvent(sfevent);
  }
}



///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//***************************************************************************************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

void SFApp::OnUpdateWorld() {

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//*************************************Handle Player Movement****************************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

 const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
 	if(keyboardState[SDL_SCANCODE_DOWN]) {
 		player->GoSouth();
 }
 	if(keyboardState[SDL_SCANCODE_UP]) {
 		player->GoNorth();
 }
 	if(keyboardState[SDL_SCANCODE_LEFT]) {
 		player->GoWest();
 }
 	if(keyboardState[SDL_SCANCODE_RIGHT]) {
 		player->GoEast();
 }
 	if(keyboardState[SDL_SCANCODE_SPACE]) {
			if(!is_paused){
				if(fire < fireN){
					fire++;
					FireProjectile();
				}
 			}
 }

int w, h;
SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//************************************* Check Events ************************************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
 
// if the player has run out of lives - call the gameover function
if(numberOfLives == 0){
		GameOver(1);
	}

// the player hits 0 health
// sleep the game temporarily ( not sure this is the best way )
// remove a life and remove the players asset
// spawn a new player and remove some points as a penalty
if(PlayerHP <= 0){
		PlayerHP = 100;
		numberOfLives = numberOfLives - 1;
		player->HandleCollision();
		usleep(1000000);
			player  = make_shared<SFAsset>(SFASSET_PLAYER, sf_window);
  		auto player_pos = Point2(320, 88.0f);
  			player->SetPosition(player_pos);
		Points = Points - 500;
	}

// when a player collects a pick up power time is set to the current second
// when 10 seconds have gone by the players fire rate is set back to normal
	if(currentSecond > powertime + 10){
		fireN = firePower;
	}

// if the player collects enough pickups increase the players firepower
// if not already at maximum
	if(PickUpsCollected == 2 && fireN < 20){
		firePower++;
		PickUpsCollected = 0;
	}

// as the player collects points the healthpack seed is increased
// when the seed reaches a certain amount a healthpack is spawned
// the seed is then set back to 0
	if(HealthPackSeed >= 1000){
			HealthPackSeed = HealthPackSeed - 1000;
			SpawnHealthPack(1);
	}

// as the player collects points the pick seed is increased
// when the seed reaches a certain amount a pickup is spawned
// the seed is then set back to 0

	if(PickUpSeed >= 250){
		PickUpSeed = PickUpSeed - 250;
		SpawnPickUp(1);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//*************************************Handle Wave Generation****************************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	 if(Wave == 1  && EnemiesDead >= 25){
		  WaveCriteriaMet = true;
  cout << "Wave Criteria Met" << endl;
		if(WaveCriteriaMet == true && AssetsAlive == 0){
	   	ClearAssets(1);
		 	SpawnWave2(1);
		 	EnemiesDead = 0;
		 	Wave++;
		 	cout<< "WAVE 2 INCOMING" << endl;
			SpawnHealthPack(1);
			WaveCriteriaMet = false;
		}
	 }

	 if(Wave == 2 && EnemiesDead >= 30 && Points > 1000){
		  WaveCriteriaMet = true;
      cout << "Wave Criteria Met" << endl;
		if(WaveCriteriaMet == true && AssetsAlive == 0){
		 ClearAssets(1);
	 	 SpawnWave3(1);
	   EnemiesDead = 0;
	   Wave++;
		 cout<< "WAVE 3 INCOMING" << endl;
		 SpawnHealthPack(1);
		 WaveCriteriaMet = false;
		}	
	 }

	 if(Wave == 3 && Points > 4000){
		  WaveCriteriaMet = true;
  cout << "Wave Criteria Met" << endl;
		if(WaveCriteriaMet == true && AssetsAlive == 0){
	   ClearAssets(1);
		 SpawnBonusWave(1);
		 EnemiesDead = 0;
		 CoinsCollected = 0;
		 Wave++;	
				 	cout<< "BONUS WAVE INCOMING" << endl;
	 WaveCriteriaMet = false;
		}	
   }

	 if(Wave == 4 && CoinsCollected == 10){
		 Wave++;
	   ClearAssets(1);
		 healthpacks.clear();
		 EnemiesDead = 0;
		 SpawnWave4(1);
			 	cout<< "WAVE 4 INCOMING" << endl;
		}	

	if(Wave == 5 && EnemiesDead >= 60 && Points >= 5000){
		  WaveCriteriaMet = true;
  cout << "Wave Criteria Met" << endl;
		if(WaveCriteriaMet == true && AssetsAlive == 0){
		ClearAssets(1);
		SpawnBoss1(1);
		Wave++;
				 	cout<< "BOSS INCOMING" << endl;
			SpawnHealthPack(2);
	 WaveCriteriaMet = false;
		}	
	}

	if(Wave == 6 && Boss1Dead == true){
		Boss1Dead = false;
		ClearAssets(1);	
    SpawnBonusWave(2);
		Wave++;
		CoinsCollected = 0;
				 	cout<< "BONUS WAVE INCOMING" << endl;
	}

	if(Wave == 7 && CoinsCollected == 20){
		ClearAssets(1);
		SpawnStartWave(3);
		Wave++;
					cout<< "WAVE 5 INCOMING" << endl;
		SpawnHealthPack(1);
	}
	
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//*************************************Handle Misc Assets********************************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////


// cloud movement 
// if a cloud goes off screen - remove asset and spawn a new one
	for(auto cd1: clouds){
		cd1->CollectibleM();
      auto cpos = cd1->GetPosition();		
	 if(cpos.getY() < -30.0f){
			cd1->HandleCollision();
				SpawnCloudsOnTop(1);
		}
	}

	for(auto cd2: clouds2){
		cd2->CollectibleM();
      auto cpos = cd2->GetPosition();		
	 if(cpos.getY() < -30.0f){
			cd2->HandleCollision();
				SpawnCloudsOnBottom(1);
		}
	}

// move an explosion asset 
// if an explosion goes off screen , remove the asset
  for(auto ex:explosions){
			auto expos = ex->GetPosition();
			ex->ExplosionM();
		 if(expos.getY() < -30.0f){
			ex->HandleCollision();
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//handle projectile movement
	for(auto p: projectiles) {
		p->GoNorth();
// Remove projectile when it hits the top of the screen
		auto p_pos = p->GetPosition();
		if(p_pos.getY() > h) {
			p->HandleCollision();
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//*************************************Handle Collectibles*******************************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// healthpack movement
	for(auto hp : healthpacks){
		hp->CollectibleM();
		auto hpPos = hp->GetPosition();
// check player collision with healthpack
// if player collects a health pack , gain 25 health and remove the healthpack asset
		if(player->CollidesWith(hp)) {
				PlayerHP = PlayerHP + 25;
				hp->HandleCollision();
		}
// if the health pack goes off screen then remove the asset
		else if(hpPos.getY() < -30.0f){
			hp->HandleCollision();
	}

// if player collects a health pack that makes their health go over 100, set hp to 100.
	if(PlayerHP > 100){
		PlayerHP = 100;
	}

}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// pickup movement
	for(auto pu : pickups){
		pu->CollectibleM();
		auto puPos = pu->GetPosition();
// check player collision with pickup
// if player collects a pickup - set there maximum projectiles to 20
// and remove the pick up asset
		if(player->CollidesWith(pu)) {
					pu->HandleCollision();
					powertime = currentSecond;
	      	fireN = 20;
						PickUpsCollected++;   			
		}	
// if the pickup goes off screen then remove the asset
		else if(puPos.getY() < -30.0f){
			pu->HandleCollision();
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// coin movement
	for(auto c : coins) {
		c->CollectibleM();
		auto cPos = c->GetPosition();
// Check player collision with coin
// if the player collects a coin gain 100 points and increase the healthpack spawning seed by 100
// remove the asset and spawn a new one
		if(player->CollidesWith(c)) {
			Points = Points + 100;
			HealthPackSeed = HealthPackSeed + 100;
				c->HandleCollision();
						SpawnCoin(1);
						CoinsCollected++;
		}
// if a coin goes off screen , remove the asset and spawn a new one
		else if(cPos.getY() < -30.0f){
			c->HandleCollision();
				SpawnCoin(1);
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//****************************************WORK IN PROGRESS*******************************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//Handle attacker events and movement
// essentially the idea here is creat a single entity of a current enemy with more health
// that stops at the top of the screen and starts moving left and right , firing at the player.
// very close to working however still buggy

/*
	for(auto AT : attackers){
			auto atPos= AT->GetPosition();
  	int canvas_w, canvas_h;
  		SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);
		if(alienfire < alienfireN){
			alienfire++;
			AlienFire(atPos,1);	
		}

		if(!(atPos.getY() <  400)){
			AT->AttackerM();
		}
		else if(!(atPos.getX()-30.0f < 0)) {
			AT->GoWest();
		}
		else (!(atPos.getX()+30.0f > w)) {
			AT->GoEast();
		}
	}	
		
		for(auto af : alienfires){
			af->AlienFireM();
			auto afPos= af->GetPosition();
		if(player->CollidesWith(af)){
			PlayerHP = PlayerHP -10;
			af->HandleCollision();
		}
		else if( afPos.getY() <-30.0f){
		af->HandleCollision();
	  }
	}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//***********************Handling Enemy Movement and Player Collision********************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// if an enemy asset collides with the player, remove some of the players health, remove the enemy asset
// spawn an explosion in its place and a new enemy asset to replace it.
// if the enemy asset misses the player and goes off screen , remove the asset and spawn a new one


// handle boss movement and his firing

	for(auto boss: bosses){
		boss->BossM();
	}
		
	for(auto af : alienfires){
			af->AlienFireM();
			auto afPos= af->GetPosition();
		if(player->CollidesWith(af)){
			PlayerHP = PlayerHP -5;
			af->HandleCollision();
		}
		else if( afPos.getY() <-30.0f){
			af->HandleCollision();
			AlienFire(1);
		}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Update alien positions
	for(auto a : aliens) {
		a->AlienM();
		auto aPos = a->GetPosition();
// Check player collision with alien
    if(player->CollidesWith(a) && WaveCriteriaMet == true){
      	PlayerHP = PlayerHP - 15;
			    a->HandleCollision();		
					SpawnExplosion(aPos,1);
          AssetsAlive--;
    }
    else if(aPos.getY() < -30.0f && WaveCriteriaMet == true){
				a->HandleCollision();
        AssetsAlive--;
    }
		else if(player->CollidesWith(a)) {
			PlayerHP = PlayerHP - 15;
			a->HandleCollision();		
					SpawnAlien(1);
					SpawnExplosion(aPos,1);
		}
		else if(aPos.getY() < -30.0f){
				a->HandleCollision();
				SpawnAlien(1);
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Update rangers
	for(auto r : rangers) {
		r->RangerM();
		auto rPos = r->GetPosition();
// Check player collision with alien
    if(player->CollidesWith(r) && WaveCriteriaMet == true){
      	PlayerHP = PlayerHP - 10;
			    r->HandleCollision();		
					SpawnExplosion(rPos,1);
          AssetsAlive--;
    }
    else if(rPos.getY() < -30.0f && WaveCriteriaMet == true){
				r->HandleCollision();
        AssetsAlive--;
    }
		else if(player->CollidesWith(r)) {
			PlayerHP = PlayerHP - 10;
			r->HandleCollision();
					SpawnRanger(1);
					SpawnExplosion(rPos,1);
		}
		else if(rPos.getY() < -30.0f){
				r->HandleCollision();
				SpawnRanger(1);
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Update scouts
	for(auto s : scouts) {
		s->ScoutM();
		auto sPos = s->GetPosition();
// Check player collision with alien
    if(player->CollidesWith(s) && WaveCriteriaMet == true){
      	PlayerHP = PlayerHP - 5;
			    s->HandleCollision();		
					SpawnExplosion(sPos,1);
          AssetsAlive--;
    }
    else if(sPos.getY() < -30.0f && WaveCriteriaMet == true){
				s->HandleCollision();
        AssetsAlive--;
    }
		else if(player->CollidesWith(s)) {
			PlayerHP = PlayerHP - 5;
			s->HandleCollision();
					SpawnScout(1);
					SpawnExplosion(sPos,1);
		}
		else if(sPos.getY() < -30.0f){
				s->HandleCollision();
				SpawnScout(1);
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//***********************Detecting Projectile Collisions*********************************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// if a projectil hits an asset, check its health and remove some.
// if assets health is less than or equal to 0 , remove the asset
// spawn an explosion in its place and increase the players score.

// then increase the seeds for healthpack and pickup spawning
// increase the number of dead enemies
// and spawn a new enemy asset in its place


// Detect projectile collision with alien
	for(auto p : projectiles) {
		for(auto a : aliens) {
				auto aPos = a->GetPosition();
      	if(p->CollidesWith(a) && WaveCriteriaMet == true) {
				a->SetHealth(a->GetHealth()-5);
				p->HandleCollision();
				if(a->GetHealth() <= 0){
					Points = Points + 15;
					HealthPackSeed = HealthPackSeed + 15;
					PickUpSeed = PickUpSeed + 15;
					a->HandleCollision();
							EnemiesDead++;
              AssetsAlive--;
							SpawnExplosion(aPos,1);
				}
        }
			else if(p->CollidesWith(a)) {
				a->SetHealth(a->GetHealth()-5);
				p->HandleCollision();
				if(a->GetHealth() <= 0){
					Points = Points + 15;
					HealthPackSeed = HealthPackSeed + 15;
					PickUpSeed = PickUpSeed + 15;
					a->HandleCollision();
							EnemiesDead++;
							SpawnAlien(1);
							SpawnExplosion(aPos,1);
				}
			}
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Detect projectile collision with ranger
	for(auto p : projectiles) {
		for(auto r: rangers) {
				auto rPos = r->GetPosition();
        if(p->CollidesWith(r) && WaveCriteriaMet == true) {
					r->SetHealth(r->GetHealth()-5);
					p->HandleCollision();
				if(r->GetHealth() <= 0){
					Points = Points + 10;
					HealthPackSeed = HealthPackSeed + 10;
					PickUpSeed = PickUpSeed + 10;
					p->HandleCollision();
					r->HandleCollision();
							EnemiesDead++;
							AssetsAlive--;
							SpawnExplosion(rPos,1);
				}
        }
			else if(p->CollidesWith(r)) {
					r->SetHealth(r->GetHealth()-5);
					p->HandleCollision();
				if(r->GetHealth() <= 0){
					Points = Points + 10;
					HealthPackSeed = HealthPackSeed + 10;
					PickUpSeed = PickUpSeed + 10;
					p->HandleCollision();
					r->HandleCollision();
							EnemiesDead++;
							SpawnRanger(1);
							SpawnExplosion(rPos,1);
				}
			}
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Detect projectile collision with scout
	for(auto p : projectiles) {
		for(auto s: scouts) {
				auto sPos = s->GetPosition();
			if(p->CollidesWith(s) && WaveCriteriaMet == true) {
				s->SetHealth(s->GetHealth()-5);
				p->HandleCollision();
					if(s->GetHealth() <= 0){
						Points = Points + 5;
						HealthPackSeed = HealthPackSeed + 5;
						PickUpSeed = PickUpSeed + 5;
							p->HandleCollision();
							s->HandleCollision();
							AssetsAlive--;
							SpawnExplosion(sPos,1);	
				  }
        }
			else if(p->CollidesWith(s)) {
				s->SetHealth(s->GetHealth()-5);
				p->HandleCollision();
					if(s->GetHealth() <= 0){
						Points = Points + 5;
						HealthPackSeed = HealthPackSeed + 5;
						PickUpSeed = PickUpSeed + 5;
							p->HandleCollision();
							s->HandleCollision();
							EnemiesDead++;
							SpawnScout(1);
							SpawnExplosion(sPos,1);	
				}
			}
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Detect projectile collision with boss
	for( auto boss: bosses){
		for(auto p : projectiles){
				auto bossPos = boss->GetPosition();
				auto pPos = p->GetPosition();
			if(p->CollidesWith(boss)) {
				boss->SetHealth(boss->GetHealth()-5);
				p->HandleCollision();
				SpawnExplosion(pPos,1);
				HealthPackSeed = HealthPackSeed +  20;
				PickUpSeed = PickUpSeed + 2;
			if(boss->GetHealth() <= 0){
				boss->HandleCollision();
				Points = Points + 1000;
				Boss1Dead = true;
			}	
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//******************************** Removing Dead Assets *********************************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	// remove dead explosions
	list<shared_ptr<SFAsset>> exTemp;
		for(auto ex : explosions) {
			if(ex->IsAlive()) {
				exTemp.push_back(ex);
			}
		}
			explosions.clear();
			explosions = list<shared_ptr<SFAsset>>(exTemp);

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Remove all dead projectiles
	list<shared_ptr<SFAsset>> projTemp;
		for(auto p : projectiles) {
			if(p->IsAlive()) {
				projTemp.push_back(p);
			}
			else{
				fire--;
			}
		}
			projectiles.clear();
			projectiles = list<shared_ptr<SFAsset>>(projTemp);
			
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Remove all dead alienfire
	list<shared_ptr<SFAsset>> afTemp;
		for(auto af : alienfires) {
			if(af->IsAlive()) {
				afTemp.push_back(af);
			}
			else{
				alienfire--;
			}
		}
			alienfires.clear();
			alienfires = list<shared_ptr<SFAsset>>(afTemp);

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Remove all dead attackers
	list<shared_ptr<SFAsset>> atTemp;
		for(auto at : attackers) {
			if(at->IsAlive()) {
				atTemp.push_back(at);
			}
		}
			attackers.clear();
			attackers = list<shared_ptr<SFAsset>>(atTemp);
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Remove all dead aliens
	list<shared_ptr<SFAsset>> alTemp;
		for(auto a : aliens) {
			if(a->IsAlive()) {
				alTemp.push_back(a);
			}
		}
			aliens.clear();
			aliens = list<shared_ptr<SFAsset>>(alTemp);
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Remove all dead rangers
	list<shared_ptr<SFAsset>> rTemp;
		for(auto r : rangers) {
			if(r->IsAlive()) {
				rTemp.push_back(r);
			}
		}
			rangers.clear();
			rangers = list<shared_ptr<SFAsset>>(rTemp);

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Remove all dead scouts
	list<shared_ptr<SFAsset>> sTemp;
		for(auto s : scouts) {
			if(s->IsAlive()) {
				sTemp.push_back(s);
			}
		}
			scouts.clear();
			scouts = list<shared_ptr<SFAsset>>(sTemp);

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Remove all dead coins
	list<shared_ptr<SFAsset>> cTemp;
		for(auto c : coins) {
			if(c->IsAlive()) {
				cTemp.push_back(c);
			}
		}
			coins.clear();
			coins = list<shared_ptr<SFAsset>>(cTemp);

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Remove all dead healthpacks
	list<shared_ptr<SFAsset>> hTemp;
		for(auto h : healthpacks) {
			if(h->IsAlive()) {
				hTemp.push_back(h);
			}
		}
			healthpacks.clear();
			healthpacks = list<shared_ptr<SFAsset>>(hTemp);

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

	// Remove remove dead bosses
	list<shared_ptr<SFAsset>> bossTemp;
		for(auto boss : bosses) {
			if(boss->IsAlive()) {
				bossTemp.push_back(boss);
			}
		}
			bosses.clear();
			bosses = list<shared_ptr<SFAsset>>(bossTemp);
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Remove all dead Pickups
	list<shared_ptr<SFAsset>> puTemp;
		for(auto pu : pickups) {
			if(pu->IsAlive()) {
				puTemp.push_back(pu);
			}
		}
			pickups.clear();
			pickups = list<shared_ptr<SFAsset>>(puTemp);
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Remove interfaces
	list<shared_ptr<SFAsset>> interTemp;
		for(auto go : interfaces) {
			if(go->IsAlive()) {
				interTemp.push_back(go);
			}
		}
			interfaces.clear();
			interfaces = list<shared_ptr<SFAsset>>(interTemp);
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

	// Remove the clouds
	list<shared_ptr<SFAsset>> cl1Temp;
		for(auto cl : clouds) {
			if(cl->IsAlive()) {
				cl1Temp.push_back(cl);
			}
		}
			clouds.clear();
			clouds = list<shared_ptr<SFAsset>>(cl1Temp);

	// Remove the cloud2s
	list<shared_ptr<SFAsset>> cl2Temp;
		for(auto cl : clouds2) {
			if(cl->IsAlive()) {
				cl2Temp.push_back(cl);
			}
		}
			clouds2.clear();
			clouds2 = list<shared_ptr<SFAsset>>(cl2Temp);

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

//Increase Time counter every ms
Timer++;
// divide by 60 to get seconds
currentSecond = Timer / 60;

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

 // these lists are constantly updated and dont have collisions 
 // so they need to be constantly cleared otherwise they endlessly spawn until 
 // no more memory is available.
 // this is just for the user interface

	list<shared_ptr<SFAsset>> hbTemp;
	healthbars.clear();
	healthbars = list<shared_ptr<SFAsset>>(hbTemp);
  PlayerHealth(PlayerHP);

	list<shared_ptr<SFAsset>> lfTemp;
	lives.clear();
	lives = list<shared_ptr<SFAsset>>(lfTemp);
  SpawnLives(numberOfLives);

	list<shared_ptr<SFAsset>> scTemp;
	points.clear();
	points = list<shared_ptr<SFAsset>>(scTemp);
	SpawnScore(Points);

	list<shared_ptr<SFAsset>> powTemp;
	pows.clear();
	pows = list<shared_ptr<SFAsset>>(powTemp);
	SpawnPower(fireN);
	
} 

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//*********************************Rendering Assets**************************************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////


  void SFApp::OnRender() {
  SDL_RenderClear(sf_window->getRenderer());


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
  		//render clouds2 ** rendered first so everything else renders on top
  	for(auto cd2: clouds2) {
					cd2->OnRender();	
		}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	// render the player
	if(player->IsAlive()) {
			player->OnRender();
		}
	
	// render projectiles
	 for(auto p: projectiles) {
		if(p->IsAlive()) {
			p->OnRender();
		}
	}

	// render explosions
	for(auto ex: explosions){
			ex->OnRender();
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Render enemy assets
  	for(auto a: aliens) {
					a->OnRender();				
		}

		for(auto r: rangers) {
					r->OnRender();	
		}

		for(auto s: scouts) {
					s->OnRender();	
		}

		for(auto at:attackers){
			at->OnRender();
		}
		
		for(auto af: alienfires){
			af->OnRender();
		}

 // render the big bad boss
	for(auto boss: bosses){
		boss->OnRender();
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

	//render coins
  	for(auto c: coins) {
				c->OnRender();	
  	}
	
		// render pickups
	 for(auto pu: pickups) {
			pu->OnRender();
	 }

		// render healthpacls
	 for(auto hp: healthpacks) {
			hp->OnRender();
	 }

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
 		//clouds on top /// rendered after enemies, collectibles and player to appear on top to give illusion of layers
  	for(auto cd1: clouds) {
					cd1->OnRender();	
		}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Healthbars ,points and huds // spawn last so they are always on top
	
	// render healthbars
	for (auto hb : healthbars){
		hb->OnRender();
	}
		
	//	render the hud after the healthbars so that the aestheic overlays the hpbars
	//	whilst the other assets overlay the hud
		hud->OnRender();


	// render the players lives
		for(auto lf : lives){
			lf ->OnRender();
		}
	
	// render the points
	for(auto po : points){
		po->OnRender();
	}
	
	// render players firepower
	for(auto pow : pows){
		pow->OnRender();
	}

	// render gameover interface
	for(auto gameover :interfaces){
    gameover->OnRender();
	}


  // Switch the off-screen buffer to be on-screen
  SDL_RenderPresent(sf_window->getRenderer());

	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//**********************************Hud Spawning Functions**************** **************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

////Render the health bar 
void SFApp::PlayerHealth(int PlayerHP){
  
	//the space for health bars in the hud is 100pixels wide, the health bar assets are 5 pixels wide so
	//since the players health starts at 100, divided it by 5 to get a number between 1 and 20
	int number_of_healthbars = PlayerHP / 5;
	
	// for 1 to number of healthbars
	for(int i = 0; i < number_of_healthbars; i++) {
	//If the players health is above 50 render the blue bars
		if(PlayerHP >= 50){
			auto healthbarblue = make_shared<SFAsset>(SFASSET_HEALTHBARBLUE, sf_window);
			auto pos = Point2(15+ (i*5), 472);
			healthbarblue->SetPosition(pos);
			healthbars.push_back(healthbarblue);
		}
	// if the health is lower than 50 but above 25 , render the yellow bars
		else if(PlayerHP >= 25){
			auto healthbaryellow = make_shared<SFAsset>(SFASSET_HEALTHBARYELLOW, sf_window);
			auto pos = Point2(15+ (i*5), 472);
			healthbaryellow->SetPosition(pos);
			healthbars.push_back(healthbaryellow);
		}
	// if the health is lower than 25 render the red bars
		else{
			auto healthbarred= make_shared<SFAsset>(SFASSET_HEALTHBARRED, sf_window);
			auto pos = Point2(15 + (i*5), 472);
			healthbarred->SetPosition(pos);
			healthbars.push_back(healthbarred);	
		}
	}
}

//////////////////////////////////////////////
//////////////////////////////////////////////
// spawn an asset for the number of lives
void SFApp::SpawnLives(int lifeNo){
	for(int i = 0; i < lifeNo; i++){
			auto life = make_shared<SFAsset>(SFASSET_LIFE, sf_window);
			auto pos = Point2(55+ (i*14), 448);
			life->SetPosition(pos);
			lives.push_back(life);
	}
}

//////////////////////////////////////////////
//////////////////////////////////////////////
// spawn an asset for the  firepower
void SFApp::SpawnPower(int fireN){
	for(int i = 0; i < fireN; i++){
			auto pow = make_shared<SFAsset>(SFASSET_POW, sf_window);
			auto pos = Point2(25 , 35 + (i*6));
				pow->SetPosition(pos);
				pows.push_back(pow);
	}
}


//////////////////////////////////////////////
//////////////////////////////////////////////
// spawn the score
void SFApp::SpawnScore(int Points)
{
	// take the int for points and convert it into a string
	string ScoreString;
	ostringstream ScoreConvert;
	ScoreConvert << Points;
	ScoreString = ScoreConvert.str();

	// check each character in the string and spawn the necessary number
for(int i = 0; i < ScoreString.size(); i++){
	if(ScoreString[i] == '0') {
			auto zero = make_shared<SFAsset>(SFASSET_NUM0, sf_window);
			auto pos = Point2(570+ (i*6), 469);
			zero->SetPosition(pos);
			points.push_back(zero);
		}
	if(ScoreString[i] == '1'){
				auto one = make_shared<SFAsset>(SFASSET_NUM1, sf_window);
				auto pos = Point2(570+ (i*6), 469);
				one->SetPosition(pos);
				points.push_back(one);
	}
	if(ScoreString[i] == '2'){
				auto two = make_shared<SFAsset>(SFASSET_NUM2, sf_window);
				auto pos = Point2(570+ (i*6), 469);
				two->SetPosition(pos);
				points.push_back(two);
	}
	if(ScoreString[i] == '3'){
				auto three = make_shared<SFAsset>(SFASSET_NUM3, sf_window);
				auto pos = Point2(570+ (i*6), 469);
				three->SetPosition(pos);
				points.push_back(three);
	}
	if(ScoreString[i] == '4'){
				auto four = make_shared<SFAsset>(SFASSET_NUM4, sf_window);
				auto pos = Point2(570+ (i*6), 469);
				four->SetPosition(pos);
				points.push_back(four);
	}
	if(ScoreString[i] == '5'){
				auto five = make_shared<SFAsset>(SFASSET_NUM5, sf_window);
				auto pos = Point2(570+ (i*6), 469);
				five->SetPosition(pos);
				points.push_back(five);
	}
	 if(ScoreString[i] == '6'){
				auto six = make_shared<SFAsset>(SFASSET_NUM6, sf_window);
				auto pos = Point2(570+ (i*6), 469);
				six->SetPosition(pos);
				points.push_back(six);
	}
	if(ScoreString[i] == '7'){
				auto seven = make_shared<SFAsset>(SFASSET_NUM7, sf_window);
				auto pos = Point2(570+ (i*6), 469);
				seven->SetPosition(pos);
				points.push_back(seven);
	}
	if(ScoreString[i] == '8'){
				auto eight = make_shared<SFAsset>(SFASSET_NUM8, sf_window);
				auto pos = Point2(570+ (i*6), 469);
				eight->SetPosition(pos);
				points.push_back(eight);
	}
	if (ScoreString[i] == '9'){
				auto nine = make_shared<SFAsset>(SFASSET_NUM9, sf_window);
				auto pos = Point2(570+ (i*6), 469);
				nine->SetPosition(pos);
				points.push_back(nine);
	}

}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//*********************************Enemy Spawn Functions*********************************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//		Spawn an alien, set its health to 15
void SFApp::SpawnAlien(int AlienNo){
	for(int i = 0; i < AlienNo; i++) {
		auto alien = make_shared<SFAsset>(SFASSET_ALIEN, sf_window);
		auto pos = Point2 (rand() % 550 + 60, rand() % 3000 + 600);
				alien->SetPosition(pos);
				aliens.push_back(alien);
				alien->SetHealth(15);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//		Spawn a ranger, set its health to 10
void SFApp::SpawnRanger(int RangerNo){
	for(int i = 0; i < RangerNo; i++) {
		auto ranger = make_shared<SFAsset>(SFASSET_RANGER, sf_window);
		auto pos = Point2 (rand() % 550 + 60, rand() % 3000 + 600);
				ranger->SetPosition(pos);
				rangers.push_back(ranger);
				ranger->SetHealth(10);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//		Spawn a scout, set its health to 5
void SFApp::SpawnScout(int ScoutNo){
	for(int i = 0; i < ScoutNo; i++) {
		auto scout = make_shared<SFAsset>(SFASSET_SCOUT, sf_window);
		auto pos = Point2 (rand() % 550 + 60, rand() % 3000 + 600);
				scout->SetPosition(pos);
				scouts.push_back(scout);
				scout->SetHealth(5);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//		Spawn the boss , set its health to 250 and have 50 alien projectiles coming at the player
void SFApp::SpawnBoss1(int x){
		for(int i=0; i< x; i++){
			auto boss = make_shared<SFAsset>(SFASSET_BOSS, sf_window);
			auto bosspos = Point2 (320, 2000);
					boss->SetPosition(bosspos);
					bosses.push_back(boss);
					boss->SetHealth(250);
					AlienFire(50);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

//// This part works but their actual movement is a work in progress
void SFApp::SpawnAttackers(int AttackerID,int AttackerNO){

	for(int i = 0; i < AttackerNO; i++) {
		if(AttackerID == 1){
			auto attacker = make_shared<SFAsset>(SFASSET_ALIEN, sf_window);
			auto pos = Point2 (rand() % 600 + 40, rand() % 3000 + 600);
				attacker->SetPosition(pos);
				attackers.push_back(attacker);
		}
	if(AttackerID == 2){
			auto attacker = make_shared<SFAsset>(SFASSET_RANGER, sf_window);
			auto pos = Point2 (rand() % 600 + 40, rand() % 3000 + 600);
				attacker->SetPosition(pos);
				attackers.push_back(attacker);
		}
	if(AttackerID == 3){
			auto attacker = make_shared<SFAsset>(SFASSET_SCOUT, sf_window);
			auto pos = Point2 (rand() % 600 + 40, rand() % 3000 + 600);
				attacker->SetPosition(pos);
				attackers.push_back(attacker);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//*********************************Collectible Spawning Functions************************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

void SFApp::SpawnCoin(int CoinNo){

	for(int i = 0; i < CoinNo; i++) {
		auto coin = make_shared<SFAsset>(SFASSET_COIN, sf_window);
		auto pos = Point2 (rand() % 600 + 40, rand() % 3000 + 600);
				coin->SetPosition(pos);
				coins.push_back(coin);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
void SFApp::SpawnHealthPack(int HealthPackNo){

	for(int i = 0; i < HealthPackNo; i++) {
		auto healthpack = make_shared<SFAsset>(SFASSET_HEALTHPACK, sf_window);
		auto pos = Point2 (rand() % 600 + 40, rand() % 3000 + 600);
				healthpack->SetPosition(pos);
				healthpacks.push_back(healthpack);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
void SFApp::SpawnPickUp(int PickUpNo){

	for(int i = 0; i < PickUpNo; i++) {
		auto pickup = make_shared<SFAsset>(SFASSET_PICKUP, sf_window);
		auto pos = Point2 (rand() % 600 + 40, rand() % 3000 + 600);
				pickup->SetPosition(pos);
				pickups.push_back(pickup);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//********************************* Misc Spawn Functions*********************************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
void SFApp::AlienFire(int fireN){
	for (int i = 0; i < fireN; i++){
		auto AT = make_shared<SFAsset>(SFASSET_ALIENFIRE, sf_window);
		auto ATPos = Point2 (rand() % 600 + 40, rand() % 3000 + 1000);
				AT->SetPosition(ATPos);
				alienfires.push_back(AT);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
void SFApp::FireProjectile() {
  auto pb = make_shared<SFAsset>(SFASSET_PROJECTILE, sf_window);
  auto v  = player->GetPosition();
  	pb->SetPosition(v);
  	projectiles.push_back(pb);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
void SFApp::SpawnExplosion(Point2 pos, int explosionN){
	for (int i = 0; i < explosionN; i++){
		auto explode = make_shared<SFAsset>(SFASSET_EXPLOSION, sf_window);
				explode->SetPosition(pos);
				explosions.push_back(explode);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
void SFApp::SpawnCloudsOnTop(int CloudNo){
		for(int i=0; i<CloudNo; i++) {
			auto cloud1 = make_shared<SFAsset>(SFASSET_CLOUD, sf_window);
			auto c1pos = Point2 (rand() % 600 , rand() % 3000 + 2000 );
					cloud1->SetPosition(c1pos);
					clouds.push_back(cloud1);
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
void SFApp::SpawnCloudsOnBottom(int CloudNo){
		for(int i=0; i<CloudNo; i++) {
			auto cloud2 = make_shared<SFAsset>(SFASSET_CLOUD, sf_window);
			auto c2pos = Point2 (rand() % 600 , rand() % 3000 + 2000 );
					cloud2->SetPosition(c2pos);
					clouds2.push_back(cloud2);
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//******************************Wave Spawning Functions**********************************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

void SFApp::SpawnStartWave(int x){
	for(int i=0; i< x; i++){
	SpawnAlien(1);
	SpawnRanger(1);
	SpawnScout(10);
	SpawnCoin(2);
	SpawnCloudsOnTop(2);
	SpawnCloudsOnBottom(2);
  AssetsAlive = 12;
	}
}
//////////////////////////////////////////////
//////////////////////////////////////////////
void SFApp::SpawnWave2(int x){
	for(int i=0; i< x; i++){
		SpawnAlien(1);
		SpawnRanger(15);
		SpawnScout(1);
		SpawnCoin(2);
    AssetsAlive = 17;
	}
}
//////////////////////////////////////////////
//////////////////////////////////////////////

void SFApp::SpawnWave3(int x){
	for(int i=0; i< x; i++){
		SpawnAlien(10);
		SpawnRanger(2);
		SpawnScout(2);
		SpawnCoin(3);
    AssetsAlive = 14;
	}
}
//////////////////////////////////////////////
//////////////////////////////////////////////

void SFApp::SpawnWave4(int x){
	for(int i=0; i< x; i++){
		SpawnAlien(8);
		SpawnRanger(8);
		SpawnScout(8);
		SpawnCoin(3);
    AssetsAlive = 24;
	}
}

//////////////////////////////////////////////
//////////////////////////////////////////////

void SFApp::SpawnBonusWave(int x){
	for(int i=0; i< x; i++){
		SpawnCoin(30);
		SpawnHealthPack(4);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//********************************* Misc and GameOver Functions**************************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Spawn Game Over asset  
void SFApp::SpawnGameOver(int GONo){	
	for(int i=0; i< GONo; i++){
	auto gameover = make_shared<SFAsset>(SFASSET_GAMEOVER, sf_window);
    		auto go_pos = Point2(320,480);
  			gameover->SetPosition(go_pos);
			interfaces.push_back(gameover);
	}
}
//////////////////////////////////////////////
//////////////////////////////////////////////
// Clear some asset lists 
void SFApp::ClearAssets(int Clearonce){
	for(int i=0; i< Clearonce; i++){
		aliens.clear();
		scouts.clear();
		rangers.clear();
		coins.clear();
		interfaces.clear();
    alienfires.clear();
	}
}
//////////////////////////////////////////////
//////////////////////////////////////////////
// reset all game variables to there originals and clear lists
void SFApp::GameReset(int Resetonce){
	for(int i=0; i< Resetonce; i++){
  int lives = 3 ;
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
		GameEnded = false;
		Boss1Dead = false;

		aliens.clear();
		scouts.clear();
		rangers.clear();
		coins.clear();
		interfaces.clear();
		clouds.clear();
		clouds2.clear();
		pickups.clear();
		healthpacks.clear();
		SpawnStartWave(1);
		
 }
}
//////////////////////////////////////////////
//////////////////////////////////////////////
// Spawn the gameover asset and pause the game
void SFApp::GameOver(int Callonce){
	for(int i=0; i< Callonce; i++){
		SpawnGameOver(1);
		is_paused = true;
	}
}
//////////////////////////////////////////////
//////////////////////////////////////////////
// function to allow the game to be paused
void SFApp::Pause(){
  if(is_paused){
		is_paused = false;
	}
	else{
		is_paused = true;
	}

}

