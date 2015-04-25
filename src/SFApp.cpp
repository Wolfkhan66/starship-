#include "SFApp.h"

	SFApp::SFApp(std::shared_ptr<SFWindow> window) : fire(0),	is_running(true), sf_window(window) {
  	int canvas_w, canvas_h;
  		SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);

  app_box = make_shared<SFBoundingBox>(Vector2(canvas_w, canvas_h), 	canvas_w, canvas_h);
  	player  = make_shared<SFAsset>(SFASSET_PLAYER, sf_window);
  		auto player_pos = Point2(canvas_w/2, 88.0f);
  			player->SetPosition(player_pos);

		// spawn aliens
	const int number_of_aliens = 7;
		for(int i=0; i<number_of_aliens; i++) {
			auto alien = make_shared<SFAsset>(SFASSET_ALIEN, sf_window);
			auto pos = Point2 (rand() % (40 + 600), rand() %(600 + 3000));
					alien->SetPosition(pos);
					aliens.push_back(alien);
	}

		//spawn debris
  const int number_of_debris = 10;
		for(int i=0; i<number_of_debris; i++) {
			auto debris = make_shared<SFAsset>(SFASSET_DEBRIS, sf_window);
			auto debris_pos = Point2 (rand() % (40 + 600), rand() %(600 + 3000));
					debris->SetPosition(debris_pos);
					debrise.push_back(debris);
	}

	//spawn coins
	const int number_of_coins = 3;
		for(int i=0; i<number_of_coins; i++) {
			auto coin = make_shared<SFAsset>(SFASSET_COIN, sf_window);
			auto pos  = Point2 (rand() % (40 + 600), rand() %(600 + 3000));
					coin->SetPosition(pos);
					coins.push_back(coin);
		}
	
	//spawn stars
	const int number_of_stars = 25;
		for(int i=0; i<number_of_stars; i++) {
			auto star = make_shared<SFAsset>(SFASSET_STAR, sf_window);
			auto pos = Point2 (rand() %(40 + 600), rand() %(600+800));
					star->SetPosition(pos);
					stars.push_back(star);
		}
	

	//Spawn healthpack
		const int number_of_healthpacks = 1;
					for(int i=0; i<number_of_healthpacks; i++) {
			auto healthpack = make_shared<SFAsset>(SFASSET_HEALTHPACK, sf_window);
			auto hp_pos  = Point2 (320,5000);
					healthpack->SetPosition(hp_pos);
					healthpacks.push_back(healthpack);
					}


}
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

SFApp::~SFApp() {
}

/**
 * Handle all events that come from SDL.
 * These are timer or keyboard events.
 */
void SFApp::OnEvent(SFEvent& event) {
  SFEVENT the_event = event.GetCode();
  switch (the_event) {
  	case SFEVENT_QUIT:
   		 is_running = false;
    break;
  	case SFEVENT_UPDATE:
    		OnUpdateWorld();
    		OnRender();
    break;
		default:
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


//////////////////////////////////////////////////
//////////////////////////////////////////////////


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
 		fire ++;
 		FireProjectile();
 }

int w, h;
SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);


// star movement
	for(auto s: stars){
		s->CoinM();
		}



// Update projectile positions
	for(auto p: projectiles) {
		p->GoNorth();
// Remove projectile when it hits the top of the screen
		auto p_pos = p->GetPosition();
		if(p_pos.getY() > h) {
			p->HandleCollision();
		}
	}

// healthpack movement
	for(auto hp : healthpacks){
				hp->CoinM();
// check player collision with healthpack
			if(player->CollidesWith(hp)) {
				PlayerHP = PlayerHP + 25;
				hp->HandleCollision();
				cout << "Collected Health!" << endl;
				cout << "HP"<< PlayerHP << endl;
		}
	}
	
// coin movement
	for(auto c : coins) {
		c->CoinM();
// Check player collision with coin
		if(player->CollidesWith(c)) {
			Points = Points + 100;
			HealthPackSeed = HealthPackSeed + 100;
			c->HandleCollision();
			cout << "Collected coin!" << endl;
			cout << "Score:" << Points << endl;
		}
	}

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

