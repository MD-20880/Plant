#define SDL_MAIN_HANDLED
#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>	
#include <array>

#define WIDTH 320
#define HEIGHT 240

int BRUSHMODE = 0;
std::array<std::array<int,HEIGHT>, WIDTH> image;

std::array<std::array<int,HEIGHT>, WIDTH> emptyWorld() {
	std::array<std::array<int,HEIGHT>, WIDTH> image;
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {
			image[x][y] = 0;
		}
	}
	return image;
}



enum CellType
{
	WATER = 0,
	PLANT = 1,
	SOIL = 2,
	TAN = 3
};

uint32_t fromRGB(uint8_t r, uint8_t g, uint8_t b) {
	return (r << 16) | (g << 8) | b;
}

std::array<std::array<int,HEIGHT>, WIDTH> readPgm(std::string filename) {
	std::ifstream file(filename, std::ios::binary);
	std::string line;
	std::getline(file, line);
	if (line != "P5") {
		std::cout << "Not a PGM file" << std::endl;
		return std::array<std::array<int,HEIGHT>, WIDTH>();
	}
	int width, height, max;
	file >> width >> height >> max;
	std::cout << "Reading " << filename << " " << width << "x" << height << "-" << max  << std::endl;
	std::getline(file, line);
	std::array<std::array<int,HEIGHT>, WIDTH> image;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			unsigned char value;
			file.read((char *) &value, sizeof(value));
			image[x][y] = (int) value;
		}
	}
	return image;
}


uint32_t checkType(int value){
	//water
	if (value == 0){
		return fromRGB(0, 0, 255);
	}
	//plant
	else if (value == 1){
		return fromRGB(0, 255, 0);
	}
	//soil
	else if (value == 2){
		return fromRGB(255,248,220);
	}
	//else .     just put a tan colour here for now
	else if (value == 3){
		return fromRGB(210,180,140);
	}
	else{
		return fromRGB(0, 0, 0);
	}
}


void draw(int x, int y, std::array<std::array<int,HEIGHT>, WIDTH> &image, int brushMode, float radius) {
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			if (i < 0 || i >= WIDTH || j < 0 || j >= HEIGHT) continue;
			
			float dist = sqrt(pow(i - x, 2) + pow(j - y, 2));
			if (dist < radius) {
				if (brushMode == 1) {
					image[i][j] = 0;
				}
				else if (brushMode == 2) {
					image[i][j] = 1;
				}
				else if (brushMode == 3) {
					image[i][j] = 2;
				}
			}
		}
	}
	
}


void renderFrame(DrawingWindow &window, std::array<std::array<int,HEIGHT>, WIDTH> image) {
	for(int x = 0; x < WIDTH; x++) {
		for(int y = 0; y < HEIGHT; y++) {
			int value = image[x][y];
			uint32_t colour = checkType(value);
			window.setPixelColour(x, y, colour);
		}
	}
}

void handleEvent(SDL_Event event, DrawingWindow &window) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		if (event.key.keysym.sym == SDLK_1) BRUSHMODE = 1;
		if (event.key.keysym.sym == SDLK_2) BRUSHMODE = 2;
		if (event.key.keysym.sym == SDLK_3) BRUSHMODE = 3;

	} else if (event.type == SDL_MOUSEBUTTONDOWN) {
		std::cout << "Pressed" << std::endl;
		while (event.type != SDL_MOUSEBUTTONUP)
		{
			
			int x = event.motion.x;
			int y = event.motion.y;
			draw(x, y, image, BRUSHMODE, 10);
			window.renderFrame();
			if (window.pollForInputEvents(event)) handleEvent(event, window);
		}
		std::cout << "Released" << std::endl;

	}
}

int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
	image = readPgm("x.pgm");
	if (image.empty()) return 0;
	while (true) {
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		renderFrame(window,image);
		window.renderFrame();
	}
}
