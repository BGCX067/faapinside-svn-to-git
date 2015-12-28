#include <hge.h>
#include <hgesprite.h>
#include <ctime>
#include <math.h>


HGE *hge = 0;





class camera{
private:
	float width;
	float height;
	float x; //right
	float y; //down
	float screenX;
	float screenY;
public:
	float getWidth(){return width;}
	float getHeight(){return height;}
	float getX(){return x;}
	float getY(){return y;}

	void setWidth(float tWidth){width = tWidth;}
	void setHeight(float tHeight){height = tHeight;}
	void setX(float tX){x = tX;}
	void setY(float tY){y = tY;}

	camera(){width = 800; height = 600; x = 128; y = 128;}
	~camera(){}

	void lookAt(int lookX, int lookY){x = lookX - (width/2) + 16; y = lookY - (height/2) + 16;}

};



class NPC : public hgeSprite
{
private:
	float _x, _y;
	float _w, _h;
	float oldX, oldY;
	bool _solid;

public:
	//NPC();
	NPC(float x, float y, float w, float h, bool solid);
	~NPC();

	float velocityX, velocityY;

	float getX() { return _x; }
	float getY() { return _y; }
	float getW() { return _w; }
	float getH() { return _h; }
	bool getSolid() { return _solid; }
	float getVX(){return velocityX;}
	float getVY(){return velocityY;}

	void setVX(float tVX){velocityX = tVX;}
	void setVY(float tVY){velocityY = tVY;}

	void switchSolid() { _solid = _solid ? false : true; }

	bool isVisible(camera cam)
	{
		if (_x > cam.getX() - 64 && _x < cam.getX() + cam.getWidth() + 64
		 && _y > cam.getY() - 64 && _y < cam.getY() + cam.getHeight() + 64)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool collidesNPC(NPC temp){
		if (temp.getX() > _x - temp.getW() && temp.getY() > _y - temp.getH() && temp.getX() < _x + _w && temp.getY() < _y + _h)
		return true;
		return false;
	}

	void go()
	{
		oldX = _x;
		oldY = _y;
		_x += velocityX;
		_y += velocityY;
	}

	void stop() //MOAR LIEK BOUNCE
	{

			if (fabs(velocityX) == fabs(velocityY) || velocityX == 0 || velocityY == 0){
			velocityX = -velocityX;
			velocityY = -velocityY;} else 

		if (fabs(velocityX) > fabs(velocityY)){
			velocityY = -velocityY; } else 
			
			if(fabs(velocityY) > fabs(velocityX)) {
				velocityX = -velocityX;}


		_x = oldX;
		_y = oldY;


	}

	void doInertia(){
		if (velocityX != 0) velocityX = velocityX * 999/1000;
		if (velocityY != 0) velocityY = velocityY * 999/1000;
	}
};

NPC::NPC(float x, float y, float w, float h, bool solid) : hgeSprite(NULL, x, y, w, h)
{
	_x = x;
	_y = y;
	_w = w;
	_h = h;
	_solid = solid;
}

NPC::~NPC() {}

NPC treeNPC(500, 500, 64, 64, true);

NPC avatar(600,600, 32, 32, true);

class tile:public hgeSprite{
private:
	float x;
	float y;
	float w;
	float h;
	bool solid;
public:
	tile();
	tile(HTEXTURE, bool, float, float, float, float);//TODO body
	~tile();

	float getX(){return x;}
	float getY(){return y;}
	float getW(){return w;}
	float getH(){return h;}
	bool getSolid(){return solid;}

	void setX(float tX){x = tX;}
	void setY(float tY){y = tY;}
	void setW(float tW){w = tW;}
	void setH(float tH){w = tH;}
	void setSolid(bool tSolid){solid = tSolid;}

};

// a single tile outside of main//GLOBALS!



