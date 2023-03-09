#!/bin/bash

read -p "Enter Commit message: " commit;
git add .;
git commit -m "$commit"
git push

if [ $? -eq 0 ]
then
    echo "Pushed Succefully";
else
    git reset --soft HEAD~1
    git pull
    if [ $? -eq 0 ]
    then
        git add .
        git commit -m "Pull Request"
        git push
    fi
fi