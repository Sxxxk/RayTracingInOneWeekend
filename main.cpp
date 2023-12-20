#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

//添加一个球体并判断射线是否与其相交
//半球体方程的向量表示为（p-c)·(p-c) = R^2，其中p是球面上的点，c是球心
//如果射线p(t)与球体相交，那么必然存在一点t使得p(t)满足球面方程，即(p(t)-c)·(p(t)-c) = R^2
//展开可得(a+t*b-c)·(a+t*b-c)=R^2,即(b·b)t^2 + 2*b·(a-c)*t+常数项，即二元一次方程
// 根据求根公式b^2-4ac即可得到有几个解（代码中的discriminant），即有几个交点
double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    /*auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;*/
    auto a = r.direction().length_squared();
    //由于之后要除以2a，因此可以在这里先把b除以2
    auto half_b = dot(oc, r.direction());
    //用接口代替点乘
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;

    //判断是否相交
    //return (discriminant >= 0);

    //返回表面的法线方向
    //表面法线即为射线与球面的交点减去球心
    if (discriminant < 0) {
        //射线与球不相交
        return -1.0;
    }
    else{
        //否则返回较小的t值（较小的t离相机更近，是近交点。较大的t是远交点）
        //return (-b - sqrt(discriminant)) / (2.0 * a);
        return (-half_b - sqrt(discriminant)) / a;
    }
}

//颜色采用了线性插值的方法
color ray_color(const ray& r) {
    //如果与射线与球面相交，就将这个像素设置为红，即将球所占的区域设置为红色
    /*if (hit_sphere(point3(0, 0, -1), 0.5, r))
        return color(1, 0, 0);*/

    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        //如果存在交点,则可以得到法线的方向，并且归一化
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
        //归一化之后从(-1~1)映射到(0~1)
        return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
    }

    //得到方向的归一化向量
    vec3 unit_direction = unit_vector(r.direction());
    //以y方向的坐标作为插值的系数,注意y方向的范围是-1~1
    auto a = 0.5 * (unit_direction.y() + 1.0);
    //a作为系数，在蓝色和白色之间插值
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}


int main() {

    // Image
    //得到宽高比aspect
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    //计算图像的高度，并确保其至少为1
    int image_height = static_cast<int>(image_width / aspect_ratio);
    
    //Camera
    //焦距，即相机到虚拟视口的距离
    auto focal_length = 1.0;
    //定义相机的虚拟视口
    auto viewport_height = 2.0;
    //虚拟视口宽度 = 视口高度 *宽高比。因为虚拟视口的比例要和图片的比例相同
    auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
    //相机的位置
    auto camera_center = point3(0, 0, 0);

    //得到视口的uv坐标轴.注意在教程中，视口的uv原点位于屏幕的左上角，因此u向右v向下
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    //得到各个像素之间的距离差
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;
    //-vec3(0, 0, focal_length)得到相机到虚拟视口的投影（正中心）
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    //得到(0,0)像素的世界空间坐标
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            //得到当前着色像素的世界坐标
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            //得到光线方向
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}