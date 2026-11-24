#!/bin/bash



for file in /Users/rantisi/projects/jrlib/resources/*; do 
    if [ -f "$file" ]; then
        /bin/cp -p "$file" /Users/rantisi/projects/jvmc/resources/
    fi 
done
