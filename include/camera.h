#ifndef __CAM_H__
#define __CAM_H__

enum CameraMovement {
	FORWARD, LEFT, BACKWARD, RIGHT
};

class Cam {
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	f32       yaw;
	f32       pitch;
	f32       zoom;
	f32       mouseSensitivity;
	f32       speed;

public:
	Cam(f32 px    =   0.0f, f32 py    =   0.0f, f32 pz    =   0.0f,
			f32 fx    =   0.0f, f32 fy    =   0.0f, f32 fz    =  -1.0f,
			f32 ux    =   0.0f, f32 uy    =   1.0f, f32 uz    =   0.0f,
			f32 yaw   = -90.0f, f32 pitch =   0.0f, f32 zoom  =  45.0f)
			:	position({px, py, pz}), front({fx, fy, fz}), up({ux, uy, uz}),
				worldUp({ux, uy, uz}), yaw(yaw), pitch(pitch), zoom(zoom),
				mouseSensitivity(0.1f), speed(2.5f) {
		updateVectors();
	}

	glm::mat4 getViewM4() {
		return glm::lookAt(position, position + front, up);
	}
	f32 getZoom() {
		return glm::radians(zoom);
	}

	void processKeyboard(CameraMovement direction, f32 deltaTime) {
		f32 velocity = speed * deltaTime;
		if (direction == FORWARD) {
			position += front * velocity;
		} else if (direction == BACKWARD) {
			position -= front * velocity;
		} else if (direction == LEFT) {
			position -= right * velocity;
		} else if (direction == RIGHT) {
			position += right * velocity;
		}
	}
	void processMouse(f32 xoffset, f32 yoffset, bool constrainPitch = true) {
		yaw   = xoffset * mouseSensitivity;
		pitch = yoffset * mouseSensitivity;
		if (constrainPitch) {
			if (pitch > 89.0f) {
				pitch = 89.0f;
			} else if (pitch < -89.0f) {
				pitch = -89.0f;
			}
		}
		updateVectors();
	}
	void processScroll(f32 yoffset) {
		zoom -= (f32)yoffset;
		if (zoom < 1.0f) {
			zoom = 1.0f;
		} else if (zoom > 45.0f) {
			zoom = 45.0f;
		}
	}

private:
	void updateVectors() {
		glm::vec3 f;

		f.x   = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		f.y   = sin(glm::radians(pitch));
		f.z   = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(f);
		right = glm::normalize(glm::cross(front, worldUp));
		up    = glm::normalize(glm::cross(right, front));
	}
};

#endif
