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
	grayscale(gl_FragColor, fragTexCoord);
}
