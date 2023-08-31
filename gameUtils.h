#ifndef gameUtils
#define gameUtils

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <time.h>
#include <vector>


class makeRectangle{

public:

	SDL_Rect rect;
		
	int red, green, blue = 0;
	
	int alpha = 255;
	
	SDL_Renderer* renderer;
	
	SDL_Window* window;
	
	SDL_Surface* spriteSurface = nullptr;
	
	SDL_Texture* spriteTexture = nullptr;
	
	SDL_Rect spriteRect;
	
	SDL_Rect spriteSheetLocation;
	
	bool playAnimation = false;
	
	std::vector<SDL_Rect> animationList;
	
	makeRectangle(int x, int y, int w, int h, SDL_Renderer* r, SDL_Window* win){
	
		renderer = r;
	
		window = win;
	
		rect.x = x;
	
		rect.y = y;
	
		rect.w = w;
	
		rect.h = h;
		
	}
	
	void setTexture(std::string path, SDL_Rect sheetLocation){
	
		spriteSheetLocation = sheetLocation;
	
		spriteSurface = IMG_Load(path.c_str());
		
		//spriteSurface = SDL_ConvertSurfaceFormat(spriteSurface, SDL_GetWindowPixelFormat(window), 0);
		
		spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
		
		SDL_FreeSurface(spriteSurface);
		
		spriteRect = {rect.x, rect.y, rect.w, rect.h};
		
	}
	
	void addAnimationFrame(SDL_Rect frame){
	
		animationList.push_back(frame);
	
	}
	
	void setColor(int r, int g, int b, int a){
	
		red = r;
		
		green = g;
		
		blue = b;
		
		alpha = a;
	
	}
	
	void draw(int animationFrame){
	
		if (spriteTexture != nullptr){
		
			spriteRect = rect;
		
			if (animationList.size() < 2 || !playAnimation) {
			
				SDL_RenderCopy(renderer, spriteTexture, &spriteSheetLocation, &spriteRect);
			
			} else{
			
				int adjustedAnimationFrame = (animationFrame / 200) % animationList.size();
			
				SDL_RenderCopy(renderer, spriteTexture, &animationList[adjustedAnimationFrame], &spriteRect);
			
			}
		
		} else{
		
			SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);

			SDL_RenderFillRect(renderer, &rect);
		
		}
	
	}
	
	bool checkCollision(SDL_Rect rectA, SDL_Rect rectB){
	
		bool isCollision = (rectA.x < rectB.x + rectB.w &&
				rectA.x + rectA.w > rectB.x &&
				rectA.y < rectB.y + rectB.h &&
				rectA.y + rectA.h > rectB.y);
				
		return isCollision;
	
	}
	
	bool checkCollisionWithMouse(int x, int y){
	
		bool isCollision = (rect.x < x &&
							rect.x + rect.w > x &&
							rect.y < y &&
							rect.y + rect.h > y
		);
	
		if (isCollision){
		
			SDL_Log("Collision");
		
		}
	
		return isCollision;
	
	}
	
	void cleanUp(){
	
		SDL_DestroyTexture(spriteTexture);
	
	}
	
};

class makeText{

public:

	SDL_Texture* textTexture = nullptr;
	
	SDL_Surface* textSurface = nullptr;
	
	SDL_Renderer* renderer;

	SDL_Color textColor;
	
	SDL_Rect textRect;
	
	TTF_Font* font;

	int xPosition, yPosition, width, height;

	makeText(int x, int y, int w, int h, std::string text, SDL_Color color, TTF_Font* newFont, SDL_Renderer* r){
	
		xPosition = x; yPosition = y; width = w; height = h;
		
		renderer = r;
		
		font = newFont;
		
		textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
	
		SDL_Log(TTF_GetError());
		
		textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		
		SDL_FreeSurface(textSurface);
		
		textRect = {x, y, w, h};
		
		if (textSurface == nullptr){
		
			SDL_Log(TTF_GetError());
		
		}
		
	}
	
	void updateText(std::string newText) {
	
		textSurface = TTF_RenderText_Blended(font, newText.c_str(), textColor);
	
		textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		
		SDL_FreeSurface(textSurface);
	
	}
	
	void draw(){
	
		SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
	
	}
	
	void cleanUp(){
	
		SDL_DestroyTexture(textTexture);
	
	}

};

class sdlHolder{

public:

	SDL_Window* window = nullptr;

	SDL_Renderer* renderer = nullptr;

	std::vector<makeRectangle*> drawList;

	const int FPS = 60;
	
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart, frameTime;
	
	Uint32 lastFrameTime;
	
	TTF_Font* font;
	
	sdlHolder(int xResolution, int yResolution, std::string title){
	
		srand(time(0));

		TTF_Init();
		
		font = TTF_OpenFont("Arial.ttf", 56);
		
		IMG_Init(IMG_INIT_PNG);
	
		if (SDL_Init(SDL_INIT_VIDEO) != 0){

			SDL_Log("SDL initialization failed");
	
		}
	
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, xResolution, yResolution, SDL_WINDOW_SHOWN);

		if (!window){
	
			SDL_Log("Window Creation Failed");
	
			SDL_Quit();
	
			exit(0);
	
		}

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	
	}
	
	void updateScreen(int animationFrame){
	
		int totalDrawList = drawList.size();
		
		for(int i = 0; i<totalDrawList; i++){
		
			drawList[i]->draw(animationFrame);
		
		}
		
		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

		SDL_RenderPresent(renderer);

	
	}
	
	void cleanUp(){
	
		TTF_CloseFont(font);

		SDL_DestroyRenderer(renderer);

		SDL_DestroyWindow(window);
	
		SDL_Quit();
	
	}

};

#endif
