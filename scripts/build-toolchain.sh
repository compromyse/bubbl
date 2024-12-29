#!/usr/bin/env bash

set -xe

tar cf - toolchain/host toolchain/cross | pigz > toolchain.tar.gz
