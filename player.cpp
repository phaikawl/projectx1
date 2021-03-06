#include "player.h"
#include <SDL.h>
#include <stdio.h>
#include <cmath>
#include <string>
#include "texture.h"
#include "bullet.h"

namespace xx {
    const int Player::mMaxAmmo = 5;
    const int Player::MAX_FIREANGLES = 30;

    Player::Player(Entity * e):
        mVel(cpvzero),
        mEntity(e),
        Rpressed(false),
        Lpressed(false),
        mFiredNumber(0),
        mVectp(cpvzero),
        mHp(1000),
        mMaxHp(1000),
        mHpLossTimer(150)
    {
        cpBodySetUserData(mEntity->body(), this);
    }

    Player::~Player() {
    }

    void Player::free() {
        for(int i=0; i<mMaxAmmo; i++) {
            mAmmo[i].destroy();
            mAmmo[i].free();
        }
    }

    cpFloat angleAdd(cpFloat angle, cpFloat delta) {
        angle += delta;
        if (angle >= 2*M_PI) {
            angle -= 2*M_PI;
        }
        if (angle < 0) {
            angle += 2*M_PI;
        }
        return angle;
    }

    cpVect Player::vectorForward() {
        cpFloat vx = sin(cpBodyGetAngle(mEntity->body()))*PLAYER_VEL;
        cpFloat vy = -cos(cpBodyGetAngle(mEntity->body()))*PLAYER_VEL;
        return cpv(vx, vy);
    }

    void Player::rightPressCheck(cpVect & moveVect) {
        if (Rpressed) {
            mVel = vectorForward();
            moveVect = cpvadd(moveVect, vect(PLAYER_VEL, cpBodyGetAngle(mEntity->body())));
        }
    }

    void Player::handleEvent(SDL_Event e, SDL_Renderer *r, cpSpace *space, Skillmanager *sManager, cpVect & moveVect) {
        //Rotation
        if( e.type == SDL_MOUSEMOTION) {
            int x,y;
            SDL_GetRelativeMouseState(&x,&y);
            if( x<-1 )
                rotLeft();
            if( x>1 )
                rotRight();
        }

        //If the right button was pressed
        if ( e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT ) {
            Rpressed = true;
        } else if ( e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_RIGHT ) {
                Rpressed = false;
                mVel = cpvzero;
        }

        //If the left button was pressed
        if ( e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT ) {
            Lpressed = true;
        } else if ( e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT ) {
            Lpressed = false;
        }

        // If q was pressed
        if ( (e.type == SDL_KEYDOWN) && (e.key.keysym.sym == SDLK_q)) {
                if (sManager->cdCheck(1) == 1 ) {
                    sManager->resetCd(1);
                    mVectp = cpvmult(vectorForward(),50);
                    moveVect = cpvadd(moveVect,mVectp);
                }
        }
    }

    void Player::handleFire(SDL_Renderer *r, cpSpace *space, utils::Timer & fireTimer, cpFloat fireAngle, bool isclient) {
        //If holding the left button
        if (!isclient || Lpressed) {
            if (fireTimer.exceededReset()) {
                if (isclient && mFiredNumber < MAX_FIREANGLES) {
                    mFiredAngle[ mFiredNumber++ ] = cpBodyGetAngle(body());
                }
                for( int i=0; i<mMaxAmmo; i++) {
                    if ( !mAmmo[i].checkExist()) {
                        mAmmo[i].createBullet(r, space, 700, this, !isclient);
                        break;
                    }
                }
            }
        }
    }

    void Player::rotLeft() {
        cpFloat angle = cpBodyGetAngle(mEntity->body());
        angle = angleAdd(angle, -PLAYER_RAD);
        cpBodySetAngle(mEntity->body(), angle);
        //printf("%f %f\n", mAngle, mEntity->body()->a);
    }

    void Player::rotRight() {
        cpFloat angle = cpBodyGetAngle(mEntity->body());
        angle = angleAdd(angle, PLAYER_RAD);
        cpBodySetAngle(mEntity->body(), angle);
         //printf("%f %f\n", mAngle, mEntity->body()->a);
    }

    void Player::renderBullets(SDL_Renderer * r) {
        for(int i=0; i<mMaxAmmo; i++)
        {
            if( mAmmo[i].checkExist() )
                mAmmo[i].render(r);
        }
    }

    //!!!WARNING: Code in this method should not depend on how many times this method is run
    void Player::updateState() {
        //states
        if (mHpLossTimer.exceededReset()) {
            hurt(mInCloud);
        }

        //Apply impulse
        cpVect totalVel = cpvadd(mVel,mVectp);
        if (cpvlength(totalVel) != 0) {
            cpBodyApplyImpulseAtWorldPoint(mEntity->body(), totalVel, cpv(0, 0));
            mVel = cpvzero;
            mVectp = cpvzero;
        }

        mVectp = cpvzero;
        cpBody * body = mEntity->body();
        for(int i=0; i<mMaxAmmo; i++)
            if( mAmmo[i].checkExist() )
                mAmmo[i].move();

        //Move around screen
        if( body->p.x > SCREEN_WIDTH + mEntity->width()/2 )
            body->p.x = -mEntity->width();
        if( body->p.x < -mEntity->width() )
            body->p.x = SCREEN_WIDTH + mEntity->width()/2;
        if( body->p.y > SCREEN_HEIGHT + mEntity->height()/2)
            body->p.y = -mEntity->height();
        if( body->p.y < -mEntity->height() )
            body->p.y = SCREEN_HEIGHT + mEntity->height()/2;
    }

    void Player::hurt(int dam){
        mHp -= dam;
        if (mHp < 0) {
            mHp = 0;
        }
    }

    void Player::setInCloud(int num) {
        mInCloud += num;
    }
}
