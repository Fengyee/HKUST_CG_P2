// The sample model.  You should build a file
// very similar to this for when you make your model.
#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "modelerui.h"
#include <FL/gl.h>

#include "modelerglobals.h"
#include "LSystem.h"
#include <vector>

// Global variable
vector<LSystem*> *LSystemVec = NULL;
enum NODE_OPERATIONS {
	F = 0, RL, RR,
	RL_SPLIT_PUSH,
	RR_POP_SPLIT
};


enum NODE_TYPE {
	A = 0, B, C, D, E
};
// function declaration
void initControls(ModelerControl* controls);
void changeLight();
void initLSystem();
void drawLSystem();

// To make a MyCreature, we inherit off of ModelerView
class MyCreature : public ModelerView 
{
public:
    MyCreature(int x, int y, int w, int h, char *label) 
        : ModelerView(x,y,w,h,label), textureImage(NULL) { }
	GLuint texName;
	unsigned char* textureImage;
	
	void createTexture(int _width);
    virtual void draw();
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createMyCreature(int x, int y, int w, int h, char *label)
{ 
    return new MyCreature(x,y,w,h,label); 
}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out MyCreature
void MyCreature::draw()
{
    // This call takes care of a lot of the nasty projection 
    // matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...
    ModelerView::draw();

	// Change the default light source to illuminate your scene more dynamically
	changeLight();

	
	if ((int)VAL(DISPLAY_LSYSTEM)) {
		drawLSystem();
	}

	// draw the floor
	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(COLOR_RED);
	glPushMatrix();
	glTranslated(-5, -2,-5);
	drawBox(10,0.01f,10);
	glPopMatrix();

	// draw the sample model
	setAmbientColor(.1f,.1f,.1f);

	glPushMatrix();

	int level_of_detail = (int)VAL(DRAW_LEVEL);
	glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));
	if ((int)VAL(DRAW_TEXTURE))
	{
		glPushMatrix();
		glTranslated(0, 0, 0);
		createTexture(5);
		glPopMatrix();
	}
	    
    //set variables
	float unit_block = 0.75;
	float body_x = 2.0;
	float body_y = 3.0;
	float body_z = 1.0;
	float head = 2.0;
	float arm_y = 1.5;
	float arm_x = 0.9;
	float arm_z = 1.0;
	float foot_y = 0.5;
	float foot_x = 0.9;
	float foot_z = 1.1;
	float body_init_y = -2 + unit_block * (arm_y * 2 + foot_y);
	float body_init_x = -1 * body_y / 2 * unit_block;
	float body_init_z = -1;
	float thick = 0.01;
	float hair_thick = 0.1;
	float eye_x = 0.5;
	float eye_y = 0.25;
	float mouth_x = 0.5;
	float mouth_y = 0.25;
	float sleeve_y = 0.25;
		// draw body
		glPushMatrix();
		glTranslated(body_init_x, body_init_y, body_init_z);

		// waist
		setDiffuseColor(COLOR_WAIST);
		glPushMatrix();
		glTranslated(0, 0.5 * unit_block, -1 * thick);
		glScaled(body_x * unit_block, sleeve_y * unit_block, 1);
		drawBox(1, 1, thick);
		glTranslated(0, 0, body_z * unit_block + thick);
		glScaled(1, 1, 1);
		drawBox(1, 1, thick);
		glPopMatrix();
		glPushMatrix();
		glTranslated(-1 * thick, 0.5 * unit_block, -1 * thick);
		glScaled(1, sleeve_y * unit_block, body_z * unit_block);
		drawBox(thick, 1, 1);
		glTranslated(body_x * unit_block + thick, 0, 0);
		glScaled(1, 1, 1);
		drawBox(thick, 1, 1);
		glPopMatrix();

		// NECK
		glPushMatrix();
		glTranslated(body_x * unit_block / 2, (body_y - 0.95) * unit_block, body_z * unit_block);
		glRotated(45, 0.0, 0.0, 1.0);
		setDiffuseColor(COLOR_DEC);
		glPushMatrix();
		glTranslated(-1 * sleeve_y * unit_block, -1 * sleeve_y * unit_block, 0);
		glScaled(sleeve_y * unit_block, 1 + sleeve_y * unit_block * 2, thick);
		drawBox(1, 1, 1);
		glPopMatrix();
		glPushMatrix();
		glTranslated(-1 * sleeve_y * unit_block, -1 * sleeve_y * unit_block, 0);
		glScaled(1 + sleeve_y * unit_block * 2, sleeve_y * unit_block, thick);
		drawBox(1, 1, 1);
		glPopMatrix();
		setDiffuseColor(COLOR_SKIN);
		glScaled(1, 1, thick);
		drawTriangle(1, 0, 0, 0, 1, 0, 0, 0, 1);
		glPopMatrix();



		// draw head
		
		glPushMatrix();
		glTranslated(0, body_y * unit_block, -1 * (head - body_z) / 2 * unit_block);
		glTranslated(head / 2 * unit_block, 0, head / 2 * unit_block);
		glScaled(VAL(HEAD_WIDTH), VAL(HEAD_HEIGHT), VAL(HEAD_DEPTH));
		glTranslated(-1 * head / 2 * unit_block, 0, -1 * head / 2 * unit_block);
		// HEAD ROTATION IMPLEMENTATION
		glTranslated(head / 2 * unit_block, 0, head / 2 * unit_block);
		glRotated(VAL(HEAD_ROTATION_X), 1.0, 0.0, 0.0);
		glRotated(VAL(HEAD_ROTATION_Y), 0.0, 1.0, 0.0);
		glRotated(VAL(HEAD_ROTATION_Z), 0.0, 0.0, 1.0);
		glTranslated(-1 * head / 2 * unit_block, 0, -1 * head / 2 * unit_block);
		
		// eyes
		glPushMatrix();
		setDiffuseColor(COLOR_WHITE);
		glTranslated(0.25 * unit_block, 0.75 * unit_block, head * unit_block);
		glScaled(eye_x / 2 * unit_block, eye_y * unit_block, thick);
		drawBox(1, 1, 1);
		setDiffuseColor(COLOR_GREEN);
		glTranslated(1, 0, 0);
		glScaled(1, 1, 1);
		drawBox(1, 1, 1);
		glPopMatrix();
		glPushMatrix();
		setDiffuseColor(COLOR_GREEN);
		glTranslated((0.25+head / 2) * unit_block, 0.75 * unit_block, head * unit_block);
		glScaled(eye_x / 2 * unit_block, eye_y * unit_block, thick);
		drawBox(1, 1, 1);
		setDiffuseColor(COLOR_WHITE);
		glTranslated(1, 0, 0);
		glScaled(1, 1, 1);
		drawBox(1, 1, 1);
		glPopMatrix();

		// mouth
		glPushMatrix();
		setDiffuseColor(COLOR_PINK);
		glTranslated(0.75 * unit_block, 0.25 * unit_block, head * unit_block);
		glScaled(mouth_x * unit_block, mouth_y * unit_block, thick);
		drawBox(1, 1, 1);
		glPopMatrix();

		// hair
		glPushMatrix();
		setDiffuseColor(COLOR_HAIR);
		glTranslated(0, head * unit_block, head * unit_block);
		glRotated(-90, 0.0, 0.0, 1.0);
		glScaled(head / 2 * unit_block, head / 2 * unit_block, hair_thick);
		drawTriangle(1, 0, 0, 0, 1, 0, 0, 0, 1);
		glPopMatrix();
		glPushMatrix();
		glTranslated(head * unit_block, head * unit_block, head * unit_block);
		glRotated(180, 0.0, 0.0, 1.0);
		glScaled(head / 2 * unit_block, head / 2 * unit_block, hair_thick);
		drawTriangle(1, 0, 0, 0, 1, 0, 0, 0, 1);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, 0, -1 * hair_thick);
		glScaled(head * unit_block, head * unit_block, hair_thick);
		drawBox(1, 1, 1);
		glPopMatrix();

		glPushMatrix();
		glTranslated(-1 * hair_thick, head * unit_block, -1 * hair_thick);
		glScaled(head * unit_block + 2 * hair_thick, hair_thick, head * unit_block + 2 * hair_thick);
		drawBox(1, 1, 1);
		glPopMatrix();

		glPushMatrix();
		glTranslated(-1 * hair_thick, head / 2 * unit_block, -1 * hair_thick);
		glScaled(hair_thick, head / 2 * unit_block, head * unit_block + 2 * hair_thick);
		drawBox(1, 1, 1);
		glTranslated(0, -1, 0);
		glScaled(1, 1, 0.6);
		drawBox(1, 1, 1);
		glPopMatrix();

		glPushMatrix();
		glTranslated(head * unit_block, head / 2 * unit_block, -1 * hair_thick);
		glScaled(hair_thick, head / 2 * unit_block, head * unit_block + 2 * hair_thick);
		drawBox(1, 1, 1);
		glTranslated(0, -1, 0);
		glScaled(1, 1, 0.6);
		drawBox(1, 1, 1);
		glPopMatrix();

		// back to head
		glScaled(head * unit_block, head * unit_block, head * unit_block);
		setDiffuseColor(COLOR_SKIN);
		drawBox(1, 1, 1);
		glPopMatrix();

		// draw left hand
		glPushMatrix();
		//glTranslated(-1, 0.8, 0.75);
		//glRotated(-90, 0.0, 1.0, 0.0);
		//drawCylinder(0.4, 0.2, 0.2);

		//glRotated(90, 0.0, 1.0, 0.0);
		glTranslated(-1 * arm_x * unit_block, body_y * unit_block - arm_y * unit_block, 0);

		// WHOLE ARM ROTATION IMPLEMENTATION
		glTranslated(arm_x * unit_block, arm_y * unit_block, arm_z / 2 * unit_block);
		glRotated(VAL(LEFT_UPPER_ARM_ROTATION_X), 1.0, 0.0, 0.0);
		glRotated(VAL(LEFT_UPPER_ARM_ROTATION_Y), 0.0, 1.0, 0.0);
		glRotated(VAL(LEFT_UPPER_ARM_ROTATION_Z), 0.0, 0.0, 1.0);
		glTranslated(-1 * arm_x * unit_block, -1 * arm_y * unit_block, -1 * arm_z / 2 * unit_block);
		
		// sleeve
		setDiffuseColor(COLOR_DEC);
		glPushMatrix();
		glTranslated(0, 0, -1 * thick);
		glScaled(arm_x * unit_block, sleeve_y * unit_block, 1);
		drawBox(1, 1, thick);
		glTranslated(0, 0, arm_z * unit_block + thick);
		glScaled(1, 1, 1);
		drawBox(1, 1, thick);
		glPopMatrix();
		glPushMatrix();
		glTranslated(-1 * thick, 0, -1 * thick);
		glScaled(1, sleeve_y * unit_block, arm_z * unit_block);
		drawBox(thick, 1, 1);
		glTranslated(arm_x * unit_block + thick, 0, 0);
		glScaled(1, 1, 1);
		drawBox(thick, 1, 1);
		glPopMatrix();

		// lower left arm
		glPushMatrix();
		setDiffuseColor(COLOR_SKIN);
		glTranslated(0, -1 * arm_y * unit_block, 0);
		// lower arm rotation implementation
		glTranslated(arm_x / 2 * unit_block, arm_y * unit_block, arm_z / 2 * unit_block);
		glRotated(VAL(LEFT_LOWER_ARM_ROTATION_X), 1.0, 0.0, 0.0);
		glTranslated(-1 * arm_x / 2 * unit_block, -1 * arm_y * unit_block, -1 * arm_z / 2 * unit_block);
		glScaled(arm_x * unit_block, arm_y * unit_block, arm_z * unit_block);
		// draw left lower arm
		drawBox(1, 1, 1);
		glPopMatrix();
		
		// back to upper left arm
		setDiffuseColor(COLOR_CYAN);
		glScaled(arm_x * unit_block, arm_y * unit_block, arm_z * unit_block);
		drawBox(1, 1, 1);
		glPopMatrix();

		// draw right hand
		glPushMatrix();
		//glTranslated(1.0, 0.8, 0.75);
		//glRotated(90, 0.0, 1.0, 0.0);
		//drawCylinder(0.4, 0.2, 0.2);

		//glRotated(-90, 0.0, 1.0, 0.0);
		
		glTranslated(body_x * unit_block, body_y * unit_block - arm_y * unit_block, 0);

		// WHOLE ARM ROTATION IMPLEMENTATION
		glTranslated(0, arm_y * unit_block, arm_z / 2 * unit_block);
		glRotated(VAL(RIGHT_UPPER_ARM_ROTATION_X), 1.0, 0.0, 0.0);
		glRotated(VAL(RIGHT_UPPER_ARM_ROTATION_Y), 0.0, 1.0, 0.0);
		glRotated(VAL(RIGHT_UPPER_ARM_ROTATION_Z), 0.0, 0.0, 1.0);
		glTranslated(0, -1 * arm_y * unit_block, -1 * arm_z / 2 * unit_block);
		// SLEEVE
		setDiffuseColor(COLOR_DEC);
		glPushMatrix();
		glTranslated(0, 0, -1 * thick);
		glScaled(arm_x * unit_block, sleeve_y * unit_block, 1);
		drawBox(1, 1, thick);
		glTranslated(0, 0, arm_z * unit_block + thick);
		glScaled(1, 1, 1);
		drawBox(1, 1, thick);
		glPopMatrix();
		glPushMatrix();
		glTranslated(-1 * thick, 0, -1 * thick);
		glScaled(1, sleeve_y * unit_block, arm_z * unit_block);
		drawBox(thick, 1, 1);
		glTranslated(arm_x * unit_block + thick, 0, 0);
		glScaled(1, 1, 1);
		drawBox(thick, 1, 1);
		glPopMatrix();

		// lower right arm
		glPushMatrix();
		setDiffuseColor(COLOR_SKIN);

		glTranslated(0, -1 * arm_y * unit_block, 0);
		// lower arm rotation implementation
		glTranslated(arm_x / 2 * unit_block, arm_y * unit_block, arm_z / 2 * unit_block);
		glRotated(VAL(RIGHT_LOWER_ARM_ROTATION_X), 1.0, 0.0, 0.0);
		glTranslated(-1 * arm_x / 2 * unit_block, -1 * arm_y * unit_block, -1 * arm_z / 2 * unit_block);
		glScaled(arm_x * unit_block, arm_y * unit_block, arm_z * unit_block);
		// draw right lower arm
		drawBox(1, 1, 1);
		glPopMatrix();

		// BACK TO ARM
		setDiffuseColor(COLOR_CYAN);
		glScaled(arm_x * unit_block, arm_y * unit_block, arm_z * unit_block);
		drawBox(1, 1, 1);
		glPopMatrix();

		// draw left leg
		glPushMatrix();
		glTranslated((body_x / 2 - arm_x) / 2 * unit_block, -1 * arm_y * unit_block, 0);

		// WHOLE LEG ROTATION IMPLEMENTATION
		glTranslated(arm_x * unit_block, arm_y * unit_block, arm_z / 2 * unit_block);
		glRotated(VAL(LEFT_UPPER_LEG_ROTATION_X), 1.0, 0.0, 0.0);
		glRotated(VAL(LEFT_UPPER_LEG_ROTATION_Y), 0.0, 1.0, 0.0);
		glRotated(VAL(LEFT_UPPER_LEG_ROTATION_Z), 0.0, 0.0, 1.0);
		glTranslated(-1 * arm_x * unit_block, -1 * arm_y * unit_block, -1 * arm_z / 2 * unit_block);

		// sleeve
		setDiffuseColor(COLOR_DARK);
		glPushMatrix();
		glTranslated(0, 0, -1 * thick);
		glScaled(arm_x * unit_block, sleeve_y * unit_block, 1);
		drawBox(1, 1, thick);
		glTranslated(0, 0, arm_z * unit_block + thick);
		glScaled(1, 1, 1);
		drawBox(1, 1, thick);
		glPopMatrix();
		glPushMatrix();
		glTranslated(-1 * thick, 0, -1 * thick);
		glScaled(1, sleeve_y * unit_block, arm_z * unit_block);
		drawBox(thick, 1, 1);
		glTranslated(arm_x * unit_block + thick, 0, 0);
		glScaled(1, 1, 1);
		drawBox(thick, 1, 1);
		glPopMatrix();

		// left lower leg
		glPushMatrix();
		glTranslated(0, -1 * arm_y * unit_block, 0);
		// lower leg rotation implementation
		glTranslated(arm_x / 2 * unit_block, arm_y * unit_block, arm_z * unit_block);
		glRotated(VAL(LEFT_LOWER_LEG_ROTATION_X), 1.0, 0.0, 0.0);
		glTranslated(-1 * arm_x / 2 * unit_block, -1 * arm_y * unit_block, -1 * arm_z * unit_block);

		// left foot
		glPushMatrix();
		setDiffuseColor(COLOR_GRAY);
		glTranslated((arm_x - foot_x) / 2 * unit_block, -1 * foot_y * unit_block, (arm_z - foot_z) / 2 * unit_block);
		// foot rotation implementation
		glTranslated(foot_x / 2 * unit_block, foot_y * unit_block, foot_z / 2 * unit_block);
		glRotated(VAL(LEFT_FOOT_ROTATION_X), 1.0, 0.0, 0.0);
		glTranslated(-1 * foot_x / 2 * unit_block, -1 * foot_y * unit_block, -1 * foot_z / 2 * unit_block);
		glScaled(foot_x * unit_block, foot_y * unit_block, foot_z * unit_block);
		drawBox(1, 1, 1);
		glPopMatrix();

		setDiffuseColor(COLOR_SKIN);
		glScaled(arm_x * unit_block, arm_y * unit_block, arm_z * unit_block);
		drawBox(1, 1, 1);
		glPopMatrix();

		// back to leg
		setDiffuseColor(COLOR_BROWN);
		glScaled(arm_x * unit_block, arm_y * unit_block, arm_z * unit_block);
		drawBox(1, 1, 1);
		glPopMatrix();


		// draw right leg
		glPushMatrix();
		glTranslated(body_x / 2 * unit_block + (body_x / 2 - arm_x) / 2 * unit_block, -1 * arm_y * unit_block, 0);

		// WHOLE LEG ROTATION IMPLEMENTATION
		glTranslated(arm_x * unit_block, arm_y * unit_block, arm_z / 2 * unit_block);
		glRotated(VAL(RIGHT_UPPER_LEG_ROTATION_X), 1.0, 0.0, 0.0);
		glRotated(VAL(RIGHT_UPPER_LEG_ROTATION_Y), 0.0, 1.0, 0.0);
		glRotated(VAL(RIGHT_UPPER_LEG_ROTATION_Z), 0.0, 0.0, 1.0);
		glTranslated(-1 * arm_x * unit_block, -1 * arm_y * unit_block, -1 * arm_z / 2 * unit_block);

		// sleeve
		setDiffuseColor(COLOR_DARK);
		glPushMatrix();
		glTranslated(0, 0, -1 * thick);
		glScaled(arm_x * unit_block, sleeve_y * unit_block, 1);
		drawBox(1, 1, thick);
		glTranslated(0, 0, arm_z * unit_block + thick);
		glScaled(1, 1, 1);
		drawBox(1, 1, thick);
		glPopMatrix();
		glPushMatrix();
		glTranslated(-1 * thick, 0, -1 * thick);
		glScaled(1, sleeve_y * unit_block, arm_z * unit_block);
		drawBox(thick, 1, 1);
		glTranslated(arm_x * unit_block + thick, 0, 0);
		glScaled(1, 1, 1);
		drawBox(thick, 1, 1);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, -1 * arm_y * unit_block, 0);
		// lower leg rotation implementation
		glTranslated(arm_x / 2 * unit_block, arm_y * unit_block, arm_z * unit_block);
		glRotated(VAL(RIGHT_LOWER_LEG_ROTATION_X), 1.0, 0.0, 0.0);
		glTranslated(-1 * arm_x / 2 * unit_block, -1 * arm_y * unit_block, -1 * arm_z * unit_block);

		// right foot
		glPushMatrix();
		setDiffuseColor(COLOR_GRAY);
		glTranslated((arm_x - foot_x) / 2 * unit_block, -1 * foot_y * unit_block, (arm_z - foot_z) / 2 * unit_block);
		// foot rotation implementation
		glTranslated(foot_x / 2 * unit_block, foot_y * unit_block, foot_z / 2 * unit_block);
		glRotated(VAL(RIGHT_FOOT_ROTATION_X), 1.0, 0.0, 0.0);
		glTranslated(-1 * foot_x / 2 * unit_block, -1 * foot_y * unit_block, -1 * foot_z / 2 * unit_block);
		glScaled(foot_x * unit_block, foot_y * unit_block, foot_z * unit_block);
		drawBox(1, 1, 1);
		glPopMatrix();

		setDiffuseColor(COLOR_SKIN);
		glScaled(arm_x * unit_block, arm_y * unit_block, arm_z * unit_block);
		drawBox(1, 1, 1);
		glPopMatrix();

		// back to leg
		setDiffuseColor(COLOR_BROWN);
		glScaled(arm_x * unit_block, arm_y * unit_block, arm_z * unit_block);
		drawBox(1, 1, 1);
		glPopMatrix();


		// back body
		setDiffuseColor(COLOR_CYAN);
		glScaled(body_x * unit_block, body_y * unit_block, body_z * unit_block);
		drawBox(1, 1, 1);

		glPopMatrix();
		/*
		glPushMatrix();
		glRotated(1, 0.0, 1.0, 0.0);
		glRotated(-90, 1.0, 0.0, 0.0);
		drawCylinder(2, 0.1, 0.1);

		glTranslated(0.0, 0.0, 1);
		drawCylinder(1, 1.0, 0.9);

		glTranslated(0.0, 0.0, 1);
		glRotated(90, 1.0, 0.0, 0.0);
		drawCylinder(1, 0.1, 0.2);
		glPopMatrix();
		*/
	glPopMatrix();
}

