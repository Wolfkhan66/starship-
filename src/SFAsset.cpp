#include "SFAsset.h"

int SFAsset::SFASSETID=0;

SFAsset::SFAsset(SFASSETTYPE type, std::shared_ptr<SFWindow> window): type(type), sf_window(window) {
  this->id   = ++SFASSETID;

  switch (type) {

///////////////////////////player///////////////////////////////
  case SFASSET_PLAYER:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/player/player.png");
    break;
  case SFASSET_PROJECTILE:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/player/projectile.png");
    break;

///////////////////////////enemies///////////////////////////////
  case SFASSET_ALIEN:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/enemies/alien.png");
    break;
  case SFASSET_SCOUT:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/enemies/scout.png");
    break;
  case SFASSET_RANGER:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/enemies/ranger.png");
    break;
  case SFASSET_ALIENFIRE:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/enemies/alienfire.png");
    break;
  case SFASSET_BOSS:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/enemies/boss.png");
    break;


///////////////////////////collectibles///////////////////////////////
  case SFASSET_COIN:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/collectibles/coin.png");
    break;
  case SFASSET_PICKUP:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/collectibles/pickup.png");
    break;
  case SFASSET_HEALTHPACK:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/collectibles/health.png");
    break;

///////////////////////////misc///////////////////////////////
  case SFASSET_HUD:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/misc/hud.png");
    break;
	case SFASSET_CLOUD:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/misc/cloud.png");
    break;
  case SFASSET_EXPLOSION:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/misc/explosion.png");
    break;
  case SFASSET_GAMEOVER:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/misc/gameover.png");
    break;
  case SFASSET_POW:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/misc/power.png");
    break;

///////////////////////////healthbar///////////////////////////////
  case SFASSET_LIFE:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/healthbar/lives.png");
    break;
  case SFASSET_HEALTHBARBLUE:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/healthbar/healthbarblue.png");
    break;
  case SFASSET_HEALTHBARYELLOW:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/healthbar/healthbaryellow.png");
    break;
  case SFASSET_HEALTHBARRED:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/healthbar/healthbarred.png");
    break;

///////////////////////////NumberAssets/////////////////////////
  case SFASSET_NUM0:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/numbers/num0.png");
    break;
  case SFASSET_NUM1:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/numbers/num1.png");
    break;
  case SFASSET_NUM2:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/numbers/num2.png");
    break;
  case SFASSET_NUM3:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/numbers/num3.png");
    break;
  case SFASSET_NUM4:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/numbers/num4.png");
    break;
  case SFASSET_NUM5:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/numbers/num5.png");
    break;
  case SFASSET_NUM6:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/numbers/num6.png");
    break;
  case SFASSET_NUM7:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/numbers/num7.png");
    break;
  case SFASSET_NUM8:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/numbers/num8.png");
    break;
  case SFASSET_NUM9:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/numbers/num9.png");
    break;

}
  if(!sprite) {
    cerr << "Could not load asset of type " << type << endl;
    throw SF_ERROR_LOAD_ASSET;
  }

  // Get texture width & height
  int w, h;
  SDL_QueryTexture(sprite, NULL, NULL, &w, &h);

  // Initialise bounding box
  bbox = make_shared<SFBoundingBox>(SFBoundingBox(Vector2(0.0f, 0.0f), w, h));
}

SFAsset::SFAsset(const SFAsset& a) {
  sprite = a.sprite;
  sf_window = a.sf_window;
  bbox   = a.bbox;
  type   = a.type;
}

SFAsset::~SFAsset() {
  bbox.reset();
  if(sprite) {
    SDL_DestroyTexture(sprite);
    sprite = nullptr;
  }
}

/**
 * The logical coordinates in the game assume that the screen
 * is indexed from 0,0 in the bottom left corner.  The blittable
 * coordinates of the screen map 0,0 to the top left corner. We
 * need to convert between the two coordinate spaces.  We assume
 * that there is a 1-to-1 quantisation.
 */
