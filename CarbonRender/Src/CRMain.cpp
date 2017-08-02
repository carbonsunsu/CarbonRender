#include "..\Inc\CRGloble.h"
#include "..\Inc\CRFileReader.h"
#include "..\Inc\CRMath.h"
#include "..\Inc\CRFbxImportManager.h"

void mainDisplay()
{
	glFlush();
}

void Init()
{
	FbxImportManager::instance();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1920, 1080);
	glutCreateWindow(argv[0]);

	if (glewInit())
	{
		std::cout << "GLEW init fail" << std::endl;
	}

	glutDisplayFunc(mainDisplay);
	glutMainLoop();
}