#include <iostream>
#include <math.h>
#include <random>
#include <vector>
#include <ctime>
#include <algorithm>

#include <Windows.h>
#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#undef max
#undef min

using namespace std;

#include "Sphere.h"
#include "Plane.h"


template<typename T>
T clamp(const T &val, const T &min, const T &max)
{
	return std::max(min, std::min(max, val));
}


// Helpers for random number generation
std::mt19937 mersenneTwister;
std::uniform_real_distribution<double> uniform;

#define Width 800
#define Height 800
#define PI 3.1415926536
#define DepthThreshold 5
#define MaxIterations 15

#define Random (2.0*uniform(mersenneTwister)-1.0)
#define Random2 (uniform(mersenneTwister))

const double inf = 1e9;
const double Epsilon = 1e-6;
const double refractionIndex = 1.5;

// All game objects in the scene.
std::vector<GameObject*> gameObjects;

// Just to output how many rays have been generated in total.
int totalRays;



// Perspective Camera
Vector3 Camera(const double x, const double y)
{
	double w = Width;
	double h = Height;

	float fovX = PI / 4;
	float fovY = (h / w) * fovX;

	double _x = ((2 * x - w) / w) * tan(fovX);
	double _y = -((2 * y - h) / h) * tan(fovY);

	return Vector3(_x, _y, -1.0);
}


// given v1, set v2 and v3 so they form an orthonormal system
// (we assume v1 is already normalized)
void OrthonormalSystem(const Vector3& v1, Vector3& v2, Vector3& v3)
{
	if (std::abs(v1.x) > std::abs(v1.y))
	{
		// project to the y = 0 plane and construct a normalized orthogonal vector in this plane
		float invLen = 1.f / sqrtf(v1.x * v1.x + v1.z * v1.z);
		v2 = Vector3(-v1.z * invLen, 0.0f, v1.x * invLen);
	}
	else
	{
		// project to the x = 0 plane and construct a normalized orthogonal vector in this plane
		float invLen = 1.0f / sqrtf(v1.y * v1.y + v1.z * v1.z);
		v2 = Vector3(0.0f, v1.z * invLen, -v1.y * invLen);
	}

	v3 = Vector3::Cross(v1, v2);
}

Vector3 GetHemisphere(double u1, double u2)
{
	const double r = sqrt(1.0 - u1 * u1);
	const double phi = 2 * PI * u2;

	return Vector3(cos(phi) * r, sin(phi) * r, u1);
}



bool GetIntersection(const Ray& ray, int &id, double &distance)
{
	distance = inf;
	for (int i = 0; i < gameObjects.size(); i++)
	{
		auto intersection = gameObjects[i]->Intersection(ray);
		if (intersection > Epsilon && intersection < distance)
		{
			distance = intersection;
			id = i;
		}
	}

	return (id > -1) ? true : false;
}


void ReflectRay(Ray &ray, Vector3 hitNormal, GameObject* hit)
{
	// Diffuse BRDF
	if (hit->material.type == 1)
	{
		Vector3 rotX, rotY;
		OrthonormalSystem(hitNormal, rotX, rotY);
		Vector3 sampledDir = GetHemisphere(Random2, Random2);
		Vector3 rotatedDir;
		rotatedDir.x = Vector3::Dot(Vector3(rotX.x, rotY.x, hitNormal.x), sampledDir);
		rotatedDir.y = Vector3::Dot(Vector3(rotX.y, rotY.y, hitNormal.y), sampledDir);
		rotatedDir.z = Vector3::Dot(Vector3(rotX.z, rotY.z, hitNormal.z), sampledDir);
		ray.direction = rotatedDir;	// already normalized
	}

	// Specular BRDF
	if (hit->material.type == 2)
	{
		double cost = Vector3::Dot(ray.direction, hitNormal);
		ray.direction = (ray.direction - hitNormal * (cost * 2)).Normalized();
	}

	// Glass/refractive BRDF
	if (hit->material.type == 3)
	{
		double n = refractionIndex;
		double R0 = (1.0 - n) / (1.0 + n);
		R0 = R0 * R0;

		// we're inside the medium
		if (Vector3::Dot(hitNormal, ray.direction) > 0)
		{
			hitNormal = hitNormal * -1;
			n = 1 / n;
		}

		n = 1 / n;
		double cost1 = Vector3::Dot(hitNormal, ray.direction) * -1; // cosine of theta_1
		double cost2 = 1.0 - n * n * (1.0 - cost1 * cost1); // cosine of theta_2
		double Rprob = R0 + (1.0 - R0) * pow(1.0 - cost1, 5.0); // Schlick-approximation

		// refraction direction
		if (cost2 > 0 && Random2 > Rprob)
			ray.direction = ((ray.direction * n) + (hitNormal * (n * cost1 - sqrt(cost2)))).Normalized();
		// reflection direction
		else
			ray.direction = (ray.direction + hitNormal * (cost1 * 2)).Normalized();
	}
}


Vector3 LightSource(Vector3 hitPoint)
{
	if ((abs(hitPoint.y - 2.95) < 0.1) && hitPoint.x >= -0.7 && hitPoint.x <= 0.7
		&& hitPoint.z <= -3.05 && hitPoint.z >= -3.45)
		return Vector3(50, 50, 50);

	return Vector3(0, 0, 0);
}