		tile tiles[100][100];
				camera cam1;
				float oldAvatarX;
				float oldAvatarY;
				float avatarSpeedY;
				float avatarSpeedX;
			//	NPC tree;



void doCamera(camera cam1);








bool RenderFunc()
{
	// Begin rendering quads.
	// This function must be called
	// before any actual rendering.
	hge->Gfx_BeginScene();

	// Clear screen with black color
	hge->Gfx_Clear(0);

	//ACTUAL SHIT
	doCamera(cam1);

	// End rendering and update the screen
	hge->Gfx_EndScene();

	// RenderFunc should always return false
	return false;
}


bool FrameFunc() //main LOOP/LOGIC!
{
	treeNPC.go();

	avatar.go();



	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true; //exit loop upon ESC key

		if (hge->Input_GetKeyState(HGEK_LEFT))  avatar.setVX(avatar.getVX() - 0.001f); 
		if (hge->Input_GetKeyState(HGEK_RIGHT))  avatar.setVX(avatar.getVX() + 0.001f);
		if (hge->Input_GetKeyState(HGEK_UP))  avatar.setVY(avatar.getVY() - 0.001f);
		if (hge->Input_GetKeyState(HGEK_DOWN)) avatar.setVY(avatar.getVY() + 0.001f);

		



		//tree follows player.
		if (treeNPC.getX() > avatar.getX()) treeNPC.setVX(treeNPC.getVX() - 0.0004f);
		if (treeNPC.getX() < avatar.getX()) treeNPC.setVX(treeNPC.getVX() + 0.0004f);
		if (treeNPC.getY() > avatar.getY()) treeNPC.setVY(treeNPC.getVY() - 0.0004f);
		if (treeNPC.getY() < avatar.getY()) treeNPC.setVY(treeNPC.getVY() + 0.0004f);
		if (treeNPC.collidesNPC(avatar)) treeNPC.stop(); 
		treeNPC.doInertia();


		if (avatar.getX() < 450 || avatar.getX() > 2500) avatar.stop(); //limit avatar
		if (avatar.getY() < 450 || avatar.getY() > 2500) avatar.stop();
		if (avatar.collidesNPC(treeNPC) == true) avatar.stop();
		avatar.doInertia();

//		cam1.setX(cam1.getX() + (cam1.getX() + (((cam1.getWidth() / 2) + avatar.getX()) / 2) - (cam1.getWidth() / 2))); //wtf.
		cam1.lookAt((int)avatar.getX(), (int)avatar.getY());
		
	return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) //technical main, used for initialization and bs, bleghck.
{

	hge = hgeCreate(HGE_VERSION);

	// Set our frame function
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);

	hge->System_SetState(HGE_RENDERFUNC, RenderFunc); ///////////

	// Set the window title
	hge->System_SetState(HGE_TITLE, "THE TREE IS SOLID");
	
	// Run in windowed mode
	// Default window size is 800x600
	hge->System_SetState(HGE_WINDOWED, true); //olol

	// Don't use BASS for sound
	hge->System_SetState(HGE_USESOUND, false);


	if(hge->System_Initiate())
	{
		// Starts running FrameFunc().
		// Note that the execution "stops" here
		// until "true" is returned from FrameFunc().


		//LOAD RESOURCES
		
		srand((unsigned)time(0));

		HTEXTURE grass = hge->Texture_Load("grass.PNG",0,0);
		HTEXTURE t_tree = hge->Texture_Load("tree.PNG",0,0);
		HTEXTURE fuck = hge->Texture_Load("butterfuck.PNG",0,0);
		
		for (int parse = 0; parse < 100; parse++){
			for (int parsley = 0; parsley < 100; parsley++){
				tiles[parsley][parse].SetTexture(grass);
				tiles[parsley][parse].setX(parsley * 32.0f);
				tiles[parsley][parse].setY(parse * 32.0f);
			}
		}

		
		treeNPC.SetTexture(t_tree);
		treeNPC.SetTextureRect(0, 0, 64, 64);
		avatar.SetTexture(fuck);
		avatar.SetTextureRect(0,0,32,32);

		avatarSpeedX = 0.0f;
		avatarSpeedY = 0.0f;

		hge->System_Start();
	}

	hge->System_Shutdown();
	hge->Release();

	return 0;
}

tile::~tile(){
	SetTexture(NULL);
}

tile::tile():hgeSprite(NULL, 0, 0, 32, 32){
x = 0;
y = 0;
w = 32;
h = 32;
solid = 0;
}

tile::tile(HTEXTURE tTex, bool tSolid, float tX, float tY, float tW = 32, float tH = 32):hgeSprite(tTex, tX, tY, tW, tH){
x = tX;
y = tY;
w = tW;
h = tH;
solid = tSolid;
}

void doCamera(camera cam1){
	int x = 0;
	int y = 0;

	for (int parse = (int)(cam1.getY() / 32) - 2; parse < (int)((cam1.getY() / 32) + (cam1.getHeight() / 32)) + 4; parse++)
	{
		for (int parsley = (int)(cam1.getX() / 32) - 2; parsley < (int)((cam1.getX() / 32) + (cam1.getWidth() / 32)) + 2; parsley++)
		{
			tiles[parsley][parse].Render((float)((x * 32) - (int)cam1.getX() % 32),
										 (float)((y * 32) - (int)cam1.getY() % 32));
			x++;
		}

		x = 0;
		y++;
	}

	if (treeNPC.isVisible(cam1))
	{
		treeNPC.Render(treeNPC.getX() - cam1.getX(), treeNPC.getY() - cam1.getY());
	}

//	if(tree.getInCam(cam1) == true) tree.getSprite().Render(tree.getX() - cam1.getX(), tree.getY() - cam1.getY());

	if (avatar.getX() > cam1.getX() && avatar.getY() > cam1.getY() && avatar.getX() < cam1.getX() + cam1.getWidth() && avatar.getY() < cam1.getY() + cam1.getHeight())
		avatar.Render(avatar.getX() - cam1.getX(), avatar.getY() - cam1.getY());
}

