#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <raylib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Metaballs"

#define BALL_COUNT 5

typedef struct {
	int x, y, r;
	int velX, velY;
} Ball;

int rand_range(int min, int max)
{
	return rand() % (max - min) + min;
}

int main(void)
{
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

	srand(69);
	Ball balls[BALL_COUNT] = {0};

	for (size_t i = 0; i < BALL_COUNT; i++)
	{
		balls[i] = (Ball){
			.x = rand_range(0, WINDOW_WIDTH),
			.y = rand_range(0, WINDOW_HEIGHT),
			.r = rand_range(50, 100),
			.velX = rand_range(1, 10),
			.velY = rand_range(1, 10),
		};

		printf("(x: %.2d, y: %.2d, r: %.2d)\n", balls[i].x, balls[i].y, balls[i].r);
	}

	Image i = {
		.width = WINDOW_WIDTH,
		.height = WINDOW_HEIGHT,
		.mipmaps = 1,
		.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
		.data = malloc(sizeof(int) * WINDOW_WIDTH * WINDOW_HEIGHT)
	};

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		for (size_t i = 0; i < BALL_COUNT; i++)
		{
			DrawCircle(balls[i].x, balls[i].y, 10, RED);
			DrawCircleLines(balls[i].x, balls[i].y, balls[i].r, RED);

			Ball *b = &balls[i];
			b->x += b->velX;
			b->y += b->velY;

			if (b->x > WINDOW_WIDTH || b->x < 0)
				b->velX *= -1;
			if (b->y > WINDOW_HEIGHT || b->y < 0)
				b->velY *= -1;
		}

		for (size_t y = 0; y < WINDOW_HEIGHT; y++)
		{
			for (size_t x = 0; x < WINDOW_WIDTH; x++)
			{
				float brightness = 0.0f;
				for (size_t ballIdx = 0; ballIdx < BALL_COUNT; ballIdx++)
				{
					Ball ball = balls[ballIdx];
					int xDst = ball.x - x; 
					int yDst = ball.y - y;
					float value = sqrtf(xDst*xDst + yDst*yDst);

					brightness += ball.r / value * 0.5;
				}

				size_t pixelIdx = y * WINDOW_WIDTH + x;
				void *pixelAddress = (int *)i.data + pixelIdx;

				brightness = brightness > 1.0 ? 1.0 : brightness;
				unsigned char grayscale = brightness  * 255;
				
				SetPixelColor(pixelAddress, (Color) {grayscale, grayscale, grayscale, 255}, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
			}
		}

		Texture t = LoadTextureFromImage(i);
		DrawTexture(t, 0, 0, WHITE);

		DrawFPS(0, 0);
		EndDrawing();

		UnloadTexture(t);
		((int *)i.data)[0] = 0XFFFF0000;
	}

	CloseWindow();
	return 0;
}