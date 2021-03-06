#include "SDL.h"
#include <SDL_image.h>
#include <bits/stdc++.h>
#undef main

using namespace std;


bool isRunning = true;
bool fullscreen = false;
string u_src;
string u_dst;
map<string, int> u_m1;
int n_dev;
SDL_Renderer* ren;
SDL_Window* window;
SDL_Surface* hub;
SDL_Surface* message;
SDL_Surface* device;
SDL_Surface* green_tick;
SDL_Surface* red_tick;
SDL_Texture* hub_texture;
SDL_Texture* message_texture;
SDL_Texture* device_texture;
SDL_Texture* green_tick_texture;
SDL_Texture* red_tick_texture;





void delay(double t)
{
	return;
	
	time_t start = time(nullptr);
	time_t stop = time(nullptr);
	
	while ((stop-start)*1000< t)
		stop = time(nullptr);
	

}




vector<string>  generate_address(int n)
{
	cout << n << endl;

	srand(time(0));

	map<int, char> m1;

	for (int i = 10; i <= 15; i++)
		m1[i] = 'A' + i - 10;
	vector<string> vs1;


	for (int i = 0; i < n; i++)
	{
		string s1 = "";

		for (int j = 0; j < 6; j++)
		{
			string s2;

			int gen;
			for (int i = 0; i < 2; i++)
			{
				gen = rand() % 16;
				s2 += gen >= 10 ? m1[gen] : '0' + gen;
			}

			s1 += s2;
			if (j != 5)
				s1 += ':';
		}

		vs1.push_back(s1);
	}



	return vs1;
}

void create_topology()
{

	cout << "Enter the number of end-devices> ";
	int n;
	cin >> n;
	vector<string> vs1;
	vs1 = generate_address(n);

	map<string, int> devices;
	cout << "|------------------------------------|" << endl;
	cout << "|  Device Id |   Mac Address         |" << endl;
	cout << "|------------------------------------|" << endl;
	for (int i = 0; i < vs1.size(); i++)
	{
		cout << "|     " << i + 1 << "      |   " << vs1[i] << "   |" << endl;
		devices[vs1[i]] = i + 1;
	}
	cout << "|------------------------------------|" << endl;



	cout << "\nEnter the source address> ";
	string src;
	cin >> src;
	cout << "Enter the destination address> ";
	string dst;
	cin >> dst;


	cout << "\n\nData to be sent from Device-" << devices[src] << " to Device-" << devices[dst] << endl;
	delay(2000);
	cout << "\n\nData sent to the hub";
	for (int i = 0; i < 3; i++)
	{
		delay(1000);
		cout << ".";
	}
	delay(1000);
	cout << "             Data recieved succesfully by the hub\n" << endl;

	cout << "---------------------------------------------------------------------------" << endl;

	for (int i = 0; i < n; i++)
	{
		if (i != devices[src] - 1)
		{
			cout << "Sending data to Device-" << i + 1;
			for (int j = 0; j < 3; j++)
			{
				delay(1000);
				cout << ".";

			}
			delay(1000);
			if (i != devices[dst] - 1)
				cout << "         Data discarded by Device-" << i + 1 << "            |" << endl;
			else
				cout << "         Data succesfully recieved by Device-" << i + 1 << " |" << endl;
			delay(100);
		}
	}
	cout << "---------------------------------------------------------------------------" << endl;


	
	cout << "\n\nACK sent to the hub";
	for (int i = 0; i < 3; i++)
	{
		delay(1000);
		cout << ".";
	}
	delay(1000);
	cout << "             ACK recieved succesfully by the hub\n" << endl;

	cout << "---------------------------------------------------------------------------" << endl;

	for (int i = 0; i < n; i++)
	{
		if (i != devices[dst] - 1)
		{
			cout << "Sending ACK to Device-" << i + 1;
			for (int j = 0; j < 3; j++)
			{
				delay(1000);
				cout << ".";

			}
			delay(1000);
			if (i != devices[src] - 1)
				cout << "         ACK discarded by Device-" << i + 1 << "            |" << endl;
			else
				cout << "         ACK succesfully recieved by Device-" << i + 1 << " |" << endl;
			delay(100);
		}
	}
	cout << "---------------------------------------------------------------------------" << endl;

	u_m1 = devices;
	u_src = src;
	u_dst = dst;
	n_dev = n;

}








