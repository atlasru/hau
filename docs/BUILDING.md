# Building Parley desktop

## Linux x86_64 (Ubuntu 24.04)

C++20, CMake 3.16+, Ninja, Qt 6.4+ (Core/Gui/Widgets/Network/Svg/Xml/Concurrent/
Test/Linguist), FFmpeg, SQLCipher, libsodium, OpenAL, Opus, VPX, QRencode, EXIF.
Optional: KF6 Sonnet for spelling; X11/Xss for idle detection; libunwind.

```sh
sudo apt-get update
sudo apt-get install -y build-essential cmake ninja-build git pkg-config \
  qt6-base-dev qt6-tools-dev qt6-tools-dev-tools qt6-l10n-tools libqt6svg6-dev \
  libavcodec-dev libavdevice-dev libavformat-dev libavutil-dev libswscale-dev \
  libexif-dev libqrencode-dev libsqlcipher-dev libsodium-dev libopus-dev \
  libvpx-dev libopenal-dev libv4l-dev libxss-dev libunwind-dev
./scripts/build-linux.sh
./build/Parley
```

The script builds pinned c-toxcore v0.2.23 (with AV enabled), including the pinned
`third_party/cmp` submodule. Existing compatible `toxcore.pc` installations can
also be selected with `PKG_CONFIG_PATH` and a direct CMake build. SQLCipher must
be enabled; do not substitute a plaintext SQLite library for encrypted history.

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DUPDATE_CHECK=OFF
cmake --build build --parallel 4
ctest --test-dir build --output-on-failure --parallel 2
```

Tests use the offscreen Qt backend. Set `PARLEY_PUBLIC_NETWORK_TESTS=1` to
opt into the external bootstrap endpoint test; its default is an explicit skip. Test `transport` uses real localhost UDP,
not public bootstrap nodes; sandbox network restrictions may prevent it from
running. Hardware AV is explicitly outside the headless test claim.

## Windows x64

Use MSYS2 UCRT64/MinGW, Qt6 and matching MSYS2 dependencies. Do not mix MSVC Qt,
MinGW Qt, static vcpkg imports and MSYS2 libraries in one link. The new CI uses
MSYS2, CMake and windeployqt and bundles required non-system DLLs. The old HAU
MSVC/vcpkg foundation build is retained only under `legacy/foundation/`.

Runtime output: `build/Parley.exe`. A Windows artifact is only considered built
when the Windows job has completed successfully; a Linux binary is not an EXE.

## Source and licensing

The source snapshot pins TokTok/qTox at
`592c2d3506487c42ffee13e95d67f6efcee2394e`. Root `LICENSE` and upstream per-file
notices apply. Ship corresponding source and these build scripts with binaries.
No proprietary-only redistribution. Internal upstream CMake target is `qtox`;
its output name is `Parley`. `legacy/foundation/` is not linked into that target.
