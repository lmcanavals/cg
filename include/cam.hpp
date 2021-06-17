#ifndef __CAM_H__
#define __CAM_H__

#include <glutil.hpp>

enum CamMove { FORWARD, LEFT, BACKWARD, RIGHT };

class Cam {

public:
	glm::vec3 pos;
	glm::vec3 up;
	glm::vec3 lookat;
	glm::vec3 right;
	glm::vec3 worldUp;
	f32       yaw;
	f32       pitch;
	f32       zoom;
	f32       mouseSensitivity;
	f32       zoomSensitivity;
	f32       speed;

	Cam(glm::vec3 pos = {0.0f, 0.0f,  0.0f}, glm::vec3 up = {0.0f, 1.0f,  0.0f},
			f32 yaw = -M_PI / 2.0f, f32 pitch = 0.0f)
			:	pos(pos), up(up), lookat({0.0f, 0.0f, 0.0f}),
				right(glm::vec3(0.0f)), worldUp(up),
				yaw(yaw), pitch(pitch), zoom(M_PI / 4.0f),
				mouseSensitivity(0.01f), zoomSensitivity(0.005f),
				speed(2.5f), firstMouse(true) {
		updateVectors();
	}

	glm::mat4 getViewM4() {
		return glm::lookAt(pos, pos + lookat, up);
	}

	void movePov(f64 xpos, f64 ypos) {
		if (firstMouse) {
			lastx = xpos;
			lasty = ypos;
			firstMouse = false;
			return;
		}
		processPov((f32)(xpos - lastx), (f32)(lasty - ypos));
		lastx = xpos;
		lasty = ypos;
	}
	void stopPov() {
		firstMouse = true;
	}

	void processKeyboard(CamMove direction, f32 deltaTime) {
		f32 velocity = speed * deltaTime;
		if (direction == FORWARD) {
			pos += lookat * velocity;
		} else if (direction == BACKWARD) {
			pos -= lookat * velocity;
		} else if (direction == LEFT) {
			pos -= right * velocity;
		} else if (direction == RIGHT) {
			pos += right * velocity;
		}
	}
	void processScroll(f32 yoffset) {
		zoom -= (f32)yoffset * zoomSensitivity;
		if (zoom < M_PI / 180.0f) {
			zoom = M_PI / 180.0f;
		} else if (zoom > M_PI / 4.0f) {
			zoom = M_PI / 4.0f;
		}
	}

private:
	f32  lastx;
	f32  lasty;
	bool firstMouse;

	void processPov(f32 xoffset, f32 yoffset, bool constrainPitch = true) {
		yaw   += xoffset * mouseSensitivity;
		pitch += yoffset * mouseSensitivity;
		if (constrainPitch) {
			if (pitch > M_PI / 2.02f) {
				pitch = M_PI / 2.02f;
			} else if (pitch < -M_PI / 2.02f) {
				pitch = -M_PI / 2.02f;
			}
		}
		updateVectors();
	}
	void updateVectors() {
		glm::vec3 temp;

		temp.x = cos(yaw) * cos(pitch);
		temp.y = sin(pitch);
		temp.z = sin(yaw) * cos(pitch);
		lookat = glm::normalize(temp);
		right  = glm::normalize(glm::cross(lookat, worldUp));
		up     = glm::normalize(glm::cross(right, lookat));
	}
};

#endif
