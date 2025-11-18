#ifndef GET_CATALOG_MESSAGE_H
#define GET_CATALOG_MESSAGE_H

#include "clienthandlermessage.h"

class GetCatalogMessage: public ClientHandlerMessage {
public:
    explicit GetCatalogMessage(int client_id):
            ClientHandlerMessage(client_id, MsgType::GET_CAR_CATALOG) {}
};

#endif
