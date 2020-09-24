{ pkgs ? import <nixpkgs> { } }:

with pkgs;

mkShell {
  buildInputs = with pkgs; [
    gnumake
    valgrind
    gdb
    gcc
    pkg-config
    ccls
    clang
    cmake

    # Markdown
    pandoc
    nodePackages.prettier
  ];
}
