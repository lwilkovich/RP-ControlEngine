#!/bin/sh
set -e

mkdir -p ./Source/Third-Party
wget https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp -O ./Source/Third-Party/json.hpp