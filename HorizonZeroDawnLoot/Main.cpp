#include <glut.h>
#include <math.h>
#include <SOIL2.h>
#include <stdio.h>

// Camera position
GLfloat camX = 0.0;
GLfloat camY = 0.0;
GLfloat camZ = 0.0;

// Scene translation and rotation
GLfloat sceRX = 0.0;
GLfloat sceRY = 0.0;
GLfloat sceRZ = 0.0;
GLfloat sceTX = 0.0;
GLfloat sceTY = 0.0;
GLfloat sceTZ = 0.0;

// Object rotation and scene translation
double objR = 0.0;
double sceneX = 0.0;
double sceneZ = 0.0;
double rotateSceneY = 0.0;

// Window dimensions
int width;
int height;
int width_sand;
int height_sand;

// Constants
constexpr float PI = 3.14159265358979323846;

// Texture data
unsigned char* firelog;
unsigned char* land;
GLuint tex, tex1;
GLuint textureID, textureIDRock, fireIconTexture;

#define IMAGE_ROWS 64
#define IMAGE_COLS 64

GLubyte imageData[IMAGE_ROWS][IMAGE_COLS][3];

void loadTextureDataFromImage() {
	land = SOIL_load_image("land.jpg", &width_sand, &height_sand, 0, SOIL_LOAD_RGB);

	if (firelog == NULL || land == NULL) {
		printf("Error : %s", SOIL_last_result());
	}
}

void loadSandTexture() {
	loadTextureDataFromImage();
	glGenTextures(1, &tex1);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, IMAGE_COLS, IMAGE_ROWS, 0, GL_RGB,
		GL_UNSIGNED_BYTE, land);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

void loadTexture() {
	textureID = SOIL_load_OGL_texture(
		"log.jpg",  
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
	);

	if (!textureID) {
		printf("Texture loading failed: %s\n", SOIL_last_result());
	}
}

void loadTextureRock() {
	textureIDRock = SOIL_load_OGL_texture(
		"rock.jpg",  
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
	);

	if (!textureIDRock) {
		printf("Texture loading failed: %s\n", SOIL_last_result());
	}
}

