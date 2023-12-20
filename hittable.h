#pragma once
//考虑多个球
//碰撞点记录数据
class hit_record {
public:
	point3 p;//碰撞点位置
	vec3 normal;//碰撞点的表面法线
	double t;//碰撞在射线上的时间t参数
};
// 与射线碰撞的物体基类
class hittable {
public:
	virtual ~hittable() = default;
	// 计算和射线碰撞的函数，如果碰撞会返回碰撞点记录数据，本函数由子类实现
	virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};