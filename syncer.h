#ifndef SYNCER_H_INCLUDED
#define SYNCER_H_INCLUDED
#include <thread>
#include "client.h"
#include "utils.h"
#include "player.h"
#include "proto/player.pb.h"
#include "physics.h"

namespace xx {
    class Syncer {
        Client *mClient;
        Player *mPlayer;
        Player *mOtherPlayer;

        uint32 mLastRecvUpdate;
    public:
        Syncer(Client *, Player *, Player *);
        ~Syncer();
        void stop();
        void updateBodies(Physics * physics, Update & update, bool updated);
        void playerHostSync(bool *stopped, Update & update, bool & updated);
        void playerSendUpdate(bool *stopped, cpVect * mvVect);
    };
}

#endif // SYNCER_H_INCLUDED
