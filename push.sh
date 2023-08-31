#!/bin/bash

if [-z "$1"]; then

    echo "Usage: $0 <commit_message>"
    exit 1

fi

git remote add origin https://github.com/Garfiend/redesigned-fishstick.git

git branch -M main

git add .

git commit -m "$1"

git push origin main
