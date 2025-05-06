{
  description = "Minimal PSX Development Environment";

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
        /*
        crossPkgs = import nixpkgs {
          localSystem = { inherit system; };
          crossSystem = { system = "mipsel-none-elf"; };
        };
        */
        crossPkgs = pkgs.pkgsCross.mips-embedded;
      in
      {
        default = pkgs.mkShell {
          packages = [

            # crossPkgs.gccWithoutLibc
            # crossPkgs.buildPackages.gcc
            # crossPkgs.buildPackages.binutils-unwrapped
          ];

          buildInputs = [
            pkgs.SDL2
          ];
        };
      });
    };
}
