with import <nixpkgs> {};

stdenv.mkDerivation {
  name = "dev-environment";
  buildInputs = [
    pkg-config
    cmake
    clang
    libxkbcommon
    libffi
    xorg.libX11
    xorg.libXrandr
    xorg.libXinerama
    xorg.libXcursor
    xorg.libXi
    libGL
    libGLU
    glfw
    libinput
    wayland
    xwayland
    xorg.xcbproto
    xorg.xcbutil
    libglvnd
    libseat
    pixman
    wayland-protocols
    glfw-wayland
    binutils
    libdrm
    mesa
    ninja
  ];
  shellHook = ''
    export CC="clang";
    export CXX="clang++";
  '';

}
