//standard libraries
#include <iostream>
using namespace std;

//opengl headers
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL\GLAux.h>
#include "l3ds.h" // Header file for the L3ds Library
//#include "l3ds.cpp" // Cpp file for the L3ds Library



#pragma comment(lib, "OpenGL32.Lib")
#pragma comment(lib, "GLU32.lib")


//opengl mathematics
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

//functions for shader compilation and linking
#include "shaderhelper.h"
//object for drawing
#include "Branch.h"
#include "Leaf.h"
#include "konus.h"
#include <time.h>
#include "tree.h"



#define Pi 3.1415
#define start_radius 15
#define step_radius 0.04


//model for drawing: a square from two triangles
tree* ptree;
vector <tree*> les;

bool fullscreen = true;

int look_tree=1;

//window size
int windowWidth = 800;
int windowHeight = 600;
int windowWidth_copy = 800;
int windowHeight_copy = 600;



//last mouse coordinates
int mouseX,mouseY;

double angleXZ=0, angleYZ=-2*Pi*300, rotationX=0.0, rotationY=0.0,
	rotationZ=0.0, radius = start_radius, speed = 1;

float deltaAngle = 0.0f,deltaAngle_y = 0.0f;
int xOrigin = -1;
int yOrigin = -1;
float angle=0.25,angle_y=1.49;
float lx=0,lz=-1,ly=0;
float x=-3.02273f, y=2.06351f, z=15.4476f;


//camera position
glm::vec3 eye(start_radius,0,0);
//reference point position
glm::vec3 cen(0.0,0,0);
//up vector direction (head of observer)
glm::vec3 up(0,1,0);

//matrices
glm::mat4x4 modelMatrix;
glm::mat4x4 modelViewMatrix;
glm::mat4x4 projectionMatrix;
glm::mat4x4 modelViewProjectionMatrix;
glm::mat4x4 normalMatrix;



/////////////////////////////////////////////////////////////////////
///is called when program starts
void init()
{
	srand( time( NULL ) );
	//enable depth test
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
	initTexture();
	ptree = new tree();
	ptree->build(glm::vec3(0,0,0));	

	les.push_back(new tree());les.push_back(new tree());les.push_back(new tree()); les.push_back(new tree());
	les[0]->osnovnoe = 0; les[1]->osnovnoe = 0; les[2]->osnovnoe = 0; les[3]->osnovnoe = 0;
	les[0]->build(glm::vec3(5,0,5));  les[1]->build(glm::vec3(-5,0,-5));  les[2]->build(glm::vec3(5,0,-5));	les[3]->build(glm::vec3(-5,0,5));
	les[0]->MAX_EL_svoy = 150;les[1]->MAX_EL_svoy = 250;les[2]->MAX_EL_svoy = 230;les[3]->MAX_EL_svoy = 190;
	//test
	/*for (int ii=0; ii<5000; ii++)
	{
		les[3]->rost();
		les[1]->rost();
		les[2]->rost();
		les[0]->rost();
	}*/
	//test

	// start eye
	rotationZ=sin(angleXZ)*cos(angleYZ);
	rotationX=cos(angleXZ)*cos(angleYZ);			
	rotationY=sin(angleYZ);
	eye = glm::vec3((radius)*rotationX,(radius)*rotationY,(radius)*rotationZ);
	cen = glm::vec3(0.0,0,0);
	up = glm::vec3(0,1,0);


	

}


/////////////////////////////////////////////////////////////////////
///called when window size is changed
void reshape(int width, int height)
{
  windowWidth = width;
  windowHeight = height;
  //set viewport to match window size
  glViewport(0, 0, width, height);
  
  float fieldOfView = 45.0f;
  float aspectRatio = float(width)/float(height);
  float zNear = 0.1f;
  float zFar = 100.0f;
  //set projection matrix
  projectionMatrix = glm::perspective(fieldOfView,aspectRatio,zNear,zFar);
}


////////////////////////////////////////////////////////////////////
///actions for single frame
void display()
{
	if (look_tree==1)
	{
		 angleXZ+=2*Pi*speed*10;
		rotationZ=sin(angleXZ)*cos(angleYZ);
		rotationX=cos(angleXZ)*cos(angleYZ);			
		rotationY=sin(angleYZ);
		eye = glm::vec3((radius)*rotationX,(radius)*rotationY,(radius)*rotationZ);
	}
	if (look_tree==1) cen = glm::vec3(0,0,0);

	else { 

		cen = glm::vec3(lx+x,ly+y,lz+z);
		eye = glm::vec3(x,y,z);
	}

		up = glm::vec3(0,1,0);

  glm::mat4x4 viewMatrix = glm::lookAt(eye,cen,up);

  
 glm::vec3 light = glm::vec3(0,4.0f,6.0f);
 double cos_a;
 if (look_tree==1){
		 cos_a = (light.x*eye.x+light.z*eye.z)/(sqrt((light.x*light.x+light.z*light.z)*(eye.x*eye.x+eye.z*eye.z)));
		 cos_a = -cos_a;
		 cos_a+=1;
		 cos_a/=2;
 }
 else{
		cos_a = (light.x*x+light.z*z)/(sqrt((light.x*light.x+light.z*light.z)*(x*x+z*z)));
		 cos_a = -cos_a;
		 cos_a+=1;
		 cos_a/=2;
 }

 glClearColor(cos_a,cos_a,0.3,1);
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
 
 




  
  //les
  les[0]->rost(); les[1]->rost(); les[2]->rost();  les[3]->rost();
  les[0]->drow(viewMatrix, modelViewMatrix, projectionMatrix, modelViewProjectionMatrix, normalMatrix,eye);
  les[1]->drow(viewMatrix, modelViewMatrix, projectionMatrix, modelViewProjectionMatrix, normalMatrix,eye);
  les[2]->drow(viewMatrix, modelViewMatrix, projectionMatrix, modelViewProjectionMatrix, normalMatrix,eye);
  les[3]->drow(viewMatrix, modelViewMatrix, projectionMatrix, modelViewProjectionMatrix, normalMatrix,eye);
  

  ptree->rost();
  ptree->drow(viewMatrix, modelViewMatrix, projectionMatrix, modelViewProjectionMatrix, normalMatrix,eye);


  
  //end frame visualization
  glutSwapBuffers();
  
}

