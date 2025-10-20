file(GLOB EVENTS_SRC "events/*.h")

target_sources(taller_server
    PRIVATE
        main.cpp
        client_handler.cpp
        acceptor.cpp
        receiver.cpp
        sender.cpp
    PUBLIC
        sender.h
        receiver.h
        monitorclients.h
        client_handler.h
        acceptor.h
        ${EVENTS_SRC}
    )
