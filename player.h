#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include <SDL.h>
#include "global.h"
#include "entity.h"
#include "texture.h"
#include "bullet.h"
#include "proto/player.pb.h"

const cpFloat PLAYER_RAD = 0.05f;
namespace xx {

class Player {
    public:

        //Maximum velocity
        constexpr static double PLAYER_VEL=30;

        constexpr static int sWidth=SCREEN_WIDTH;
        constexpr static int sHeight=SCREEN_HEIGHT;

        //Default constructor
        Player() {}

        //Initializes the variables
        Player(Entity *);

        //Deallocated Memory
        ~Player();

        //render
        void render(SDL_Renderer *r);

        //Event Handler
        void handleEvent(SDL_Event e, SDL_Renderer *r, cpSpace *space, cpVect & moveVect);

        void handleFire(SDL_Renderer *r, cpSpace *space, cpFloat &time);

        void rightPressCheck(cpVect & moveVect);
        bool leftPressCheck() {return Lpressed;}

        void setInCloud(int);

        //Move the aircraft
        void updateState();


        //HP
        int hp=10000;
        int maxhp=10000;

        //body
        cpBody *body() { return mEntity->body(); }

        //free bullet
        void freeBullet(Bullet a);

        //Hurt
        void hurt(int dam);

        void setMove(cpVect vel) { mVel = vel; }

        cpVect vectorForward();

        //in Cloud
        int mInCloud=0;

        //Getter
        double velX() { return sin( cpBodyGetAngle( mEntity->body() ) ); }
        double velY() { return -cos( cpBodyGetAngle( mEntity->body() ) ); }
        cpFloat posX() { return mEntity->body()->p.x; }
        cpFloat posY() { return mEntity->body()->p.y; }
        cpFloat angle() { return cpBodyGetAngle(mEntity->body()); }
        cpFloat sensor() { return mEntity->sprite().height()/2; }
        int firedAmmo();

    private:
        //Velocity of player
        cpVect mVel;

        //Entity
        Entity *mEntity;

        //Aircraft texture
        Texture aircraft;

        //Rotation
        void rotLeft();
        void rotRight();
        cpFloat mAngle;

        //check if button press or not
        bool Rpressed,Lpressed;

        //Maximum bullet
        int maxAmmo;
        Bullet ammo[10];

        int firedNumber;
};

}

#endif // PLAYER_H_INCLUDED
