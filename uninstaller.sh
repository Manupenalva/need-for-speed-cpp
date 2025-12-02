#!/bin/bash

NAME="NEED4SPEED"
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m'

set -e

echo -e "${YELLOW}Desinstalador de: ${NAME}${NC}"

echo -e "${YELLOW}Eliminando binarios en /usr/bin...${NC}"

for bin in "${NAME}-client" "${NAME}-server" "${NAME}-editor"; do
    if [ -f "/usr/bin/${bin}" ]; then
        echo "  Eliminando /usr/bin/${bin}"
        sudo rm -f "/usr/bin/${bin}"
    else
        echo "  /usr/bin/${bin} no existe, se omite."
    fi
done

BASE_VAR="/var/${NAME}"
if [ -d "${BASE_VAR}" ]; then
    echo -e "${YELLOW}Eliminando datos en ${BASE_VAR}...${NC}"
    sudo rm -rf "${BASE_VAR}"
else
    echo "${BASE_VAR} no existe, se omite."
fi

BASE_ETC="/etc/${NAME}"
if [ -d "${BASE_ETC}" ]; then
    echo -e "${YELLOW}Eliminando configuración en ${BASE_ETC}...${NC}"
    sudo rm -rf "${BASE_ETC}"
else
    echo "${BASE_ETC} no existe, se omite."
fi

echo -e "${GREEN}Desinstalación completada.${NC}"