//////////////////////////////////////////////////////////////////////////
///IdleFunction
void update()
{
	//make animation
	glutPostRedisplay();
}


void keyboard(unsigned char key, int mx, int my)
{
	if (key=='g') ptree->gull_see = 1 - ptree->gull_see;

	if (look_tree==1)
	{
		if (key=='d')
		{
				angleXZ=angleXZ+2*Pi*300;
		}
		if (key=='a')
		{

				angleXZ=angleXZ-2*Pi*300;
		}
		if (key=='w')
		{
			
				auto cop = angleYZ;
				angleYZ=angleYZ+2*Pi*100;
				if (cos(angleYZ)*cos(cop)<0)
					angleYZ= cop;
		}
		if (key=='s')
		{
				auto cop = angleYZ;
				angleYZ=angleYZ-2*Pi*100;
				if (cos(angleYZ)*cos(cop)<0)
					angleYZ= cop;
		}

				rotationZ=sin(angleXZ)*cos(angleYZ);
				rotationX=cos(angleXZ)*cos(angleYZ);			
				rotationY=sin(angleYZ);

		if (key=='e')
		{
			if (radius<45){
				if (radius/40>step_radius)
					radius+=radius/40;
				else radius+=step_radius;
			}
		}
		if (key=='q')
		{
			if (radius/40>step_radius)
				radius-=radius/40;
			else radius-=step_radius;
		}
   }
	else{
		if (key=='e')
			{
				if ( x<30+lx && x>-30+lx ) x -= 1 * lx * 0.17f;
				if ( z<30+lz && z>-30+lz ) z -= 1 * lz * 0.17f;
				if ( y<30+ly && y>-30+ly ) y -= 1 * ly * 0.17f;
			}
		if (key=='q')
			{
				if ( x<30-lx && x>-30-lx ) x += 1 * lx * 0.17f;
				if ( z<30-lz && z>-30-lz ) z += 1 * lz * 0.17f;
				if ( y<30-ly && y>-30-ly ) y += 1 * ly * 0.17f;
			}


	}


	if (key==' ')
	{
		ptree->frust();
	}
	if (key>='0' && key<='9')
	{
			if (speed) speed = (int)(key-'0')*speed/fabs(speed);
			else speed = (int)(key-'0');
	}
	if (key=='-')
	{
			speed = -speed;
	}
	if (key=='+')
	{
		look_tree++;
		if (look_tree==2) { look_tree=0; }
	}
	if (key=='`')
	{
		if(!fullscreen){
			windowWidth_copy=windowWidth; windowHeight_copy= windowHeight;
			glutFullScreen();
			fullscreen = true;
		} 
		else if(fullscreen){
			glutReshapeWindow(windowWidth_copy, windowHeight_copy);
			glutPositionWindow(0,0);
			fullscreen = false;
		}
		//ptree->season_st();
	}

}




void mouse(int button, int mode,int posx, int posy)
{
	if (look_tree==0) if (button==GLUT_LEFT_BUTTON)
	{
		if (mode == GLUT_DOWN)
		{
			xOrigin = posx;
			yOrigin = posy;
		}
		else  {
			angle += deltaAngle;
			xOrigin = -1;
			angle_y += deltaAngle_y;
			xOrigin = -1;
		}
	}
}

void mouseMove(int x, int y) {
 
	if (look_tree==0) {
		if (xOrigin >= 0) {
 
			// обновить deltaAngle
			deltaAngle = (x - xOrigin) * 0.003f;
			deltaAngle_y = (y - yOrigin) * 0.003f;
			// Обновление направления камеры
			lx = sin(angle + deltaAngle)*sin(angle_y + deltaAngle_y);
			lz = -cos(angle + deltaAngle)*sin(angle_y + deltaAngle_y);
			ly = cos(angle_y + deltaAngle_y);
		}
	}
}





////////////////////////////////////////////////////////////////////////
///this function is used in case of InitializationError
void emptydisplay()
{
}

////////////////////////////////////////////////////////////////////////
///entry point
int main (int argc, char* argv[])
{
  glutInit(&argc, argv);
#ifdef __APPLE__
  glutInitDisplayMode( GLUT_3_2_CORE_PROFILE| GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#else
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitContextVersion (4, 2);  
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutInitContextFlags (GLUT_FORWARD_COMPATIBLE);
  glewExperimental = GL_TRUE;
#endif

  glutCreateWindow("Воропаев Сергей 311гр. 'tree_planet'");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutReshapeWindow(windowWidth,windowHeight);
  glutFullScreen();
  glutIdleFunc(update);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(mouseMove);



  glewInit();
  const char * slVer = (const char *) glGetString ( GL_SHADING_LANGUAGE_VERSION );
  cout << "GLSL Version: " << slVer << endl;

  try
  {
	init();
  }
  catch (const char *str)
  {
	  cout << "Error During Initialiation: " << str << endl;
	  delete ptree;
	  //start main loop with empty screen
	  glutDisplayFunc(emptydisplay);
	  glutMainLoop();
	  return -1;
  }


  try
  {
	glutMainLoop();
  }
  catch (const char *str)
  {
	  cout << "Error During Main Loop: " << str << endl;
  }
  //release memory
  delete ptree;
  return 0;
}
