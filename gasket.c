#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

int selectedOption = 1;
int n = 15000;
int shape = 5;
float x1 = -1.0;
float x2 = 1.0;
float y1 = -1.0;
float y2 = 1.0;
float click_x = 0;
float click_y = 0;
float drag_x = 0;
float drag_y = 0;

int isPointInPentagon(float x, float y) {
	float vertices[] = {
		0.0f,  0.5f, // Top
		-0.5f, 0.15f, // Top left
		-0.3f, -0.5f, // Bottom left
		0.3f, -0.5f,  // Bottom right
		0.5f, 0.15f,  // Top right
	};

	int i, j, c = 0;
	int n = sizeof(vertices) / sizeof(vertices[0]) / 3;

	for (i = 0, j = n - 1; i < n; j = i++) {
		if (((vertices[3 * i + 1] > y) != (vertices[3 * j + 1] > y)) &&
			(x < (vertices[3 * j] - vertices[3 * i]) * (y - vertices[3 * i + 1]) / (vertices[3 * j + 1] - vertices[3 * i + 1]) + vertices[3 * i])) {
			c = !c;
		}
	}

	return c;
}

void menu(int menuOption) {
	if (menuOption == 0) selectedOption = 0;
	if (menuOption == 1) selectedOption = 1;
	if (menuOption == 2) selectedOption = 2;
	if (menuOption == 3) selectedOption = 3;
	if (menuOption == 4) selectedOption = 4;
	glutPostRedisplay();
}

void onDrag(int x, int y) {
	drag_x = (click_x - x)/250.0;
	drag_y = (click_y - y)/250.0;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(x1 + drag_x, x2 + drag_x, y1 - drag_y, y2 - drag_y);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}
 
void onClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		click_x = x;
		click_y = y;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		x1 = x1 + drag_x;
		x2 = x2 + drag_x;
		y1 = y1 - drag_y;
		y2 = y2 - drag_y;
	}
}

void myinit(void)
{
 
/* attributes */
	glEnable(GL_BLEND);
 	glClearColor(1.0, 1.0, 1.0, 0.0); /* white background */
	glColor3f(1.0, 0.0, 0.0); /* draw in red */

/* set up viewing */
/* 500 x 500 window with origin lower left */

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	//mouse
	glutMouseFunc(onClick);
	glutMotionFunc(onDrag);

	//menu creation
	void menu(int menuOption);
	glutCreateMenu(menu);
	glutAddMenuEntry("8000 points", 0);
	glutAddMenuEntry("15000 points & randomize color", 1);
	glutAddMenuEntry("Hexagon", 2);
	glutAddMenuEntry("Pentagon", 3);
	glutAddMenuEntry("exit", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	
}

void display( void ){
	
    typedef GLfloat point2[2];     
    point2 p ={75.0,50.0};
	if (selectedOption == 4) {
		exit(0);
	}
    glClear(GL_COLOR_BUFFER_BIT);  /*clear the window */

	point2 vertices[5] = {
		0.0f,  0.5f, // Top
		-0.5f, 0.15f, // Top left
		-0.3f, -0.5f, // Bottom left
		0.3f, -0.5f,  // Bottom right
		0.5f, 0.15f  // Top right
	};

	point2 vertices2[6] = {
		0.0f,  0.5f, // Top
		-0.5f, 0.25f, // Top left
		-0.5f, -0.25f, // Bottom left
		0.5f, -0.25f,  // Bottom right
		0.5f, 0.25f,  // Top right
		0.0f, -0.5f  // Bottom
	};

/* computes and plots 5000 new points */
	// Calculating q
	srand(time(NULL));
	float qx = (float)rand() / (float)RAND_MAX - 0.5; // Random x coordinate between -0.5 and 0.5
	float qy = (float)rand() / (float)RAND_MAX - 0.5; // Random y coordinate between -0.5 and 0.5
	// Ensure the generated point is inside the pentagon, if not regenerate
	while (!isPointInPentagon(qx, qy)) {

		qx = (float)rand() / (float)RAND_MAX - 0.5;
		qy = (float)rand() / (float)RAND_MAX - 0.5;
	}
	int successCount = 0;
	
	if (selectedOption == 1) {
		n = 15000;
	}
	else if (selectedOption == 0) {
		n = 8000;
	}
	else if (selectedOption == 2) {
		shape = 6;
	}
	else if (selectedOption == 3) {
		shape = 5;
	}
	glBegin(GL_POINTS);
	if (shape == 5) {
		while (successCount < n) {
			// Find point 1/3 of the distance between q and a randomly selected vertex
			if (selectedOption == 1) {
				glColor3f((rand() % 255) / 255.0, (rand() % 255) / 255.0, (rand() % 255) / 255.0);
			}
			int randomVertexIndex = rand() % 5;
			GLfloat vx = vertices[randomVertexIndex][0];
			GLfloat vy = vertices[randomVertexIndex][1];
			GLfloat px = (((1.0f / 3.0f) * qx) + ((2.0f / 3.0f) * vx));
			GLfloat py = (((1.0f / 3.0f) * qy) + ((2.0f / 3.0f) * vy));
			p[0] = px;
			p[1] = py;
			glVertex2fv(p);
			qx = px;
			qy = py;
			successCount++;
		}
	}
	else if (shape == 6) {
		while (successCount < n) {
			// Find point 1/3 of the distance between q and a randomly selected vertex
			if (selectedOption == 1) {
				glColor3f((rand() % 255) / 255.0, (rand() % 255) / 255.0, (rand() % 255) / 255.0);
			}
			int randomVertexIndex = rand() % 6;
			GLfloat vx = vertices2[randomVertexIndex][0];
			GLfloat vy = vertices2[randomVertexIndex][1];
			GLfloat px = (((1.0f / 3.0f) * qx) + ((2.0f / 3.0f) * vx));
			GLfloat py = (((1.0f / 3.0f) * qy) + ((2.0f / 3.0f) * vy));
			p[0] = px;
			p[1] = py;
			glVertex2fv(p);
			qx = px;
			qy = py;
			successCount++;
		}
	}
	else {
		printf("ERROR");
	}
	
	glEnd();
	glutSwapBuffers();
 }

void main(int argc, char** argv)
{

/* Standard GLUT initialization */

		glutInit(&argc,argv);
		glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB); /* default, not needed */
		glutInitWindowSize(500,500); /* 500 x 500 pixel window */
		glutInitWindowPosition(0,0); /* place window top left on display */
		glutCreateWindow("1st_Project"); /* window title */
		glutDisplayFunc(display); /* display callback invoked when window opened */
		glDrawBuffer(GL_BACK);
		myinit(); /* set attributes */
		glutMainLoop(); /* enter event loop */
}

