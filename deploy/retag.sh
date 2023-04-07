#!/bin/sh

git tag -d latest
git push --delete origin latest
git tag latest
git push origin latest