Vector2 GameSpaceToScreenSpace(SDL_Renderer* renderer, Vector2 &r) {
  int w, h;
  SDL_GetRendererOutputSize(renderer, &w, &h);

  return Vector2 (
                  r.getX(),
                  (h - r.getY())
                  );
}

//////////////////////////////////////////////
//////////////////////////////////////////////

void SFAsset::SetPosition(Point2 & point) {
  Vector2 v(point.getX(), point.getY());
  bbox->SetCentre(v);
}

Point2 SFAsset::GetPosition() {
  return Point2(bbox->centre->getX(), bbox->centre->getY());
}

SFAssetId SFAsset::GetId() {
  return id;
}


//////////////////////////////////////////////
//////////////////////////////////////////////

void SFAsset::OnRender() {
  // 1. Get the SDL_Rect from SFBoundingBox
  SDL_Rect rect;

  Vector2 gs = (*(bbox->centre) + (*(bbox->extent_x) * -1)) + (*(bbox->extent_y) * -1);
  Vector2 ss = GameSpaceToScreenSpace(sf_window->getRenderer(), gs);
  rect.x = ss.getX();
  rect.y = ss.getY();
  rect.w = bbox->extent_x->getX() * 2;
  rect.h = bbox->extent_y->getY() * 2;

  // 2. Blit the sprite onto the level
  SDL_RenderCopy(sf_window->getRenderer(), sprite, NULL, &rect);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//************************************* Handle Asset Movement****************************************//
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

void SFAsset::GoWest() {
 Vector2 c = *(bbox->centre) + Vector2(-5.0f, 0.0f);
 	if(!(c.getX()-25.0f < 0)) {
 		bbox->centre.reset();
 		bbox->centre = make_shared<Vector2>(c);
 	}
 }
//////////////////////////////////////////////
//////////////////////////////////////////////

void SFAsset::GoEast() {

 int w, h;
 SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);

 Vector2 c = *(bbox->centre) + Vector2(5.0f, 0.0f);
 	if(!(c.getX()+25.0f > w)) {
		bbox->centre.reset();
 		bbox->centre = make_shared<Vector2>(c);
 }
 }

//////////////////////////////////////////////
//////////////////////////////////////////////

void SFAsset::GoNorth() {
   if(SFASSET_PLAYER == type) {
 int w, h;
 SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);

	Vector2 c = *(bbox->centre) + Vector2(0.0f, 4.0f);

 if(!(c.getY()-20.0f > h)) {
 bbox->centre.reset();
 bbox->centre = make_shared<Vector2>(c);
 }
 }
 if(SFASSET_PROJECTILE == type){
 Vector2 c = *(bbox->centre) + Vector2(0.0f, 10.0f);
bbox->centre.reset();
bbox->centre = make_shared<Vector2>(c);
	}
}

//////////////////////////////////////////////
//////////////////////////////////////////////

void SFAsset::GoSouth() {

if(SFASSET_PLAYER == type) {
 Vector2 c = *(bbox->centre) + Vector2(0.0f, -3.0f);

 if(!(c.getY() < 60.0f)) {
 bbox->centre.reset();
 bbox->centre = make_shared<Vector2>(c);
 }
}
}
 

//////////////////////////////////////////////
//////////////////////////////////////////////
// handle movement for the Collectibles asset
void SFAsset::CollectibleM() {
int w, h;
SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);

 Vector2 c = *(bbox->centre) + Vector2(0.0f, -3.0f);

  bbox->centre.reset();
  bbox->centre = make_shared<Vector2>(c);
if(SFASSET_CLOUD == type) {
 Vector2 c = *(bbox->centre) + Vector2(0.0f, -2.0f);

  bbox->centre.reset();
  bbox->centre = make_shared<Vector2>(c);
}
}

