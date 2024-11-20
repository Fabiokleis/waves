with import <nixpkgs> {};

stdenv.mkDerivation {
  name = "dev-environment";
  buildInputs = [ pkg-config cmake clang libxkbcommon libffi xorg.libX11 xorg.libXrandr xorg.libXinerama xorg.libXcursor xorg.libXi libGL libGLU glfw wayland ];
  shellHook = ''
    export CC="clang";
    export CXX="clang++";
  '';

}
