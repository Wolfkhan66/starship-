
#include "SFApp.h"




	SFApp::SFApp(std::shared_ptr<SFWindow> window) :	is_running(true),is_paused(false), sf_window(window) {
  	int canvas_w, canvas_h;
  		SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);

  app_box = make_shared<SFBoundingBox>(Vector2(canvas_w, canvas_h), 	canvas_w, canvas_h);

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
		// call Spawning functions
		SpawnStartWave(1);
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

//	Spawn Player  
	player  = make_shared<SFAsset>(SFASSET_PLAYER, sf_window);
  		auto player_pos = Point2(320, 88.0f);
  			player->SetPosition(player_pos);

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////


//	Spawn hud for healthbars and score
	hphud  = make_shared<SFAsset>(SFASSET_HPHUD, sf_window);
  		auto hphud_pos = Point2(60, 480);
  			hphud->SetPosition(hphud_pos);

	scorehud  = make_shared<SFAsset>(SFASSET_SCOREHUD, sf_window);
  		auto schud_pos = Point2(580, 486);
  			scorehud->SetPosition(schud_pos);
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
//Check Events
 
//check player health
	if(PlayerHP <= 0){
		player->HandleCollision();
		lives --; 
	}
	
	if( lives == 0){

		player->HandleCollision();
		GameOver();
	}

// check pickup is active
	if(currentSecond > powertime + 10){
		fireN = 1;
	}

// check healthpack spawn
	if(HealthPackSeed >= 1000){
			HealthPackSeed = HealthPackSeed - 1000;
			SpawnHealthPack(1);
	}

