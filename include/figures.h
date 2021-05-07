#ifndef __FIGURES_H__
#define __FIGURES_H__

class Cube {

	f32   width;
	f32   height;
	f32   depth;
	f32*  vertices;
	u32* indices;

public:
	Cube(f32 width=1.0f, f32 height=1.0f, f32 depth=1.0f)
			: width(width), height(height), depth(depth),
				vertices(new f32[16*8]), indices(new u32[6*6]) {
		f32 wm = width / 2.0f;
		f32 hm = height / 2.0f;
		f32 dm = depth / 2.0f;
		f32 temp[] = {
			 // posiciones        colores          texturas
			-wm,  hm,  dm,   1.0f, 0.0, 0.0,   0.0f, 1.0f,  // 0
			 wm,  hm,  dm,   1.0f, 0.0, 0.0,   1.0f, 1.0f,  // 1
			-wm, -hm,  dm,   1.0f, 0.0, 0.0,   0.0f, 0.0f,  // 2
			 wm, -hm,  dm,   1.0f, 0.0, 0.0,   1.0f, 0.0f,  // 3
			-wm,  hm, -dm,   1.0f, 0.0, 0.0,   1.0f, 1.0f,  // 4
			 wm,  hm, -dm,   1.0f, 0.0, 0.0,   0.0f, 1.0f,  // 5
			-wm, -hm, -dm,   1.0f, 0.0, 0.0,   1.0f, 0.0f,  // 6
			 wm, -hm, -dm,   1.0f, 0.0, 0.0,   0.0f, 0.0f,  // 7

			-wm,  hm,  dm,   1.0f, 0.0, 0.0,   0.0f, 0.0f,  // 8
			 wm,  hm,  dm,   1.0f, 0.0, 0.0,   1.0f, 0.0f,  // 9
			-wm, -hm,  dm,   1.0f, 0.0, 0.0,   0.0f, 0.0f,  // 10
			 wm, -hm,  dm,   1.0f, 0.0, 0.0,   1.0f, 0.0f,  // 11
			-wm,  hm, -dm,   1.0f, 0.0, 0.0,   0.0f, 1.0f,  // 12
			 wm,  hm, -dm,   1.0f, 0.0, 0.0,   1.0f, 1.0f,  // 13
			-wm, -hm, -dm,   1.0f, 0.0, 0.0,   0.0f, 1.0f,  // 14
			 wm, -hm, -dm,   1.0f, 0.0, 0.0,   1.0f, 1.0f}; // 15
		for (u32 i = 0; i < 16*8; ++i) {
			vertices[i] = temp[i];
		}
		u32 temp2[] = {
			 0,  1,  2,   1,  2,  3,
			 8,  9, 12,   9, 12, 13,
			 1,  5,  3,   3,  5,  7,
			11, 14, 15,  10, 11, 14,
			 0,  4,  6,   0,  2,  6,
			 4,  5,  6,   5,  6,  7};
		for (u32 i = 0; i < 6*6; ++i) {
			indices[i] = temp2[i];
		}
	}
	~Cube() {
		delete[] vertices;
		delete[] indices;
	}
	f32* getVertices() {
		return vertices;
	}
	u32* getIndices() {
		return indices;
	}
	u32 getVSize() {
		return 16*8;
	}
	u32 getISize() {
		return 6*6;
	}
};

#endif
