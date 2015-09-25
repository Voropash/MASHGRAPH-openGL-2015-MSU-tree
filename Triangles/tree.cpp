#include "tree.h"
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
//#include <stdafx.h>
#include <iostream>
#include <algorithm>
#include <GL\GLAux.h>

using namespace std;

int PERIOD_DOB = 10;

//#define MAX_BR 30
//#define MAX_L 30
#define L_COEF 0.7
#define W_COEF 0.5
#define L_STVOL 0.02
#define W_STVOL 0.01
#define MAX_EL 3000
#define POPR_COEF 0.99
#define LENG_SES 500


int season = 0; // 0 -vesna ... 1 2 3 4 1 2 3 4 

#define SLOTS 4 //+1

#define START_SIZE_LIST 0.07

#define COEF_L_BRANCH 10
#define COEF_W_BRANCH 1
#define COEF_L_LEAF 0.225
#define COEF_W_LEAF 0.225
#define MAX_GROW_ITEM 500

#define VER_LIST 7 // 7:1


GLuint texId[5];

//int shag = PERIOD_DOB;

int sun_time=0;


char VertexShaderName[] = "Vertex.vert";
char FragmentShaderName[] = "Fragment.frag";
ShaderProgram shaderProgram;
void initTexture();


float frust_upper=0; // Колыхание
float max_frust_upper;
float frust_rot=0;
int frust_timer = -1;
#define MAX_frust_timer 1000

item::~item(void){}
item::item(int t=-1)
{
	if (t != -1) 
		type = t;
	else
	{
		type = rand()%(1+VER_LIST);
		if (type>0)
			type=1;
	}
}

void item::init(item*r)
{
	rod = r;
	rod->son.push_back(this);
	if (type == 0) rod->branch_sons++;
	if (type==0)
	{
		start_l = rod->start_l*L_COEF;
		start_w = rod->start_w*W_COEF;
	}
	if (type==1)
	{
		start_l = START_SIZE_LIST;
		start_w = START_SIZE_LIST;
	}
	l=start_l/10;
	w=start_w/10 ;
	time = 0;
	if (type == 0)
	{
		if ((rod->branch_sons == 1))
		{
			high = 1;
			rod_slot=SLOTS;
		}
		else
		{
			vector <int> empty_slots;
			for (int i = 1; i<= SLOTS; i++)
				empty_slots.push_back(i);
			for (int i = 0; i< rod->son.size()-1; i++)
			{
				if (rod->son[i]->type==0)
				{
					if (rod->son[i]->rod_slot!=-1)
					{
						auto it = find(empty_slots.begin(), empty_slots.end(), rod->son[i]->rod_slot);
						if (it!=empty_slots.end())
							empty_slots.erase(it);
					}
				}
			}
			int rand_sl = rand() % (empty_slots.size());
			float ran = (rand()%100)*1.0f /(100*SLOTS);
			high = -ran+(empty_slots[rand_sl]+1)*1.0f/(SLOTS+1);
			rod_slot = empty_slots[rand_sl];
		}
	}
	else high = (25+rand()%75)*1.0f/100;
	
	rot = rand()%360;
	if ((type == 0) && ((rod->branch_sons == 1)))
		upper = 90;
	else
		upper = rand()%(max_upp-min_upp)+min_upp;
	//proverka verticali
	if (type==0)
	{
		auto locmodel = rod->model;
		locmodel= glm::translate(locmodel,glm::vec3(0.0f,high*rod->l*COEF_L_BRANCH*POPR_COEF,0.0f));
		locmodel = glm::rotate(locmodel,rot,glm::vec3(0.0f,1.0f,0.0f));
		locmodel = glm::rotate(locmodel,-90.0f+upper,glm::vec3(0.0f,0.0f,1.0f)); 

		auto v = locmodel*glm::vec4(0,1,0,0);
		if (v.y<0)
			rot+=180;
	}

	branch_sons = 0;
}

void item::makestvol(int osnovnoe = 0)
{
	start_l = L_STVOL*(1-(1-osnovnoe)*0.5);
	start_w = W_STVOL*(1-(1-osnovnoe)*0.5);
	l=start_l;
	w=start_w;
	time = 0;
	rod = NULL;
	high = 0;
	rot = 0;
	upper = 90;
	rod_slot=-1;
	branch_sons = 0;
}

