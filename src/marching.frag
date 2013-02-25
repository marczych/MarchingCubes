uniform vec3 uNormal;

void main() {
  gl_FragColor = vec4(uNormal.x, uNormal.y, uNormal.y, 1.0);
}
