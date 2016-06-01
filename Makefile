
all:
	gcc main.cpp core.cpp -o main -I/usr/local/include -lallegro_ttf -lallegro_font -lallegro_image -lallegro_primitives -L/usr/local/lib -lallegro
