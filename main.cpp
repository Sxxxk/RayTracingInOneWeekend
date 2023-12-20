#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

//���һ�����岢�ж������Ƿ������ཻ
//�����巽�̵�������ʾΪ��p-c)��(p-c) = R^2������p�������ϵĵ㣬c������
//�������p(t)�������ཻ����ô��Ȼ����һ��tʹ��p(t)�������淽�̣���(p(t)-c)��(p(t)-c) = R^2
//չ���ɵ�(a+t*b-c)��(a+t*b-c)=R^2,��(b��b)t^2 + 2*b��(a-c)*t+���������Ԫһ�η���
// ���������ʽb^2-4ac���ɵõ��м����⣨�����е�discriminant�������м�������
double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    /*auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;*/
    auto a = r.direction().length_squared();
    //����֮��Ҫ����2a����˿����������Ȱ�b����2
    auto half_b = dot(oc, r.direction());
    //�ýӿڴ�����
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;

    //�ж��Ƿ��ཻ
    //return (discriminant >= 0);

    //���ر���ķ��߷���
    //���淨�߼�Ϊ����������Ľ����ȥ����
    if (discriminant < 0) {
        //���������ཻ
        return -1.0;
    }
    else{
        //���򷵻ؽ�С��tֵ����С��t������������ǽ����㡣�ϴ��t��Զ���㣩
        //return (-b - sqrt(discriminant)) / (2.0 * a);
        return (-half_b - sqrt(discriminant)) / a;
    }
}

//��ɫ���������Բ�ֵ�ķ���
color ray_color(const ray& r) {
    //����������������ཻ���ͽ������������Ϊ�죬��������ռ����������Ϊ��ɫ
    /*if (hit_sphere(point3(0, 0, -1), 0.5, r))
        return color(1, 0, 0);*/

    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        //������ڽ���,����Եõ����ߵķ��򣬲��ҹ�һ��
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
        //��һ��֮���(-1~1)ӳ�䵽(0~1)
        return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
    }

    //�õ�����Ĺ�һ������
    vec3 unit_direction = unit_vector(r.direction());
    //��y�����������Ϊ��ֵ��ϵ��,ע��y����ķ�Χ��-1~1
    auto a = 0.5 * (unit_direction.y() + 1.0);
    //a��Ϊϵ��������ɫ�Ͱ�ɫ֮���ֵ
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}


int main() {

    // Image
    //�õ���߱�aspect
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    //����ͼ��ĸ߶ȣ���ȷ��������Ϊ1
    int image_height = static_cast<int>(image_width / aspect_ratio);
    
    //Camera
    //���࣬������������ӿڵľ���
    auto focal_length = 1.0;
    //��������������ӿ�
    auto viewport_height = 2.0;
    //�����ӿڿ�� = �ӿڸ߶� *��߱ȡ���Ϊ�����ӿڵı���Ҫ��ͼƬ�ı�����ͬ
    auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
    //�����λ��
    auto camera_center = point3(0, 0, 0);

    //�õ��ӿڵ�uv������.ע���ڽ̳��У��ӿڵ�uvԭ��λ����Ļ�����Ͻǣ����u����v����
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    //�õ���������֮��ľ����
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;
    //-vec3(0, 0, focal_length)�õ�����������ӿڵ�ͶӰ�������ģ�
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    //�õ�(0,0)���ص�����ռ�����
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            //�õ���ǰ��ɫ���ص���������
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            //�õ����߷���
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}