void item::rost()
{
	if (time<MAX_GROW_ITEM)
	{
		if (type == 0)
		{
			l += 0.05*start_l/2;
			w += 0.05*start_w/2;
		}
		else
		{
			l += 0.05*start_l/2;
			w += 0.05*start_w/2;
		}
		time++;
	}
}

void item::increas_t(int t=1) { time+=t; }

void item::add_li(tree * tr)
{
	if (type != 0)
		return;
	//Есть ли пустые слоты
	if (branch_sons>=SLOTS)
		return;
	//Есть ли пустые слоты
	tr->add_new(this);
	return;
}


void item::buildmodel()
{
	 model=glm::mat4();
	 if (rod == NULL)
	 {
		 model= glm::translate(model,bas);
		 model = glm::rotate(model,0.0f,glm::vec3(0.0f,0.0f,1.0f));
		 model = glm::rotate(model,-90.0f+upper+frust_upper,(glm::vec3(sin(frust_rot),0.0f,cos(frust_rot)))); 
		 //model = glm::scale(model,glm::vec3(w*COEF_W,l*COEF_L,w*COEF_W)); 
	 }
	 else
	 {
		 model = rod->model;
		 //model = glm::scale(model,glm::vec3(w,l,w));
		 model= glm::translate(model,glm::vec3(0.0f,high*rod->l*COEF_L_BRANCH*POPR_COEF,0.0f));
		 model = glm::rotate(model,rot,glm::vec3(0.0f,1.0f,0.0f));
		 model = glm::rotate(model,-90.0f+upper,glm::vec3(0.0f,0.0f,1.0f)); 
	 }
	 for (int i = 0; i<son.size();i++)
	 {
		son[i]->buildmodel();
	 }
	 return;
}


void item::frust()
{
	//last_upper=upper;
	upper = last_upper + sin(0.01*(MAX_frust_timer-frust_timer))*coef_frust;
	for (int i = 0; i<son.size();i++)
	{
		son[i]->frust();
	}
}

///////////////////////////////////treeeeeeeeeeeeeeee//////////////////////////////
///////////////////////////////////treeeeeeeeeeeeeeee//////////////////////////////
///////////////////////////////////treeeeeeeeeeeeeeee//////////////////////////////
///////////////////////////////////treeeeeeeeeeeeeeee//////////////////////////////
///////////////////////////////////treeeeeeeeeeeeeeee//////////////////////////////
///////////////////////////////////treeeeeeeeeeeeeeee//////////////////////////////
///////////////////////////////////treeeeeeeeeeeeeeee//////////////////////////////
///////////////////////////////////treeeeeeeeeeeeeeee//////////////////////////////

int time = 0;

void tree::add_new(item* r)
{
	if (obj.size() > MAX_EL_svoy-1)
		return;
	//else
	item *el;
	if ((osnovnoe==1) && (time_leaf<100) || (time_leaf<50))
	{	
		el = new item(0);
		time_leaf++;
	}
	else el = new item();
	el->init(r);
	obj.push_back(el);
	return;
}



tree::tree(){
	gull_see = true;
	osnovnoe = 1;
	MAX_EL_svoy = MAX_EL;
	//initTexture();
	shaderProgram.init(VertexShaderName,FragmentShaderName);
	glUseProgram(shaderProgram.programObject);
	b = new Branch();
	b->initData();
	b->initGLBuffers(shaderProgram.programObject,"pos","nor","tex");
	b2 = new Branch();
	b2->initData(12,10);
	b2->initGLBuffers(shaderProgram.programObject,"pos","nor","tex");
	l = new Leaf();
	l->initData();
	l->initGLBuffers(shaderProgram.programObject,"pos","nor","tex");
	l2 = new Leaf2();
	l2->initData();
	l2->initGLBuffers(shaderProgram.programObject,"pos","nor","tex");
	k = new Konus();//3-ug
	k->initData();
	k->initGLBuffers(shaderProgram.programObject,"pos","nor","tex");
	s = new Sun();
	s->initData();
	s->initGLBuffers(shaderProgram.programObject,"pos","nor","tex");
}
tree::~tree(void)
{
	delete b;
	delete l;
	for (int i = 0 ; i< obj.size(); i++)
	{
		delete obj[i];
	}
	glDeleteTextures(1,texId);
}

void tree::build(glm::vec3 base)
{
	if (obj.size()==0) //empty
	{
		item *stvol = new item(0);
		stvol->makestvol(osnovnoe);
		stvol->bas = base;
		obj.push_back(stvol);
	}
	else
	{
		cout<<"ERROR";
	}
	time=0;
	time_leaf = 0;
	shag = PERIOD_DOB;
	return;
}


	glm::vec3 temn = glm::vec3 (0.15,-0.1,-0.1); glm::vec3 temn_copy = glm::vec3 (0.15,-0.1,-0.1);