int main()
{
	initLSystem();

	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
    ModelerControl controls[NUMCONTROLS];
	initControls(controls);

    ModelerApplication::Instance()->Init(&createMyCreature, controls, NUMCONTROLS);
    return ModelerApplication::Instance()->Run();
}

void initControls(ModelerControl* controls)
{
	controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
	controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
	controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);

	controls[LIGHT0_X] = ModelerControl("Light0 X", -10, 10, 0.1f, 4);
	controls[LIGHT0_Y] = ModelerControl("Light0 Y", -10, 10, 0.1f, 2);
	controls[LIGHT0_Z] = ModelerControl("Light0 Z", -10, 10, 0.1f, -4);
	controls[LIGHT0_DIFFUSE] = ModelerControl("Light0 Diffuse", 0, 5, 0.01f, 1);

	controls[LIGHT1_X] = ModelerControl("Light1 X", -10, 10, 0.1f, -2);
	controls[LIGHT1_Y] = ModelerControl("Light1 Y", -10, 10, 0.1f, 1);
	controls[LIGHT1_Z] = ModelerControl("Light1 Z", -10, 10, 0.1f, 5);
	controls[LIGHT1_DIFFUSE] = ModelerControl("Light1 Diffuse", 0, 5, 0.01f, 1);
	controls[ENABLE_LIGHT] = ModelerControl("Enable lights", 0, 1, 1, 0);

	controls[HEAD_ROTATION_X] = ModelerControl("Head rotation X", -30, 30, 0.01f, 0);
	controls[HEAD_ROTATION_Y] = ModelerControl("Head rotation Y", -60, 60, 0.01f, 0);
	controls[HEAD_ROTATION_Z] = ModelerControl("Head rotation Z", -30, 30, 0.01f, 0);
	controls[HEAD_WIDTH] = ModelerControl("Head width", 0.0, 2.0, 0.01f, 1.0);
	controls[HEAD_HEIGHT] = ModelerControl("Head height", 0.0, 2.0, 0.01f, 1.0);
	controls[HEAD_DEPTH] = ModelerControl("Head depth", 0.0, 2.0, 0.01f, 1.0);

	controls[LEFT_UPPER_ARM_ROTATION_X] = ModelerControl("Right arm rotation X", -180, 50, 0.01f, 0);
	controls[LEFT_UPPER_ARM_ROTATION_Y] = ModelerControl("Right arm rotation Y", -90, 90, 0.01f, 0);
	controls[LEFT_UPPER_ARM_ROTATION_Z] = ModelerControl("Right arm rotation Z", -180, 30, 0.01f, 0);
	controls[LEFT_LOWER_ARM_ROTATION_X] = ModelerControl("Right lower arm rotation X", -180, 0, 0.01f, 0);
	controls[RIGHT_UPPER_ARM_ROTATION_X] = ModelerControl("Left arm rotation X", -180, 50, 0.01f, 0);
	controls[RIGHT_UPPER_ARM_ROTATION_Y] = ModelerControl("Left arm rotation Y", -90, 90, 0.01f, 0);
	controls[RIGHT_UPPER_ARM_ROTATION_Z] = ModelerControl("Left arm rotation Z", -30, 180, 0.01f, 0);
	controls[RIGHT_LOWER_ARM_ROTATION_X] = ModelerControl("Left lower arm rotation X", -180, 0, 0.01f, 0);

	controls[LEFT_UPPER_LEG_ROTATION_X] = ModelerControl("Right leg rotation X", -120, 50, 0.01f, 0);
	controls[LEFT_UPPER_LEG_ROTATION_Y] = ModelerControl("Right leg rotation Y", -90, 90, 0.01f, 0);
	controls[LEFT_UPPER_LEG_ROTATION_Z] = ModelerControl("Right leg rotation Z", -180, 180, 0.01f, 0);
	controls[LEFT_LOWER_LEG_ROTATION_X] = ModelerControl("Right lower leg rotation X", 0, 120, 0.01f, 0);
	controls[LEFT_FOOT_ROTATION_X] = ModelerControl("Right foot rotation X", -30, 30, 0.01f, 0);
	controls[RIGHT_UPPER_LEG_ROTATION_X] = ModelerControl("Left leg rotation X", -120, 50, 0.01f, 0);
	controls[RIGHT_UPPER_LEG_ROTATION_Y] = ModelerControl("Left leg rotation Y", -90, 90, 0.01f, 0);
	controls[RIGHT_UPPER_LEG_ROTATION_Z] = ModelerControl("Left leg rotation Z", -180, 180, 0.01f, 0);
	controls[RIGHT_LOWER_LEG_ROTATION_X] = ModelerControl("Left lower leg rotation X", 0, 120, 0.01f, 0);
	controls[RIGHT_FOOT_ROTATION_X] = ModelerControl("Left foot rotation X", -30, 30, 0.01f, 0);

	controls[WAIST_ROTATION_X] = ModelerControl("Waist rotation X", -90, 90, 0.01f, 0);
	controls[WAIST_ROTATION_Y] = ModelerControl("Waist rotation Y", -90, 90, 0.01f, 0);
	controls[WAIST_ROTATION_Z] = ModelerControl("Waist rotation Z", -30, 30, 0.01f, 0);

	controls[DRAW_LEVEL] = ModelerControl("Level of details", 0, 5, 1, 5);
	controls[DRAW_TEXTURE] = ModelerControl("Draw texture", 0, 1, 1, 0);

	controls[DISPLAY_LSYSTEM] = ModelerControl("Display LSystem", 0, 1, 1, 0);
	controls[LSYSTEM_TYPE] = ModelerControl("LSystem type", 1, LSystemVec->size(), 1, 1);
	controls[LSYSTEM_ITER] = ModelerControl("LSystem Iterations", 0, 5, 1, 1);
	controls[LS_LENGTH] = ModelerControl("LSystem Unit Length", 0, 5, 0.01f, 1);
}

