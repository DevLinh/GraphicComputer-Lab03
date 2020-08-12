#include <stdio.h>
#include <SDL2/SDL.h>
#ifdef _MSC_VER // tham số liên kết với Visual C++
# pragma comment (lib, "sdl2main.lib")
# pragma comment (lib, "sdl2.lib")
# pragma comment (linker, "/entry:\"mainCRTStartup\"" )
# pragma comment (linker, "/subsystem:WINDOWS")
#endif

#define WIDTH 320 //chieu rong cua so
#define HEIGHT 240 //chieu cao cua so
#define BLOCK_SIZE 16 //kich thuoc hinh vuong

SDL_Window* wnd; //con tro cua so SDL
SDL_Renderer* rend; //con tro doi tuong dung hinh
int x = WIDTH / 2, vx = 2; //troa do va van toc di chuyen
int y = HEIGHT / 2, vy = 2; // ban dau cua hinh vuong

void block(int x, int y) {
	SDL_Rect r = { x, y, BLOCK_SIZE, BLOCK_SIZE };
	SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
	SDL_RenderFillRect(rend, &r);
}

void init(int vsync) { //Khoi tao moi truong do hoa SDL, vsync = 1 neu muon su dung V-Sync
	SDL_Init(SDL_INIT_VIDEO); //khoi trao he thong do hoa
	//tao cua so tai vi tri (100,100) kich thuoc 640 x 480
	wnd = SDL_CreateWindow("FPS", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	//tao doi tuong dung hinh 2d, kiem tra viec kich hoat V-Sync
	rend = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED | (vsync ? SDL_RENDERER_PRESENTVSYNC : 0));
}

int event() { //xu ly su kien
	SDL_Event e; //bien luu tru su kien
	int running = 1; //bien kiem soat vong lap
	while (SDL_PollEvent(&e) != 0) //lay 1 su kien ra khoi hang doi neu co
	{
		//Neu nhan phim ESC thi thoat
		if (e.type == SDL_KEYDOWN)
			if (e.key.keysym.sym == SDLK_ESCAPE)
				running = 0; //neu dung, ta yeu cau dong cua chuong trinh
		//Neu nhat vao nut [X] thi thoat
		if (e.type == SDL_QUIT)
			running = 0;
	}
	return running;
}

void update() //cap nhat trang thai chuong trinh
{
	x += vx; y += vy; //di chuyen hinh vuong
	//nay nguoc lai neu cham cac canh cua so
	if (x < 0 || x > WIDTH - BLOCK_SIZE) vx = -vx;
	if (y < 0 || y > HEIGHT- BLOCK_SIZE) vy = -vy;
}

void draw() //xu ly thao tac dung hinh
{
	//xoa toan bo framebuffer ve mau den
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
	SDL_RenderClear(rend);
	block(x, y); //ve hinh vuong tai vi tri hien hanh
	SDL_RenderPresent(rend); //copy framebuffer -> screen
}

void done() { //don dep ung dung
	SDL_DestroyRenderer(rend); // huy doi tuong ngu canh dung hinh
	SDL_DestroyWindow(wnd); //huy cua so SDL
	SDL_Quit(); // cham dut phien lam viec SDL
}


int main(int argc, char** argv) {
	int timeBegin, frameCount; //bo dem FPS
	float fps;
	int running = 1;
	char buf[64]; //buffer tam de hien thi noi dung FPS

	init(1); //Khoi tao ung dung voi VSync bat
	frameCount = 0;
	timeBegin = SDL_GetTicks(); //lay thoi gian bat dau vong lap
	while (running) {
		running = event(); // xy ly su kien tren cua so
		update();
		draw();
		frameCount++;

		fps = frameCount / ((SDL_GetTicks() - timeBegin) / 1000.0f);

		sprintf_s(buf, "FPS=%0.2f", fps);
		SDL_SetWindowTitle(wnd, buf);
	}
	done();
	return 0;
}