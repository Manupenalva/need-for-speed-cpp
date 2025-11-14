#ifndef SERVER_EVENTS_CHEATMESSAGE_H
#define SERVER_EVENTS_CHEATMESSAGE_H

#include "clienthandlermessage.h"
#include "../../common/msgType.h"
#include "../../common/cheatcodes.h"

class CheatMessage : public ClientHandlerMessage {

   public:
    CheatMessage(int client_id, const CheatCode cheat_code)
        : ClientHandlerMessage(client_id, MsgType::CHEAT_CODE), cheat_code(cheat_code) {}

    CheatCode get_cheat_code() const {
        return cheat_code;
    }

   private:
    CheatCode cheat_code;
};
#endif
