uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform vec3 uColor;
uniform vec3 uNormal;

attribute vec3 aPosition;

varying vec3 vColor;

void main() {
   vec4 vPosition;

   /* First model transforms */
   vPosition = uModelMatrix * vec4(aPosition.x, aPosition.y, aPosition.z, 1);
   vPosition = uViewMatrix * vPosition;
   gl_Position = uProjMatrix * vPosition;
   /* vColor = vec3(uColor.r, uColor.g, uColor.b); */
   vColor = uNormal;
}