void tree::rost()
{
	{ // 1 season
		if (season == 0 && time< MAX_EL*PERIOD_DOB*1.0/34 && time>=0) //>=0 для кустов
		{
			if (osnovnoe) time++;
			if (osnovnoe==1 ) if (MAX_EL>obj.size()) {Sleep(12);}
			shag--;
			if (shag == 0 )
			{
				int j = obj.size();
				for (int i = 0; i<j; i++)
				{
					obj[i]->add_li(this);
				}
				obj[0]->buildmodel();
				shag = PERIOD_DOB;
			}
			else 
			{
				for (int i = 0; i<obj.size(); i++)
				{
					obj[i]->rost();
				}
				obj[0]->buildmodel();
			}
		}
		else if (season == 0 && osnovnoe==1) { season++; time = 0;  temn = glm::vec3 (0.15,-0.1,-0.1); } 
		//else if (osnovnoe==0) { time = -1; } 
	}	
	{ // leto
		if (season == 1 && time< LENG_SES)
		{
			if (osnovnoe) time++;
			if (osnovnoe) temn += glm::vec3(0.0006,0.0006,0.0006);
		}
		else if (season == 1 && osnovnoe==1)  { season++; time = 0; temn_copy = temn; }
	}	
	{ // osen
		if (season == 2 && time< LENG_SES*1.5)
		{
			if (osnovnoe) time++;
			if (time< LENG_SES*1.5/2)
			{
				if (osnovnoe) temn += glm::vec3(-0.0035,-0.002,0.0055);
			}
			else 
			{
				if (osnovnoe) temn += glm::vec3(+0.0019,0.0065,0.0050);
			}
		}
		else if (season == 2 && osnovnoe==1)  { season++; time = 0; 
			for (int i = 0; i<obj.size(); i++)
			{
				obj[i]->zima = 0;
			}
		} 
	}	
	{ // zima
		if (season == 3 && time< LENG_SES*3)
		{
			if (osnovnoe) time++;
			int i = 0;
			while (time+LENG_SES*1.5*i<obj.size())
			{
				obj[time+LENG_SES*1.5*i]->zima=1;
				i++;
			}
			if (time>LENG_SES)
			{
				if (osnovnoe) temn = temn + glm::vec3(glm::vec3 (0.15/(LENG_SES*1.5),-0.1/(LENG_SES*1.5),-0.1/(LENG_SES*1.5))-glm::vec3(temn.x/(LENG_SES*1.5),temn.y/(LENG_SES*1.5),temn.z/(LENG_SES*1.5)));
			}
		}
		else if (season == 3 && osnovnoe==1)  { season++; time = 0; temn = glm::vec3 (0.15,-0.1,-0.1); } 
	}	
	{ // vesna
		if (season == 4 && time< LENG_SES*1.5)
		{
			if (osnovnoe) time++;
			int i = 0;
			while (time+LENG_SES*1.5*i<obj.size())
			{
				obj[time+LENG_SES*1.5*i]->zima=0;
				i++;
			}			
		}
		else if (season == 4 && osnovnoe==1)  { season=1; time = 0; } 
	}	
	//Колыхание
	{
		if (osnovnoe==1){
			if (frust_timer>0)
			{
				frust_upper=sin(0.01*(MAX_frust_timer-frust_timer)*max_frust_upper);
				for (int i = 0; i<obj[0]->son.size();i++)
				{
					obj[0]->son[i]->frust();
				}
				obj[0]->buildmodel();
				frust_timer--;
			}
			if (frust_timer==0) 
			{
				frust_upper=0;
				for (int i = 1; i<obj.size();i++)
				{
						obj[i]->upper = obj[i]->last_upper;
				}
				obj[0]->buildmodel();
				frust_timer--;
			}
		}
	}
}

