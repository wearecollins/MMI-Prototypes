uniform sampler2DRect mask;
uniform sampler2DRect draw;
uniform float alpha;

void main(){
    vec2 st = gl_TexCoord[0].st;
    vec4 c = texture2DRect(mask, st);
    vec4 d = texture2DRect(draw, st);
    if ( c.w < 1.0 ){
        discard;
    } else {
        gl_FragColor = vec4(d.rgb, d.a * alpha);
    }
}