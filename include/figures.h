#ifndef __FIGURES_H__
#define __FIGURES_H__

struct Rgb {
	f32 r, g, b;
};

class Object {

protected:
	f32* vertices;
	u32* indices;
	u32  nv;
	u32  ni;
	u32  n;

public:
	Object(u32 n, u32 m)
			: vertices(new f32[24*n]), indices(new u32[m]), nv(24*n), ni(m), n(n) {}

	f32* getVertices() {
		return vertices;
	}
	u32* getIndices() {
		return indices;
	}
	u32 getVSize() {
		return nv;
	}
	u32 getISize() {
		return ni;
	}
	u32 len() {
		return sizeof(f32)*n;
	}
	void* skip(u32 i) {
		return (void*)(i * sizeof(u32));
	}
};

class Cube : public Object {

public:
	Cube(f32 width=1.0f, f32 height=1.0f, f32 depth=1.0f,
	     Rgb a={0.8f, 0.9f, 1.0f}, Rgb b={0.9f, 0.1f, 0.2f},
	     Rgb c={0.8f, 0.8f, 0.2f}, Rgb d={0.3f, 0.9f, 0.8f},
	     Rgb e={1.0f, 0.5f, 0.2f}, Rgb f={0.2f, 0.9f, 0.3f})
			: Object(11, 36) {
		f32 wm =  width / 2.0f;
		f32 hm = height / 2.0f;
		f32 dm =  depth / 2.0f;
		f32 temp[] = {
			// posiciones       colores           normals          texturas
			-wm,  hm,  dm,   a.r, a.g, a.b,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,  // 0
			 wm,  hm,  dm,   a.r, a.g, a.b,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,  // 1
			-wm, -hm,  dm,   a.r, a.g, a.b,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // 2
			 wm, -hm,  dm,   a.r, a.g, a.b,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,  // 3

			-wm,  hm,  dm,   b.r, b.g, b.b,    0.0f, 1.0f, 0.0f,  0.0f, 1.0f,  // 4
			 wm,  hm,  dm,   b.r, b.g, b.b,    0.0f, 1.0f, 0.0f,  1.0f, 1.0f,  // 5
			-wm,  hm, -dm,   b.r, b.g, b.b,    0.0f, 1.0f, 0.0f,  0.0f, 0.0f,  // 6
			 wm,  hm, -dm,   b.r, b.g, b.b,    0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  // 7

			 wm,  hm,  dm,   c.r, c.g, c.b,    1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // 8
			 wm,  hm, -dm,   c.r, c.g, c.b,    1.0f, 0.0f, 0.0f,  1.0f, 0.0f,  // 9
			 wm, -hm,  dm,   c.r, c.g, c.b,    1.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // 10
			 wm, -hm, -dm,   c.r, c.g, c.b,    1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // 11

			-wm, -hm,  dm,   d.r, d.g, d.b,    0.0f, -1.0f, 0.0f,  0.0f, 1.0f,  // 12
			 wm, -hm,  dm,   d.r, d.g, d.b,    0.0f, -1.0f, 0.0f,  1.0f, 1.0f,  // 13
			-wm, -hm, -dm,   d.r, d.g, d.b,    0.0f, -1.0f, 0.0f,  0.0f, 0.0f,  // 14
			 wm, -hm, -dm,   d.r, d.g, d.b,    0.0f, -1.0f, 0.0f,  1.0f, 0.0f,  // 15

			-wm,  hm,  dm,   e.r, e.g, e.b,    -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // 16
			-wm,  hm, -dm,   e.r, e.g, e.b,    -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,  // 17
			-wm, -hm,  dm,   e.r, e.g, e.b,    -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // 18
			-wm, -hm, -dm,   e.r, e.g, e.b,    -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // 19

			-wm,  hm, -dm,   f.r, f.g, f.b,    0.0f, 0.0f, -1.0f,  0.0f, 1.0f,  // 20
			 wm,  hm, -dm,   f.r, f.g, f.b,    0.0f, 0.0f, -1.0f,  1.0f, 1.0f,  // 21
			-wm, -hm, -dm,   f.r, f.g, f.b,    0.0f, 0.0f, -1.0f,  0.0f, 0.0f,  // 22
			 wm, -hm, -dm,   f.r, f.g, f.b,    0.0f, 0.0f, -1.0f,  1.0f, 0.0f   // 23
		};
		for (u32 i = 0; i < nv; ++i) {
			vertices[i] = temp[i];
		}
		for (u32 i = 0; i < 6; ++i) {
			indices[i*6 + 0] = i*4 + 0;
			indices[i*6 + 1] = i*4 + 1;
			indices[i*6 + 2] = i*4 + 2;
			indices[i*6 + 3] = i*4 + 1;
			indices[i*6 + 4] = i*4 + 2;
			indices[i*6 + 5] = i*4 + 3;
		}
	}
};

#endif
