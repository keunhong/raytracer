#ifndef KP_CAMERA_H
#define KP_CAMERA_H

#include <algorithm>
#include <cmath>
#include "vec3.hpp"
#include "scene.hpp"
#include <opencv/cv.h>
#include "ray.hpp"

using std::max;
using std::min;
using std::cout;
using std::endl;

#define TRACE_EMPTY true
#define TRACE_VALID false



struct Trace{
    Color color;
    bool empty;
    Primitive *primitive;
    Trace(Color c, bool e, Primitive *p) : color(c), empty(e), primitive(p){}
};

class Camera{
    Scene *scene;

    Vec3 focal_point;
    Vec3 optical_center;
    Vec3 look_at;
    
    int width;
    int height;
    double fovx;
    double fovy;
    double focal_length;

  public:
    Camera();
    Camera(Scene *scene_, Vec3 image_center, int width_, int height_, double fovx_);
    void render(cv::Mat &im, int rays_per_pixel, double exposure, double time_step) const;
    int get_width() const;
    int get_height() const;

  private:
    double get_plane_z() const;
    Trace trace_ray(const Ray& ray, double rho, unsigned int depth) const;
    Intersection find_nearest_intersection(const Ray& ray) const;
};

#endif
