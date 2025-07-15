#version 450

//layout(binding = 1) uniform sampler2D texSampler;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    float time;
} ubo;

layout (location = 0) in vec3 fragColor;
//layout (location = 1) in vec2 fragTexCoord;
layout (location = 0) out vec4 color;

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main(){
    color = vec4(fragColor  * rand(gl_FragCoord.xy * ubo.time), 1); //vec4(fragColor * texture(texSampler, fragTexCoord).rgb, 1);
}