void changeLight()
{
	double lightSensitivity = 0.1;
	double lightDimension[3] = { 0.2, 0.2, 0.2 };
	static GLfloat lightPosition0[4];
	static GLfloat lightDiffuse0[4];
	static GLfloat lightPosition1[4];
	static GLfloat lightDiffuse1[4];
	lightPosition0[0] = VAL(LIGHT0_X);
	lightPosition0[1] = VAL(LIGHT0_Y);
	lightPosition0[2] = VAL(LIGHT0_Z);
	lightPosition0[3] = 0;
	lightPosition1[0] = VAL(LIGHT1_X);
	lightPosition1[1] = VAL(LIGHT1_Y);
	lightPosition1[2] = VAL(LIGHT1_Z);
	lightPosition1[3] = 0;
	lightDiffuse0[0] = lightDiffuse0[1] = lightDiffuse0[2] = lightDiffuse0[3] = VAL(LIGHT0_DIFFUSE);
	lightDiffuse1[0] = lightDiffuse1[1] = lightDiffuse1[2] = lightDiffuse1[3] = VAL(LIGHT1_DIFFUSE);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse1);

	if (VAL(ENABLE_LIGHT))
	{
		// printf("dd%d\n", (int)VAL(ENABLE_LIGHT));
		// printf("enable the light...\n");
		setDiffuseColor(1.0, 1.0, 1.0);
		// light source 0
		glPushMatrix();
		glTranslatef(VAL(LIGHT0_X) - lightSensitivity,
			VAL(LIGHT0_Y) - lightSensitivity, VAL(LIGHT0_Z) - lightSensitivity);
		drawBox(lightDimension[0], lightDimension[1], lightDimension[2]);
		glPopMatrix();
		// light source 1
		glPushMatrix();
		glTranslatef(VAL(LIGHT1_X) - lightSensitivity,
			VAL(LIGHT1_Y) - lightSensitivity, VAL(LIGHT1_Z) - lightSensitivity);
		drawBox(lightDimension[0], lightDimension[1], lightDimension[2]);
		glPopMatrix();
	}
}

