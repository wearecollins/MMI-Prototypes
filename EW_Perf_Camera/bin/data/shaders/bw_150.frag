#version 150

uniform sampler2DRect tex;

in vec2 texCoordVarying;

out vec4 fragColor;

// Texture coordinates
vec2 texcoord0 = texCoordVarying;

const mat4 colorMatrix = mat4(1, 1, 1, 0, 0, -0.344, 1.773, 0,1.403, -0.714, 0, 0, 0, 0, 0, 1) * mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, -0.5, -0.5, 1);

void main(void)
{
    vec4 c = texture(tex, texcoord0.st);
//    vec4 cRGB = clamp(colorMatrix* vec4(c.r,c.g,c.a,1), 0.0, 1.0);
    vec4 cRGB = c;
    
    //convert to grayscale using NTSC conversion weights
    float gray = min(1.0, dot(cRGB.rgb, vec3(0.299, 0.587, 0.114)));
    //
    c = vec4(vec3(gray),1);
    
    fragColor = c;
}