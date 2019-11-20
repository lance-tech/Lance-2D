#version 330 core
// Interpolated values from the vertex shaders
in vec3 FragPos;  
in vec3 Normal;  
in vec2 UV;
in vec3 eyeVector;

// Ouput data
out vec4 fragColor;

// Values that stay constant for the whole mesh.
uniform vec4 inputColor;
uniform float Time;
uniform float specularFactor;

float brickH=.04;
float brickW=.1;
float mortarR=0.006;
float mortarW=0.005;
vec3 brickC = pow(vec3(218,114,61)/255.,vec3(2.2));
vec3 mortarC = pow(vec3(227,210,170)/255.,vec3(2.2));
vec3 permute(vec3 x) { return mod(((x*34.0)+1.0)*x, 289.0); }

float snoise(vec2 v){
  const vec4 C = vec4(0.211324865405187, 0.366025403784439,
           -0.577350269189626, 0.024390243902439);
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);
  vec2 i1;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;
  i = mod(i, 289.0);
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
  + i.x + vec3(0.0, i1.x, 1.0 ));
  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy),
    dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;
  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return pow((130.0 * dot(m, g)+1.0)/2.0,1.0);
}

float derez(float a,float b){
    return ceil(a/b)*b;
}

float capSlope(float a,float r,float w){
    float ir=1.0-r;
    return (max(a+w,ir)-ir)/r;
}
    

float peak(float a,float r,float w){

	return pow(max(
        capSlope(1.0-a,r,w)+capSlope(a,r,w)
    ,0.0),2.0);
}

float mortar(vec2 uv){
    float isOffset=mod(uv.y,brickH*2.)<brickH?0.:1.;
	return (
        //vertical mortar lines
        peak(mod(uv.x+isOffset*brickW*.5,brickW)/brickW,mortarR/brickW,mortarW/brickW)+
        //horizontal mortar lines
        peak(mod(uv.y,brickH)/brickH,mortarR/brickH,mortarW/brickH)
    )>1.?1.:0.;
}

vec3 lightAjust (vec3 color,float amount){
    return 1.0-pow(1.0-color,vec3(amount));
}

vec3 toneMap (vec3 color){
    float maxColor=max(color.r,max(color.g,color.b));
    vec3 foo=lightAjust(color/maxColor,maxColor);

    return min(foo,color);
}

void main()
{
	vec3 ambient = vec3(0.1, 0.1, 0.1) * inputColor.rgb;
    vec3 norm = normalize(Normal);
    vec3 lightPos = vec3(-50, 0, -50);
    vec3 lightDir = normalize(lightPos);  
    vec3 viewDir = normalize(eyeVector);
    vec3 reflectDir = -reflect(viewDir, norm);  
	float shininess = pow(35.0, specularFactor * 2);
    float spec = pow(max(dot(reflectDir, lightDir), 0.0), shininess);
    float lambert = max(dot(lightDir, norm), 0.0);
    vec3 fresnel = pow(1 - max(dot(viewDir, norm), 0.0), 4.0) * (1-lambert) * (inputColor.rgb * 0.5);
    vec3 result = (lambert * inputColor.rgb) + fresnel + (spec * specularFactor) + ambient;



    vec2 uv = UV.xy / vec2(1.0)*1.0+2.0;
    float pi =3.14152654;
    
    float brickHNoise=snoise(vec2(
        1.0,
        uv.y*2.0
    ));
    
    brickH+=brickHNoise*0.002;
    
    float brickWNoise=snoise(vec2(
        uv.x,
        derez(uv.y,brickH)
    ));
    
    brickW+=brickWNoise*.01;

    
    float isOffset=mod(uv.y,brickH*2.)<brickH?0.:1.;
    
    float brickLNoise=snoise(vec2(
        derez(
            (uv.x+isOffset*brickW*.5)*4.0,
            brickW*4.0
        ),
        derez(
            uv.y*4.0,
            brickH*4.0
        )
    ));
    float brickCNoise=snoise(vec2(
        derez(
            ((uv.x)+isOffset*brickW*.5)*5.0,
            brickW*5.0
        ),
        derez(
            (uv.y)*5.0,
            brickH*5.0
        )
    ));
    
    
    brickC*=brickLNoise*.6+.4;
    
    brickC*=vec3((1.-brickCNoise)*0.2+0.8,brickCNoise*0.2+0.8,1);
    
    
    vec3 color = mix(brickC,mortarC,mortar(uv));
    
    float highlight = clamp(
        mortar(uv+vec2(-.005,.005))-mortar(uv)
    ,0.,1.);
    float shadow = clamp(
        mortar(uv+vec2(.005,-.005))-mortar(uv)
    ,0.,1.);
    shadow += clamp(
        (1.-mortar(uv+vec2(-.005,.005)))-(1.-mortar(uv))
    ,0.,1.);
    
    shadow=clamp(shadow,0.,1.);

    float light = mix(highlight,1.0-shadow,0.5);
    
    float textureHighlight=clamp(
        snoise((uv/.01)+vec2(-.1,.1))-snoise(uv/.01)
    ,0.,1.);
    float textureShadow=clamp(
        snoise((uv/.01)+vec2(.1,-.1))-snoise(uv/.01)
    ,0.,1.);
    float texture= mix(textureHighlight,1.0-textureShadow,0.5);
   	
    
    light = mix(light,texture,0.5);
    
    vec3 lightColor = mix(vec3(0,0,1),vec3(1,1,0),light);
    
    fragColor = pow(vec4(
        lightAjust(result * inputColor.rgb * color*lightColor,2.5)
    ,1),vec4(1./2.2));
}