void loadFireIcon() {
	fireIconTexture = SOIL_load_OGL_texture(
		"fire.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);

	if (!fireIconTexture) {
		printf("Fire icon loading failed: %s\n", SOIL_last_result());
	}
}


void init(void) {
	glClearColor(0.14, 0.58, 0.75, 1.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
}

void setLighting() {
	// Lighting set up
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	// Set lighting intensity and color
	GLfloat qaAmbientLight[] = { 0.7, 0.7, 0.6, 1.0 };
	GLfloat qaDiffuseLight[] = { 0.8, 0.8, 0.7, 1.0 };
	GLfloat qaSpecularLight[] = { 1.0, 1.0, 1.0, 1.0 };


	glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

	// Set the light position
	GLfloat qaLightPosition[] = { 200.0, -100.0, -400.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);

	//set light 1
	glLightfv(GL_LIGHT1, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT1, GL_SPECULAR, qaSpecularLight);

	//Set the light position
	GLfloat qaLightPosition2[] = { 200.0, -500.0, -400.0, 0.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, qaLightPosition2);

	//set light 3
	glLightfv(GL_LIGHT2, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT2, GL_SPECULAR, qaSpecularLight);

	//Set the light position
	GLfloat qaLightPosition3[] = { 300.0, 200.0, -400.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, qaLightPosition3);

}

void drawLog(float height) {

	GLUquadric* quad = gluNewQuadric();
	gluQuadricTexture(quad, GL_TRUE);

	glBindTexture(GL_TEXTURE_2D, textureID);

	//log body
	glPushMatrix();
	glRotated(-90.0, 1.0, 0.0, 0.0);
	gluQuadricNormals(quad, GLU_SMOOTH);
	gluCylinder(quad, 1.0, 1.0, height, 20, 10);
	glPopMatrix();

	glPushMatrix();
	gluQuadricOrientation(quad, GLU_INSIDE);
	glRotated(-90.0, 1.0, 0.0, 0.0);
	gluDisk(quad, 0.0, 1.0, 20, 1);
	glPopMatrix();

	glPushMatrix();
	gluQuadricOrientation(quad, GLU_OUTSIDE);
	glTranslatef(0.0, 8.0, 0.0);
	glRotated(-90.0, 1.0, 0.0, 0.0);
	gluDisk(quad, 0.0, 1.0, 20, 1);
	glPopMatrix();

	gluDeleteQuadric(quad);
}


void drawCampLogSection(float height,float circleRadius) {
	const int numLogs = 2; // Number of logs in the circular arrangement

	float x_rot, z_rot, rot_angle;
	for (int i = 0; i < numLogs; ++i) {


		float angle = (360.0 / numLogs) * i;
		float x = circleRadius * cos(angle * PI / 180.0);
		float y = circleRadius * sin(angle * PI / 180.0);


		glPushMatrix();
		glTranslatef(x, 0.0, y);
		glPushMatrix();
		if (i == 0) {
			x_rot = 0.0;
			z_rot = 1.0;
			rot_angle = 20.0;
		}
		else {
			x_rot = 0.0;
			z_rot = 1.0;
			rot_angle = -20.0;
		}
		glRotated(rot_angle, x_rot, 0.0, z_rot);
		drawLog(height);
		glPopMatrix();

		glPopMatrix();

	}
}

void drawCampFireLogs(float height,float circleRadius, int numLogs) {

	for (int i = 0; i < numLogs; ++i) {


		float angle = (360.0 / numLogs) * i;
		float x = circleRadius * cos(angle * PI / 180.0);
		float y = circleRadius * sin(angle * PI / 180.0);


		glPushMatrix();
		glRotatef(angle,0.0, 1.0, 0.0);

		glPushMatrix();
		drawCampLogSection(height, circleRadius);
		glPopMatrix();

		glPopMatrix();

	}
}

void drawCampFire() {
	drawCampFireLogs(16.0, 8.0,10);

	glPushMatrix();
	glRotatef(45, 0.0, 1.0, 0.0);
	drawCampFireLogs(15.0, 8.0,6);
	glPopMatrix();

	glPushMatrix();
	glRotatef(60, 0.0, 1.0, 0.0);
	drawCampFireLogs(12.0, 8.1,6);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0.0, 1.0, 0.0);
	drawCampFireLogs(10.0, 8.2,8);
	glPopMatrix();
}

void drawRock() {

	GLUquadric* quad = gluNewQuadric();
	gluQuadricTexture(quad, GL_TRUE);

	glBindTexture(GL_TEXTURE_2D, textureIDRock);

	gluSphere(quad, 1.0, 32, 32);

	gluDeleteQuadric(quad);

}

void drawCampfireSurroundRock(float circleRadius) {
	const int numRocks = 20; // Number of logs in the circular arrangement

	for (int i = 0; i < numRocks; ++i) {

		float angle = (360.0 / numRocks) * i;
		float x = circleRadius * cos(angle * PI / 180.0);
		float y = circleRadius * sin(angle * PI / 180.0);

		glPushMatrix();
		glTranslatef(x, 0.0, y);
		glScaled(2.0, 1.0, 2.0);
		drawRock();
		glPopMatrix();

	}
}

void drawLootLine() {

	glLineWidth(1.0);

	glPushMatrix();
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0, 1.0, 1.0);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0, 24.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glEnd();
	glPopMatrix();
}

void drawCircle(float radius, int n) {

	glLineWidth(1.0);
	glPushMatrix();
	glTranslatef(0.0, 25.5, 0.0);
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0, 1.0, 1.0);
	for (int i = 0; i <= n; ++i) {
		float angle = 2 * PI * i / n;
		float x = radius * cos(angle);
		float y = radius * sin(angle);

		// Calculate the normal for each vertex
		glNormal3f(x, 0.0f, y);

		glVertex2d(x, y);  // Vertex i
	}
	glEnd();
	glPopMatrix();

}


void drawCircleTransparent(float radius, int n) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	glTranslatef(0.0, 25.5, 0.0);
	glBegin(GL_POLYGON);
	glColor4f(1.0, 1.0, 1.0 ,0.5);
	for (int i = 0; i <= n; ++i) {
		float angle = 2 * PI * i / n;
		float x = radius * cos(angle);
		float y = radius * sin(angle);

		// Calculate the normal for each vertex
		glNormal3f(x, 0.0f, y);

		glVertex2d(x, y);  // Vertex i
	}
	glEnd();
	glPopMatrix();

	glDisable(GL_BLEND);

}

void displayFireIco() {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, fireIconTexture);

	glBegin(GL_QUADS);
	glColor3f(0.0, 0.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.5f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(1.5f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(1.5f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.5f, 1.0f);
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void drawLand() {
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-100, 0.0, 100);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-100, 0.0, -100);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(100, 0.0, -100);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(100, 0.0, 100);
	glEnd();
}

