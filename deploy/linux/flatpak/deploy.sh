#!/bin/sh

repo="io.github.ilya_sotnikov"
name="qUART"
app="${repo}.${name}"

flatpak-builder ${name} --user --repo ${repo} --install-deps-from=flathub --force-clean --ccache ${app}.yml

flatpak build-bundle ${repo} ${name}-linux_x86-64.flatpak ${app} --runtime-repo=https://flathub.org/repo/flathub.flatpakrepo
