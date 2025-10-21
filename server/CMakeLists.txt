file(GLOB EVENTS_SRC "events/*.h")

target_sources(taller_server
    PRIVATE
        main.cpp
        client_handler.cpp
        acceptor.cpp
        receiver.cpp
        sender.cpp
        gameloop.cpp
        gameSession.cpp
        monitorclients.cpp
        monitorGames.cpp
    PUBLIC
        sender.h
        receiver.h
        monitorclients.h
        client_handler.h
        acceptor.h
        gameloop.h
        gameSession.h
        monitorGames.h
        ${EVENTS_SRC}
    )
