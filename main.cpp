#include<math.h>
#include<allegro.h>
#ifndef M_PI
#define M_PI	3.14159265358979323846
#endif

class tViewer{
public:
	float Angle;
	float x;
	float y;
	float ox;
	float oy;
};

int ticks=0;
void ticker(){
	ticks++;
}END_OF_FUNCTION(ticker);

int seconds=0;
void clocka(){
	seconds++;
}END_OF_FUNCTION(clocka);


void main(){
	int x=0, y;
	int i;

	int WallUp;
	int WallDown;

	int Texture[64][64];

	int Scale;
	int TexCount;
	int Offset;

	float SinTable[1280];
	float CosTable[1280];
	float TanTable[1280];
	float InvTanTable[1280];
	float AtnTable[320];

	for( i = 0 ; i<1280 ; i++){
		if( i == 0 || i == 320 || i == 640 || i == 960)
			x = 1;
		SinTable[i] = sin((i + x) * M_PI / 640);
		CosTable[i] = cos((i + x) * M_PI / 640);
		TanTable[i] = tan((i + x) * M_PI / 640);
		InvTanTable[i] = 1 / tan((i + x) * M_PI / 640);
	}

	for( i = 0 ; i<320;i++)
		AtnTable[i] = atan2((i-160),160) * 640 / M_PI;

	int xStep;
	int yStep;

	float ang;
	float Slope;
	float InvSlope;

	float Slope64;
	float InvSlope64;

	int WallHitX;
	int WallHitY;

	int DirX=0;
	int DirY=0;

	float DistanceX;
	float DistanceY;
	float Distance;

	float XonY;
	float YonY;
	float XonX;
	float YonX;

	tViewer Viewer;

	#define UP -1
	#define DOWN 1
	#define LEFT -1
	#define RIGHT 1

	int Grid[20][20]={
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
	};

	Viewer.x = 3*64;
	Viewer.y = 5*64;
	Viewer.Angle = 0;

	allegro_init();
	install_timer();
	install_mouse();
	install_keyboard();
	set_gfx_mode(GFX_AUTODETECT,320,200,0,0);

	LOCK_VARIABLE(ticks);
	LOCK_FUNCTION(ticker);
	install_int_ex(ticker,BPS_TO_TIMER(60));

	LOCK_VARIABLE(seconds);
	LOCK_FUNCTION(clocka);
	install_int_ex(clocka,BPS_TO_TIMER(1));
	int frames=0,fps=0;
	
	bool bb = false;
	int xb; 

	BITMAP *bmp=create_bitmap(320,200);
	BITMAP *arma=create_bitmap(320,200);
	
	BITMAP *tiro1;
    BITMAP *tiro2;
    BITMAP *tiro3;
     
     tiro1 = load_bitmap("weapon/2.bmp",NULL);
     tiro2 = load_bitmap("weapon/3.bmp",NULL);
     tiro3 = load_bitmap("weapon/4.bmp",NULL);
	
	for( y = 0 ; y< 64 ; y++)
		for( x = 0 ; x< 64 ; x++)
			Texture[x][y] = (x ^ y) * 4;

	while(!key[KEY_ESC]){
		while(ticks && !key[KEY_ESC]){
			ticks--;
			if(key[KEY_D])
				Viewer.Angle += 16;
			if(key[KEY_A])
				Viewer.Angle -= 16;

			if( Viewer.Angle < 0 ) Viewer.Angle += 1280;
			if( Viewer.Angle >= 1280 ) Viewer.Angle -= 1280;

			Viewer.ox = Viewer.x;
			Viewer.oy = Viewer.y;

			if(key[KEY_W]){             
				Viewer.x += cos(Viewer.Angle * M_PI / 640) * 4;
				if( Grid[(int)Viewer.x / 64][(int)Viewer.y / 64])
					Viewer.x = Viewer.ox;

				Viewer.y -= sin(Viewer.Angle * M_PI / 640) * 4;
				if( Grid[(int)Viewer.x / 64][(int)Viewer.y / 64])
					Viewer.y = Viewer.oy;
			}
			
			if(key[KEY_SPACE]){
                blit(tiro1, screen, 0, 0, 110, 110, 98, 115);
                rest(100);
                    
                blit(tiro2, screen, 0, 0, 110, 110, 101, 120);
                rest(150);
                    
                blit(tiro3, screen, 0, 0, 110, 110, 81, 140);
                rest(200);
			}
			

			if(key[KEY_S]){
				Viewer.x -= cos(Viewer.Angle * M_PI / 640) * 4;
				if( Grid[(int)Viewer.x / 64][(int)Viewer.y / 64] )
					Viewer.x = Viewer.ox;

				Viewer.y += sin(Viewer.Angle * M_PI / 640) * 4;
				if( Grid[(int)Viewer.x / 64][(int)Viewer.y / 64])
					Viewer.y = Viewer.oy;
			}
		}

		for( i = -160 ; i<159 ; i++){
			ang = Viewer.Angle + AtnTable[i+160];

			if( ang < 0 ) ang += 1280;
			if( ang >= 1280 ) ang -= 1280;

			if( ang == 0 || ang == 320 || ang == 640 || ang == 960 ) ang ++;

			Slope = TanTable[(int)ang];
			Slope = fabs(Slope);
 
			InvSlope = InvTanTable[(int)ang];
			InvSlope = fabs(InvSlope);

			if(ang<320){	//'- Quadrant I
				DirX = RIGHT;
				DirY = UP;
			} else if(ang<640) {	//'- Quadrant II
				DirX = LEFT;
				DirY = UP;
			} else if(ang<960) {	//'- Quadrant III
				DirX = LEFT;
				DirY = DOWN;
			} else if(ang<1280) {	//'- Quadrant IV
				DirX = RIGHT;
				DirY = DOWN;
			}


			if( DirX == LEFT )
				XonY = int(Viewer.x / 64) * 64 - .001;
			else
				XonY = int(Viewer.x / 64) * 64 + 64;

			YonY = fabs(Viewer.x - XonY) * Slope;
			YonY = Viewer.y + YonY * DirY;

			if( DirY == UP )
				YonX = int(Viewer.y / 64) * 64 - .001;
			else
				YonX = int(Viewer.y / 64) * 64 + 64;

			XonX = fabs(Viewer.y - YonX) * InvSlope;
			XonX = Viewer.x + XonX * DirX;

			Slope64 = Slope * 64 * DirY;
			InvSlope64 = InvSlope * 64 * DirX;

			WallHitX = 0;	
			WallHitY = 0;
			xStep = 64 * DirX;	
			yStep = 64 * DirY;	

			while(WallHitY == 0 || WallHitX == 0){
				if( XonY < 0 || XonY > 640 || YonY < 0 || YonY > 640 ) WallHitY = 1;
				if( YonX < 0 || YonX > 640 || XonX < 0 || XonX > 640 ) WallHitX = 1;

				if( WallHitY == 0 ) WallHitY = Grid[int(XonY) / 64][int(YonY) / 64]; 
				if( WallHitX == 0 ) WallHitX = Grid[int(XonX) / 64][int(YonX) / 64]; 

				if( WallHitY == 0 ){
					XonY += xStep;
					YonY += Slope64;
				}

				if( WallHitX == 0 ){
					YonX += yStep;
					XonX += InvSlope64;
				}
			}

			DistanceY = fabs((XonY - Viewer.x) * CosTable[(int)Viewer.Angle] - (YonY - Viewer.y) * SinTable[(int)Viewer.Angle]);
			DistanceX = fabs((XonX - Viewer.x) * CosTable[(int)Viewer.Angle] - (YonX - Viewer.y) * SinTable[(int)Viewer.Angle]);

			if( DistanceY < DistanceX ){
				Distance = DistanceY;
				Offset = (int)YonY & 63;
			} else {
				Distance = DistanceX;
				Offset = (int)XonX & 63;
			}

			if( Distance == 0 ) Distance = 1;

			Distance = 4096 / Distance;

			vline(bmp,i+160,0,99-(int)Distance,55);

			Scale = int(6400000 / (Distance + Distance + 3));

			TexCount = 0;

			WallUp = 100 - (int)Distance;

			if( WallUp < 0 ){
				WallUp = 0;
				TexCount = int(fabs(100 - Distance) * Scale);
			}

			WallDown = 100 + (int)Distance;

			if( WallDown > 200 ) WallDown = 200;

			for( y = WallUp ; y<WallDown ; y++){
				TexCount = TexCount + Scale;
				putpixel(bmp,i+160,y,Texture[Offset][TexCount / 100000]);
			}

			vline(bmp,i+160,101+(int)Distance,200,1);
		}
		frames++;
		if(seconds){
			fps=frames/seconds;
			frames=0;
			seconds=0;
		}

        blit(bmp,screen,0,0,0,0,320,200);
        
        arma = load_bitmap("weapon/1.bmp",NULL);
        blit(arma,screen,0,0,110,110,320,200);
        rest(50);
	}
}END_OF_MAIN();
