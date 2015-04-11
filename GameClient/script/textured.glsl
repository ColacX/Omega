#version 330
#ifdef VERT
uniform mat4 pvmMatrix;
layout(location = 1) in vec2 texcoord;
layout(location = 0) in vec3 position;
out vec2 st;

void main()
{
	gl_Position = pvmMatrix * vec4(position, 1);
	st = texcoord;
}
#endif

#ifdef FRAG
uniform sampler2D sampler0;
in vec2 st;
out vec4 frag_color;

void main()
{
	frag_color = texture2D(sampler0, st);
}

#endif