void get_images()
{
	hub = IMG_Load("images//hub2.jpg");
	hub_texture = SDL_CreateTextureFromSurface(ren, hub);
	SDL_FreeSurface(hub);

	message = IMG_Load("images//message.png");
	message_texture = SDL_CreateTextureFromSurface(ren, message);
	SDL_FreeSurface(message);

	device = IMG_Load("images//device.jpg");
	device_texture = SDL_CreateTextureFromSurface(ren, device);
	SDL_FreeSurface(device);

	green_tick = IMG_Load("images//green_tick.jpg");
	green_tick_texture = SDL_CreateTextureFromSurface(ren, green_tick);
	SDL_FreeSurface(green_tick);

	red_tick = IMG_Load("images//red_tick.jpg");
	red_tick_texture = SDL_CreateTextureFromSurface(ren, red_tick);
	SDL_FreeSurface(red_tick);

}

bool render_to_devices(int count_n, int ini_x, int ini_y, int dev_x, int dev_y)
{
	double dx;
	double dy;
	dy = count_n;
	dx = ((double)abs(ini_x - dev_x) / (double)abs(ini_y - dev_y))*dy;

	

		if (ini_x > dev_x)
			ini_x = ini_x-dx-13;
		else
			ini_x = ini_x+dx;
		ini_y += dy;

		SDL_Rect texture_destination;
		texture_destination.x = ini_x;
		texture_destination.y = ini_y;
		texture_destination.w = 25;
		texture_destination.h = 25;
		if (texture_destination.y <= dev_y)
		{
			SDL_RenderCopy(ren, message_texture, NULL, &texture_destination);
			return true;
		}
		return false;
		
}


bool render_from_devices(int count_n,int ini_x, int ini_y, int dev_x, int dev_y)
{


	double dx;
	double dy;
	dy = count_n;
	dx = dy/((double)abs(ini_y - dev_y) / (double)abs(ini_x - dev_x)) ;


	
	if (ini_x > dev_x)
		dev_x = dev_x + dx - 16;
	else
		dev_x = dev_x - dx;
	dev_y -= dy;

	SDL_Rect texture_destination;
	texture_destination.x = dev_x;
	texture_destination.y = dev_y;
	texture_destination.w = 25;
	texture_destination.h = 25;
	if (texture_destination.y >= ini_y)
	{
		SDL_RenderCopy(ren, message_texture, NULL, &texture_destination);
		return true;
	}
	return false;
}

