#!/bin/bash

NAME="NEED4SPEED"
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
NC='\033[0m'

set -e 

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

echo -e "${YELLOW}Instalador de: ${NAME}${NC}"

echo -e "${YELLOW}Actualizando paquetes...${NC}"
sudo apt-get update

sudo apt-get install -y \
    build-essential \
    cmake \
    make \
    pkg-config \
    gcc \
    g++ \
    clang

sudo apt-get install -y \
    qt6-base-dev \
    qt6-multimedia-dev

sudo apt-get install -y \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-ttf-dev \
    libsdl2-mixer-dev \
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

echo -e "${YELLOW}Compilando proyecto...${NC}"
cd "${SCRIPT_DIR}"
make
echo -e "${GREEN}Compilación completada.${NC}"

echo -e "${RED}Verificando dependencias faltantes (ldd)...${NC}"
if [ -f "${SCRIPT_DIR}/build/taller_client" ]; then
    echo "taller_client:"
    ldd "${SCRIPT_DIR}/build/taller_client"  | grep "not found" || echo "  OK"
fi

if [ -f "${SCRIPT_DIR}/build/taller_server" ]; then
    echo "taller_server:"
    ldd "${SCRIPT_DIR}/build/taller_server" | grep "not found" || echo "  OK"
fi

if [ -f "${SCRIPT_DIR}/build/taller_editor" ]; then
    echo "taller_editor:"
    ldd "${SCRIPT_DIR}/build/taller_editor" | grep "not found" || echo "  OK"
fi

echo -e "${YELLOW}Ejecutando tests...${NC}"
./build/taller_tests 
echo -e "${GREEN}Tests ejecutados correctamente.${NC}"

echo -e "${YELLOW}Instalando binarios y recursos...${NC}"

BASE_VAR="/var/${NAME}"
BASE_ETC="/etc/${NAME}"
APP_DIR="${BASE_VAR}/app"

sudo mkdir -p "${BASE_VAR}"
sudo mkdir -p "${BASE_ETC}"
sudo rm -rf "${APP_DIR}"
sudo mkdir -p "${APP_DIR}"

sudo mkdir -p "${APP_DIR}/build"
sudo cp -r "${SCRIPT_DIR}/build/." "${APP_DIR}/build/"

sudo mkdir -p "${APP_DIR}/client"

sudo mkdir -p "${APP_DIR}/client/resources"
sudo cp -r "${SCRIPT_DIR}/client/resources/." "${APP_DIR}/client/resources/"

sudo mkdir -p "${APP_DIR}/client/config"
sudo cp -r "${SCRIPT_DIR}/client/config/." "${APP_DIR}/client/config/"

sudo mkdir -p "${APP_DIR}/server/assets"
sudo cp -r "${SCRIPT_DIR}/server/assets/." "${APP_DIR}/server/assets/"

sudo mkdir -p "${APP_DIR}/editor/imgs"
sudo cp -r "${SCRIPT_DIR}/editor/imgs/." "${APP_DIR}/editor/imgs/"

sudo mkdir -p "${BASE_ETC}/client"
sudo cp -r "${APP_DIR}/client/config/." "${BASE_ETC}/client/"

sudo mkdir -p "${BASE_ETC}/server/assets"
sudo cp -r "${APP_DIR}/server/assets/." "${BASE_ETC}/server/assets/"
sudo chown -R $USER:$USER "${APP_DIR}/server/assets/race_configs"


create_wrapper() {
    local target_name="$1"
    local bin_name="$2"

    sudo tee "/usr/bin/${target_name}" > /dev/null << EOF
#!/bin/bash
cd "${APP_DIR}/build"
exec ./$(printf '%q' "${bin_name}") "\$@"
EOF
    sudo chmod +x "/usr/bin/${target_name}"
}

[ -f "${APP_DIR}/build/taller_client" ] && create_wrapper "${NAME}-client" "taller_client"
[ -f "${APP_DIR}/build/taller_server" ] && create_wrapper "${NAME}-server" "taller_server"
[ -f "${APP_DIR}/build/taller_editor" ] && create_wrapper "${NAME}-editor" "taller_editor"

echo -e "${GREEN}Instalación completada.${NC}"
echo
echo "Binarios instalados:"
[ -f "/usr/bin/${NAME}-client" ] && echo "  /usr/bin/${NAME}-client"
[ -f "/usr/bin/${NAME}-server" ] && echo "  /usr/bin/${NAME}-server"
[ -f "/usr/bin/${NAME}-editor" ] && echo "  /usr/bin/${NAME}-editor"
echo "Podés ejecutar el juego con:"
echo "  ${NAME}-client"
echo "  ${NAME}-server"
echo "  ${NAME}-editor"
echo
echo "Gracias por Descargar Need For Speed. Esperamos que te guste"
