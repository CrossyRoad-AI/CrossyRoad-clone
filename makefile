CC = g++

SOURCES = $(wildcard src/*.cpp src/*/*.cpp src/*/*/*.cpp src/*/*/*/*.cpp src/*/*/*/*/*.cpp)
OBJS = ${SOURCES:.cpp=.o}

all: build run

build: ${OBJS}
	@xcopy .\assets .\dist /e /c /i /y > NUL
	@${CC} -o dist\game.exe ${OBJS} -Ldist -lglfw3 -lglew32 -lfreetype -lopengl32

%.o: %.cpp
	@${CC} -c $< -o $@ -Ilib\glfw\include -Ilib\glew\include -Ilib\glm\include -Ilib\freetype

run: dist\game.exe
	@cd dist && .\game.exe

runLeak: dist\game.exe
	drmemory.exe -- .\dist\game.exe

clean:
	@del ${OBJS}