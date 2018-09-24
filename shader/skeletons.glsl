// #include dualquaternion.glsl

#define SKELETAL_ANIMATION_MODE_LBS 0
#define SKELETAL_ANIMATION_MODE_DQS 1
#define SKELETAL_ANIMATION_MODE_CRS 2

layout (location = 4) in vec4 SkeletonBoneIndices;
layout (location = 5) in vec4 SkeletonBoneWeights;
layout (location = 6) in vec3 centerOfRotation;

struct SkeletonBone {
	vec3 pos;
	mat4 transform;
	DualQuaternion dqTransform;
};

struct Skeleton {
	SkeletonBone bone[MAX_NUM_BONES_PER_MESH];
	int numBones;
};

 // LBS is used, if no other technique is selected
uniform int SkinningMode = SKELETAL_ANIMATION_MODE_LBS;
uniform Skeleton skeleton;

#ifdef SKELETAL_ANIMATION_CRS_OUT
out vec3 cor;
#endif

mat4 perform_skinning()
{

	if (World_Mesh.SkeletonIndex < 0)
		return mat4(1.0);

	mat4 result = mat4(0);

#ifdef SKELETAL_ANIMATION_CRS_OUT
	cor = centerOfRotation;
#endif

	if (SkinningMode == SKELETAL_ANIMATION_MODE_DQS) {
		DualQuaternion dqResult = DualQuaternion(vec4(0,0,0,0), vec4(0));
		for (int i = 0; i < 4; ++i) {
			dqResult = dualquat_add(dqResult, dualquat_mult(SkeletonBoneWeights[i], skeleton.bone[int(SkeletonBoneIndices[i])].dqTransform));
			//dqResult = dualquat_normalize(dqResult);
		}

		DualQuaternion c = dualquat_normalize(dqResult);
		vec4 translation = vec4(dualquat_getTranslation(c), 1);
		result = mat4(quat_toRotationMatrix(c.real));
		result[3] = translation;
	} else if (SkinningMode == SKELETAL_ANIMATION_MODE_CRS) {
		vec4 quatRotation = vec4(0);
		mat4 lbs = mat4(0);

		for (int i = 0; i < 4; ++i) {
			quatRotation = quat_add_oriented(quatRotation, SkeletonBoneWeights[i] * skeleton.bone[int(SkeletonBoneIndices[i])].dqTransform.real);
			lbs += SkeletonBoneWeights[i] * skeleton.bone[int(SkeletonBoneIndices[i])].transform;
		}
	
		quatRotation = normalize(quatRotation);
		mat3 quatRotationMatrix = quat_toRotationMatrix(quatRotation);

		vec4 translation = vec4((lbs*vec4(centerOfRotation, 1.0) - vec4(quatRotationMatrix*centerOfRotation, 0.0)).xyz, 1.0);

		result = mat4(quatRotationMatrix);
		result[3] = translation;
	} else { // SKELETAL_ANIMATION_MODE_LBS
		for (int i = 0; i < 4; ++i) {
			result += SkeletonBoneWeights[i] * skeleton.bone[int(SkeletonBoneIndices[i])].transform;
		}
	}

	return result;
}
