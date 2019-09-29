#include <cmath>
#include <GL/glut.h>

constexpr double PI = 3.14159265358979323846;
constexpr double TWO_PI = PI * 2;
constexpr double HALF_PI = PI / 2;

void reshape(int w, int h) {
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, double(w) / double(h), 0.1, 10.0);
}

void update() {
	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		2.0 * cos(HALF_PI * time), 1.0, 2.0 * sin(HALF_PI * time),
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0
	);
}

void renderCube(float x, float y, float z, float size) {
	const float halfSize = size / 2;

	const float x0 = x - halfSize;
	const float x1 = x + halfSize;
	const float y0 = y - halfSize;
	const float y1 = y + halfSize;
	const float z0 = z - halfSize;
	const float z1 = z + halfSize;

#define P(X, Y, Z) glVertex3f(x##X, y##Y, z##Z)
#define P0 P(0, 0, 0)
#define P1 P(0, 0, 1)
#define P2 P(0, 1, 0)
#define P3 P(0, 1, 1)
#define P4 P(1, 0, 0)
#define P5 P(1, 0, 1)
#define P6 P(1, 1, 0)
#define P7 P(1, 1, 1)

	static const float colors[6][3]{
		{1.0f, 0.0f, 0.0f},
		{0.5f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.5f, 0.0f},
		{0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 0.5f},
	};

	glBegin(GL_QUADS);
	{
		// +X
		glColor3fv(colors[0]);
		P5, P7, P6, P4;

		// -X
		glColor3fv(colors[1]);
		P0, P2, P3, P1;

		// +Y
		glColor3fv(colors[2]);
		P3, P2, P6, P7;

		// -Y
		glColor3fv(colors[3]);
		P0, P1, P5, P4;

		// +Z
		glColor3fv(colors[4]);
		P1, P3, P7, P5;

		// -Z
		glColor3fv(colors[5]);
		P4, P6, P2, P0;
	}

#undef P0
#undef P1
#undef P2
#undef P3
#undef P4
#undef P5
#undef P6
#undef P7

	glEnd();
}

void renderSponge(float x, float y, float z, float size, int depth)
{
	if (depth < 1) {
		renderCube(x, y, z, size);
		return;
	}

	// 手动写死这些相对位置，不然递归起来再用逻辑判断的话性能太低
	static const int nextCubes[20][3]{
		{-1, -1, -1},
		{-1, -1,  0},
		{-1, -1,  1},
		{ 0, -1, -1},
		{ 0, -1,  1},
		{ 1, -1, -1},
		{ 1, -1,  0},
		{ 1, -1,  1},
		{-1,  0, -1},
		{-1,  0,  1},
		{ 1,  0, -1},
		{ 1,  0,  1},
		{-1,  1, -1},
		{-1,  1,  0},
		{-1,  1,  1},
		{ 0,  1, -1},
		{ 0,  1,  1},
		{ 1,  1, -1},
		{ 1,  1,  0},
		{ 1,  1,  1}
	};

	float nextSize = size / 3;
	for (int i = 0; i < 20; i++) {
		const int* nextCube = nextCubes[i];
		renderSponge(
			x + nextCube[0] * nextSize,
			y + nextCube[1] * nextSize,
			z + nextCube[2] * nextSize,
			nextSize,
			depth - 1
		);
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderSponge(0.0f, 0.0f, 0.0f, 1.0f, 3);

	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow(argv[0]);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glutReshapeFunc(reshape);
	glutIdleFunc(update);
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}