Vector3 Trace(Ray &ray)
{
	totalRays++;

	Vector3 accColor = Vector3(1, 1, 1);
	for (int i = 0; i < MaxIterations; i++)
	{
		// Russian roulette: starting at depth threshold, each recursive step will stop with a probability of 0.1
		if (i >= DepthThreshold)
		{
			const double rrStopProbability = 0.1;
			if (Random2 <= rrStopProbability)
			{
				break;
			}
		}

		int id = -1;
		double distance;
		bool intersection = GetIntersection(ray, id, distance);
		if (!intersection)
			return Vector3(0, 0, 0);

		auto hitPoint = ray.origin + ray.direction * distance;
		auto hitNormal = gameObjects[id]->GetNormal(hitPoint);
		ray.origin = hitPoint;


		Vector3 emission = LightSource(hitPoint);
		if (emission.x > Epsilon)
			return accColor * emission;


		ReflectRay(ray, hitNormal, gameObjects[id]);


		if (gameObjects[id]->material.type == 1)
		{
			double lightAttenuation = max(0.0, Vector3::Dot(ray.direction, hitNormal));
			accColor = accColor * gameObjects[id]->material.color * lightAttenuation;
		}

		if (gameObjects[id]->material.type == 3)
		{
			accColor = accColor * 1.15;
		}
	}

	return accColor;
}



void InitGameObjects()
{
	auto Add = [](GameObject* go, Vector3 color, double emission, int type)
	{
		Material mat = Material(color, emission, type);
		go->material = mat;
		gameObjects.push_back(go);
	};

	auto leftSphere = new Sphere(Vector3(-0.55, -1.55, -4.0), 1.0);
	auto rightSphere = new Sphere(Vector3(1.5, -1.75, -3.5), 0.85);

	auto bottom = new Plane(Vector3(0, 1, 0), 2.5);
	auto back = new Plane(Vector3(0, 0, 1), 5.5);
	auto left = new Plane(Vector3(1, 0, 0), 2.75);
	auto right = new Plane(Vector3(-1, 0, 0), 2.75);
	auto ceiling = new Plane(Vector3(0, -1, 0), 3.0);
	auto front = new Plane(Vector3(0, 0, -1), 0.5);


	// GameObject, Color, emission, type (diffuse = 1, reflective = 2, refractive = 3)
	Add(leftSphere, Vector3(0.9, 0.9, 0.9), 0, 2);
	Add(rightSphere, Vector3(1, 0.250, 0.019), 0, 3);

	
	Add(bottom, Vector3(0.8, 0.8, 0.8), 0, 1);
	Add(back, Vector3(0.8, 0.8, 0.8), 0, 1);
	Add(left, Vector3(1, 0.019, 0.019), 0, 1);
	Add(right, Vector3(0.007, 0.580, 0.8), 0, 1);
	Add(ceiling, Vector3(0.8, 0.8, 0.8), 0, 1);
	Add(front, Vector3(0.8, 0.8, 0.8), 0, 1);
}


// Stop the rendering anytime by pressing the A button.
// The image is then being saved with the already calculated samples.
bool OnTryExit()
{
	if ( !(GetAsyncKeyState('A') & 0x8000))
		return false;

	return true;
}

bool breakOut = false;
double curSamples;
void Render(Vector3 **pix)
{
	curSamples++;

	#pragma omp parallel for schedule(dynamic)
	for (int x = 0; x < Width; x++)
	{
		for (int y = 0; y < Height; y++)
		{
			Ray ray;
			ray.origin = (Vector3(0, 0, -0.1));
			// construct image plane coordinates
			Vector3 cam = Camera(x, y); 

			// anti-aliasing
			cam.x = cam.x + Random / 700;
			cam.y = cam.y + Random / 700;
			ray.direction = (cam - ray.origin).Normalized();

			Vector3 finalColor = Vector3(0, 0, 0);
			finalColor = Trace(ray);

			pix[x][y] = pix[x][y] + finalColor;

			if (!breakOut)
				breakOut = OnTryExit();
		}
	}
}


inline double SimpleClamp(double x) { return x < 0 ? 0 : x > 1 ? 1 : x; }
inline int ToInt(double x) { return int(pow(SimpleClamp(x), 1 / 2.2) * 255 + 0.5); }


int main()
{
	InitGameObjects();

	Vector3 **pix = new Vector3*[Width];
	for (int i = 0; i < Width; i++)
		pix[i] = new Vector3[Height];

	clock_t start = clock();

	while (!breakOut)
	{
		fprintf(stdout, "\rRendering... Current Samples: %8.2f%", curSamples);
		Render(pix);
	}
	
	

	FILE *f = fopen("ray.ppm", "w");
	fprintf(f, "P3\n%d %d\n%d\n ", Width, Height, 255);
	for (int r = 0; r < Height; r++) 
	{
		for (int c = 0; c < Width; c++) 
		{
			pix[c][r] = pix[c][r] / curSamples;
			fprintf(f, "%d %d %d ", ToInt(pix[c][r].x), ToInt(pix[c][r].y), ToInt(pix[c][r].z));
		}
		fprintf(f, "\n");
	}
	fclose(f);

	clock_t end = clock();
	double t = (double)(end - start) / CLOCKS_PER_SEC;
	printf("\nRender time: %fs.\n", t);

	std::cout << "Total Rays: " << totalRays << std::endl;
 
	return 0;
}