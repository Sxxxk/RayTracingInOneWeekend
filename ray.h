#pragma once
#include "vec3.h"



class ray {
//�����ࡣע��һ�����߾�����һ������һ�������������
public:
	//Ĭ�Ϲ��캯��
	ray(){}
	ray(const point3& origin, const vec3& dirction) :orig(origin), dir(dirction) {}
	inline point3 origin() const { return orig;}
	inline vec3 direction() const { return dir; }
	point3 at(double t) const{
		return orig + t * dir;
	}
private:
	point3 orig;
	vec3 dir;
};

