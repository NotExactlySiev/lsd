{
  description = "Minimal PSX Development Environment";

  # TODO: add my super awesome Nix build of PCSX-Redux

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }:
  let
    lib = nixpkgs.lib;
    forAllSystems = lib.genAttrs lib.systems.flakeExposed;
  in
    {
      devShells = forAllSystems (system:
      let
        pkgs = import nixpkgs {
          localSystem = { inherit system; };
        };
        crossPkgs = import nixpkgs {
          localSystem = { inherit system; };
          crossSystem = {
            system = "mipsel-linux";
            libc = "musl";
          };
        };
      in
      with pkgs; {
        default = mkShell {
          packages = [
            #crossPkgs.stdenv.cc.cc
            #(crossPkgs.stdenvNoLibs.cc.cc.override { langCC = true; })
            #crossPkgs.stdenvNoLibs.cc.bintools.bintools
          ];

          buildInputs = [
            SDL2
            openal
          ];

          nativeBuildInputs = [
            meson
            ninja
            cmake
            pkg-config
          ];
        };
      });
    };
}
