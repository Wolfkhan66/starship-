
#include "SFApp.h"




	SFApp::SFApp(std::shared_ptr<SFWindow> window) :	is_running(true),is_paused(false), sf_window(window) {
  	int canvas_w, canvas_h;
  		SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);

  app_box = make_shared<SFBoundingBox>(Vector2(canvas_w, canvas_h), 	canvas_w, canvas_h);

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//	Spawn Player  
	player  = make_shared<SFAsset>(SFASSET_PLAYER, sf_window);
  		auto player_pos = Point2(canvas_w/2, 88.0f);
  			player->SetPosition(player_pos);


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//	Spawn hud
	hud  = make_shared<SFAsset>(SFASSET_HUD, sf_window);
  		auto hud_pos = Point2(60, 480);
  			hud->SetPosition(hud_pos);
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Spawn Game Over asset  	
		gameover  = make_shared<SFAsset>(SFASSET_GAMEOVER, sf_window);
  		auto go_pos = Point2(0,0);
  			gameover->SetPosition(go_pos);

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
		// spawn aliens
	const int number_of_aliens = 7;
		for(int i=0; i<number_of_aliens; i++) {
			auto alien = make_shared<SFAsset>(SFASSET_ALIEN, sf_window);
			auto pos = Point2 (rand() % (40 + 600), rand() %(600 + 3000));
					alien->SetPosition(pos);
					aliens.push_back(alien);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
		//spawn debris
  const int number_of_rangers = 10;
		for(int i=0; i<number_of_rangers; i++) {
			auto ranger = make_shared<SFAsset>(SFASSET_RANGER, sf_window);
			auto ranger_pos = Point2 (rand() % (40 + 600), rand() %(600 + 3000));
					ranger->SetPosition(ranger_pos);
					rangers.push_back(ranger);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	//spawn coins
	const int number_of_coins = 3;
		for(int i=0; i<number_of_coins; i++) {
			auto coin = make_shared<SFAsset>(SFASSET_COIN, sf_window);
			auto pos  = Point2 (rand() % (40 + 600), rand() %(600 + 3000));
					coin->SetPosition(pos);
					coins.push_back(coin);
		}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	//Spawn healthpack
		const int number_of_healthpacks = 1;
					for(int i=0; i<number_of_healthpacks; i++) {
			auto healthpack = make_shared<SFAsset>(SFASSET_HEALTHPACK, sf_window);
			auto hp_pos  = Point2 (0,0);
					healthpack->SetPosition(hp_pos);
					healthpacks.push_back(healthpack);
					}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	//Spawn pickup
		const int number_of_pickups = 1;
					for(int i=0; i<number_of_pickups; i++) {
			auto pickup = make_shared<SFAsset>(SFASSET_PICKUP, sf_window);
			auto pu_pos  = Point2 (0,0);
					pickup->SetPosition(pu_pos);
					pickups.push_back(pickup);
					}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	//Spawn scout
		const int number_of_scouts = 5;
					for(int i=0; i<number_of_scouts; i++) {
			auto scout = make_shared<SFAsset>(SFASSET_SCOUT, sf_window);
			auto sc_pos  = Point2 (320,5000);
					scout->SetPosition(sc_pos);
					scouts.push_back(scout);
					}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	//spawn cloud on top
	const int number_of_clouds1 = 3;
		for(int i=0; i<number_of_clouds1; i++) {
			auto cloud1 = make_shared<SFAsset>(SFASSET_CLOUD, sf_window);
			auto c1pos = Point2 (rand() %(40 + 600), rand() %(600+3000));
					cloud1->SetPosition(c1pos);
					clouds.push_back(cloud1);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	//spawn cloud behind
	const int number_of_clouds2 = 3;
		for(int i=0; i<number_of_clouds2; i++) {
			auto cloud2 = make_shared<SFAsset>(SFASSET_CLOUD, sf_window);
			auto c2pos = Point2 (rand() %(40 + 600), rand() %(600+3000));
					cloud2->SetPosition(c2pos);
					clouds2.push_back(cloud2);

	}

/*		
	//Spawn Healthbar
		const int number_of_healthbars = 10;
					for(int i=0; i<number_of_healthbars; i++) {
		        hbars[i] = i;
			auto healthbar = make_shared<SFAsset>(SFASSET_HEALTHBAR, sf_window);
			auto hb_pos = Point2((660/number_of_healthbars) * i, 50.0f);
					healthbar->SetPosition(hb_pos);
					healthbars.push_back(healthbar);
					}



*/
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
//GameChecks

//check player health
	if(PlayerHP <= 0){
		player->HandleCollision();
		GameOver();
	}

// check pickup is active
	if(currentSecond > powertime + 10){
		fireN = 1;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// cloud movement

	for(auto cd1: clouds){
		cd1->CollectibleM();
		}
	for(auto cd2: clouds2){
		cd2->CollectibleM();
		}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Update projectile positions
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
// healthpack movement
	for(auto hp : healthpacks){
				hp->CollectibleM();
// check player collision with healthpack
			if(player->CollidesWith(hp)) {
				PlayerHP = PlayerHP + 25;
				hp->HandleCollision();
				cout << "Collected Health!" << endl;
				cout << "HP"<< PlayerHP << endl;
		}
	}


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// pickup movement
	for(auto pu : pickups){
				pu->CollectibleM();
// check player collision with pickup
			if(player->CollidesWith(pu)) {
					pu->HandleCollision();
					powertime = currentSecond;
	      	fireN = 20;
					cout << "Collected Pickup!" << endl;
					cout << "Maximum fire"<< endl;     				
			}	
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// coin movement
	for(auto c : coins) {
		c->CollectibleM();
// Check player collision with coin
		if(player->CollidesWith(c)) {
			Points = Points + 100;
			HealthPackSeed = HealthPackSeed + 100;
			c->HandleCollision();
			cout << "Collected coin!" << endl;
			cout << "Score:" << Points << endl;
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Update alien positions
	for(auto a : aliens) {
		a->AlienM();
// Check player collision with alien
		if(player->CollidesWith(a)) {
			PlayerHP = PlayerHP - 15;
			a->HandleCollision();
			cout << "Hit by alien!" << endl;
			cout << "HP"<< PlayerHP << endl;
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Update rangers
	for(auto r : rangers) {
		r->RangerM();
// Check player collision with alien
		if(player->CollidesWith(r)) {
			PlayerHP = PlayerHP - 5;
			r->HandleCollision();
			cout << "Hit by ranger!" << endl;
			cout << "HP"<< PlayerHP << endl;
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Update scouts
	for(auto s : scouts) {
		s->ScoutM();
// Check player collision with alien
		if(player->CollidesWith(s)) {
			PlayerHP = PlayerHP - 2;
			s->HandleCollision();
			cout << "Hit by scout!" << endl;
			cout << "HP"<< PlayerHP << endl;
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Detect projectile collision with alien
	for(auto p : projectiles) {
		for(auto a : aliens) {
			if(p->CollidesWith(a)) {
				Points = Points + 5;
				HealthPackSeed = HealthPackSeed + 5;
				PickUpSeed = PickUpSeed + 5;
				p->HandleCollision();
				a->HandleCollision();
				cout << "Killed an enemy!" << endl;
				cout << "Score:" << Points << endl;
			}
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Detect projectile collision with ranger
	for(auto p : projectiles) {
		for(auto r: rangers) {
			if(p->CollidesWith(r)) {
				Points = Points + 2;
				HealthPackSeed = HealthPackSeed + 2;
				PickUpSeed = PickUpSeed + 2;
				p->HandleCollision();
				r->HandleCollision();
			 cout << "destroyed ranger!" << endl;
			 cout << "Score:" << Points << endl;
			}
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Detect projectile collision with scout
	for(auto p : projectiles) {
		for(auto s: scouts) {
			if(p->CollidesWith(s)) {
				Points = Points + 1;
				HealthPackSeed = HealthPackSeed + 1;
				PickUpSeed = PickUpSeed + 1;
				p->HandleCollision();
				s->HandleCollision();
			 cout << "destroyed scout!" << endl;
			 cout << "Score:" << Points << endl;
			}
		}
	}

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
			

//Increase Time counter by 1
Timer++;
currentSecond = Timer / 60;

	list<shared_ptr<SFAsset>> hbTemp;
	healthbars.clear();
	healthbars = list<shared_ptr<SFAsset>>(hbTemp);
  PlayerHealth(PlayerHP);
			}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//***************************************************************************************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////


  void SFApp::OnRender() {
  SDL_RenderClear(sf_window->getRenderer());


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////


  		//clouds behind
  	for(auto cd2: clouds2) {
			auto cd2Pos = cd2->GetPosition();
				if(cd2->IsAlive() && !(cd2Pos.getY() < -30.0f)) 
					{
					cd2->OnRender();	}
				else {
					int w, h;
						SDL_GetRendererOutputSize(sf_window->getRenderer(),&w,&h);
					auto cd2pos = Point2 (rand() % (40 + 600), 2000);
						cd2->SetPosition(cd2pos);
						cd2->SetCloudAlive();
				}
  	}


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

	if(player->IsAlive()) {
			player->OnRender();
		}
	
    gameover->OnRender();
	
	// draw projectiles
	 for(auto p: projectiles) {
		if(p->IsAlive()) {
			p->OnRender();
		}
	}

	for(auto ex: explosions){
		if(ex->IsAlive()) {
			ex->OnRender();
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	//alien generation
  	for(auto a: aliens) {
			auto aPos = a->GetPosition();
				if(a->IsAlive() && !(aPos.getY() < -30.0f)) {
					a->OnRender();	}
				else {
					int w, h;
						SDL_GetRendererOutputSize(sf_window->getRenderer(),&w,&h);
					auto pos = Point2 (rand() % (40 + 600), 3000);
						a->SetPosition(pos);
						a->SetAlienAlive();
  			}
		}


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	//ranger generation
		for(auto r: rangers) {
			auto rPos = r->GetPosition();
				if(r->IsAlive() && !(rPos.getY() < -30.0f)) {
					r->OnRender();	}
				else {
					int w, h;
						SDL_GetRendererOutputSize(sf_window->getRenderer(),&w,&h);
					auto pos = Point2 (rand() % (40 + 600), 3000);
						r->SetPosition(pos);
						r->SetRangerAlive();
				}
		}


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	//scout generation
		for(auto s: scouts) {
			auto sPos = s->GetPosition();
				if(s->IsAlive() && !(sPos.getY() < -30.0f)) {
					s->OnRender();	}
				else {
					int w, h;
						SDL_GetRendererOutputSize(sf_window->getRenderer(),&w,&h);
					auto pos = Point2 (rand() % (40 + 600), 3000);
						s->SetPosition(pos);
						s->SetScoutAlive();
				}
		}


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

	//coin generation
  	for(auto c: coins) {
			auto cPos = c->GetPosition();
				if(c->IsAlive() && !(cPos.getY() < -30.0f)) 
					{
					c->OnRender();	}
				else {
					int w, h;
						SDL_GetRendererOutputSize(sf_window->getRenderer(),&w,&h);
					auto pos = Point2 (rand() % (40 + 600), 3000);
						c->SetPosition(pos);
						c->SetCoinAlive();
				}
  	}


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

		// draw pickups
	 for(auto pu: pickups) {
	 		auto puPos = pu->GetPosition();
	 	// if the player gains 1000 points - spawn a pickup
	 	if( PickUpSeed > 100){
			PickUpSeed = PickUpSeed - 100;
			int w, h;
						SDL_GetRendererOutputSize(sf_window->getRenderer(),&w,&h);
			auto pos = Point2 (rand() % (500), 600);
						pu->SetPosition(pos);
						pu->SetPickUpAlive();
		}	
		// render the alive pickups
		else if(pu->IsAlive()&& !(puPos.getY()< 0)) {
			pu->OnRender();
			}
		else {
		// after the initial pickup is spawned, hold the pickup off screen until player gains sufficient points.
		int w, h;
						SDL_GetRendererOutputSize(sf_window->getRenderer(),&w,&h);
			auto pos = Point2 (-10, -10);
						pu->SetPosition(pos);
						pu->SetPickUpAlive();
	}
	}

	
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

		// draw healthpacks
	 for(auto hp: healthpacks) {
	 		auto hpPos = hp->GetPosition();
	 	// if the player gains 1000 points - spawn a health pack
	 	if( HealthPackSeed > 1000){
			HealthPackSeed = HealthPackSeed - 1000;
			int w, h;
						SDL_GetRendererOutputSize(sf_window->getRenderer(),&w,&h);
			auto pos = Point2 (rand() % (500), 600);
						hp->SetPosition(pos);
						hp->SetHealthPackAlive();
		}	
		// render the alive healthpacks
		else if(hp->IsAlive()&& !(hpPos.getY()< 0)) {
			hp->OnRender();
			}
		else {
		// after the initial health pack is spawned, hold the health pack off screen until player gains sufficient points.
		int w, h;
						SDL_GetRendererOutputSize(sf_window->getRenderer(),&w,&h);
			auto pos = Point2 (-10, -10);
						hp->SetPosition(pos);
						hp->SetHealthPackAlive();
	}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////


 		//clouds on top
  	for(auto cd1: clouds) {
			auto cd1Pos = cd1->GetPosition();
				if(cd1->IsAlive() && !(cd1Pos.getY() < -30.0f)) 
					{
					cd1->OnRender();	}
				else {
					int w, h;
						SDL_GetRendererOutputSize(sf_window->getRenderer(),&w,&h);
					auto cd1pos = Point2 (rand() % (40 + 600), 2000);
						cd1->SetPosition(cd1pos);
						cd1->SetCloudAlive();
				}
  	}
 
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Healthbars
		for (auto hb : healthbars){
			hb->OnRender();
		}

// hud 
	hud->OnRender();


  // Switch the off-screen buffer to be on-screen
  SDL_RenderPresent(sf_window->getRenderer());

	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//***************************************************************************************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

void SFApp::PlayerHealth(int PlayerHP){

	int number_of_healthbars = PlayerHP / 5;

	for(int i = 0; i < number_of_healthbars; i++) {
		// place an alien at width/number_of_aliens * i
		auto healthbar = make_shared<SFAsset>(SFASSET_HEALTHBAR, sf_window);
		auto pos = Point2(15+ (i*5), 470);
		// Make enemy at position and set it's health
			healthbar->SetPosition(pos);
			healthbars.push_back(healthbar);	
	}
}
//////////////////////////////////////////////
//////////////////////////////////////////////
void SFApp::FireProjectile() {
  auto pb = make_shared<SFAsset>(SFASSET_PROJECTILE, sf_window);
  auto v  = player->GetPosition();
  pb->SetPosition(v);
  projectiles.push_back(pb);
}


//////////////////////////////////////////////
//////////////////////////////////////////////

void SFApp::GameOver(){
	cout <<"You Died!" << endl;
	cout <<"Your Final Score was: " << Points << endl;
    		auto go_pos = Point2(320,480);
  			gameover->SetPosition(go_pos);
		is_paused=true;
}
//////////////////////////////////////////////
//////////////////////////////////////////////

void SFApp::Pause(){
  if(is_paused){
		is_paused = false;
	}
	else{
		is_paused = true;
	}
}




