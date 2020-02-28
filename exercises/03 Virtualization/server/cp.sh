#!/usr/bin/env sh
# Sketching out behavious for compiling and executing a file...

# using a low-effort activity to do transfer before execution
docker container run --rm --name something executor tail -f /dev/null >
docker container exec something python tst.py
docker container kill something

# even better, though...
# replace pwd with {cwd} from os
docker container run --rm -v "$(pwd)/src":/src executor /bin/sh -c "g++ /src/thing.cpp && ./a.out"
