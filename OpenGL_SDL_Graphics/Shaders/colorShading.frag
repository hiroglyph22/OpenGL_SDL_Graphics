#version 130

// Fragment shading calculates stuff at the pixel level

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;

// variables get deleted if they're not used
uniform sampler2D mySampler;

void main() {

    // cos(x) returns a number between -1 and 1. To convert it into the range 0 to 
    // do (cos(x) + 1.0) * 0.5

    vec4 textureColor = texture(mySampler, fragmentUV);

    color = fragmentColor * textureColor;
}