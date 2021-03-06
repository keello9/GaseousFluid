#ifndef _FLUID_H_
#define _FLUID_H_

#include "displayVec.h"
#include <GL/freeglut.h>
#include <vector>
#include <Eigen/Eigen>

struct Pos
{
	int x;
	int y;
	Pos() {}
	Pos(int _x, int _y): x(_x), y(_y) {}
};

struct Velo
{
	float x;
	float y;
	Velo() {}
	Velo(float _x, float _y): x(_x), y(_y) {}
};

enum GRIDTYPE
{
	FLUID,
	AIR,
	SOLID
};

enum DISPLAYMODE
{
	NONE,
	VOTICITY,
	LIC,
	DENS,
	VELOCITY
};

class FluidCube2D
{
private:
	float h;
	float h2;
	float hi;
	float dt;
	float diff;
	float visc;

	float *d;
	float *Vx;
	float *Vy;
	GRIDTYPE *type;
	std::vector<Pos> obstacle;
	std::vector<std::pair<Pos, int>> cylinder;
	float max_d;
	float max_vx;
	float max_vy;

	//Projection using Conjugate Gradient
	Pos dir[4];
	int fluidNum;
	int **neighbor;
	int *neighNum;
	int *pos2index;
	Eigen::SparseMatrix<double> A;
	Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;

	//Advection using BFECC
	float *fai_b;
	float *fai_f;

	DisplayVec *displayVec;
	float *Vx_lic;
	float *Vy_lic;
	float *d_lic;
	DISPLAYMODE mode;

public:
	int size;
	float *d0;
	float *Vx0;
	float *Vy0;

private:
	void dens_step(float *amount);
	void addDensity(float *amount);
	void diffuseDensity();
	void advectDensity();
	
	void vel_step(float *amountX, float *amountY);
	void addVelocity(float *amountX, float *amountY);
	void diffuseVelosity();
	void advectVelosity();
	void projectVelosity();
	
	void add_source(float *x, float *amount);
	void diffuse(int b, float *u0, float *u, float diffusion);
	void advect(int b, float *u0, float *u, float* vx, float* vy, bool backward);
	void swap(float *x0, float *x);
	void set_bnd(int b, float *x);

	void draw_dens();
	void draw_velo(int i, int j, float vx, float vy);
	Eigen::Vector3f interpolate(float X, float Y);
	void interpolateForLic();
	void output(float *u);

public:
	FluidCube2D(float diffusion, float viscosity, float dt);
	~FluidCube2D();
	void simulate(bool idle);
	void setDisplayMode(DISPLAYMODE m) {mode = m; }
};
#endif