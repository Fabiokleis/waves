{ pkgs ? import <nixpkgs> {} }:

pkgs.stdenv.mkDerivation {
  name = "dev-environment";
  buildInputs = [
    pkgs.pkg-config
    pkgs.cmake
    pkgs.clang
    # Graphics/Window system dependencies
    pkgs.glfw
    pkgs.libGL
    pkgs.libglvnd
    pkgs.mesa
    # X11 dependencies
    pkgs.xorg.libX11
    pkgs.xorg.libXcursor
    pkgs.xorg.libXrandr
    pkgs.xorg.libXinerama
    pkgs.xorg.libXi
    pkgs.xorg.libxcb
    pkgs.xorg.libXxf86vm
    # Wayland dependencies
    pkgs.wayland
    pkgs.libxkbcommon  # Fix typo: "pkgs", not "pkgs"
    pkgs.wayland-protocols
    # Additional utilities
    pkgs.libinput
    pkgs.libseat
    pkgs.pixman
  ];

  MY_LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath [
    pkgs.glfw
    pkgs.xorg.libXcursor
    pkgs.xorg.libX11
  ];

  shellHook = ''
    # Set compiler
    export CC="clang";
    export CXX="clang++";
    
    # Combine all library paths from build inputs
    export LD_LIBRARY_PATH=$MY_LD_LIBRARY_PATH:$LD_LIBRARY_PATH
    
    # Optional verification
    echo "LD_LIBRARY_PATH set to: $LD_LIBRARY_PATH"
  '';
}