void MyCreature::createTexture(int _width)
{
	static int width = 0;
	static int height = 0;

	ModelerUserInterface* mui = ModelerApplication::Instance()->getModelerUI();
	if (textureImage == NULL)
	{
		unsigned char *image;
		if (mui->textureFile == NULL || (image = readBMP(mui->textureFile, width, height)) == NULL)
		{
			fl_alert("Can't load bitmap file...");
			ModelerApplication::Instance()->SetControlValue(DRAW_TEXTURE, 0);
			return;
		}
		textureImage = image;
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glShadeModel(GL_FLAT);
		glEnable(GL_DEPTH_TEST);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &texName);
		glBindTexture(GL_TEXTURE_2D, texName);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if (_width <= 0) _width = width;
	if (width > 0 && height > 0)
	{
		double hwfactor = height / (double)width;
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glBindTexture(GL_TEXTURE_2D, texName);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.0, _width * hwfactor, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(_width, _width * hwfactor, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(_width, 0.0, 0.0);
		glEnd();
		glFlush();
		glDisable(GL_TEXTURE_2D);
	}
}

void initLSystem()
{
	list<int> base_path = list<int>{ A };
	LSystemVec = new vector<LSystem*>;
	map<int, list<int> > rules;
	// rules[A] = list<int>{ A, B, A, C};
	// rules[A] = list<int>{ A, B, A, C, A};
	rules[A] = list<int>{ A, B, A, C, A, B, A };
	map<int, pair<int, int> > nodeOperation;
	nodeOperation[A] = make_pair(F, 1);
	// nodeOperation[B] = make_pair(RR, 60);
	nodeOperation[B] = make_pair(RL, 60);
	nodeOperation[C] = make_pair(RR, 120);
	LSystemVec->push_back(new LSystem(rules, base_path, nodeOperation));

	rules[A] = list<int>{ A, B, A, C, A, B, A, D};
	nodeOperation[B] = make_pair(RR, 60);
	nodeOperation[C] = make_pair(RR, 120);
	nodeOperation[D] = make_pair(RR, 180);
	LSystemVec->push_back(new LSystem(rules, base_path, nodeOperation));
	
	rules[A] = list<int>{ B, C, A, D, A };
	rules[B] = list<int>{ B, B };
	nodeOperation[0] = make_pair(F, 1);
	nodeOperation[1] = make_pair(F, 1);
	nodeOperation[2] = make_pair(RL_SPLIT_PUSH, 45);
	nodeOperation[3] = make_pair(RR_POP_SPLIT, 45);
	LSystemVec->push_back(new LSystem(rules, base_path, nodeOperation));
	// LSystemVec->push_back();
}

void drawLSystem()
{
	setDiffuseColor(0.13f, 0.694f, 0.298f);
	int LSystemSelection = (int)VAL(LSYSTEM_TYPE);
	if (LSystemSelection >= 1)
	{
		glPushMatrix();
		glTranslated(-3, 0, -3);
		glRotated(90, -1, 0, 0);

		LSystem* ls = (*LSystemVec)[LSystemSelection - 1];
		ls->generatePath((int)VAL(LSYSTEM_ITER));
		ls->setForwardLength((int)VAL(LS_LENGTH));
		for (std::list<int>::iterator it = ls->path->begin(); it != ls->path->end(); it++)
		{
			if (ls->nodeOperation.find(*it) != ls->nodeOperation.end())
			{
				pair<int, int> operation = ls->nodeOperation[*it];
				switch (operation.first)
				{
				case F:
					drawBox(0.05 * operation.second, 0.02, 0.02);
					glTranslated(0.05 * operation.second, 0, 0);
					break;
				case RL:
					glRotated(-operation.second, 0, 1, 0);
					break;
				case RR:
					glRotated(operation.second, 0, 1, 0);
					break;
				case RL_SPLIT_PUSH:
					glRotated(-operation.second, 0, 1, 0);
					glPushMatrix();
					break;
				case RR_POP_SPLIT:
					glPopMatrix();
					glRotated(operation.second, 0, 1, 0);
					break;
				default:
					break;
				}
			}
		}
		glPopMatrix();
	}
}