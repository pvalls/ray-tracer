#include <iostream>
#include <stdlib.h> /* srand, rand */
#include <vector>
#include <algorithm>
#include "core/film.h"
#include "core/matrix4x4.h"
#include "core/ray.h"
#include "core/utils.h"
#include "shapes/sphere.h"
#include "shapes/disk.h"
#include "shapes/cylinder.h"
#include "cameras/ortographic.h"
#include "cameras/perspective.h"
#include "shaders/intersectionshader.h"
#include "shaders/depthshader.h"
#include "materials/phong.h"
#include "shaders/directshader.h"
#include "shapes/infiniteplane.h"
#include "materials/mirror.h"
#include "materials/transmissive.h"
#include "shaders/globalshader.h"

#define _USE_MATH_DEFINES
#include <math.h>

void buildSceneSphere(Camera* &cam, Film* &film,
                      std::vector<Shape*>* &objectsList,
                      std::vector<LightSource*>* &lightSourceList)
{
    /* **************************** */
    /* Declare and place the camera */
    /* **************************** */
    // By default, this gives an ID transform
    //  which means that the camera is located at (0, 0, 0)
    //  and looking at the "+z" direction
    Matrix4x4 cameraToWorld;
    double fovDegrees = 60;
    double fovRadians = Utils::degreesToRadians(fovDegrees);
    cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);

    /* ************************** */
    /* DEFINE YOUR MATERIALS HERE */
    /* ************************** */
    
     Material *green_50 = new Phong (Vector3D(0.2, 0.7, 0.3), Vector3D(0.2, 0.6, 0.2), 50);
	 Material *blue_50 = new Phong(Vector3D(0.0, 0.9, 0.9), Vector3D(0.1, 0.9, 0.9), 50);
	 Material *yellow_50 = new Phong(Vector3D(1, 1, 0.2), Vector3D(1, 1, 0.2), 20);


    /* ******* */
    /* Objects */
    /* ******* */
    // Create a heterogeneous list of objects of type shape
    // (some might be triangles, other spheres, plans, etc)
    objectsList = new std::vector<Shape*>;

    // Define and place a sphere
    Matrix4x4 sphereTransform1;
    sphereTransform1 = sphereTransform1.translate(Vector3D(-1.0, -0.5, 2*std::sqrt(2.0)));
    Shape *s1 = new Sphere (0.25, sphereTransform1, green_50);

    // Define and place a sphere
    Matrix4x4 sphereTransform2;
    sphereTransform2 = sphereTransform2.translate(Vector3D(1.0, 0.0, 6));
    Shape *s2 = new Sphere (1, sphereTransform2, blue_50);

    // Define and place a sphere
    Matrix4x4 sphereTransform3;
    sphereTransform3 = sphereTransform3.translate(Vector3D(0.3, -0.75, 3.5));
    Shape *s3 = new Sphere (0.25, sphereTransform3, yellow_50);

    // Store the objects in the object list
    objectsList->push_back(s1);
    objectsList->push_back(s2);
    objectsList->push_back(s3);

	//Infinite Planes
	Material *greyDiffuse = new Phong(Vector3D(0.8, 0.8, 0.8), Vector3D(0, 0, 0), 100);
	Shape *bottomPlan = new InfinitePlane(Vector3D(0, -1, 0), Vector3D(0, 1, 0), greyDiffuse);

	//Store to objects in the object list
	objectsList->push_back(bottomPlan);

    /* ****** */
    /* Lights */
    /* ****** */
    //
	// Define Point Light Sources with position and intensity.
	LightSource *pls1 = new PointLightSource(Vector3D(0,3,3), Vector3D(10, 10, 10));
	//PointLightSource pls2(Vector3D(3,-2,5), Vector3D(10, 10, 10));
	//PointLightSource pls3(Vector3D(2,2,3), Vector3D(10, 10, 10));

	// Store the light sources in the "lightSourceList"
	lightSourceList = new std::vector<LightSource*>;
	lightSourceList->push_back(pls1);
	//lightSourceList->push_back(pls2);
	//lightSourceList->push_back(pls3);

}

void raytrace(Camera* &cam, Shader* &shader, Film* &film,
              std::vector<Shape*>* &objectsList, std::vector<LightSource*>* &lightSourceList)
{
    unsigned int sizeBar = 40;

    size_t resX = film->getWidth();
    size_t resY = film->getHeight();

    // Main raytracing loop
    // Out-most loop invariant: we have rendered lin lines
    for(size_t lin=0; lin<resY; lin++)
    {
        // Show progression
        if ( lin%(resY/sizeBar) == 0)
            std::cout << ".";

        // Inner loop invariant: we have rendered col columns
        for(size_t col=0; col<resX; col++)
        {
            // Compute the pixel position in NDC
            double x = (double)(col + 0.5) / resX;
            double y = (double)(lin + 0.5) / resY;

            // Generate the camera ray
            Ray cameraRay = cam->generateRay(x, y);

            // Compute ray color according to the used shader
            Vector3D pixelColor = shader->computeColor( cameraRay, *objectsList, *lightSourceList );

            // Store the pixel color
            film->setPixelValue(col, lin, pixelColor);
        }
    }
}

