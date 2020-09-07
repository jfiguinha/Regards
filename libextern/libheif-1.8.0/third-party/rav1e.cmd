: # This install script was originally taken from libavif but might have been modified.

: # The odd choice of comment style in this file is to try to share this script between *nix and win32.

: # cargo must be in your PATH. (use rustup or brew to install)

: # If you're running this on Windows targeting Rust's windows-msvc, be sure you've already run this (from your VC2017 install dir):
: #     "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvars64.bat"
: #
: # Also, the error that "The target windows-msvc is not supported yet" can safely be ignored provided that rav1e/target/release
: # contains rav1e.h and rav1e.lib.

git clone -b v0.3.3 --depth 1 https://github.com/xiph/rav1e.git

cd rav1e
cargo install cbindgen
cbindgen -c cbindgen.toml -l C -o target/release/include/rav1e/rav1e.h --crate rav1e .

cargo build --lib --release --features capi
cd ..
