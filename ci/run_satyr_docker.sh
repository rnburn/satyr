#!/bin/bash

set -e

BUILD_IMAGE=satyr-build
docker image inspect "$BUILD_IMAGE" &> /dev/null || {
  docker build -t "$BUILD_IMAGE" ci
}

docker run -v "$PWD":/src -w /src --privileged -it "$BUILD_IMAGE" /bin/bash -l
