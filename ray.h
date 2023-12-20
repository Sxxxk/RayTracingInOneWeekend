#pragma once
#include "vec3.h"



class ray {
//射线类。注意一条射线就是以一个起点和一个方向所定义的
public:
	//默认构造函数
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

