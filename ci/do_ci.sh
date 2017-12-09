#!/bin/bash

set -e

export SATYR_SRC_DIR=/src
export SATYR_CI_DIR=${SATYR_SRC_DIR}/ci

cd "${SATYR_CI_DIR}"

export BAZEL_OPTIONS="\
  --package_path=%workspace%:${SATYR_SRC_DIR} \
  --define threading=tbb \
  --define with_blas=1 \
  --define with_lapack=1 \
"

case "$1" in
  bazel.debug)
    echo "Building..."
    bazel build ${BAZEL_OPTIONS} -c dbg //...
    echo "Testing..."
    bazel test ${BAZEL_OPTIONS} -c dbg //...
    exit 0
    ;;
  *)
    echo $"Usage: $0 {bazel.debug}"
    exit 1
esac
