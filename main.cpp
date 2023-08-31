#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "gameUtils.h"
#include "primitiveUtils.h"
#include <time.h>
#include <vector>
#include <string>
#include "game.h"

int main(){

	sdlHolder SDLHOLDER(800, 600, "SDL Tester");
	
	makeText score(0, 0, 100, 100, "0", {0, 0, 0, 255}, SDLHOLDER.font, SDLHOLDER.renderer);
	
	makeText gameOver(400, 300, 300, 100, "Game Over", {0, 0, 0}, SDLHOLDER.font, SDLHOLDER.renderer);
	
	makeRectangle dinosaur(50, 50, 50, 50, SDLHOLDER.renderer, SDLHOLDER.window);
	
 	dinosaur.setTexture("./dinoSpritesheet.png", {41, 4, 42, 45});
	
 	dinosaur.setColor(200, 0, 0, 0);
	
 	dinosaur.addAnimationFrame({936, 2, 44, 47}); //Cycle 1

 	dinosaur.addAnimationFrame({980, 2, 44, 47}); //Cycle 2

 	dinosaur.playAnimation = true;

	SDLHOLDER.drawList.push_back(&dinosaur);

	//Define the two ground objects

	makeRectangle ground1(0, 500, 800, 14, SDLHOLDER.renderer, SDLHOLDER.window);

	ground1.setColor(255, 255, 255, 255);

	ground1.setTexture("./dinoSpritesheet.png", {3, 54, 1190, 14});

	SDLHOLDER.drawList.push_back(&ground1);

	makeRectangle ground2(800, 500, 800, 14, SDLHOLDER.renderer, SDLHOLDER.window);

	ground2.setColor(255, 255, 255, 255);

	ground2.setTexture("./dinoSpritesheet.png", {3, 54, 1190, 14});

	SDLHOLDER.drawList.push_back(&ground2);

	//Define the obstacle

	makeRectangle obstacle(400, 425, 50, 75, SDLHOLDER.renderer, SDLHOLDER.window);
	
	obstacle.setColor(0, 0, 0, 255);

	obstacle.setTexture("./dinoSpritesheet.png", {332, 2, 25, 48});

	obstacle.addAnimationFrame({181, 3, 44, 29}); //Cycle 1

	obstacle.addAnimationFrame({134, 9, 47, 33}); //Cycle 2

	SDLHOLDER.drawList.push_back(&obstacle);
	
	//Set up the game

	SDL_Event event;
	
	gameHolder GAMEHOLDER;

	//int quit = 0;

	float acceleration = 0;
	
	//float GAMEHOLDER.gravityMultiplier = 1;

	bool canMove = false;

	//float GAMEHOLDER.totalFrames = 0;
	
	//int GAMEHOLDER.scoreCount = 0;
	
	//bool GAMEHOLDER.loss = false;

	while (!GAMEHOLDER.quit){
	
		SDLHOLDER.frameStart = SDL_GetTicks();
		
		if (!GAMEHOLDER.loss) {
		
			GAMEHOLDER.scoreCount = GAMEHOLDER.totalFrames;
		
			score.updateText(std::to_string(GAMEHOLDER.scoreCount));
			
		}
		
		GAMEHOLDER.totalFrames++;
	
		float obstacleMoveSpeed = (5 + clamp(GAMEHOLDER.totalFrames / 500, 0, 10));
	
		if(canMove and !GAMEHOLDER.loss){
		
			obstacle.rect.x -= obstacleMoveSpeed;
			
			ground1.rect.x -= static_cast<int>(obstacleMoveSpeed);
			
			ground2.rect.x -= static_cast<int>(obstacleMoveSpeed);
			
			if (ground1.rect.x <= -800){
			
				ground1.rect.x = 0;
			
			}
			
			if (ground2.rect.x <= 0){
			
				ground2.rect.x = 800;
			
			}
			
			if (obstacle.rect.x < 0) {
			
				obstacle.rect.x = 800;
			
				int obstacleType = random(1, 5);
			
				if (obstacleType == 1){ //Bird 1 air
				
					obstacle.rect.y = 300;
					
					obstacle.rect.w = 75;
					
					obstacle.rect.h = 40;
					
					obstacle.playAnimation = true;
				
				}else if(obstacleType == 2){ //Bird 2 ground
				
					obstacle.rect.y = 425;
					
					obstacle.rect.w = 75;
					
					obstacle.rect.h = 40;
				
					obstacle.playAnimation = true;
				
				}else{ //Cactus
				
					obstacle.rect.y = 425;
					
					obstacle.rect.w = 50;
					
					obstacle.rect.h = 75;
				
					obstacle.playAnimation = false;
				
					obstacle.setTexture("./dinoSpritesheet.png", {332, 2, 25, 48});
				
				}
			}
		
		}
	
		if (dinosaur.rect.y < 450 && acceleration > -4){
		
			GAMEHOLDER.gravityMultiplier += 0.05;
		
			acceleration -= (6 * GAMEHOLDER.gravityMultiplier);
			
		}else if(dinosaur.rect.y > 450) {
		
			acceleration = 0;
			
			GAMEHOLDER.gravityMultiplier = 1;
			
		 	dinosaur.rect.y = 450;
			
			canMove = true;
		
		}
	
	 	dinosaur.rect.y -= acceleration;
		
		while (SDL_PollEvent(&event)){

			if (event.type == SDL_QUIT){

				GAMEHOLDER.quit = 1;

			} else if (event.type == SDL_KEYDOWN) {
			
				if (event.key.keysym.sym == SDLK_UP && dinosaur.rect.y >= 420){
				
					acceleration += 50;
					
				} 
				
			} else if (event.type == SDL_MOUSEBUTTONDOWN){
			
				if (event.button.button == SDL_BUTTON_LEFT){
				
					dinosaur.checkCollisionWithMouse(event.button.x, event.button.y);
				
					SDL_Log(std::to_string(alignGrid(event.button.x, GAMEHOLDER.gridSize)).c_str());
				
					if (GAMEHOLDER.loss){
				
						int mouseX = event.button.x;
						
						int mouseY = event.button.y;
						
						obstacle.rect.x = 800;
						
						GAMEHOLDER.totalFrames = 0;
						
						GAMEHOLDER.loss = false;
					
					} else {
					
						acceleration += 50;
					
					}
					
				}
			
			}

		}

		SDL_RenderClear(SDLHOLDER.renderer);

		score.draw();		
		
		if (GAMEHOLDER.loss) gameOver.draw();
		
		SDLHOLDER.updateScreen(SDLHOLDER.frameStart);
		
		bool isHit = dinosaur.checkCollision(dinosaur.rect, obstacle.rect);

		if (isHit) {
		
			GAMEHOLDER.loss = true;
		
		}

		SDLHOLDER.frameTime = SDL_GetTicks() - SDLHOLDER.frameStart;
		
		if (SDLHOLDER.frameTime < SDLHOLDER.frameDelay) {
		
			SDL_Delay(SDLHOLDER.frameDelay - SDLHOLDER.frameTime);
			
		}

	}
	
	SDLHOLDER.cleanUp();

	return 0;

}
