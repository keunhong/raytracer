#include <iostream>
#include "vec3.hpp"
#include "scene.hpp"
#include "material.hpp"
#include "camera.hpp"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cmath>

#include <google/profiler.h>

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
    cout << "RayTracer 2012" << endl; 
    cout << "Initializing Scene" << endl;
    
    Material material1(Color::RED, 0.4, 1, 1, 0, 1.33);
    Material material2(Color::GREEN, 0.4, 1, 1, 0, 1.33);
    Material material3(Color::BLUE, 0.1, 1, 0.3, 0.9, 1.33);
    Material material4(Color::WHITE, 0, 0.3, 0.5, 0, 1.33);
    Material material5(Color::WHITE, 1, 0, 0, 0, 1.7);
    Material material6(Color::WHITE, 0, 1, 0.5, 0.5, 1.7);

    Scene scene;
    scene.create_lightsource(Vec3(0, 0, -300), Color(0.5, 0.5, 0.5));
    scene.create_lightsource(Vec3(0, 0, 50), Color(0.5, 0.5, 0.5));

    Primitive *floor = scene.create_plane(Vec3(0.2,1,0).normalize(), Vec3(0,-310,0), &material4); // floor
    scene.create_plane(Vec3(0,-1,0), Vec3(0,310,0), &material4); // ceiling
    scene.create_plane(Vec3(0,0,1), Vec3(0,0,-480), &material4); // back wall
    scene.create_plane(Vec3(1,0,0), Vec3(-320,0,0), &material4); // left wall
    scene.create_plane(Vec3(-1,0,0), Vec3(320,0,0), &material4); // right wall

    scene.create_sphere(Vec3(-120, -100, -320), 100, &material1);
    scene.create_sphere(Vec3(120, -100, -320), 100, &material2);
    scene.create_sphere(Vec3(-120, -140, -140), 50, &material3);
    scene.create_sphere(Vec3(120, -140, -140), 50, &material6);

    Camera camera(&scene, Vec3(0, 0, 0), 640, 480, M_PI/2);
    cv::Mat result(cv::Size(camera.get_width(),camera.get_height()), CV_8UC3, cv::Scalar(0));

    camera.render(result, 4);

    cv::startWindowThread();
    cv::namedWindow("result", CV_WINDOW_NORMAL | CV_GUI_EXPANDED);
    while(true){
        char c = cv::waitKey(10);
        // Quit on escape
        if(c == 27){
            break;
        }
        cv::imshow("result", result);
    }
    cv::destroyWindow("result");

    cv::imwrite("result.png", result);

    return EXIT_SUCCESS;
}
