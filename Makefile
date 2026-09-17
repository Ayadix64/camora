b:
	#mkdir build
	rm -rf ./build/*
	gcc ./src/*.c ./ticktackto/src/*.c ./glad/glad.c -lm -lglfw3 -o ./build/camora
run: b
	./build/camora
