//Plant.vp
#version 330

uniform mat4 WORLD;
uniform mat4 VP;

layout(lines) in;
layout (line_strip, max_vertices=2) out;
 
 void main()
{
    gl_Position = VP*gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = VP*gl_in[1].gl_Position;
    EmitVertex();


}