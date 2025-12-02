file(GLOB EVENTS_SRC "events/*.h")
file(GLOB MONITORS_SRC "monitors/*.h")
file(GLOB MONITORS_SRC_CPP "monitors/*.cpp")
file(GLOB COMM_THREADS_SRC "comm_threads/*.h")
file(GLOB COMM_THREADS_SRC_CPP "comm_threads/*.cpp")
file(GLOB RACE_LOGIC_SRC "race_logic/*.h")
file(GLOB RACE_LOGIC_SRC_CPP "race_logic/*.cpp")

target_sources(taller_server
    PRIVATE
        server_main.cpp
        configLoader.cpp
        gameSession.cpp
        lobby.cpp
        server.cpp
        ${MONITORS_SRC_CPP}
        ${COMM_THREADS_SRC_CPP}
        ${RACE_LOGIC_SRC_CPP}
    PUBLIC
        server.h
        configLoader.h
        gameSession.h
        lobby.h
        ${MONITORS_SRC}
        ${COMM_THREADS_SRC}
        ${EVENTS_SRC}
        ${RACE_LOGIC_SRC}
    )
