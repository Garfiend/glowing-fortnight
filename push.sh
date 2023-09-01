#!/bin/bash

git init

if [-z "$1"]; then

    echo "Usage: $0 <commit_message>"
    exit 1

fi



git add .

git commit -m "$1"

git branch -M main

git remote add origin https://github.com/Garfiend/glowing-fortnight.git

git push -u origin main
