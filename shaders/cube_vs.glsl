#version 410 core

#define MAX_BONES 100
#define NUM_BONES_PER_VERTEX 4

layout (location = 5) in vec3 cubePos;
layout (location = 6) in vec3 cubeNormal;
layout (location = 7) in vec2 cubeTexCoords;

out vec2 texCoords;
out	vec3 fragPos;
out	vec3 normal;

uniform mat4	model;
uniform mat4	view;
uniform mat4	projection;
uniform mat4	modelScale;
uniform mat4	bones[MAX_BONES];
uniform vec3	bonesPos[MAX_BONES];
uniform int		boneID;

// // work but scale the cube according to the modelScale
// void main() {
//     mat4 boneTransform = mat4(1.0);
//     boneTransform += bones[boneID];

// 	vec3 cPos = cubePos + bonesPos[boneID];

//     vec4 pos = boneTransform * vec4(cPos, 1.0);

//     vec4 boneNormal = boneTransform * vec4(cubeNormal, 0.0);

// 	texCoords = cubeTexCoords;
// 	fragPos = vec3(modelScale * vec4(cPos, 1.0));
// 	normal = mat3(transpose(inverse(modelScale))) * boneNormal.xyz;

// 	gl_Position = projection * view * model * modelScale * pos;
// }


void main() {
    mat4 boneTransform = mat4(1.0);
	// work until we add the animation
    // boneTransform += (bones[boneID] * modelScale);

	vec4 cPos = boneTransform * modelScale * vec4(bonesPos[boneID], 1.0);

    vec4 pos = vec4(cubePos * 0.1, 0.0) + cPos;

    vec4 boneNormal = boneTransform * vec4(cubeNormal, 0);

	texCoords = cubeTexCoords;
	fragPos = vec3(modelScale * cPos);
	normal = mat3(transpose(inverse(modelScale))) * boneNormal.xyz;

	gl_Position = projection * view * model * pos;
}