//////////////////////////////////////////////
//////////////////////////////////////////////
// handle movement for the Explosions asset
void SFAsset::ExplosionM() {
int w, h;
SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);

 Vector2 c = *(bbox->centre) + Vector2(0.0f, -5.0f);

  bbox->centre.reset();
  bbox->centre = make_shared<Vector2>(c);

}


//////////////////////////////////////////////
//////////////////////////////////////////////
// handle movement for the AlienFire asset
void SFAsset::AlienFireM() {
	int w, h;
		SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);
 			Vector2 c = *(bbox->centre) + Vector2(0.0f, -4.0f);
  	bbox->centre.reset();
  	bbox->centre = make_shared<Vector2>(c);		
}


//////////////////////////////////////////////
/////////////***Work in Progress***///////////
// handle movement for the Attacker asset
void SFAsset::AttackerM() {
	int w, h;
	SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);

 	Vector2 c = *(bbox->centre) + Vector2(0.0f, -5.0f);
		if(!(c.getY() <  380)) {
  		bbox->centre.reset();
  		bbox->centre = make_shared<Vector2>(c);
		}	
}
//////////////////////////////////////////////
//////////////////////////////////////////////
// handle movement for the Boss asset
void SFAsset::BossM() {
	int w, h;
		SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);
 			Vector2 c = *(bbox->centre) + Vector2(0.0f, -1.0f);
		if(!(c.getY() <  545)) {
  		bbox->centre.reset();
  		bbox->centre = make_shared<Vector2>(c);
		}	
}
//////////////////////////////////////////////
//////////////////////////////////////////////
// handle movement for the Ranger asset
void SFAsset::RangerM() {
	int w, h;
		SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);
 			Vector2 c = *(bbox->centre) + Vector2(0.0f, -4.0f);
  bbox->centre.reset();
  bbox->centre = make_shared<Vector2>(c);
}

//////////////////////////////////////////////
//////////////////////////////////////////////
// handle movement for the Scout asset
void SFAsset::ScoutM() {
	int w, h;
		SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);
 			Vector2 c = *(bbox->centre) + Vector2(0.0f, -5.0f);
  bbox->centre.reset();
  bbox->centre = make_shared<Vector2>(c);
}

//////////////////////////////////////////////
//////////////////////////////////////////////
// handle movement for the Alien asset
void SFAsset::AlienM() {
	int w, h;
		SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);
 			Vector2 c = *(bbox->centre) + Vector2(0.0f, -3.0f);
  bbox->centre.reset();
  bbox->centre = make_shared<Vector2>(c);
}

//////////////////////////////////////////////
//////////////////////////////////////////////

bool SFAsset::CollidesWith(shared_ptr<SFAsset> other) {
  return bbox->CollidesWith(other->bbox);
}

shared_ptr<SFBoundingBox> SFAsset::GetBoundingBox() {
  return bbox;
}

//////////////////////////////////////////////
//////////////////////////////////////////////
// set and return the health for assets
int SFAsset::GetHealth() {
return this->AssetHealth;
}

void SFAsset::SetHealth(int health) {
AssetHealth = health;
}

//////////////////////////////////////////////
//////////////////////////////////////////////

void SFAsset::SetNotAlive() {
  type = SFASSET_DEAD;
}

//////////////////////////////////////////////
//////////////////////////////////////////////

bool SFAsset::IsAlive() {
  return (SFASSET_DEAD != type);
}

//////////////////////////////////////////////
//////////////////////////////////////////////

void SFAsset::HandleCollision() {
  if(SFASSET_PROJECTILE == type || SFASSET_ALIEN == type || SFASSET_COIN == type || SFASSET_RANGER == type || SFASSET_HEALTHPACK == type|| SFASSET_SCOUT == type|| SFASSET_PICKUP == type || SFASSET_EXPLOSION == type|| SFASSET_GAMEOVER == type || SFASSET_PLAYER == type || SFASSET_ALIENFIRE == type || SFASSET_BOSS == type || SFASSET_CLOUD == type ) {
    SetNotAlive();
  }
}
