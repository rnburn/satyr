#!/bin/bash

set -e

export SATYR_SRC_DIR=/src
export SATYR_CI_DIR=${SATYR_SRC_DIR}/ci

cd "${SATYR_CI_DIR}"

export BAZEL_OPTIONS="--package_path=%workspace%:${SATYR_SRC_DIR}"

case "$1" in
  bazel.debug)
    echo "Testing..."
    bazel test ${BAZEL_OPTIONS} -c dbg //...
    exit 0
    ;;
  *)
    echo $"Usage: $0 {bazel.debug}"
    exit 1
esac
