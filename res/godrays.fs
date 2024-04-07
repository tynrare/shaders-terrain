#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;           // Texture coordinates (sampler2D)
varying vec4 fragColor;              // Tint color

// Uniform inputs
uniform sampler2D texture0;
uniform vec2 resolution;        // Viewport resolution (in pixels)
uniform vec2 mouse;             // Mouse pixel xy coordinates
uniform float time;             // Total run time (in secods)


// godrays copied from https://www.shadertoy.com/view/Mfd3R7
// some minor tweaks to fit into current glsl environment
// also this shader does not draw original image, only rays itself

vec2 VogelDiskSample(int sampleIndex, int sampleCount, float phi)
{
    float sampleIndexf = float(sampleIndex);
    float sampleCountf = float(sampleCount);
    
    float goldenAngle = 2.39996;

    float r = sqrt((sampleIndexf + 0.5) / sampleCountf);
    float theta = sampleIndexf * goldenAngle + phi;

    return vec2(cos(theta), sin(theta)) * r;
}

void godrays( out vec4 out_fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord;
    
    // config variables:
    int radialSmpCount = 32;
    int softnessSmpCount = 4;
    float range = -0.7;
    float blurRadius = 0.08;
    float attenuationSoftness = 0.8;
 
    float rangePerSample = range / float(radialSmpCount);
    
    vec2 radialOrigin = vec2(0.1, 0.9); 
   // radialOrigin += vec2(sin(time), cos(time)) * vec2(0.1, 0.05) + vec2(0.1);
    
		radialOrigin = mouse.xy / resolution.xy ;
    
    vec4 buffer = vec4(0);
    for(int softnessSmpIdx = 0; softnessSmpIdx < softnessSmpCount; softnessSmpIdx++)
    {
        vec2 softnessOffset = VogelDiskSample(softnessSmpIdx, softnessSmpCount, 0.0) * blurRadius;
        vec2 uvOffset = softnessOffset + radialOrigin;
        vec2 uvCentered = uv - uvOffset;
        for(int radialIdx = 0; radialIdx < radialSmpCount; radialIdx++)
        {
            float stepSize = float(radialIdx) * rangePerSample;
            vec2 uv = uvCentered * (1.0 + stepSize) + uvOffset ;
            buffer += max(vec4(0.0), texture2D(texture0, uv) - vec4(0.9));; 
        }
    }
  
    vec4 radialBlurTarget = buffer / float(softnessSmpCount * radialSmpCount);
    // compute attenuation
    {
        vec2 vecToOrigin = radialOrigin - uv;
        
        float invSqrFalloff = 1.0 / (dot(vecToOrigin, vecToOrigin) + attenuationSoftness);
        radialBlurTarget *= invSqrFalloff;
    }
    radialBlurTarget *=  exp2(4.0);
    
    // composing.
    vec4 background = texture2D(texture0, uv)  ;
    vec4 finalColor = radialBlurTarget + background;
    
    // frankenstein reinhard tonemapping
    finalColor = (finalColor / (2.0 + finalColor)) * 2.0;
    
    
    // Output to screen 
    out_fragColor = radialBlurTarget;
}

void grayscale( out vec4 out_fragColor, in vec2 fragCoord ) {
    // Texel color fetching from texture sampler
    vec4 texelColor = texture2D(texture0, fragCoord)*fragColor;
    // Convert texel color to grayscale using NTSC conversion weights
    float gray = dot(texelColor.rgb, vec3(0.299, 0.587, 0.114));
    // Calculate final fragment color
    out_fragColor = vec4(gray, gray, gray, texelColor.a);
}

void main()
{
	//grayscale(gl_FragColor, fragTexCoord);
	godrays(gl_FragColor, fragTexCoord);
}
