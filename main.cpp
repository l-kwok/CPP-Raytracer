#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include <GL/glut.h>
#include "Vec.h"
#include "Sphere.h"
#include "Light.h"
#include "Plane.h"

#define PI 3.141592653589793
#define INF 100000000

int WIDTH = 400;
int HEIGHT = 400;
double FOV = 15;
double LIGHT_INTENSITY = 0.5; //1 is 100%
Vec BG_COLOR = Vec(1); //0 = black, 1 = white;
Vec *image = new Vec[WIDTH * HEIGHT];
int MAX_RAY_DEPTH = 50;
double INDEX_OF_REFRACTION = 1.5;


/*
https://stackoverflow.com/a/2654860/4250923
This function is based on this stackoverflow post about writing RGB pixels to a .bmp image format.
The code has been modified to work with my code
*/
void savebmp (const char *filename, Vec* image) {
	FILE *f;
	int nPixels = WIDTH*HEIGHT;
	int filesize = 54 + 3*nPixels;
	
	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};
	
	bmpfileheader[ 2] = (unsigned char)(filesize);
	bmpfileheader[ 3] = (unsigned char)(filesize>>8);
	bmpfileheader[ 4] = (unsigned char)(filesize>>16);
	bmpfileheader[ 5] = (unsigned char)(filesize>>24);
	
	bmpinfoheader[ 4] = (unsigned char)(WIDTH);
	bmpinfoheader[ 5] = (unsigned char)(WIDTH>>8);
	bmpinfoheader[ 6] = (unsigned char)(WIDTH>>16);
	bmpinfoheader[ 7] = (unsigned char)(WIDTH>>24);
	
	bmpinfoheader[ 8] = (unsigned char)(HEIGHT);
	bmpinfoheader[ 9] = (unsigned char)(HEIGHT>>8);
	bmpinfoheader[10] = (unsigned char)(HEIGHT>>16);
	bmpinfoheader[11] = (unsigned char)(HEIGHT>>24);
	
	f = fopen(filename,"wb");
	
	fwrite(bmpfileheader,1,14,f);
	fwrite(bmpinfoheader,1,40,f);
	
	for (int i = nPixels; i > 0; i--) {

		 unsigned char color[3] = {(unsigned char)(std::min(double(1), image[i].z) * 255),(unsigned char)(std::min(double(1), image[i].y) * 255),(unsigned char)(std::min(double(1), image[i].x) * 255)};
		
		fwrite(color,1,3,f);
	}
	
	fclose(f);
    std::cout<<"Scene Rendered, Filename: \""<<filename<<"\""<<std::endl;
}

Vec trace(Vec ray_origin, Vec ray_direction, std::vector<Sphere> objects, int curr_depth){
    double near = INF;
    double r1, r2;
    Sphere* sphere = nullptr;
    for (int i = 0; i < (int)objects.size(); i++) {
        if (objects[i].intersect(ray_origin, ray_direction, r1, r2)) {
            if (r1 < near) {
                near = r1;
                sphere = &objects[i];
            }
        }
    }
    if (!sphere)
        return BG_COLOR;
    
    Vec final_color = 0; 
    Vec intersect_pos = ray_origin + ray_direction * near; 
    Vec intersect_pos_normal = (intersect_pos - sphere->center).normalize();

    double accuracy = 0.000001; 
    bool inside = false;
    if (ray_direction.dot(intersect_pos_normal) > 0){
        intersect_pos_normal = -intersect_pos_normal;
        inside = true;
    } 

    if ((sphere->material.opacity > 0 || sphere->material.shininess > 0) && curr_depth < MAX_RAY_DEPTH) {
        Vec reflection_direction = (ray_direction - intersect_pos_normal * 2 * ray_direction.dot(intersect_pos_normal)).normalize();
        Vec reflection = trace(intersect_pos + intersect_pos_normal * accuracy, reflection_direction, objects, curr_depth + 1);
        Vec refraction = 0;
  
        if (sphere->material.opacity) {
            double idor_actual; //index of refraction actual
            if(inside){
                idor_actual = INDEX_OF_REFRACTION;
            }else{
                idor_actual = 1/INDEX_OF_REFRACTION;
            }
            double cos_angle = -intersect_pos_normal.dot(ray_direction);
            Vec refraction_direction = (ray_direction * idor_actual + intersect_pos_normal * (idor_actual *  cos_angle - sqrt(1 - pow(idor_actual,2) * (1 - pow(cos_angle,2))))).normalize();
            refraction = trace(intersect_pos - intersect_pos_normal * accuracy, refraction_direction, objects, curr_depth + 1);
        }  
    
        final_color = (reflection * 0.75 + refraction * 0.5 * sphere->material.opacity) * sphere->material.color;
    }
    else {
        for (int i = 0; i < (int)objects.size(); i++) {
            if (objects[i].material.emitted_color.x > 0) {
                Vec ray_direction = (objects[i].center - intersect_pos).normalize(); 
                Vec trans = 1;
                for (int j = 0; j < (int)objects.size(); j++) {     
                    if (i != j) {  
                        double r1, r2;
                        if (objects[j].intersect(intersect_pos + intersect_pos_normal * accuracy, ray_direction, r1, r2)) {
                            trans = 0;
                            break;
                        } 
                    }
                }
                
                final_color = final_color + (sphere->material.color * trans *std::max(double(0), intersect_pos_normal.dot(ray_direction)) * objects[i].material.emitted_color);
            }
        } 
    }
    
    return (final_color + sphere->material.emitted_color)*LIGHT_INTENSITY;
}

