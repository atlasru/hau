#!/usr/bin/env bash
set -euo pipefail
root_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
deps_dir="${PARLEY_DEPENDENCIES:-$root_dir/_build-deps}"
parallel_jobs="${PARLEY_BUILD_JOBS:-4}"
mkdir -p "$deps_dir"
if [[ ! -d "$deps_dir/c-toxcore/.git" ]]; then
  git clone --depth 1 --branch v0.2.23 --recurse-submodules --shallow-submodules \
    https://github.com/TokTok/c-toxcore.git "$deps_dir/c-toxcore"
fi
if [[ "$(git -C "$deps_dir/c-toxcore" describe --tags --exact-match)" != v0.2.23 ]]; then
  echo 'Dependency checkout must be exactly c-toxcore v0.2.23' >&2
  exit 1
fi
cmake -S "$deps_dir/c-toxcore" -B "$deps_dir/toxcore-build" -G Ninja \
  -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$deps_dir/prefix" \
  -DUNITTEST=OFF -DAUTOTEST=OFF -DDHT_BOOTSTRAP=OFF -DBOOTSTRAP_DAEMON=OFF \
  -DBUILD_TOXAV=ON -DENABLE_SHARED=OFF -DENABLE_STATIC=ON
cmake --build "$deps_dir/toxcore-build" --parallel "$parallel_jobs"
cmake --install "$deps_dir/toxcore-build"
export PKG_CONFIG_PATH="$deps_dir/prefix/lib/pkgconfig${PKG_CONFIG_PATH:+:$PKG_CONFIG_PATH}"
cmake -S "$root_dir" -B "$root_dir/build" -G Ninja \
  -DCMAKE_BUILD_TYPE=Release -DUPDATE_CHECK=OFF
cmake --build "$root_dir/build" --parallel "$parallel_jobs"
ctest --test-dir "$root_dir/build" --output-on-failure --parallel 2
