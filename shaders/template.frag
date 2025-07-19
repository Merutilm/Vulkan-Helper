#version 450

//layout(binding = 1) uniform sampler2D texSampler;

layout(push_constant) uniform Push{
    vec2 resolution;
}push;

layout(set = 0, binding = 0) uniform Time{
    float time;
} ubo;

layout (location = 0) in vec3 fragColor;
//layout (location = 1) in vec2 fragTexCoord;
layout (location = 0) out vec4 color;


void main(){
    vec3 mul = vec3(sin(gl_FragCoord.x / push.resolution.x + ubo.time) / 2 + 0.5, sin(gl_FragCoord.y / push.resolution.y + ubo.time * 1.198) / 2 + 0.5, sin(gl_FragCoord.y / push.resolution.y + ubo.time * 1.321) / 2 + 0.5);
    color = vec4(fragColor * mul, 1);//vec4(fragColor * texture(texSampler, fragTexCoord).rgb, 1);
}