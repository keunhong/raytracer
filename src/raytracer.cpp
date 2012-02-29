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
    
    Material red_diffuse(Color::RED, 0.8, 0, 0, 0, 1.33);
    Material green_diffuse(Color::GREEN, 0.8, 0, 0, 0, 1.33);
    Material white_diffuse(Color::WHITE, 1, 0, 0, 0, 1.33);

    Material red_specular(Color::RED, 0.3, 0.5, 0.5, 0, 1.33);
    Material green_specular(Color::GREEN, 0.3, 0.5, 0.5, 0, 1.33);

    Material material3(Color::BLUE, 0.1, 1, 0.3, 0.9, 1.33);
    Material material4(Color::WHITE, 0, 0.3, 0.5, 0, 1.33);
    Material material6(Color::WHITE, 0, 0.5, 0.2, 0.8, 1.7);

    Scene scene;
    //scene.create_luminaire(Vec3(0, 0, -300), Color(0.5, 0.5, 0.5));
    //scene.create_luminaire(Vec3(0, 0, 50), Color(0.5, 0.5, 0.5));

    Primitive *ceiling_lamp = scene.create_rectangle(Vec3(0,-1,0), Vec3(0,309,-250), 150.0, 100.0, &red_diffuse);
    scene.create_luminaire(ceiling_lamp, Color(0.5, 0.5, 0.5));

    scene.create_plane(Vec3(0,1,0).normalize(), Vec3(0,-310,0), &white_diffuse); // floor
    scene.create_plane(Vec3(0,-1,0), Vec3(0,310,0), &white_diffuse); // ceiling
    scene.create_plane(Vec3(0,0,1), Vec3(0,0,-480), &white_diffuse); // back wall
    scene.create_plane(Vec3(1,0,0), Vec3(-320,0,0), &red_diffuse); // left wall
    scene.create_plane(Vec3(-1,0,0), Vec3(320,0,0), &green_diffuse); // right wall
    scene.create_plane(Vec3(0,0,1), Vec3(0,0,480), &white_diffuse); // front wall wall

    scene.create_sphere(Vec3(-120, -100, -320), 100, &red_specular);
    scene.create_sphere(Vec3(120, -100, -320), 100, &green_specular);
    //scene.create_sphere(Vec3(-120, -140, -140), 50, &material3);
    //scene.create_sphere(Vec3(120, -140, -140), 50, &material6);
    scene.create_sphere(Vec3(-50, 0, -70), 50, &material6);

    Camera camera(&scene, Vec3(0, 0, 0), 640, 480, M_PI/3);
    cv::Mat result(cv::Size(camera.get_width(),camera.get_height()), CV_8UC3, cv::Scalar(0));

    camera.render(result, 2);

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
