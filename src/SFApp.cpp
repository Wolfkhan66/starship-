#include "SFApp.h"

	SFApp::SFApp(std::shared_ptr<SFWindow> window) : fire(0), 	is_running(true), sf_window(window) {
  	int canvas_w, canvas_h;
  		SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);

  app_box = make_shared<SFBoundingBox>(Vector2(canvas_w, canvas_h), 	canvas_w, canvas_h);
  	player  = make_shared<SFAsset>(SFASSET_PLAYER, sf_window);
  		auto player_pos = Point2(canvas_w/2, 88.0f);
  			player->SetPosition(player_pos);

		// spawn aliens
	const int number_of_aliens = 5;
		for(int i=0; i<number_of_aliens; i++) {
			auto alien = make_shared<SFAsset>(SFASSET_ALIEN, sf_window);
			auto pos = Point2 (rand() % (40 + 640), rand() %(600 + 3000));
					alien->SetPosition(pos);
					aliens.push_back(alien);
	}

		// spawn background1
			auto background1 = make_shared<SFAsset>(SFASSET_BACKGROUND1, sf_window);
			auto pos = Point2 (0,0);
					background1->SetPosition(pos);
					background1s.push_back(background1);
		
		//spawn debris
  const int number_of_debris = 8;
		for(int i=0; i<number_of_debris; i++) {
			auto debris = make_shared<SFAsset>(SFASSET_DEBRIS, sf_window);
			auto debris_pos = Point2 (rand() % (40 + 640), rand() %(600 + 3000));
					debris->SetPosition(debris_pos);
					debrise.push_back(debris);
	}

		//spawn coins
	const int number_of_coins = 3;
		for(int i=0; i<number_of_coins; i++) {
			auto coin = make_shared<SFAsset>(SFASSET_COIN, sf_window);
			auto pos  = Point2 (rand() % (40 + 640), rand() %(600 + 3000));
					coin->SetPosition(pos);
					coins.push_back(coin);
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

// Update projectile positions
	for(auto p: projectiles) {
		p->GoNorth();
// Remove projectile when it hits the top of the screen
		auto p_pos = p->GetPosition();
		if(p_pos.getY() > h) {
			p->HandleCollision();
		}
	}


	for(auto c : coins) {
		c->CoinM();
// Check player collision with coin
		if(player->CollidesWith(c)) {
			cout << "Collected coin!" << endl;
			Points = Points + 100;
			c->HandleCollision();
		}
	}

// Update alien positions
	for(auto a : aliens) {
		a->AlienM();
// Check player collision with alien
		if(player->CollidesWith(a)) {
			cout << "Hit by alien!" << endl;
			cout << "HP"<< PlayerHP << endl;
			PlayerHP = PlayerHP - 15;
			a->HandleCollision();
		}
	}

// Update enemy positions
	for(auto d : debrise) {
		d->DebrisM();
// Check player collision with alien
		if(player->CollidesWith(d)) {
			cout << "Hit by debris!" << endl;
			cout << "HP"<< PlayerHP << endl;
			PlayerHP = PlayerHP - 5;
			d->HandleCollision();
		}
	}

// Detect projectile collision with alien
	for(auto p : projectiles) {
		for(auto a : aliens) {
			if(p->CollidesWith(a)) {
				cout << "Killed an enemy!" << endl;
				cout << "Score:" << Points << endl;
				Points = Points + 5;
				p->HandleCollision();
				a->HandleCollision();
			}
		}
	}

// Detect projectile collision with debris
	for(auto p : projectiles) {
		for(auto d : debrise) {
			if(p->CollidesWith(d)) {
				cout << "destroyed debris!" << endl;
				Points = Points + 1;
				p->HandleCollision();
				d->HandleCollision();
			}
		}
	}

/*// Remove dead aliens (the long way)
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
*/

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


//////////////////////////////////////////////////////
//////////////////////////////////////////////////////


  void SFApp::OnRender() {
  SDL_RenderClear(sf_window->getRenderer());

	//render background 1
for (auto b1:background1s){
	b1->OnRender();
	}

  // draw the player
  	player->OnRender();
	
	// draw projectiles
	 for(auto p: projectiles) {
		if(p->IsAlive()) {
			p->OnRender();
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
					auto pos = Point2 (rand() % (20 + 500), 550);
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
					auto pos = Point2 (rand() % (500), 550);
						d->SetPosition(pos);
						d->SetDebrisAlive();
				}
		}


	//coin generation
  	for(auto c: coins) {
			auto cPos = c->GetPosition();
				if(c->IsAlive() && !(cPos.getY() < -30.0f)) {
					c->OnRender();	}
				else {
					int w, h;
						SDL_GetRendererOutputSize(sf_window->getRenderer(),&w,&h);
					auto pos = Point2 (rand() % (20 + 500), 550);
						c->SetPosition(pos);
						c->SetCoinAlive();
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
