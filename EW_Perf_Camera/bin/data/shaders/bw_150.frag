uniform sampler2DRect tex;
varying vec2 texCoord;

void main(void){
    vec4 c = texture2DRect(tex, gl_TexCoord[0].st);
    
    //convert to grayscale using NTSC conversion weights
    float gray = min(1.0, dot(c.rgb, vec3(0.299, 0.587, 0.114)));
//
    c = vec4(vec3(gray),1);
    
    gl_FragColor = c;
}