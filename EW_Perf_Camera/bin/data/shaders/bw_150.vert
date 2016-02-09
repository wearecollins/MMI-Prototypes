#version 120
#extension GL_EXT_gpu_shader4 : enable
#extension GL_ARB_texture_rectangle : enable

varying vec2 texCoord;

void main()
{
    
    gl_TexCoord[0] = gl_MultiTexCoord0;
    
    //get our current vertex position so we can modify it
    vec4 pos = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
    
	texCoord = vec2(gl_MultiTexCoord0.xy);
	gl_Position = pos;
}
