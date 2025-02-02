{
  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs, ... }: let
      pkgs = import nixpkgs { system = "x86_64-linux"; config.allowUnfree = true; };
    in {
      devShells.x86_64-linux.default = pkgs.mkShell {
        buildInputs = with pkgs; [
          clang.cc
          lld
          nasm
          cmake
          ninja

          grub2
          xorriso

          qemu_kvm
          clang-tools
          ccls
        ];
      };
    };
}
