#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include <string>
#include <SDL.h>
#include "texture.h"
#include <chipmunk_private.h>


static const std::string BULLET_IMG = "bullet.png";
static const int BULLET_SPEED = 4;


namespace xx {

class Player;

class Bullet {
    public:
        bool checkExist() { return mExist; }
        //Initializes the variables
        Bullet();

        //Deallocate memory
        ~Bullet();

        //Free
        void free();

        //Create bullet
        void createBullet( SDL_Renderer *r, cpSpace *space, double range, Player * p, bool noRender = false);

        //Move the bullet
        void move();

        void destroy();

        //return body
        cpBody * body() { return mBody; }
        Player * player() { return mPlayer; }

        //render
        void render(SDL_Renderer *r);
    private:
        //Velocity of bullet
        double mVelX,mVelY;

        //Bullet texture
        Texture img;

        //Bullet properties
        double mRange;//maximum range
        bool mExist;
        cpBody *mBody;
        cpFloat mPosX, mPosY,mX;
        bool mLoadedImg;
        Player *mPlayer;
        cpFloat mInitialAngle;
        bool mNoRender;
};

}
#endif // BULLET_H_INCLUDED