// Update enemy positions
	for(auto d : debrise) {
		d->DebrisM();
// Check player collision with alien
		if(player->CollidesWith(d)) {
			PlayerHP = PlayerHP - 5;
			d->HandleCollision();
			cout << "Hit by debris!" << endl;
			cout << "HP"<< PlayerHP << endl;
		}
	}

// Detect projectile collision with alien
	for(auto p : projectiles) {
		for(auto a : aliens) {
			if(p->CollidesWith(a)) {
				Points = Points + 5;
				HealthPackSeed = HealthPackSeed + 5;
				p->HandleCollision();
				a->HandleCollision();
				cout << "Killed an enemy!" << endl;
				cout << "Score:" << Points << endl;
			}
		}
	}

// Detect projectile collision with debris
	for(auto p : projectiles) {
		for(auto d : debrise) {
			if(p->CollidesWith(d)) {
				Points = Points + 1;
				HealthPackSeed = HealthPackSeed + 1;
				p->HandleCollision();
				d->HandleCollision();
			 cout << "destroyed debris!" << endl;
			 cout << "Score:" << Points << endl;
			}
		}
	}
	
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
			
			}

/*
// Remove dead aliens (the long way)
	list<shared_ptr<SFAsset>> alienTemp;
		for(auto a : aliens) {
			if(a->IsAlive()) {
				alienTemp.push_back(a);
			}
		}
			aliens.clear();
		  aliens = list<shared_ptr<SFAsset>>(alienTemp);


// Remove dead debris 
	list<shared_ptr<SFAsset>> debrisTemp;
		for(auto d : debrise) {
			if(d->IsAlive()) {
				debrisTemp.push_back(d);
			}
		}
			debrise.clear();
			debrise = list<shared_ptr<SFAsset>>(debrisTemp);



// Remove all dead healthpacks
	list<shared_ptr<SFAsset>> hpTemp;
		for(auto hp : healthpacks) {
			if(hp->IsAlive()) {
				hpTemp.push_back(hp);
		}
			healthpacks.clear();
			healthpacks = list<shared_ptr<SFAsset>>(hpTemp);
			
				// Remove all dead coins
	list<shared_ptr<SFAsset>> coinTemp;
		for(auto c : coins) {
			if(c->IsAlive()) {
				coinTemp.push_back(c);
			}
		}
			coins.clear();
			coins = list<shared_ptr<SFAsset>>(coinTemp);
*/
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////


  void SFApp::OnRender() {
  SDL_RenderClear(sf_window->getRenderer());

  // draw the player
  	player->OnRender();
	
	// draw projectiles
	 for(auto p: projectiles) {
		if(p->IsAlive()) {
			p->OnRender();
		}
	}
	
	
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

	//debris generation
		for(auto d: debrise) {
			auto dPos = d->GetPosition();
				if(d->IsAlive() && !(dPos.getY() < -30.0f)) {
					d->OnRender();	}
				else {
					int w, h;
						SDL_GetRendererOutputSize(sf_window->getRenderer(),&w,&h);
					auto pos = Point2 (rand() % (40 + 600), 3000);
						d->SetPosition(pos);
						d->SetDebrisAlive();
				}
		}


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
  	
  		//star generation
  	for(auto s: stars) {
			auto sPos = s->GetPosition();
				if(s->IsAlive() && !(sPos.getY() < -30.0f)) 
					{
					s->OnRender();	}
				else {
					int w, h;
						SDL_GetRendererOutputSize(sf_window->getRenderer(),&w,&h);
					auto pos = Point2 (rand() % (40 + 600), 700);
						s->SetPosition(pos);
						s->SetStarAlive();
				}
  	}

  // Switch the off-screen buffer to be on-screen
  SDL_RenderPresent(sf_window->getRenderer());

	}

////////////////////////////////////////////////////
////////////////////////////////////////////////////

void SFApp::FireProjectile() {
  auto pb = make_shared<SFAsset>(SFASSET_PROJECTILE, sf_window);
  auto v  = player->GetPosition();
  pb->SetPosition(v);
  projectiles.push_back(pb);
}






