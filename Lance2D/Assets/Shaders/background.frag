#version 330 core

in vec3 fragmentColor;
in vec2 UV;

out vec4 color;
uniform vec4 inputColor;
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

vec3 CloudSky(vec2 uv)
{
    vec3 color = mix(vec3(0.0275, 0.0314, 0.0314), vec3(0.0314, 0.0235, 0.0471), uv.y);

    vec3 cl1 = mix(vec3(0.1451, 0.1451, 0.2392), vec3(0.1137, 0.1137, 0.1216),uv.y);
    float d1 = mix(0.9,0.1,pow(uv.y, 0.7));
    color = mix(color, cl1, cloud(uv,2.,8.,d1,0.4,0.04));
    color = mix(color, vec3(0.1098, 0.1176, 0.1451), 8.*cloud(uv,14.,18.,0.9,0.75,0.02) * cloud(uv,2.,5.,0.6,0.15,0.01)*uv.y);
    color = mix(color, vec3(0.0588, 0.0667, 0.0745), 5.*cloud(uv,12.,15.,0.9,0.75,0.03) * cloud(uv,2.,8.,0.5,0.0,0.02)*uv.y);
    
    return color;
}

float gauss(float s, float d, float r)
{
		return (1.0/(sqrt(2.0*3.1415)*s))*exp(-pow(d-r,2.0)/(s*s));
}	
float bgauss(float s, float d, float r)
{
	if(d>=r)
		return gauss(s,d,r);
	else
		return gauss(s,0.0,0.0);
}

void main()
{
	float building1 = step(sin((UV.x + (Time * 0.005)) * 30), 0.) * 0.05;
	float building2 = step(sin((UV.x + (Time * 0.0025) + 0.15) * 20), 0.) * 0.025;
	float building3 = step(sin((UV.x + (Time * 0.0015) + 0.75) * 50), 0.) * 0.075;
	float ground = (1-(UV.y + 0.15)) + (building1 + building2 + building3);
	float groundLine = step(ground, 0.5) + (UV.y * 2.0);

	vec3 buildingLines = vec3(groundLine);
	vec3 Clouds = CloudSky(UV);

	// moon	
	vec2 uv = UV.xy-0.5;
	uv.y*=0.66;
	vec2 point = vec2(-0.25,0.15);
	float s=0.03;
	float r=0.005;
	float moon = length(uv-point);
	moon=bgauss(s,moon,r);
	moon=min(moon,1.0);
	moon*=(1.0+2.0*length(uv-point));
	float n1 = noise(vec2(UV.x, UV.y) * 50);
	moon=moon*(0.9+(0.3-moon)*0.3*vec3(n1).y);
	
	vec3 moonNight = (Clouds * (1-moon)) + ((1-(Clouds * 4.2)) * (moon *0.45));

	color = vec4(buildingLines * moonNight, 1.0);
	// color = vec4(moonNight, 1.0);
}
