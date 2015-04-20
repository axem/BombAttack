CC=g++ 
LFLAGS=-lSDL -lGL -lGLU -lSDL_mixer -lSDL_image  $(shell sdl-config --libs)
CFLAGS=-mtune=i686 -o2 -Wall -pedantic $(shell sdl-config --cflags) 
OBJ1=bombattack.o bomb.o bombscounter.o congratz.o counter.o 
OBJ2=crazycounter.o credits.o enemy.o events.o 
OBJ3=explosion.o extended_collision_base.o 
OBJ4=field.o game.o gameover.o globals.o hint.o 
OBJ5=instructions.o levelcounter.o level_loader.o 
OBJ6=lifecounter.o man.o menu.o movingobject.o myfield.o
OBJ7=object.o rangecounter.o sound.o space.o texture.o logo.o
OBJS=$(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6) $(OBJ7)

bombattack: $(OBJS)
	$(CC) $(LFLAGS) $^ -o $@

$(OBJS): %.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean: 
	rm *.o bombattack *~

