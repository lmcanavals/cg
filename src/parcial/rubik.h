#ifndef __RUBIK_H__
#define __RUBIK_H__

#include <vector>

// TODO validate all the things in the universe

class Rubik {

	struct Pos {
		glm::mat4 model;
		glm::vec3 pos;
		u32       idx;
	};
	typedef std::vector<Pos*> v1d;
	typedef std::vector<v1d>  v2d;
	typedef std::vector<v2d>  v3d;

	v3d cube;

	public:

	Rubik() : cube(3, v2d(3, v1d(3, nullptr))) {
		for (u32 i = 0; i < cube.size(); ++i) {
			for (u32 j = 0; j < cube.size(); ++j) {
				for (u32 k = 0; k < cube.size(); ++k) {
					auto model = glm::mat4(1.0f);
					auto pos   = glm::vec3(i - 1.0f, j - 1.0f, k - 1.0f);
					model = glm::translate(model, pos);
					cube[i][j][k] = new Pos{ model, pos, i*9 + j*3 + k };
				}
			}
		}
	}
	~Rubik() {
		for (auto f : cube) for (auto r : f) for (auto c : r) delete c;
	}

	void rotateX(u32 i, f32 dir=1.0f) { // π/2 in 1 go, animation for finals!
		for (u32 j = 0; j < cube.size(); ++j) {
			for (u32 k = 0; k < cube.size(); ++k) {
				auto block = cube[i][j][k];
				block->model = glm::translate(block->model, -block->pos);
				block->model = glm::rotate(block->model, (f32)M_PI/-2.0f,
						{1.0f, 0.0f, 0.0f});
				block->model = glm::translate(block->model, block->pos);
			}
		}
		// ugh, this part is hard, now physically rotate everything
		auto temp = cube[i][0][0];
		cube[i][0][0] = cube[i][0][2];
		cube[i][0][2] = cube[i][2][2];
		cube[i][2][2] = cube[i][2][0];
		cube[i][2][0] = temp;

		temp = cube[i][0][1];
		cube[i][0][1] = cube[i][1][2];
		cube[i][1][2] = cube[i][2][1];
		cube[i][2][1] = cube[i][1][0];
		cube[i][1][0] = temp;
	}

	void rotateY(u32 j, f32 dir=1.0f) { // π/2 in 1 go, animation for finals!
		for (u32 i = 0; i < cube.size(); ++i) {
			for (u32 k = 0; k < cube.size(); ++k) {
				auto block = cube[i][j][k];
				block->model = glm::translate(block->model, -block->pos);
				block->model = glm::rotate(block->model, (f32)M_PI/-2.0f,
						{0.0f, 1.0f, 0.0f});
				block->model = glm::translate(block->model, block->pos);
			}
		}
		// ugh, this part is hard, now physically rotate everything
		auto temp = cube[0][j][0];
		cube[0][j][0] = cube[0][j][2];
		cube[0][j][2] = cube[2][j][2];
		cube[2][j][2] = cube[2][j][0];
		cube[2][j][0] = temp;

		temp = cube[0][j][1];
		cube[0][j][1] = cube[1][j][2];
		cube[1][j][2] = cube[2][j][1];
		cube[2][j][1] = cube[1][j][0];
		cube[1][j][0] = temp;
	}

	glm::mat4 getModel(u32 i, u32 j, u32 k) {
		return cube[i][j][k]->model;
	}

};

#endif

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

