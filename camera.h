#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "util.h"

class camera
{
public:
	camera(void);
	~camera(void);

	camera(vec3f* position);

        void pitch(float pitch);
        void yaw(float yaw);
        mat4f GetMatrix();

	void strafe(float dir);
	void move(float dir);

	void updateTime(float time);

        vec3f up_, right_, look_, position_;
        float difference_, pitch_, yaw_;
};

#endif /* _CAMERA_H_ */
