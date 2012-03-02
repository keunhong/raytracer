#include <iostream>
#include "vec3.hpp"
#include "scene.hpp"
#include "material.hpp"
#include "camera.hpp"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cmath>
#include <cstdlib>
#include <ctime>

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
    std::srand(std::time(NULL));

    cout << "RayTracer 2012" << endl; 
    cout << "Initializing Scene" << endl;
    
    Material red_diffuse(Color::RED, 0.8, 0, 0, 0, 1.33);
    Material green_diffuse(Color::GREEN, 0.8, 0, 0, 0, 1.33);
    Material white_diffuse(Color::WHITE, 1, 0, 0, 0, 1.33);

    Material red_specular(Color::RED, 0.3, 0.5, 0.5, 0, 1.33);
    Material green_specular(Color::GREEN, 0.3, 0.5, 0.5, 0, 1.33);
    Material blue_specular(Color::BLUE, 0.3, 0.5, 0.5, 0, 1.33);
    Material red1(Color::RED, 0.7, 0.5, 0.3, 0, 1.33);

    Material material3(Color::BLUE, 0.1, 1, 0.3, 0.9, 1.33);
    Material material4(Color::WHITE, 0, 0.3, 1, 0, 1.33);
    Material material6(Color::WHITE, 0, 0.5, 0.0, 1, 1.7);

    Scene scene;
    //Primitive *orb_light = scene.create_sphere(Vec3(0, 0, -300), 40, &blue_specular);
    //scene.create_luminaire(orb_light, Color(0.5, 0.5, 0.5));

    Rectangle *ceiling_lamp = scene.create_rectangle(Vec3(-150,309,-100), Vec3(0, 0, -300), Vec3(300, 0, 0), &red_diffuse);
    scene.create_luminaire(ceiling_lamp, Color(0.5, 0.5, 0.5));
    ceiling_lamp->generate_samples(10,10);

    scene.create_plane(Vec3(0,1,0).normalize(), Vec3(0,-310,0), &white_diffuse); // floor
    scene.create_plane(Vec3(0,-1,0), Vec3(0,310,0), &white_diffuse); // ceiling
    scene.create_plane(Vec3(0,0,1), Vec3(0,0,-1000), &white_diffuse); // back wall
    scene.create_plane(Vec3(1,0,0), Vec3(-320,0,0), &red_diffuse); // left wall
    scene.create_plane(Vec3(-1,0,0), Vec3(320,0,0), &green_diffuse); // right wall
    scene.create_plane(Vec3(0,0,-1), Vec3(0,0,1000), &white_diffuse); // front wall wall

    scene.create_sphere(Vec3(-120, -210+0.1, -700), 130, &blue_specular);
    scene.create_sphere(Vec3(120, -260+0.1, -100), 50, &green_specular);
    scene.create_sphere(Vec3(120, -210+0.1, -360), 100, &material4);
    //scene.create_sphere(Vec3(-120, -140, -140), 50, &material3);
    //scene.create_sphere(Vec3(120, -140, -140), 50, &material6);
    Primitive *red_ball = scene.create_sphere(Vec3(-100, -310+70.1, -200), 70, &red1);
    red_ball->set_velocity(Vec3(-10, 0, 15));
    Primitive *glass_ball = scene.create_sphere(Vec3(0.0, -260+0.1, 0.0), 50, &material6);
    glass_ball->set_velocity(Vec3(10, 0, 0));

    Camera camera(&scene, Vec3(0, 0, 500), 640, 480, M_PI/2);
    cv::Mat result(cv::Size(camera.get_width(),camera.get_height()), CV_8UC3, cv::Scalar(0));

    //camera.render(result, 1, 4.0, 0.1);
    camera.render(result, 4, 1.0, 1.0);

//    cv::flip(result, result, -1);
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
