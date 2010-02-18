// simple shader that squashes objects and makes them "wavy"
// www.lighthouse3d.com

void main(void)
{
		vec4 v = vec4(gl_Vertex);		
		v.z = sin(5.0*v.x )*0.25;
		
		gl_Position = gl_ModelViewProjectionMatrix * v;
} 