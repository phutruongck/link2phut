#include "stdafx.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"

#include <math.h>
#define PI 3.14159265f

char title[] = "Bouncing Ball (2D)";
int windowWidth = 640;
int windowHeight = 480;
int windowPosX = 50;
int windowPosY = 50;

GLfloat ballRadius = 0.5f; 
GLfloat ballX = 0.0f;
GLfloat ballY = 0.0f; 
GLfloat ballXMax, ballXMin, ballYMax, ballYMin;
GLfloat xSpeed = 0.02f;  
GLfloat ySpeed = 0.007f; 
int refreshMillis = 30;

GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;


void initGL() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
}
	
void display() {
	glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer 
	glMatrixMode(GL_MODELVIEW); // To operate on the model-view matrix 
	glLoadIdentity(); // Reset model-view matrix

	glTranslatef(ballX, ballY, 0.0f); // Translate to (xPos, yPos) // Use triangular segments to form a circle

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f, 0.0f, 1.0f); // Blue
	glVertex2f(0.0f, 0.0f); // Center of circle
	int numSegments = 100;
	GLfloat angle;
	for (int i = 0; i <= numSegments; i++) { // Last vertex same as first vertex 
		angle = i * 2.0f * PI / numSegments; // 369 deg for all segments 
		glVertex2f(cos(angle) * ballRadius, sin(angle) * ballRadius);
	}
	glEnd();

	glutSwapBuffers(); // Swap front and back buffers (of double buffered mode)
								  // Animation Control - compute the location for the next refresh
	ballX += xSpeed;
	ballY += ySpeed; // Check if the ball exceeds the edges 

	if (ballX > ballXMax) {
		ballX = ballXMax;
		xSpeed = -xSpeed;
	}
	else if (ballX < ballXMin) {
		ballX = ballXMin;
		xSpeed = -xSpeed;
	}
	if (ballY > ballYMax) {
		ballY = ballYMax;
		ySpeed = -ySpeed;
	}
	else if (ballY < ballYMin) {
		ballY = ballYMin;
		ySpeed = -ySpeed;
	}
}

void reshape(GLsizei width, GLsizei height) { 
	// Compute aspect ratio of the new window
	if (height == 0) height = 1;
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(width >= height){
		clipAreaXLeft = -1.0 * aspect;
		clipAreaXRight = 1.0 * aspect;
		clipAreaYBottom = -1.0;
		clipAreaYTop = 1.0;
	}
	else {
		clipAreaXLeft = -1.0;
		clipAreaXRight = 1.0;
		clipAreaYBottom = -1.0 / aspect;
		clipAreaYTop = 1.0 / aspect;
	}
	gluOrtho2D(clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop);
	ballXMin = clipAreaXLeft + ballRadius;
	ballXMax = clipAreaXRight - ballRadius;
	ballYMin = clipAreaYBottom + ballRadius;
	ballYMax = clipAreaYTop - ballRadius;
}
/* Called back when the timer expired */ 
void Timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(refreshMillis, Timer, 0);
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(windowPosX, windowPosY);
	glutCreateWindow(title);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, Timer, 0);
	initGL();
	glutMainLoop();
	return 0;
}

