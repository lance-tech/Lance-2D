#version 330 core
in vec2 UV;
in vec3 Color;

out vec4 fragColor;

uniform sampler2D TextureSampler0;
uniform float Time;


#define saturate(x) clamp(x,0.,1.)

float rand(float x) { return fract(sin(x) * 71523.5413291); }
float rand(vec2 x) { return rand(dot(x, vec2(13.4251, 15.5128))); }

float noise(vec2 x)
{
    vec2 i = floor(x);
    vec2 f = x - i;
    f *= f*(3.-2.*f);
    return mix(mix(rand(i), rand(i+vec2(1,0)), f.x),
               mix(rand(i+vec2(0,1)), rand(i+vec2(1,1)), f.x), f.y);
}

float fbm(vec2 x)
{
    float r = 0.0, s = 1.0, w = 1.0;
    for (int i=0; i<5; i++)
    {
        s *= 2.0;
        w *= 0.5;
        r += w * noise(s * x);
    }
    return r;
}

float cloud(vec2 uv, float scalex, float scaley, float density, float sharpness, float speed)
{
    return pow(saturate(fbm(vec2(scalex,scaley)*(uv+vec2(speed,0)*Time))-(1.0-density)), 1.0-sharpness);
}

vec3 render(vec2 uv)
{
    // sky
    vec3 color = mix(vec3(0.0745, 0.0667, 0.1333), vec3(0.0745, 0.0667, 0.102), uv.y);

    // clouds
    vec3 cl1 = mix(vec3(0.102, 0.098, 0.1333), vec3(0.1137, 0.1137, 0.1216),uv.y);
    float d1 = mix(0.9,0.1,pow(uv.y, 0.7));
    color = mix(color, cl1, cloud(uv,2.,8.,d1,0.4,0.04));
    color = mix(color, vec3(0.1882, 0.1961, 0.2275), 8.*cloud(uv,14.,18.,0.9,0.75,0.02) * cloud(uv,2.,5.,0.6,0.15,0.01)*uv.y);
    color = mix(color, vec3(0.1961, 0.2039, 0.2196), 5.*cloud(uv,12.,15.,0.9,0.75,0.03) * cloud(uv,2.,8.,0.5,0.0,0.02)*uv.y);
    
    return color;
}


void main()
{
    vec2 iResolution = vec2(1.0, 1.0);
    vec2 uv = UV.xy/iResolution.xy;
    uv.x -= 0.5;
    uv.x *= iResolution.x / iResolution.y;
    
	fragColor = vec4(render(uv),1.0);
}