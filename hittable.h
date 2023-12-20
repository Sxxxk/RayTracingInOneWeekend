#pragma once
//���Ƕ����
//��ײ���¼����
class hit_record {
public:
	point3 p;//��ײ��λ��
	vec3 normal;//��ײ��ı��淨��
	double t;//��ײ�������ϵ�ʱ��t����
};
// ��������ײ���������
class hittable {
public:
	virtual ~hittable() = default;
	// �����������ײ�ĺ����������ײ�᷵����ײ���¼���ݣ�������������ʵ��
	virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};