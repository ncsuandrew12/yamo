#!/bin/bash

set -e

mkdir -p bin

cd components

cd server

make BUILD_MODE=debug