void render(std::vector<Sphere> objects, const char* filename){

    //x = red, y = green, z = blue

	Vec *curr_pixel = image; 
    double x_amnt;
    double y_amnt;
    // Trace rays
    for (int h = 0; h < HEIGHT; h++) {
        for (int w = 0; w < WIDTH; w++) {
            x_amnt = (2 * ((w + 0.5)/WIDTH) - 1) * tan(PI * FOV / 180);
            y_amnt = (1 - 2 * ((h + 0.5)/HEIGHT)) * tan(PI * FOV / 180);
            Vec ray_direction(x_amnt, y_amnt, -1);
            *curr_pixel = trace(Vec(0), ray_direction.normalize(), objects, 0);
            curr_pixel++;
        }
    }
	savebmp(filename, image);
}

 
void scene1(){
    std::cout<<"Rendering Scene 1..."<<std::endl;
    std::vector<Sphere> objects;
    Material floor_mat(Vec(0.20, 0.20, 0.20), 0, 0);
    Material aqua(Vec(0.46, 0.76, 0.90), 0, 0);
    
    objects.push_back(Plane(Vec( 0.0, -10004, -20), floor_mat));
    objects.push_back(Sphere(Vec( 0.0, -1.75, -20), 2, aqua));
    
    objects.push_back(Light(Vec( -30, 100, 50), 3, Vec(3)));
    render(objects, "scene1.bmp");
}

void scene2(){
    std::cout<<"Rendering Scene 2..."<<std::endl;     
    std::vector<Sphere> objects; 
    LIGHT_INTENSITY = 0.7;
    Material floor_mat(Vec(0.20, 0.20, 0.20), 0, 0);
    Material chrome(Vec(1, 1, 1), 0, 1);
    Material aqua(Vec(0.46, 0.76, 0.90), 0, 0);
    Material red(Vec(0.9, 0.4, 0.46), 0, 0);
    Material yellow(Vec(0.9, 0.76, 0.3), 0, 0);
    
    objects.push_back(Plane(Vec( 0.0, -10004, -20), floor_mat));
    objects.push_back(Sphere(Vec( 0.0, 0.5, -30), 4, chrome));
    objects.push_back(Sphere(Vec(3.0, -3.25, -27), 0.75, aqua));
    objects.push_back(Sphere(Vec(-3.0, -3.25, -27), 0.75, red));
    objects.push_back(Sphere(Vec(0, -3.25, -20), 0.75, yellow));

    objects.push_back(Light(Vec( -30, 100, 50), 3, Vec(3)));

    render(objects, "scene2.bmp");
}

void scene3(){
    std::cout<<"Rendering Scene 3..."<<std::endl;
    std::vector<Sphere> objects; 
    Material floor_mat(Vec(0.20, 0.20, 0.20), 0, 0);
    Material clear(Vec(1, 1, 1), 1, 1);
    Material aqua(Vec(0.46, 0.76, 0.90), 0, 0);
    Material red(Vec(0.9, 0.4, 0.46), 0, 0);
    Material yellow(Vec(0.9, 0.76, 0.3), 0, 0);
    
    objects.push_back(Plane(Vec( 0.0, -10004, -20), floor_mat));
    objects.push_back(Sphere(Vec( 0.0, 0.5, -30), 4, clear));
    objects.push_back(Sphere(Vec( 4.0, 0.5, -40), 4, aqua));
    objects.push_back(Sphere(Vec(-4.0, -0.75, -40), 3, red));
    objects.push_back(Light(Vec( -30, 100, 50), 3, Vec(3)));
    render(objects, "scene3.bmp");
}

void scene4(){
    std::cout<<"Rendering Scene 4..."<<std::endl;
    std::vector<Sphere> objects;
    LIGHT_INTENSITY = 1;
    Material red(Vec(1,0.25,0.25), 0, 1);
    Material green(Vec(0.25,1,0.25),0, 1);
    Material blue(Vec(0.25,0.25,1), 0, 1);
    Material blackChrome(Vec(0.2, 0.2, 0.2), 0, 0.5);
    
    objects.push_back(Plane(Vec( 0.0, -10004, -20), blackChrome));
    objects.push_back(Sphere(Vec( 0.0, -1.75, -30), 2, red));
    objects.push_back(Sphere(Vec( 4.5, -1.75, -30), 2, green));
    objects.push_back(Sphere(Vec( -4.5, -1.75, -30), 2, blue));

    objects.push_back(Light(Vec( -30, 100, 50), 3, Vec(3)));
    render(objects, "scene4.bmp");
}

void display(void){ 
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);  
    int curr_pixel = WIDTH*HEIGHT;
    for(int w = 0; w < WIDTH; w++){
        for(int h = 0; h < HEIGHT; h++){
            glBegin(GL_POINTS);             
                glColor3f(image[curr_pixel].x, image[curr_pixel].y, image[curr_pixel].z); // Red
                glVertex2f(h,  w);
            glEnd();
            curr_pixel--;
        }
    }

    glFlush();
} 

void init (void) {
    glViewport(0,0,WIDTH, HEIGHT);
    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
}

int main(int argc, char **argv){
    int scene = 0;
    if(argc > 1){
        scene = *argv[1]-'0';
    }
    switch(scene){
        case 1:
            scene1();
            break;
        case 2:
            scene2();
            break;
        case 3:
            scene3();
            break;
        case 4:
            scene4();
            break;
        default:
            scene1();
            scene2();
            scene3();
            scene4();
            break;
    }

    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(10, 1);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Assignment 3");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}