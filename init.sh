#!/bin/bash

OLDDIR="$(pwd)"
DIR="$( cd "$( dirname "$0" )" && pwd )"

echo "Entering $DIR"
cd $DIR

echo "Checking for update"
git remote update

if [ "$(git status -uno)" ]
then
    echo "Update found, pulling changes"
    git pull origin master

    echo "Making"
    make
fi

echo "Starting Driver Test"
bin/drivertest

echo "Leaving $DIR"
cd $OLDDIR
