#version 330 core
void main() {
	float t = (gl_VertexID + 0.5) / 99.0;
	float x = -1.0 *           pow(1.0 - t, 3.0) +
	          -1.0 * 3.0 * t * pow(1.0 - t, 2.0) +
	           1.0 * 3.0 *     pow(      t, 2.0) * (1.0 - t) +
	           1.0 *           pow(      t, 3.0);
	float y = -1.0 *           pow(1.0 - t, 3.0) +
	           1.0 * 3.0 * t * pow(1.0 - t, 2.0) +
	          -1.0 * 3.0 *     pow(      t, 2.0) * (1.0 - t) +
	           1.0 *           pow(      t, 3.0);

	gl_Position = vec4(x, y, 0.0, 1.0);
	gl_PointSize = 3.0;
}

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

