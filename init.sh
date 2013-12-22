#!/bin/bash

OLDDIR="$(pwd)"
DIR="$( cd "$( dirname "$0" )" && pwd )"

echo "Entering $DIR"
cd $DIR

echo "Checking for update"
git remote update

if [ "$(git status -suno)" ]
then
    echo "Update found, pulling changes"
    git pull origin master

    echo "Making"
    make
else
    echo "No update required"
fi

echo "Starting Driver Test"
bin/main

echo "Leaving $DIR"
cd $OLDDIR
