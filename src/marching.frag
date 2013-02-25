varying vec3 vNormal;

void main() {
  gl_FragColor = vec4(vNormal.x, vNormal.y, vNormal.z, 1.0);
}