void buildSceneCornellBox(Camera* &cam, Film* &film,
	std::vector<Shape*>* &objectsList, std::vector<LightSource*>* &lightSourceList, int scene)
{
	/* **************************** */
	/* Declare and place the camera */
	/* **************************** */
	Matrix4x4 cameraToWorld = Matrix4x4::translate(Vector3D(0, 0, -3));
	double fovDegrees = 60;
	double fovRadians = Utils::degreesToRadians(fovDegrees);
	cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);

	/* ********* */
	/* Materials */
	/* ********* */
	Material *redDiffuse = new Phong(Vector3D(0.7, 0.2, 0.2), Vector3D(0, 0, 0), 100);
	Material *greenDiffuse = new Phong(Vector3D(0.2, 0.6, 0.3), Vector3D(0, 0, 0), 100);
	Material *greyDiffuse = new Phong(Vector3D(0.8, 0.8, 0.8), Vector3D(0, 0, 0), 100);
	Material *blueDiffuse = new Phong(Vector3D(0.4, 0.3, 0.8), Vector3D(0, 0, 0), 100);
	Material *yellowDiffuse = new Phong(Vector3D(0.9, 0.9, 0.5), Vector3D(0, 0, 0), 100);

	//More Phong Materials
	Material *yellow_20 = new Phong(Vector3D(1, 1, 0.2),	Vector3D(1, 1, 0.2), 20);
	Material *red_100 = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0.7, 0.7, 0.2), 100);
	Material *blue_50 = new Phong(Vector3D(0.0, 0.9, 0.9), Vector3D(0.1, 0.9, 0.9), 50);
	Material *orange_70 = new Phong(Vector3D(1, 0.6, 0.2), Vector3D(0, 0, 0), 70);

	//Other Materials
	Material *transmissive = new Transmissive(1.1, Vector3D(1));
	Material *mirror = new Mirror(Vector3D(1, 0.9, 0.85));

	/* ******* */
	/* Objects */
	/* ******* */

	// Construct the Cornell Box
	objectsList = new std::vector<Shape*>;
	double offset = 3.0;
	Matrix4x4 idTransform;


	/********  InfinitePlanes ********/
	Shape *leftPlan = new InfinitePlane(Vector3D(-offset, 0, 0), Vector3D(1, 0, 0), greenDiffuse);
	Shape *rightPlan = new InfinitePlane(Vector3D(offset, 0, 0), Vector3D(-1, 0, 0), redDiffuse);
	Shape *topPlan = new InfinitePlane(Vector3D(0, offset, 0), Vector3D(0, -1, 0), greyDiffuse);
	Shape *bottomPlan = new InfinitePlane(Vector3D(0, -offset, 0), Vector3D(0, 1, 0), greyDiffuse);
	Shape *backPlan = new InfinitePlane(Vector3D(0, 0, 3 * offset), Vector3D(0, 0, -1), blueDiffuse);
	objectsList->push_back(leftPlan);
	objectsList->push_back(rightPlan);
	objectsList->push_back(topPlan);
	objectsList->push_back(bottomPlan);
	objectsList->push_back(backPlan);

	/********   Spheres  **********/
	Matrix4x4 sphereTransform1;
	double radius = 0.5;
	sphereTransform1 = Matrix4x4::translate(Vector3D(-1, 1, 1));
	Shape *s1 = new Sphere(0.5, sphereTransform1, mirror);

	Matrix4x4 sphereTransform2;
	sphereTransform2 = Matrix4x4::translate(Vector3D(1.0, 0, 2));
	Shape *s2 = new Sphere(1, sphereTransform2, transmissive);
	Matrix4x4 sphereTransform3;

	radius = 0.85;
	sphereTransform3 = Matrix4x4::translate(Vector3D(1, -offset + radius, 5));
	Shape *s3 = new Sphere(radius, sphereTransform3, red_100);

	//objectsList->push_back(s1);
	//objectsList->push_back(s2);
	//objectsList->push_back(s3);


	/*********  Disks  *********/
	radius = .5;
	double height = 3.5;
	double innerRadius = 0.15;
	double phiMax = M_PI*2;
	Matrix4x4 diskTranslation1 = Matrix4x4::translate(Vector3D(-1, -1, 0));
	Matrix4x4 diskRotation1 = Matrix4x4::rotate(Utils::degreesToRadians(10), Vector3D(0, 1, 0));
	Matrix4x4 diskTransform1 = diskTranslation1*diskRotation1;
	Shape *d1 = new Disk(radius, diskTransform1, red_100, height, innerRadius, phiMax);
	//objectsList->push_back(d1);

	Matrix4x4 diskTransform2;
	radius = 1.5;
	height = offset*3-0.001;
	innerRadius = 0;
	phiMax = M_PI * 2;
	diskTransform2 = Matrix4x4::translate(Vector3D(0, 0, 0));
	Shape *d2 = new Disk(radius, diskTransform2, red_100, height, innerRadius, phiMax);
	//objectsList->push_back(d2);


	/******* Cylinders *********/
	radius = 0.7;
	double zMin = -1;
	double zMax = 5;
	Matrix4x4 cylindertranslation1 = Matrix4x4::translate(Vector3D(-offset + radius, -offset + radius, 7));
	Matrix4x4 rotation1 = Matrix4x4::rotate(Utils::degreesToRadians(90), Vector3D(0, 1, 0));
	Matrix4x4 cylinderTransform1 = cylindertranslation1*rotation1;
	Shape *c1 = new Cylinder(radius, zMin, zMax, cylinderTransform1, mirror);
	//objectsList->push_back(c1);

	/* ****** */
	/* Lights */
	/* ****** */

	lightSourceList = new std::vector<LightSource*>;
	Vector3D intensity = Vector3D(10, 10, 10); // Radiant intensity (watts/sr)

	// Pointlightsources //

	Vector3D lightPosition1 = Vector3D(0, offset - 0.0001, 2 * offset);
	Vector3D lightPosition2 = Vector3D(0, offset - 1, 0);
	Vector3D lightPosition3 = Vector3D(0, offset - 1, offset);
	Vector3D lightPosition4 = Vector3D(0, offset - 0.5, 5);

	LightSource *pointLS1 = new PointLightSource(lightPosition1, intensity);
	LightSource *pointLS2 = new PointLightSource(lightPosition2, intensity);
	LightSource *pointLS3 = new PointLightSource(lightPosition3, intensity);
	LightSource *pointLS4 = new PointLightSource(lightPosition4, intensity*2);

	//lightSourceList->push_back(pointLS1);
	//lightSourceList->push_back(pointLS2);
	//lightSourceList->push_back(pointLS3);
	//lightSourceList->push_back(pointLS4);

	
	// Arealightsources //
	double sizeLightSource = 0.7;
	double pointsSpacing = 0.1;

	Vector3D topPlanPos = Vector3D(0, offset - 0.0001, 2*offset);
	LightSource *areaLS1 = new AreaLightSource(topPlanPos, intensity, Vector3D(0, 0, 1), Vector3D(1, 0, 0), sizeLightSource, pointsSpacing);
	//lightSourceList->push_back(areaLS1);

	Vector3D leftPlanPos = Vector3D(-offset + 0.0001, -1, 2);
	LightSource *areaLS2 = new AreaLightSource(leftPlanPos, intensity, Vector3D(0, 1, 0), Vector3D(0, 0, 1), sizeLightSource, pointsSpacing);
	//lightSourceList->push_back(areaLS2);

	Vector3D rightPlanPos = Vector3D(offset - 0.0001, 0, 5);
	LightSource *areaLS3 = new AreaLightSource(Vector3D(offset - 0.0001, 0, 5), intensity, Vector3D(0, 1, 0), Vector3D(0, 0, 1), sizeLightSource, pointsSpacing);
	//lightSourceList->push_back(areaLS3);


	LightSource *areaLS4 = new AreaLightSource(Vector3D(0, offset - 1, 4), intensity, Vector3D(0, 1, 0), Vector3D(0, 0, 1), sizeLightSource, pointsSpacing);
	//lightSourceList->push_back(areaLS4);

	objectsList->push_back(c1);
	objectsList->push_back(s1);
	objectsList->push_back(s2);
	lightSourceList->push_back(pointLS1);
	lightSourceList->push_back(areaLS1);
	lightSourceList->push_back(areaLS2);
	lightSourceList->push_back(areaLS3);


}