void initTexture()
{
	AUX_RGBImageRec *lef;
	lef = auxDIBImageLoad("l.bmp");

    //generate as many textures as you need
	glGenTextures(1,&texId[0]);
	
    //enable texturing and zero slot
    glActiveTexture(GL_TEXTURE0);
    //bind texId to 0 unit
	glBindTexture(GL_TEXTURE_2D,texId[0]);

	//don't use alignment
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	
	// Set nearest filtering mode for texture minification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//TODO: load texture from file 
	//set Texture Data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, lef->sizeX,lef->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, lef->data);


	AUX_RGBImageRec *br;
	br = auxDIBImageLoad("b.bmp");
	//generate as many textures as you need
	glGenTextures(1,&texId[1]);
	
    //enable texturing and zero slot
    glActiveTexture(GL_TEXTURE0);
    //bind texId to 0 unit
	glBindTexture(GL_TEXTURE_2D,texId[1]);

	//don't use alignment
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	
	// Set nearest filtering mode for texture minification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//TODO: load texture from file 
	//set Texture Data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, br->sizeX,br->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, br->data);


	AUX_RGBImageRec *gr;
	gr = auxDIBImageLoad("ground.bmp");
	//generate as many textures as you need
	glGenTextures(1,&texId[2]);
	
    //enable texturing and zero slot
    glActiveTexture(GL_TEXTURE0);
    //bind texId to 0 unit
	glBindTexture(GL_TEXTURE_2D,texId[2]);

	//don't use alignment
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	
	// Set nearest filtering mode for texture minification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//TODO: load texture from file 
	//set Texture Data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gr->sizeX,gr->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, gr->data);


	AUX_RGBImageRec *sun;
	sun = auxDIBImageLoad("sun.bmp");
	//generate as many textures as you need
	glGenTextures(1,&texId[3]);
	
    //enable texturing and zero slot
    glActiveTexture(GL_TEXTURE0);
    //bind texId to 0 unit
	glBindTexture(GL_TEXTURE_2D,texId[3]);

	//don't use alignment
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	
	// Set nearest filtering mode for texture minification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//TODO: load texture from file 
	//set Texture Data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sun->sizeX,sun->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, sun->data);
	
	AUX_RGBImageRec *gull;
	gull = auxDIBImageLoad("GULL.bmp");
	//generate as many textures as you need
	glGenTextures(1,&texId[4]);
	
    //enable texturing and zero slot
    glActiveTexture(GL_TEXTURE0);
    //bind texId to 0 unit
	glBindTexture(GL_TEXTURE_2D,texId[4]);

	//don't use alignment
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	
	// Set nearest filtering mode for texture minification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//TODO: load texture from file 
	//set Texture Data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gull->sizeX,gull->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, gull->data);


}






