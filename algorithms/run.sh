if [ -z "$@" ]
then
	cmake --build build && ./build/algorithm/a.out
elif [ "$1" = "debug" ]
then
	cmake --build debug && ./debug/algorithm/a.out
elif [ "$1" = "build" ]
then
	cmake -S . -B build
	cmake -DCMAKE_BUILD_TYPE=Debug -S . -B debug
else
	echo "Unrecognized command: $1"
fi