void call_first_render()
{
	static int count_n = 0;

	SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(ren);

	int delay_time = 10;
	int n = n_dev;
	int window_width = 800;
	int window_height = 600;
	int bot_x=(window_width/(n-1))/2;
	int top_y=100;
	int bot_y=window_height-100;
	int top_x = window_width / 2;

	int ini_x = top_x;
	int ini_y = top_y;
	int dev_x = bot_x;
	int dev_y = bot_y;
	SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);




	for (int i = 0, gap = window_width / (n - 1) - 2 * (dev_x / (n - 1)); i < n; i++)
	{

		SDL_Rect texture_destination;
		texture_destination.x = dev_x - 50 / 2;
		texture_destination.y = dev_y;
		texture_destination.w = 50;
		texture_destination.h = 50;
		SDL_RenderCopy(ren, device_texture, NULL, &texture_destination);

		if (i == n - 1)
		{
			texture_destination.x = ini_x - 50 / 2;
			texture_destination.y = ini_y - 20;
			texture_destination.w = 50;
			texture_destination.h = 50;
			SDL_RenderCopy(ren, hub_texture, NULL, &texture_destination);
		}

		SDL_RenderDrawLine(ren, ini_x, ini_y, dev_x, dev_y);

		dev_x += gap;

	}

	ini_x = top_x;
	ini_y = top_y;
	dev_x = bot_x;
	dev_y = bot_y;
	if (count_n == 0)
	{
		SDL_RenderPresent(ren);
		SDL_Delay(3000);
	}
	else 
	{

		static bool is_valid_1 = true;
		static bool is_valid_2 = true;
		static bool is_valid_3 = true;
		static bool is_valid_4 = true;
		static bool is_valid_5 = true;

		if (is_valid_1)
		{
			ini_x = top_x;
			ini_y = top_y;
			dev_x = bot_x;
			dev_y = bot_y;

			static int count_1 = 0;
			for (int i = 0, gap = window_width / (n - 1) - 2 * (dev_x / (n - 1)); i < n; i++)
			{
				if(i+1==u_m1[u_src])
				is_valid_1 = render_from_devices(count_1, ini_x, ini_y, dev_x, dev_y);
				dev_x += gap;
			}
			
			count_1++;
			SDL_RenderPresent(ren);
			SDL_Delay(delay_time);
		}
		else if(is_valid_2)
		{
			ini_x = top_x;
			ini_y = top_y;
			dev_x = bot_x;
			dev_y = bot_y;

			static int count_2=0;
			for (int i = 0, gap = window_width / (n - 1) - 2 * (dev_x / (n - 1)); i < n; i++)
			{
				if(i+1!=u_m1[u_src])
				is_valid_2=render_to_devices(count_2, ini_x, ini_y, dev_x, dev_y);
				dev_x += gap;
			}
			count_2++;
		
			SDL_RenderPresent(ren);
			SDL_Delay(delay_time);
		}
		else if(is_valid_3)
		{
			ini_x = top_x;
			ini_y = top_y;
			dev_x = bot_x;
			dev_y = bot_y;

			for (int i = 0, gap = window_width / (n - 1) - 2 * (dev_x / (n - 1)); i < n; i++)
			{
				SDL_Rect texture_destination;
				texture_destination.x = dev_x - 50 / 2;
				texture_destination.y = dev_y;
				texture_destination.w = 25;
				texture_destination.h = 25;
			
				
				

				if (i+1 != u_m1[u_src]&&i+1!=u_m1[u_dst])
				{
					SDL_RenderCopy(ren, red_tick_texture, NULL, &texture_destination);
				}
				else if (i+1 == u_m1[u_dst])
				{
					SDL_RenderCopy(ren, green_tick_texture, NULL, &texture_destination);
				}

				dev_x += gap;

			}

			is_valid_3 = false;
			

			SDL_RenderPresent(ren);

			SDL_Delay(3000);


		}
		else if (is_valid_4)
		{
			static int count_4 = 0;
			ini_x = top_x;
			ini_y = top_y;
			dev_x = bot_x;
			dev_y = bot_y;


			for (int i = 0,gap=window_width/(n-1)-2*(dev_x/(n-1)); i < n; i++)
			{
				if (i+1 == u_m1[u_dst])
				{
					is_valid_4=render_from_devices(count_4,ini_x, ini_y, dev_x, dev_y);
				}
				dev_x += gap;
			}
			count_4++;
			SDL_RenderPresent(ren);
			SDL_Delay(delay_time);
		}
		else if (is_valid_5)
		{
			static int count_5 = 0;
			ini_x = top_x;
			ini_y = top_y;
			dev_x = bot_x;
			dev_y = bot_y;

			for (int i = 0,gap=window_width/(n-1)-2*((dev_x)/(n-1)); i < n; i++)
			{
				    if(i+1!=u_m1[u_dst])
					is_valid_5=render_to_devices(count_5,ini_x, ini_y, dev_x, dev_y);
				dev_x += gap;
			}
			count_5++;
			SDL_RenderPresent(ren);
			SDL_Delay(delay_time);
		}
		else
		{

		ini_x = top_x;
		ini_y = top_y;
		dev_x = bot_x;
		dev_y = bot_y;
		
		for (int i = 0, gap = window_width / (n - 1) - 2 * (dev_x / (n - 1)); i < n; i++)
		{
			if (i +1== u_m1[u_src])
			{
				SDL_Rect texture_destination;

				texture_destination.x = dev_x;
				texture_destination.y = dev_y;
				texture_destination.w = 25;
				texture_destination.h = 25;

				SDL_RenderCopy(ren, green_tick_texture, NULL, &texture_destination);
			}
			else if(i+1!=u_m1[u_dst])
			{
				SDL_Rect texture_destination;

				texture_destination.x = dev_x;
				texture_destination.y = dev_y;
				texture_destination.w = 25;
				texture_destination.h = 25;

				SDL_RenderCopy(ren, red_tick_texture, NULL, &texture_destination);
			}
			dev_x += gap;
		}
		SDL_RenderPresent(ren);
	
		}
		
	}
	

	count_n++;
}



void handleEvents() {
	
	SDL_Event event;
	SDL_PollEvent(&event);


	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}


void render() {
		call_first_render();

}





void update() {
	
}





int main()
{

	 create_topology();
	
	int flags = 0;
	flags = SDL_WINDOW_RESIZABLE;
	if (fullscreen) {
		flags = flags | SDL_WINDOW_FULLSCREEN;
	}

	window = SDL_CreateWindow("GG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, flags);
	ren = SDL_CreateRenderer(window, -1, 0);
	
	
	SDL_Init(SDL_INIT_VIDEO);
	int flag = IMG_INIT_JPG | IMG_INIT_PNG;
	int initted = IMG_Init(flag);
	if ((initted & flag) != flag) {
		printf("IMG_Init: Failed to init required jpg and png support!\n");
		printf("IMG_Init: %s\n", IMG_GetError());
	}
	
	
	get_images();

	
	while (isRunning) {
		handleEvents();
		update();
		render();
	}
		

	IMG_Quit();
	SDL_DestroyTexture(hub_texture);
	SDL_DestroyTexture(device_texture);
	SDL_DestroyTexture(message_texture);
	SDL_DestroyTexture(green_tick_texture);
	SDL_DestroyTexture(red_tick_texture);

		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(window);	
		SDL_Quit();


		return 0;
	
}