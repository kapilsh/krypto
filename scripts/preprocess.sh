#!/usr/bin/env bash

#!/usr/bin/env bash

# GENERATE FLATBUFFERS SOURCES

BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m' # No Color

SCRIPT_PATH=$(realpath $0)
SCRIPT_DIR=$(dirname ${SCRIPT_PATH})

KRYPTO_DIR=$(realpath ${SCRIPT_DIR}/..)

GCC_VERSION=8.2
FLAT_BUFFERS_VERSION=1.9.0
FLAT_BUFFERS_BINARY_PACKAGE=flatbuffers/${FLAT_BUFFERS_VERSION}@kapilsh/release

echo -e "${BLUE}KRYPTO_DIR: ${KRYPTO_DIR}${NC}"

echo -e "${BLUE}"==================="${NC}"
echo -e "${BLUE}"--- FLATBUFFERS ---"${NC}"
echo -e "${BLUE}"==================="${NC}"

echo -e "${BLUE}FLATBUFFERS CONAN PACKAGE: ${FLAT_BUFFERS_BINARY_PACKAGE}${NC}"

export PATH=${KRYPTO_DIR}/resources/bin/:${PATH}

cd ${KRYPTO_DIR}/resources

mkdir build && cd build
conan install ${FLAT_BUFFERS_BINARY_PACKAGE} -g virtualenv
source activate.sh
cd ..

echo -e "${BLUE}"Using $(flatc --version)"${NC}"

echo -e "${BLUE}"--- COMPILE C++ FLATBUFFERS SOURCES ---"${NC}"
flatc --cpp -o ${KRYPTO_DIR}/include/krypto/serialization serialization.fbs

rm -r build