int main()
{
    std::string separator     = "\n----------------------------------------------\n";
    std::string separatorStar = "\n**********************************************\n";
    std::cout << separator << "RTIS - Ray Tracer for \"Imatge Sintetica\"" << separator << std::endl;

    // Create an empty film
    Film *film;
    film = new Film(720, 576);


    // Declare the shader
    Vector3D bgColor(0.0, 0.0, 0.0); // Background color (for rays which do not intersect anything)
    Vector3D intersectionColor(1,0,0);
	Vector3D ambientTermColor(0.1, 0.1, 0.1);
	int nSamples = 5;
	int maxDepth = 1;
    //Shader *shader = new IntersectionShader (intersectionColor, bgColor);
	//Shader *shader = new DepthShader(Vector3D(0.4, 1, 0.4), 8, bgColor);

	//Global Shader takes quite longer than Direct.
	//Shader *shader = new DirectShader(bgColor);
	Shader *shader = new GlobalShader(bgColor,ambientTermColor,nSamples, maxDepth);

    // Declare pointers to all the variables which describe the scene
    Camera *cam;
    std::vector<Shape*> *objectsList;
    std::vector<LightSource*> *lightSourceList;

    // Build the scene
   // buildSceneSphere(cam, film, objectsList, lightSourceList);

	//Uncomment the scene you want to see.
	int scene;
	scene = 1;
	//scene = 2;
	//scene = 3;
	buildSceneCornellBox(cam, film, objectsList, lightSourceList, scene);


    // Launch some rays!
    raytrace(cam, shader, film, objectsList, lightSourceList);

    // Save the final result to file
    std::cout << "\n\nSaving the result to file output.bmp\n" << std::endl;
    film->save();

    std::cout << "\n\n" << std::endl;
    return 0;
}
