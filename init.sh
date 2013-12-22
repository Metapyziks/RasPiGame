#!/bin/bash

OLDDIR="$(pwd)"
DIR="$( cd "$( dirname "$0" )" && pwd )"

echo "Entering $DIR"
cd $DIR

echo "Checking for update"
git remote update

success=true

if [ "$(git status -uno | grep behind)" ]; then
    echo "Update found, pulling changes"
    git pull origin master

    echo "Making"
    make

    if [ $? -ne 0 ]; then
        success=false
    fi
else
    echo "No update required"
fi

if [ success ]; then
    echo "Starting game"
    bin/main
else
    echo "Build failed, aborting"
fi

echo "Leaving $DIR"
cd $OLDDIR