//check pickup spawn
	if(PickUpSeed >= 100 ){
		PickUpSeed = PickUpSeed - 100;
		SpawnPickUp(1);
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

for(auto ex:explosions){
		ex->ExplosionM();
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
		auto hpPos = hp->GetPosition();
// check player collision with healthpack
		if(player->CollidesWith(hp)) {
				PlayerHP = PlayerHP + 25;
				hp->HandleCollision();
					cout << "Collected Health!" << endl;
					cout << "HP"<< PlayerHP << endl;
		}
		else if(hpPos.getY() < -30.0f){
			hp->HandleCollision();
				SpawnHealthPack(1);
	}

}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// pickup movement
	for(auto pu : pickups){
		pu->CollectibleM();
		auto puPos = pu->GetPosition();
// check player collision with pickup
		if(player->CollidesWith(pu)) {
					pu->HandleCollision();
					powertime = currentSecond;
	      	fireN = 20;
						cout << "Collected Pickup!" << endl;
						cout << "Maximum fire"<< endl;     			
		}	
		else if(puPos.getY() < -30.0f){
			pu->HandleCollision();
				SpawnPickUp(1);
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// coin movement
	for(auto c : coins) {
		c->CollectibleM();
		auto cPos = c->GetPosition();
// Check player collision with coin
		if(player->CollidesWith(c)) {
			Points = Points + 100;
			HealthPackSeed = HealthPackSeed + 100;
				c->HandleCollision();
					cout << "Collected coin!" << endl;
					cout << "Score:" << Points << endl;
						SpawnCoin(1);
		}
		else if(cPos.getY() < -30.0f){
			c->HandleCollision();
				SpawnCoin(1);
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Update alien positions
	for(auto a : aliens) {
		a->AlienM();
		auto aPos = a->GetPosition();
// Check player collision with alien
		if(player->CollidesWith(a) && EnemiesDead < 50) {
			PlayerHP = PlayerHP - 15;
			a->HandleCollision();
				cout << "Hit by alien!" << endl;
				cout << "HP"<< PlayerHP << endl;			
					EnemiesDead++;
					SpawnAlien(1);
					SpawnExplosion(aPos,1);
					xtime = currentSecond;
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
		if(player->CollidesWith(r)) {
			PlayerHP = PlayerHP - 10;
			r->HandleCollision();
				cout << "Hit by ranger!" << endl;
				cout << "HP"<< PlayerHP << endl;
					SpawnRanger(1);
					SpawnExplosion(rPos,1);
					xtime = currentSecond;
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
		if(player->CollidesWith(s)) {
			PlayerHP = PlayerHP - 5;
			s->HandleCollision();
				cout << "Hit by scout!" << endl;
				cout << "HP"<< PlayerHP << endl;
					SpawnScout(1);
					SpawnExplosion(sPos,1);
					xtime = currentSecond;
		}
		else if(sPos.getY() < -30.0f){
				s->HandleCollision();
				SpawnScout(1);
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Detect projectile collision with alien
	for(auto p : projectiles) {
		for(auto a : aliens) {
				auto aPos = a->GetPosition();
			if(p->CollidesWith(a)) {
				Points = Points + 5;
				HealthPackSeed = HealthPackSeed + 5;
				PickUpSeed = PickUpSeed + 5;
					p->HandleCollision();
					a->HandleCollision();
						cout << "Killed an enemy!" << endl;
						cout << "Score:" << Points << endl;
							SpawnAlien(1);
							SpawnExplosion(aPos,1);
							xtime = currentSecond;
			}
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Detect projectile collision with ranger
	for(auto p : projectiles) {
		for(auto r: rangers) {
				auto rPos = r->GetPosition();
			if(p->CollidesWith(r)) {
				Points = Points + 2;
				HealthPackSeed = HealthPackSeed + 2;
				PickUpSeed = PickUpSeed + 2;
					p->HandleCollision();
					r->HandleCollision();
			 			cout << "destroyed ranger!" << endl;
			 			cout << "Score:" << Points << endl;
							SpawnRanger(1);
							SpawnExplosion(rPos,1);
							xtime = currentSecond;
			}
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Detect projectile collision with scout
	for(auto p : projectiles) {
		for(auto s: scouts) {
				auto sPos = s->GetPosition();
			if(p->CollidesWith(s)) {
				Points = Points + 1;
				HealthPackSeed = HealthPackSeed + 1;
				PickUpSeed = PickUpSeed + 1;
					p->HandleCollision();
					s->HandleCollision();
			 			cout << "destroyed scout!" << endl;
			 			cout << "Score:" << Points << endl;
							SpawnScout(1);
							SpawnExplosion(sPos,1);	
							xtime = currentSecond;
			}
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	// handle dead explosions
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
	// Remove all dead Pickups
	list<shared_ptr<SFAsset>> puTemp;
		for(auto pu : pickups) {
			if(pu->IsAlive()) {
				puTemp.push_back(pu);
			}
		}
			pickups.clear();
			pickups = list<shared_ptr<SFAsset>>(puTemp);

//Increase Time counter by 1
Timer++;
currentSecond = Timer / 60;


	list<shared_ptr<SFAsset>> hbTemp;
	healthbars.clear();
	healthbars = list<shared_ptr<SFAsset>>(hbTemp);
  PlayerHealth(PlayerHP);


	list<shared_ptr<SFAsset>> scTemp;
	points.clear();
	points = list<shared_ptr<SFAsset>>(scTemp);
	SpawnScore(Points);
	
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
	
	// draw projectiles
	 for(auto p: projectiles) {
		if(p->IsAlive()) {
			p->OnRender();
		}
	}

	for(auto ex: explosions){
			ex->OnRender();
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	//alien generation
  	for(auto a: aliens) {
					a->OnRender();	
			
		}


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	//ranger generation
		for(auto r: rangers) {
					r->OnRender();	
		}


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
	//scout generation
		for(auto s: scouts) {
					s->OnRender();	
		}


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

	//coin generation
  	for(auto c: coins) {
				c->OnRender();	
  	}


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

		// draw pickups
	 for(auto pu: pickups) {
			pu->OnRender();
	}

	
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

		// draw healthpacks
	 for(auto hp: healthpacks) {
			hp->OnRender();
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
	// Healthbars ,points and huds // spawn last so they are always on top
	
	for (auto hb : healthbars){
		hb->OnRender();
	}
		
	//	render the hphud after the healthbars so that the aestheic overlays the hpbars
		hphud->OnRender();

	// this time render the score hud before the number assets so that they appear on top of the hud
		scorehud->OnRender();

	for(auto po : points){
		po->OnRender();
	}


	for(auto gameover :interfaces){
    gameover->OnRender();
	}


  // Switch the off-screen buffer to be on-screen
  SDL_RenderPresent(sf_window->getRenderer());

	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//***************************************************************************************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

////Render the health bar in the health bar hud
void SFApp::PlayerHealth(int PlayerHP){
  
	//the space for health bars in the hud is 100pixels wide, the health bar assets are 5 pixels wide so
	//since the players health starts at 100, divided it by 5 to get a number between 1 and 20
	int number_of_healthbars = PlayerHP / 5;
	
	// for 1 to number of healthbars
	for(int i = 0; i < number_of_healthbars; i++) {
	//If the players health is above 50 render the blue bars
		if(PlayerHP >= 50){
			auto healthbarblue = make_shared<SFAsset>(SFASSET_HEALTHBARBLUE, sf_window);
			auto pos = Point2(13+ (i*5), 470);
			healthbarblue->SetPosition(pos);
			healthbars.push_back(healthbarblue);
		}
	// if the health is lower than 50 but above 25 , render the yellow bars
		else if(PlayerHP >= 25){
			auto healthbaryellow = make_shared<SFAsset>(SFASSET_HEALTHBARYELLOW, sf_window);
			auto pos = Point2(13+ (i*5), 470);
			healthbaryellow->SetPosition(pos);
			healthbars.push_back(healthbaryellow);
		}
	// if the health is lower than 25 render the red bars
		else{
			auto healthbarred= make_shared<SFAsset>(SFASSET_HEALTHBARRED, sf_window);
			auto pos = Point2(13 + (i*5), 470);
			healthbarred->SetPosition(pos);
			healthbars.push_back(healthbarred);	
		}
	}
}
//////////////////////////////////////////////
//////////////////////////////////////////////
void SFApp::SpawnAlien(int AlienNo){

for(int i = 0; i < AlienNo; i++) {
		auto alien = make_shared<SFAsset>(SFASSET_ALIEN, sf_window);
		auto pos = Point2 (rand() % 600 + 40, rand() % 3000 + 600);
				alien->SetPosition(pos);
				aliens.push_back(alien);
}
}

//////////////////////////////////////////////
//////////////////////////////////////////////
void SFApp::SpawnRanger(int RangerNo){

for(int i = 0; i < RangerNo; i++) {
		auto ranger = make_shared<SFAsset>(SFASSET_RANGER, sf_window);
		auto pos = Point2 (rand() % 600 + 40, rand() % 3000 + 600);
				ranger->SetPosition(pos);
				rangers.push_back(ranger);
}
}
//////////////////////////////////////////////
//////////////////////////////////////////////
void SFApp::SpawnScout(int ScoutNo){

for(int i = 0; i < ScoutNo; i++) {
		auto scout = make_shared<SFAsset>(SFASSET_SCOUT, sf_window);
		auto pos = Point2 (rand() % 600 + 40, rand() % 3000 + 600);
				scout->SetPosition(pos);
				scouts.push_back(scout);
}
}
//////////////////////////////////////////////
//////////////////////////////////////////////
void SFApp::SpawnCoin(int CoinNo){

for(int i = 0; i < CoinNo; i++) {
		auto coin = make_shared<SFAsset>(SFASSET_COIN, sf_window);
		auto pos = Point2 (rand() % 600 + 40, rand() % 3000 + 600);
				coin->SetPosition(pos);
				coins.push_back(coin);
}
}
//////////////////////////////////////////////
//////////////////////////////////////////////
void SFApp::SpawnHealthPack(int HealthPackNo){

for(int i = 0; i < HealthPackNo; i++) {
		auto healthpack = make_shared<SFAsset>(SFASSET_HEALTHPACK, sf_window);
		auto pos = Point2 (rand() % 600 + 40, rand() % 3000 + 600);
				healthpack->SetPosition(pos);
				healthpacks.push_back(healthpack);
}
}
//////////////////////////////////////////////
//////////////////////////////////////////////
void SFApp::SpawnPickUp(int PickUpNo){

for(int i = 0; i < PickUpNo; i++) {
		auto pickup = make_shared<SFAsset>(SFASSET_PICKUP, sf_window);
		auto pos = Point2 (rand() % 600 + 40, rand() % 3000 + 600);
				pickup->SetPosition(pos);
				pickups.push_back(pickup);
}
}
//////////////////////////////////////////////
//////////////////////////////////////////////
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
			auto c1pos = Point2 (rand() %(40 + 600), rand() %(600+3000));
					cloud1->SetPosition(c1pos);
					clouds.push_back(cloud1);
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
void SFApp::SpawnCloudsOnBottom(int CloudNo){
		for(int i=0; i<CloudNo; i++) {
			auto cloud2 = make_shared<SFAsset>(SFASSET_CLOUD, sf_window);
			auto c2pos = Point2 (rand() %(40 + 600), rand() %(600+3000));
					cloud2->SetPosition(c2pos);
					clouds2.push_back(cloud2);
		}
	}
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
void SFApp::FireProjectile() {
  auto pb = make_shared<SFAsset>(SFASSET_PROJECTILE, sf_window);
  auto v  = player->GetPosition();
  pb->SetPosition(v);
  projectiles.push_back(pb);
}


//////////////////////////////////////////////
//////////////////////////////////////////////

void SFApp::ClearAssets(){
	aliens.clear();
	scouts.clear();
	rangers.clear();
	coins.clear();
	healthpacks.clear();
	pickups.clear();
	clouds.clear();
}

//////////////////////////////////////////////
//////////////////////////////////////////////

void SFApp::SpawnStartWave(int x){
	for(int i=0; i< x; i++){
	SpawnAlien(2);
	SpawnRanger(10);
	SpawnScout(2);
	SpawnCoin(2);
	SpawnCloudsOnTop(1);
	SpawnCloudsOnBottom(1);
	}
}
//////////////////////////////////////////////
//////////////////////////////////////////////
void SFApp::SpawnWave2(int x){
	SpawnAlien(2);
	SpawnRanger(2);
	SpawnScout(20);
	SpawnCoin(2);
}
//////////////////////////////////////////////
//////////////////////////////////////////////

void SFApp::SpawnWave3(int x){
	SpawnAlien(10);
	SpawnRanger(10);
	SpawnScout(10);
	SpawnCoin(3);
}
//////////////////////////////////////////////
//////////////////////////////////////////////

void SFApp::SpawnWave4(int x){
	SpawnAlien(20);
	SpawnRanger(10);
	SpawnScout(5);
	SpawnCoin(3);
}
//////////////////////////////////////////////
//////////////////////////////////////////////

void SFApp::SpawnBoss1(int x){

}
//////////////////////////////////////////////
//////////////////////////////////////////////

void SFApp::SpawnBonusWave(int x){
	SpawnCoin(10);
	SpawnHealthPack(3);
}
//////////////////////////////////////////////
//////////////////////////////////////////////

void SFApp::GameReset(){
	 fire = 0;
   fireN = 1;
   PlayerHP = 100;
   Points = 0;
   HealthPackSeed = 0;
   PickUpSeed = 0;
   EnemiesDead = 0;
   Timer = 0;
   currentSecond = 0;
   powertime = 0;
   xtime = 0;

}
//////////////////////////////////////////////
//////////////////////////////////////////////

void SFApp::GameOver(){
	SpawnGameOver(1);
	cout <<"You Died!" << endl;
	cout <<"Your Final Score was: " << Points << endl;
	ClearAssets();
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

//////////////////////////////////////////////
//////////////////////////////////////////////

void SFApp::SpawnScore(int Points)
{
string ScoreString;
ostringstream ScoreConvert;
ScoreConvert << Points;
ScoreString = ScoreConvert.str();


for(int i = 0; i < ScoreString.size(); i++){
	if(ScoreString[i] == '0') {
			auto zero = make_shared<SFAsset>(SFASSET_NUM0, sf_window);
			auto pos = Point2(575+ (i*6), 470);
			zero->SetPosition(pos);
			points.push_back(zero);
		}
	if(ScoreString[i] == '1'){
				auto one = make_shared<SFAsset>(SFASSET_NUM1, sf_window);
				auto pos = Point2(575 + (i*6),470);
				one->SetPosition(pos);
				points.push_back(one);
	}
	if(ScoreString[i] == '2'){
				auto two = make_shared<SFAsset>(SFASSET_NUM2, sf_window);
				auto pos = Point2(575 + (i*6),470);
				two->SetPosition(pos);
				points.push_back(two);
	}
	if(ScoreString[i] == '3'){
				auto three = make_shared<SFAsset>(SFASSET_NUM3, sf_window);
				auto pos = Point2(575 + (i*6),470);
				three->SetPosition(pos);
				points.push_back(three);
	}
	if(ScoreString[i] == '4'){
				auto four = make_shared<SFAsset>(SFASSET_NUM4, sf_window);
				auto pos = Point2(575 + (i*6),470);
				four->SetPosition(pos);
				points.push_back(four);
	}
	if(ScoreString[i] == '5'){
				auto five = make_shared<SFAsset>(SFASSET_NUM5, sf_window);
				auto pos = Point2(575 + (i*6),470);
				five->SetPosition(pos);
				points.push_back(five);
	}
	 if(ScoreString[i] == '6'){
				auto six = make_shared<SFAsset>(SFASSET_NUM6, sf_window);
				auto pos = Point2(575 + (i*6),470);
				six->SetPosition(pos);
				points.push_back(six);
	}
	if(ScoreString[i] == '7'){
				auto seven = make_shared<SFAsset>(SFASSET_NUM7, sf_window);
				auto pos = Point2(575 + (i*6),470);
				seven->SetPosition(pos);
				points.push_back(seven);
	}
	if(ScoreString[i] == '8'){
				auto eight = make_shared<SFAsset>(SFASSET_NUM8, sf_window);
				auto pos = Point2(575 + (i*6),470);
				eight->SetPosition(pos);
				points.push_back(eight);
	}
	if (ScoreString[i] == '9'){
				auto nine = make_shared<SFAsset>(SFASSET_NUM9, sf_window);
				auto pos = Point2(575 + (i*6),470);
				nine->SetPosition(pos);
				points.push_back(nine);
	}

}
}





