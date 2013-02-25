uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform vec3 uNormal;

attribute vec3 aPosition;

varying vec3 vNormal;

void main() {
   vec4 vPosition;

   vNormal = vec3(uModelMatrix * vec4(uNormal, 0.0));

   /* First model transforms */
   vPosition = uModelMatrix * vec4(aPosition.x, aPosition.y, aPosition.z, 1);
   vPosition = uViewMatrix * vPosition;
   gl_Position = uProjMatrix * vPosition;
}
