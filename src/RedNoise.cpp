#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>	

#define WIDTH 320
#define HEIGHT 240



std::vector<float> interpolateSingleFloats(float start, float end, int steps) {
	std::vector<float> result;
	float step = (end - start) / float(steps-1);
	for (int i = 0; i < steps; i++) {
		result.push_back(start + i * step);
	}
	return result;
}

std::vector<glm::vec3> interpolateThreeElementValues(glm::vec3 start, glm::vec3 end, int steps) {
	std::vector<glm::vec3> result;
	glm::vec3 step = (end - start) / float(steps-1);
	for (int i = 0; i < steps; i++) {
		result.push_back(start + float(i) * step);
	}
	return result;
}


void drawgray(DrawingWindow &window) {
	window.clearPixels();
	std::vector<float> grayscale = interpolateSingleFloats(255,0,WIDTH);
	for (size_t y = 0; y < window.height; y++) {
		for (size_t x = 0; x < window.width; x++) {
			float red = grayscale[x];
			float green = grayscale[x];
			float blue = grayscale[x];
			uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
			window.setPixelColour(x, y, colour);
		}
	}
}

void drawcolor(DrawingWindow &window) {
	window.clearPixels();
	glm::vec3 topleft(255,0,0);
	glm::vec3 topright(0,255,0);
	glm::vec3 bottomleft(0,0,255);
	glm::vec3 bottomright(255,255,0);

	std::vector<glm::vec3> firstCol = interpolateThreeElementValues(topleft,bottomleft,HEIGHT);
	std::vector<glm::vec3> lastCol = interpolateThreeElementValues(topright,bottomright,HEIGHT);

	for (int y = 0; y < HEIGHT; y++){
		std::vector<glm::vec3> row = interpolateThreeElementValues(firstCol[y],lastCol[y],WIDTH);
		for(int x = 0; x < WIDTH; x++){
			float red = row[x].x;
			float green = row[x].y;
			float blue = row[x].z;
			uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
			window.setPixelColour(x, y, colour);
		}
	}
 

}

void handleEvent(SDL_Event event, DrawingWindow &window) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
	} else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
}

int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
	while (true) {
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		drawcolor(window);
		window.renderFrame();
	}
}
