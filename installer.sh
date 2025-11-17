#!/bin/bash

NAME="NEED4SPEED"
GREEN='\033[0;32m'
NC='\033[0m'

set -e 

echo -e "Instalador del TP: ${NAME}"

sudo apt-get update

sudo apt-get install -y \
    build-essential \
    cmake \
    make \
    pkg-config \
    gcc \
    g++ 

sudo apt-get install -y \
    qtbase6-dev \
    qtmultimedia6-dev

sudo apt-get install -y \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-ttf-dev \
    libsdl2-mixer-dev

sudo apt-get install -y \
  libopus-dev \
  libopusfile-dev \
  libxmp-dev \
  libfluidsynth-dev \
  fluidsynth \
  libwavpack1 \
  libwavpack-dev \
  wavpack \
  libfreetype6-dev

sudo apt-get install -y libyaml-cpp-dev || true

sudo apt-get install -y libgtest-dev libgmock-dev || true

echo -e "${GREEN}Dependencias del sistema instaladas correctamente.${NC}"

make

echo -e "${GREEN}Compilacion completada.${NC}".

if [ -f ./taller_tests ]; then
    ./taller_tests
    echo -e "${GREEN}Tests ejecutados correctamete.${NC}"
else 
    echo -e "FALLO"
fi 


    