int tree::drow( glm::mat4x4 &viewMatrix, glm::mat4x4 &modelViewMatrix,glm::mat4x4 &projectionMatrix,
	glm::mat4x4 &modelViewProjectionMatrix, glm::mat4x4 &normalMatrix,glm::vec3 eye)
{

	if ((time==1)&&(season==0)&&(osnovnoe))
	{
		l3 = new L3();
		l3->initData();
		l3->initGLBuffers(shaderProgram.programObject,"pos","nor","tex");
	}

	auto modelMatrix=glm::mat4();

  int locMV = glGetUniformLocation(shaderProgram.programObject,"modelViewMatrix");
  int locN = glGetUniformLocation(shaderProgram.programObject,"normalMatrix");
  int locP = glGetUniformLocation(shaderProgram.programObject,"modelViewProjectionMatrix");
  int texLoc = glGetUniformLocation(shaderProgram.programObject,"textureSampler");
  int locFlag = glGetUniformLocation(shaderProgram.programObject,"useTexture");
  int locD = glGetUniformLocation(shaderProgram.programObject,"temn");
  if (locMV<0 || locN<0 || locP<0 || texLoc <0 || locFlag<0 || locD <0 )
  {
	  glClearColor(0,0,1,1);
	  glClear(GL_COLOR_BUFFER_BIT);
	  glutSwapBuffers();
	  return 1;
  }



  //bind texture
  
  glUniform1ui(texLoc,0);
  glUniform1i(locFlag,true); //использовать текстру
  //glUniform3f(locD,temn.x,temn.y,temn.z); // season

  int locL = glGetUniformLocation(shaderProgram.programObject,"lightPos");
  int locE = glGetUniformLocation(shaderProgram.programObject,"eyePos");

  glm::vec3	light=glm::vec3(0,4,6);
  light=glm::vec3(viewMatrix*glm::vec4(light,1));
  glUniform3f(locL,light.x,light.y,light.z);
  glUniform3f(locE,eye.x,eye.y,eye.z);

 

	for (int i = 0; i<obj.size(); i++)
	{
	
	 glm::mat4 modelMatrix = obj[i]->model;  
	  if (obj[i]->type==0)
	  {
		 if (osnovnoe!=1) modelMatrix = glm::scale(modelMatrix,glm::vec3(obj[i]->w*COEF_W_BRANCH*0.5,obj[i]->l*COEF_L_BRANCH,obj[i]->w*COEF_W_BRANCH*0.5));
		  else modelMatrix = glm::scale(modelMatrix,glm::vec3(obj[i]->w*COEF_W_BRANCH,obj[i]->l*COEF_L_BRANCH,obj[i]->w*COEF_W_BRANCH));
	  }
	  else if (obj[i]->type==1)
			modelMatrix = glm::scale(modelMatrix,glm::vec3(obj[i]->w*COEF_W_LEAF,obj[i]->l*COEF_L_LEAF,obj[i]->w*COEF_W_LEAF));


	  modelViewMatrix = viewMatrix*modelMatrix;
	  normalMatrix = glm::inverseTranspose(modelViewMatrix);
	  modelViewProjectionMatrix = projectionMatrix*modelViewMatrix;

	  glUniformMatrix4fv(locMV,1,0,glm::value_ptr(modelViewMatrix));
	  glUniformMatrix4fv(locN,1,0,glm::value_ptr(normalMatrix));
	  glUniformMatrix4fv(locP,1,0,glm::value_ptr(modelViewProjectionMatrix));




	  if (obj[i]->type==0)
	  {
		if (season == 2 || season == 3 )  glUniform3f(locD,temn_copy.x-0.15,temn_copy.y+0.1,temn_copy.z+0.1);
		else glUniform3f(locD,temn.x-0.15,temn.y+0.1,temn.z+0.1);


		glBindTexture(GL_TEXTURE_2D,texId[1]);
		if (i<30 && osnovnoe) b->draw(); // to speed drawing
		else b2->draw();

	  }
	  else if (obj[i]->type==1) 
	  {
		  if (season == 0 || season == 4)
			  glUniform3f(locD,0.15,-0.1,-0.1);
		  if (season == 1||season == 2 || season == 3) 
		  {
			  glUniform3f(locD,temn.x,temn.y,temn.z);
		  }
		  if (obj[i]->zima == 1 ) continue;
		  glBindTexture(GL_TEXTURE_2D,texId[0]);
		  l->draw();
		  modelMatrix= glm::translate(modelMatrix,glm::vec3(0.00,0.0,0.01));
		  modelViewMatrix = viewMatrix*modelMatrix;
		  normalMatrix = glm::inverseTranspose(modelViewMatrix);
		  modelViewProjectionMatrix = projectionMatrix*modelViewMatrix;

		  glUniformMatrix4fv(locMV,1,0,glm::value_ptr(modelViewMatrix));
		  glUniformMatrix4fv(locN,1,0,glm::value_ptr(normalMatrix));
		  glUniformMatrix4fv(locP,1,0,glm::value_ptr(modelViewProjectionMatrix));

		  l2->draw();

	  }
		
	}


	if (osnovnoe==1){

		if (gull_see){
			//l3
			glUniform3f(locD,0.1,0.1,0.1); // leto
			modelMatrix = glm::mat4();
			modelMatrix= glm::translate(modelMatrix,glm::vec3(-5*sin(sun_time/80.0),6.5,-5*cos(sun_time/80.0)));
			modelMatrix = glm::rotate(modelMatrix,-90.0f,glm::vec3(1,0,0));
			modelMatrix = glm::rotate(modelMatrix, -90.0f+2.855f*0.25f*sun_time ,glm::vec3(0,0,1));
			modelMatrix = glm::scale(modelMatrix,glm::vec3(3.3f,3.3f,3.3f));
			modelViewMatrix = viewMatrix*modelMatrix;
			normalMatrix = glm::inverseTranspose(modelViewMatrix);
			modelViewProjectionMatrix = projectionMatrix*modelViewMatrix;
			glUniformMatrix4fv(locMV,1,0,glm::value_ptr(modelViewMatrix));
			glUniformMatrix4fv(locN,1,0,glm::value_ptr(normalMatrix));
			glUniformMatrix4fv(locP,1,0,glm::value_ptr(modelViewProjectionMatrix));	
			glBindTexture(GL_TEXTURE_2D,texId[4]);
			l3->draw();
			//l3
		}

			//sun
			glUniform3f(locD,0,0,0); // leto
			 sun_time++;
			 if (sun_time==360*200) sun_time=0;
			modelMatrix = glm::mat4();
			modelMatrix= glm::translate(modelMatrix,glm::vec3(0,4*1.2,6*1.2));
			modelMatrix = glm::rotate(modelMatrix,2.0f*sun_time,glm::vec3(0.05f,1.0f,0.2f));
			modelMatrix = glm::scale(modelMatrix,glm::vec3(0.3f,0.3f,0.3f));
			  modelViewMatrix = viewMatrix*modelMatrix;
			  normalMatrix = glm::inverseTranspose(modelViewMatrix);
			  modelViewProjectionMatrix = projectionMatrix*modelViewMatrix;
			  glUniformMatrix4fv(locMV,1,0,glm::value_ptr(modelViewMatrix));
			  glUniformMatrix4fv(locN,1,0,glm::value_ptr(normalMatrix));
			  glUniformMatrix4fv(locP,1,0,glm::value_ptr(modelViewProjectionMatrix));	
			   glBindTexture(GL_TEXTURE_2D,texId[3]);
			s->draw();
			//sun

	
			//graund
			if (season==0 || season == 4) glUniform3f(locD,-0.05,-0.1,-0.13); 
			if (season==1) glUniform3f(locD,0.1,0.1,0.1); 
			if (season==2) glUniform3f(locD,-0.3,-0.1,0); 
			if (season==3) glUniform3f(locD,0.1,0,-0.5); 
			modelMatrix = glm::mat4();
			modelMatrix= glm::translate(modelMatrix,glm::vec3(0,-0.01,0));
			modelMatrix = glm::rotate(modelMatrix,0.0f,glm::vec3(0.0f,0.0f,1.0f));
			modelMatrix = glm::scale(modelMatrix,glm::vec3(50.0f,50.0f,50.0f));
			  modelViewMatrix = viewMatrix*modelMatrix;
			  normalMatrix = glm::inverseTranspose(modelViewMatrix);
			  modelViewProjectionMatrix = projectionMatrix*modelViewMatrix;
			  glUniformMatrix4fv(locMV,1,0,glm::value_ptr(modelViewMatrix));
			  glUniformMatrix4fv(locN,1,0,glm::value_ptr(normalMatrix));
			  glUniformMatrix4fv(locP,1,0,glm::value_ptr(modelViewProjectionMatrix));		
			  glBindTexture(GL_TEXTURE_2D,texId[2]);
			 if (eye.y>0)
			 {
				k->draw();

				modelMatrix= glm::translate(modelMatrix,glm::vec3(0,-0.0001,0));
				modelMatrix = glm::scale(modelMatrix,glm::vec3(1.5f,1.5f,1.5f));
				modelViewMatrix = viewMatrix*modelMatrix;
				  normalMatrix = glm::inverseTranspose(modelViewMatrix);
				  modelViewProjectionMatrix = projectionMatrix*modelViewMatrix;
				  glUniformMatrix4fv(locMV,1,0,glm::value_ptr(modelViewMatrix));
				  glUniformMatrix4fv(locN,1,0,glm::value_ptr(normalMatrix));
				  glUniformMatrix4fv(locP,1,0,glm::value_ptr(modelViewProjectionMatrix));	
				  k->draw();


				modelMatrix= glm::translate(modelMatrix,glm::vec3(0,-0.0001,0));
				modelMatrix = glm::scale(modelMatrix,glm::vec3(2.5f,2.5f,2.5f));
				modelViewMatrix = viewMatrix*modelMatrix;
				  normalMatrix = glm::inverseTranspose(modelViewMatrix);
				  modelViewProjectionMatrix = projectionMatrix*modelViewMatrix;
				  glUniformMatrix4fv(locMV,1,0,glm::value_ptr(modelViewMatrix));
				  glUniformMatrix4fv(locN,1,0,glm::value_ptr(normalMatrix));
				  glUniformMatrix4fv(locP,1,0,glm::value_ptr(modelViewProjectionMatrix));	
				  k->draw();
			 }
			//graund
	
	}

	return 0;
}


void tree::season_st()
{
	if (season == 0 ) { time = 0; season=1;}
}

void tree::frust()
{
	max_frust_upper = 5;
	frust_rot = rand()%360;
	frust_timer = MAX_frust_timer;
	
	for (int i = 1; i<obj.size();i++)
	{
				obj[i]->coef_frust = (rand()%100 - 100 ) * max_frust_upper *1.0f/ (rand()%200+50);
				obj[i]->last_upper = obj[i]->upper;
	}


}
