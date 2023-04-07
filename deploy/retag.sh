#!/bin/sh

git tag -d latest
git push --delete origin latest
git tag latest
git push origin latest

firefox 'https://github.com/ilya-sotnikov/qUART/releases/new?tag=latest&title=Latest&body=Latest+release'
