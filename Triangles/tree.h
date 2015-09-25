#pragma once
#include "MyObject.h"
#include "Branch.h"
#include "Leaf.h"
#include "konus.h" // 3угольник
#include "sun.h"
#include "l3ds.h"
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "shaderhelper.h"
#include <GL/glut.h>
class tree;

void initTexture();

class item
{
public:
	float last_upper; //for kolyh
	float coef_frust; //for kolyh
	int branch_sons;
	int rod_slot;
	glm::mat4 model;
	int time;
	item *rod;
	int type; // 0 - branch, 1 - leaf 
	std::vector <item*> son ;
	float high;
	float rot;
	float upper;
	float l;
	float w;
	float start_l, start_w;
	glm::vec3 bas;
	item(int t);
	~item(void);
	void makestvol(int osnovnoe);
	void rost();
	void increas_t(int t);
	void init(item*r);
	void add_li(tree *);
	void frust();
	void buildmodel();
	int zima;

static const int max_upp = 90-25; // in C
static const int min_upp = 20; // in C
};



class tree
{
public:
	bool gull_see;
	int osnovnoe;
	void add_new(item* r);
   // long long time;
	int shag;
	std::vector <item*> obj;
	void build(glm::vec3);
	void rost();
	int MAX_EL_svoy;
	int time_leaf;
	Leaf * l;
	Leaf2 * l2;
	L3 * l3;
	Branch * b;
	Branch * b2;
	MyObject * k;
	Sun * s;
	int drow( glm::mat4x4 &viewMatrix, glm::mat4x4 &modelViewMatrix,glm::mat4x4 &projectionMatrix,glm::mat4x4 &modelViewProjectionMatrix,glm::mat4x4 &normalMatrix, glm::vec3 eye);
	tree();
	void frust();
	void season_st();
	~tree(void);
};

