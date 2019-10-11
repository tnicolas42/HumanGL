#version 410 core

#define MAX_BONES 100
#define NUM_BONES_PER_VERTEX 4

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 bonesID;
layout (location = 4) in vec4 bonesWeight;

out VS_OUT {
	vec2	texCoords;
	vec3	fragPos;
	vec3	normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 bones[MAX_BONES];

void main() {
    mat4 boneTransform = mat4(1.0);
    for (int i=0; i < NUM_BONES_PER_VERTEX; i++) {
        boneTransform += bones[bonesID[i]] * bonesWeight[i];
    }

    vec4 pos = boneTransform * vec4(aPos, 1.0);

    vec4 boneNormal = boneTransform * vec4(aNormal, 0);

	vs_out.texCoords = aTexCoords;
	vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.normal = mat3(transpose(inverse(model))) * boneNormal.xyz;
	gl_Position = projection * view * model * pos;
}
