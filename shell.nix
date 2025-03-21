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
    xorg.libXxf86vm
    xorg.libxcb.dev
    xorg.libxcb
    xorg.xkbevd
    xorg.xcbutil
    xorg.xkbutils
    xorg.xcursorthemes
    xorg.xcursorgen
    xwayland
    gtk2
    gtk3
  ];
  shellHook = ''
    export CC="clang";
    export CXX="clang++";
  '';

}

