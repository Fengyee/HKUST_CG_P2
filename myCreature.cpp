// The sample model.  You should build a file
// very similar to this for when you make your model.
#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "modelerui.h"
#include <FL/gl.h>

#include "modelerglobals.h"

// function declaration
void initControls(ModelerControl* controls);
void changeLight();

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

	// draw the floor
	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(COLOR_RED);
	glPushMatrix();
	glTranslated(-5,0,-5);
	drawBox(10,0.01f,10);
	glPopMatrix();

	// draw the sample model
	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(COLOR_GREEN);
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
	glPushMatrix();
		glTranslated(-1.5, 0, -2);
		glScaled(3, 1, 4);
		drawBox(1,1,1);
		
		glPopMatrix();

		// draw cannon
		glPushMatrix();
		glRotated(1, 0.0, 1.0, 0.0);
		glRotated(-90, 1.0, 0.0, 0.0);
		drawCylinder(2, 0.1, 0.1);

		glTranslated(0.0, 0.0,10);
		drawCylinder(1, 1.0, 0.9);

		glTranslated(0.0, 0.0, 0.5);
		glRotated(90, 1.0, 0.0, 0.0);
		drawCylinder(1, 0.1, 0.2);
		glPopMatrix();

	glPopMatrix();
}

int main()
{
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
	controls[RIGHT_UPPER_LEG_ROTATION_X] = ModelerControl("Left leg rotation X", -120, 50, 0.01f, 0);
	controls[RIGHT_UPPER_LEG_ROTATION_Y] = ModelerControl("Left leg rotation Y", -90, 90, 0.01f, 0);
	controls[RIGHT_UPPER_LEG_ROTATION_Z] = ModelerControl("Left leg rotation Z", -180, 180, 0.01f, 0);
	controls[RIGHT_LOWER_LEG_ROTATION_X] = ModelerControl("Left lower leg rotation X", 0, 120, 0.01f, 0);

	controls[WAIST_ROTATION_X] = ModelerControl("Waist rotation X", -90, 90, 0.01f, 0);
	controls[WAIST_ROTATION_Y] = ModelerControl("Waist rotation Y", -90, 90, 0.01f, 0);
	controls[WAIST_ROTATION_Z] = ModelerControl("Waist rotation Z", -30, 30, 0.01f, 0);

	controls[DRAW_LEVEL] = ModelerControl("Level of details", 0, 5, 1, 5);
	controls[DRAW_TEXTURE] = ModelerControl("Draw texture", 0, 1, 1, 0);
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