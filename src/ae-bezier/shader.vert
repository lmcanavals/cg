#version 330 core
void main() {
    float t = (gl_VertexID + 0.5f) / 99.0f;
    float x = -1.0 * pow(1.0 - t, 3.0) +
              -1.0 * 3. * t * pow(1.0 - t, 2.0) +
               1.0 * 3. * pow(t, 2.0) * (1.0 - t) +
               1.0 * pow(t, 3.0);
    float y = -1.0 * pow(1. - t, 3.0) +
               1.0 * 3. * t * pow(1.0 - t, 2.0) +
              -1.0 * 3. * pow(t, 2.0) * (1.0 - t) +
               1.0 * pow(t, 3.0);

    gl_Position = vec4(x, y, 0.0f, 1.0f);
    gl_PointSize = 3.0f;
}