void drawCampfireWithLighting() {
	glShadeModel(GL_SMOOTH);

	// Set material properties
	GLfloat qaWhite[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat qaBrownLog[] = { 0.51, 0.36, 0.30, 1.0 };
	GLfloat qaBrownDiffuse[] = { 0.20, 0.6, 0.47, 1.0 };

	GLfloat qaRock[] = { 0.37, 0.35, 0.42, 1.0 };
	GLfloat qaRockDiffuse[] = { 0.38, 0.35, 0.40, 1.0 };

	GLfloat qaSky[] = { 0.14, 0.58, 0.75, 1.0 };
	
	GLfloat qaSand[] = { 0.95, 0.73, 0.24, 1.0 };
	GLfloat qaSandDark[] = { 0.85, 0.73, 0.24, 1.0 };
	GLfloat qaLowAmbient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat qaFullAmbient[] = { 1.0, 1.0, 1.0, 1.0 };

	//Set, ambient, diffuse and specular lighting. Set ambient to 20%.

	glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaLowAmbient);
	glLightfv(GL_LIGHT1, GL_AMBIENT, qaLowAmbient);
	glLightfv(GL_LIGHT2, GL_AMBIENT, qaLowAmbient);

	//start loot mark
	glEnable(GL_LIGHTING);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaWhite);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaWhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 200.0);
	drawLootLine();
	drawCircle(1.5, 32);

	//start fire icon
	glEnable(GL_TEXTURE_2D);
	loadFireIcon();
	glPushMatrix();
	glTranslatef(0.0,25.5,0.0);
	displayFireIco();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_LIGHTING);
	drawCircleTransparent(1.5, 32);

	//end fire icon
	
	//start camp fire log
	glEnable(GL_TEXTURE_2D);
	loadTexture();

	glEnable(GL_LIGHTING);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaBrownLog);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaBrownDiffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, 20.0);
	glShadeModel(GL_FLAT);

	drawCampFire();

	glDisable(GL_LIGHTING);

	glDisable(GL_TEXTURE_2D);

	//rock
	glEnable(GL_TEXTURE_2D);
	loadTextureRock();

	glEnable(GL_LIGHTING);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaRock);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaRockDiffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, 20.0);
	glShadeModel(GL_FLAT);

	drawCampfireSurroundRock(8.5);
	drawCampfireSurroundRock(8.8);

	glDisable(GL_LIGHTING);

	glDisable(GL_TEXTURE_2D);

	//land with texture
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	loadSandTexture();

	glEnable(GL_LIGHTING);

	glShadeModel(GL_FLAT);
	glMaterialfv(GL_FRONT, GL_AMBIENT, qaSand);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaSand);
	glMaterialf(GL_FRONT, GL_SHININESS, 10.0);

	drawLand();

	glDisable(GL_LIGHTING);

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	// camera orientation (eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ)
	gluLookAt(0.0,5.0 + camY, 25.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// move the scene (all the rendered environment) using keyboard keys
	glTranslatef(sceTX, sceTY, sceTZ);
	setLighting();

	glPushMatrix();
	glRotatef(objR, 0.0, 1.0, 0.0);
	drawCampfireWithLighting();
	glPopMatrix();

	glPopMatrix();
	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	GLfloat aspect_ratio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Define the Perspective projection frustum
	// (FOV_in_vertical, aspect_ratio, z-distance to the near plane from the camera position, z-distance to far plane from the camera position)
	gluPerspective(120.0, aspect_ratio, 1.0, 100.0);
}

void keyboardSpecial(int key, int x, int y) {
	if (key == GLUT_KEY_UP)
		camY += 1;

	if (key == GLUT_KEY_DOWN)
		camY -= 1;

	if (key == GLUT_KEY_RIGHT)
		sceTX += 1;

	if (key == GLUT_KEY_LEFT)
		sceTX -= 1;

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	//obj rotate clockwise
	if (key == 'l') {
		objR++;
	}
	//obj rotate anti-clockwise
	if (key == 'r') {
		objR--;
	}
	if (key == 'z') {
		sceneZ++;
	}
	if (key == 'Z') {
		sceneZ--;
	}

	if (key == 'y') {
		camY++;
	}
	if (key == 'Y') {
		camY--;
	}

	if (key == 'v') {
		rotateSceneY++;
	}
	if (key == 'V') {
		rotateSceneY--;
	}

	glutPostRedisplay();
}


int main(void) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1080, 720);
	glutInitWindowPosition(145, 0);
	glutCreateWindow("Horizon ZeroDawn Loot");
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	init();
	glutMainLoop();
	return 0;
}