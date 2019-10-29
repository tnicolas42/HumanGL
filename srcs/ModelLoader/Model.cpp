#include "Model.hpp"
#include <limits>

const float	Model::_cubeData[] = {
	// positions			// normals				// texture coords
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,		0.0f, 0.0f, -1.0f,		1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,		1.0f, 1.0f,
	0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,		1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,		0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		0.0f, 0.0f,
	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
	0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
	0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		-1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
	0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,		0.0f, -1.0f, 0.0f,		1.0f, 1.0f,
	0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,		1.0f, 0.0f,
	0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,		1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		0.0f, 1.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f,
	0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 1.0f,
	0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f
};


Model::Model(const char *path, Shader &shader, Shader &cubeShader)
: _shader(shader),
  _cubeShader(cubeShader),
  _minPos(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()),
  _maxPos(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min()),
  _model(mat::Mat4()),
  _modelScale(mat::Mat4()),
  _drawMesh(true),
  _drawCube(false) {
	loadModel(path);
	sendCubeData();
	_startAnimTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
}

Model::Model(Model const &src) :
  _shader(src.getShader()),
  _cubeShader(src.getCubeShader()) {
	*this = src;
}

Model::~Model() {
	glDeleteVertexArrays(1, &_cubeVao);
}

Model &Model::operator=(Model const &rhs) {
	if (this != &rhs) {
		_meshes = rhs.getMeshes();
		_directory = rhs.getDirectory();
		_texturesLoaded = rhs.getTexturesLoaded();

		_minPos = rhs.getMinPos();
		_maxPos = rhs.getMaxPos();
		_model = rhs.getModel();
		_modelScale = rhs.getModelScale();

		_boneMap = rhs.getBoneMap();
		_boneInfo = rhs.getBoneInfo();

		_boneInfoUniform = rhs.getBoneInfoUniform();
		_actBoneId = rhs.getActBoneId();
		_globalInverseTransform = rhs.getGlobalInverseTransform();
		_globalTransform = rhs.getGlobalTransform();

		_cubeVbo = rhs.getCubeVbo();
		_cubeVao = rhs.getCubeVao();

		_drawMesh = rhs.isDrawMesh();
		_drawCube = rhs.isDrawCube();

		_startAnimTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	}
	return *this;
}

void	Model::sendBones(int shaderId) {
	for (u_int32_t i=0; i < MAX_BONES; ++i)
		for (u_int32_t j=0; j < 16; ++j)
			_boneInfoUniform[i*16 + j] = _boneInfo[i].finalTransformation.getData()[j];
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "bones"), MAX_BONES, GL_TRUE, &(_boneInfoUniform[0]));
}

void	Model::draw() {
	_shader.use();
	if (_isAnimated) {
		std::chrono::milliseconds curTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
		std::chrono::milliseconds runningTime = (curTime - _startAnimTime);
		float timeInMillis = runningTime.count();
		float ticksPerSecond = (_curAnimation->mTicksPerSecond != 0) ? _curAnimation->mTicksPerSecond : 25.0f;
		float timeInTicks = (timeInMillis / 1000.0) * ticksPerSecond;
		//loops the animation
		float animationTime = fmod(timeInTicks, _curAnimation->mDuration);
		// set bones with animations
		setBonesTransform(animationTime, _scene->mRootNode, _scene, _globalTransform);
		sendBones(_shader.id);
	}

	if (_drawMesh) {
		for (auto &mesh : _meshes)
			mesh.draw(getShader());
	}

	if (_drawCube) {
		// drawing cube
		_cubeShader.use();
		if (_isAnimated)
			sendBones(_cubeShader.id);

		glBindVertexArray(_cubeVao);
		// std::cout << "_______________" << std::endl;
		for (auto &&elem : _boneMap) {
			_cubeShader.setInt("boneID", elem.second);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
	glBindVertexArray(0);
}

void	Model::loadModel(std::string path) {
	_scene = _importer.ReadFile(path, \
	aiProcess_Triangulate | \
	aiProcess_FlipUVs | \
	aiProcess_GenNormals | \
	aiProcess_GenUVCoords | \
	aiProcess_LimitBoneWeights);

	if (!_scene || _scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !_scene->mRootNode) {
		std::cerr << "ERROR::ASSIMP::" << _importer.GetErrorString() << std::endl;
		throw Model::AssimpError();
	}
	_directory = path.substr(0, path.find_last_of('/'));

	_globalTransform = aiToMat4(_scene->mRootNode->mTransformation);  // get global transform
	_globalInverseTransform = _globalTransform;

	processNode(_scene->mRootNode, _scene);
	if (_scene->mNumAnimations > 0) {
		_isAnimated = true;
		_curAnimation = _scene->mAnimations[0];  // set the current animation
	}
	else {
		_isAnimated = false;
		_curAnimation = nullptr;
		_shader.use();
		sendBones(_shader.id);  // send defaut values
	}

	calcModelMatrix();

	_shader.use();
	_shader.setMat4("model", _model);

	_cubeShader.use();
	mat::Mat4 model_cube;
	_cubeShader.setMat4("model", model_cube);

	_cubeShader.setFloat("cubeSize", 0.12f);

	// send bones positions
	setBonesPos(_scene->mRootNode, _globalTransform);

	std::array<float, MAX_BONES * 3>	bonePos;
	for (u_int32_t i = 0; i < MAX_BONES; ++i)
		for (u_int32_t j = 0; j < 3; ++j)
			bonePos[i * 3 + j] = _bonePos[i].getData()[j];

	glUniform3fv(glGetUniformLocation(_cubeShader.id, "bonesPos"),  MAX_BONES, &(bonePos[0]));
}

void	Model::setBonesTransform(float animationTime, aiNode *node, const aiScene *scene, mat::Mat4 parentTransform) {
	std::string nodeName(node->mName.data);
    mat::Mat4 nodeTransformation = aiToMat4(node->mTransformation);

    const aiNodeAnim* nodeAnim = findNodeAnim(_curAnimation, nodeName);

    if (nodeAnim) {
        // Interpolate scaling and generate scaling transformation matrix
        mat::Vec3 scaling;
        calcInterpolatedScaling(scaling, animationTime, nodeAnim);
        mat::Mat4 scalingM = mat::Mat4();
		scalingM = scalingM.scale(scaling);

        // Interpolate rotation and generate rotation transformation matrix
        mat::Quaternion rotationQ;
        calcInterpolatedRotation(rotationQ, animationTime, nodeAnim);
        mat::Mat4 rotationM = rotationQ.toMatrix();

        // Interpolate translation and generate translation transformation matrix
        mat::Vec3 translation;
        calcInterpolatedPosition(translation, animationTime, nodeAnim);
        mat::Mat4 translationM = mat::Mat4();
        translationM = translationM.translate(translation);

        // Combine the above transformations
        nodeTransformation = translationM * rotationM * scalingM;
    }

    mat::Mat4 globalTransformation = parentTransform * nodeTransformation;

	// if there is a bone (same name as the node)
    if (_boneMap.find(nodeName) != _boneMap.end()) {
        uint boneIndex = _boneMap[nodeName];
        _boneInfo[boneIndex].finalTransformation = _globalInverseTransform * globalTransformation *
                                                   _boneInfo[boneIndex].boneOffset;
    }

	// recursion with each of its children
	for (u_int32_t i = 0; i < node->mNumChildren; ++i) {
		setBonesTransform(animationTime, node->mChildren[i], scene, globalTransformation);
	}
}

void	Model::setBonesPos(aiNode *node, mat::Mat4 parentTransform) {
	std::string nodeName(node->mName.data);
    mat::Mat4 nodeTransformation = aiToMat4(node->mTransformation);

    mat::Mat4 globalTransformation = parentTransform * nodeTransformation;

	// if there is a bone (same name as the node)
    if (_boneMap.find(nodeName) != _boneMap.end()) {
        uint	boneIndex = _boneMap[nodeName];
		mat::Vec3	pos;
		pos.x = globalTransformation.get(0, 3);
		pos.y = globalTransformation.get(1, 3);
		pos.z = globalTransformation.get(2, 3);
        _bonePos[boneIndex] = pos;
    }

	// recursion with each of its children
	for (u_int32_t i = 0; i < node->mNumChildren; ++i) {
		setBonesPos(node->mChildren[i], globalTransformation);
	}
}

const aiNodeAnim*	Model::findNodeAnim(const aiAnimation* animation, const std::string nodeName)
{
    for (uint i = 0 ; i < animation->mNumChannels ; i++) {
        const aiNodeAnim* nodeAnim = animation->mChannels[i];

        if (std::string(nodeAnim->mNodeName.data) == nodeName) {
            return nodeAnim;
        }
    }

    return NULL;
}

u_int32_t	Model::findPosition(float animationTime, const aiNodeAnim* nodeAnim)
{
    for (uint i = 0 ; i < nodeAnim->mNumPositionKeys - 1 ; i++) {
        if (animationTime < (float)nodeAnim->mPositionKeys[i + 1].mTime)
            return i;
    }
    assert(0);
    return 0;
}

u_int32_t	Model::findRotation(float animationTime, const aiNodeAnim* nodeAnim)
{
    assert(nodeAnim->mNumRotationKeys > 0);
    for (uint i = 0 ; i < nodeAnim->mNumRotationKeys - 1 ; i++) {
        if (animationTime < (float)nodeAnim->mRotationKeys[i + 1].mTime)
            return i;
    }
    assert(0);
    return 0;
}

u_int32_t	Model::findScaling(float animationTime, const aiNodeAnim* nodeAnim)
{
    assert(nodeAnim->mNumScalingKeys > 0);
    for (uint i = 0 ; i < nodeAnim->mNumScalingKeys - 1 ; i++) {
        if (animationTime < (float)nodeAnim->mScalingKeys[i + 1].mTime)
            return i;
    }
    assert(0);
    return 0;
}

void	Model::calcInterpolatedPosition(mat::Vec3 &out, float animationTime, const aiNodeAnim* nodeAnim)
{
    if (nodeAnim->mNumPositionKeys == 1) {
        out = aiToVec3(nodeAnim->mPositionKeys[0].mValue);
        return;
    }

    uint positionIndex = findPosition(animationTime, nodeAnim);
    uint nextPositionIndex = (positionIndex + 1);
    assert(nextPositionIndex < nodeAnim->mNumPositionKeys);
    float deltaTime = (float)(nodeAnim->mPositionKeys[nextPositionIndex].mTime - nodeAnim->mPositionKeys[positionIndex].mTime);
    float factor = (animationTime - (float)nodeAnim->mPositionKeys[positionIndex].mTime) / deltaTime;
    assert(factor >= 0.0f && factor <= 1.0f);
    const mat::Vec3 &start = aiToVec3(nodeAnim->mPositionKeys[positionIndex].mValue);
    const mat::Vec3 &end = aiToVec3(nodeAnim->mPositionKeys[nextPositionIndex].mValue);
    mat::Vec3 delta = end - start;
    out = start + delta * factor;
}

void	Model::calcInterpolatedRotation(mat::Quaternion &out, float animationTime, const aiNodeAnim* nodeAnim)
{
	// we need at least two values to interpolate...
    if (nodeAnim->mNumRotationKeys == 1) {
        out = aiToQuat(nodeAnim->mRotationKeys[0].mValue);
        return;
    }

    uint rotationIndex = findRotation(animationTime, nodeAnim);
    uint nextRotationIndex = (rotationIndex + 1);
    assert(nextRotationIndex < nodeAnim->mNumRotationKeys);
    float deltaTime = (float)(nodeAnim->mRotationKeys[nextRotationIndex].mTime - nodeAnim->mRotationKeys[rotationIndex].mTime);
    float factor = (animationTime - (float)nodeAnim->mRotationKeys[rotationIndex].mTime) / deltaTime;
    assert(factor >= 0.0f && factor <= 1.0f);
    const mat::Quaternion &startRotationQ = aiToQuat(nodeAnim->mRotationKeys[rotationIndex].mValue);
    const mat::Quaternion &endRotationQ   = aiToQuat(nodeAnim->mRotationKeys[nextRotationIndex].mValue);
	out = mat::slerp(startRotationQ, endRotationQ, factor);
    out = out.normalize();
}

void	Model::calcInterpolatedScaling(mat::Vec3 &out, float animationTime, const aiNodeAnim* nodeAnim)
{
    if (nodeAnim->mNumScalingKeys == 1) {
        out = aiToVec3(nodeAnim->mScalingKeys[0].mValue);
        return;
    }

    uint ScalingIndex = findScaling(animationTime, nodeAnim);
    uint NextScalingIndex = (ScalingIndex + 1);
    assert(NextScalingIndex < nodeAnim->mNumScalingKeys);
    float deltaTime = (float)(nodeAnim->mScalingKeys[NextScalingIndex].mTime - nodeAnim->mScalingKeys[ScalingIndex].mTime);
    float factor = (animationTime - (float)nodeAnim->mScalingKeys[ScalingIndex].mTime) / deltaTime;
    assert(factor >= 0.0f && factor <= 1.0f);
    const mat::Vec3 &start = aiToVec3(nodeAnim->mScalingKeys[ScalingIndex].mValue);
    const mat::Vec3 &end   = aiToVec3(nodeAnim->mScalingKeys[NextScalingIndex].mValue);
    mat::Vec3 delta = end - start;
    out = start + delta * factor;
}

void	Model::processNode(aiNode *node, const aiScene *scene) {
	aiMesh	*mesh;

	// process all the node's _meshes (if any)
	for (u_int32_t i = 0; i < node->mNumMeshes; ++i) {
		mesh = scene->mMeshes[node->mMeshes[i]];
		_meshes.push_back(processMesh(mesh, scene));
	}
	// recursion with each of its children
	for (u_int32_t i = 0; i < node->mNumChildren; ++i)
		processNode(node->mChildren[i], scene);
}

Material	loadMaterial(aiMaterial *mat) {
	Material material;
	aiColor3D color(0.f, 0.f, 0.f);
	float shininess;

	mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	material.diffuse = mat::Vec3(color.r, color.g, color.b);

	mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
	material.ambient = mat::Vec3(color.r, color.g, color.b);

	mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
	material.specular = mat::Vec3(color.r, color.g, color.b);

	mat->Get(AI_MATKEY_SHININESS, shininess);
	material.shininess = shininess;

	return material;
}

// update min/max pos to use later to scale and center the model
void	Model::updateMinMaxPos(mat::Vec3 pos) {
	if (pos.x > _maxPos.x)
		_maxPos.x = pos.x;
	if (pos.y > _maxPos.y)
		_maxPos.y = pos.y;
	if (pos.z > _maxPos.z)
		_maxPos.z = pos.z;

	if (pos.x < _minPos.x)
		_minPos.x = pos.x;
	if (pos.y < _minPos.y)
		_minPos.y = pos.y;
	if (pos.z < _minPos.z)
		_minPos.z = pos.z;
}

// calculate the model matrix to scale and center the Model
void	Model::calcModelMatrix() {
	mat::Vec3	transl;
	float		maxDiff;
	float		scale;

	// calculate scale
	maxDiff = _maxPos.x - _minPos.x;
	if (maxDiff < _maxPos.y - _minPos.y)
		maxDiff = _maxPos.y - _minPos.y;
	if (maxDiff < _maxPos.z - _minPos.z)
		maxDiff = _maxPos.z - _minPos.z;
	maxDiff /= 2;
	scale = 1.0f / maxDiff;
	_modelScale = mat::Mat4(1.0f);

	// apply the scale
	_modelScale = _modelScale.scale(mat::Vec3(scale, scale, scale));

	// calculate the translation
	transl.x = -((_minPos.x + _maxPos.x) / 2);
	transl.y = -((_minPos.y + _maxPos.y) / 2);
	transl.z = -((_minPos.z + _maxPos.z) / 2);
	// verification due to float precision
	transl.x = scale * ((transl.x < 0.00001f && transl.x > -0.00001f) ? 0.0f : transl.x);
	transl.y = scale * ((transl.y < 0.00001f && transl.y > -0.00001f) ? 0.0f : transl.y);
	transl.z = scale * ((transl.z < 0.00001f && transl.z > -0.00001f) ? 0.0f : transl.z);
	// apply the translation
	_modelScale = _modelScale.translate(transl);

	// sent it to the shader
	_shader.use();
	_shader.setMat4("modelScale", _modelScale);

	_cubeShader.use();
	_cubeShader.setMat4("modelScale", _modelScale);
}


Mesh	Model::processMesh(aiMesh *mesh, const aiScene *scene) {
	std::vector<VertexMat>	vertices;
	VertexMat				vertex;

	std::vector<u_int32_t>	indices;
	aiFace					face;

	std::vector<Texture>	textures;
	aiMaterial				*material;
	std::vector<Texture>	diffuseMaps;
	std::vector<Texture>	specularMaps;

	u_int32_t				boneIndex;
	std::string				boneName;
	int						vertexID;
	float					weight;

    // process vertices
	for (u_int32_t i = 0; i < mesh->mNumVertices; ++i) {
		// process vertex positions
		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;
		// update the min/max pos
		updateMinMaxPos(vertex.pos);
		// process vertex normals
		vertex.norm.x = mesh->mNormals[i].x;
		vertex.norm.y = mesh->mNormals[i].y;
		vertex.norm.z = mesh->mNormals[i].z;
		// process vertex texture coordinates
		vertex.texCoords = mat::Vec2(0.0f, 0.0f);
		if (mesh->mTextureCoords[0]) {
			vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}

    // process indices
	for (u_int32_t i = 0; i < mesh->mNumFaces; ++i) {
		face = mesh->mFaces[i];
		// all face is conposed of 3 indices due to aiProcess_Triangulate
		for (u_int32_t j = 0; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}

	// process material
	material = scene->mMaterials[mesh->mMaterialIndex];
	// load diffuse textures
	diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, \
	TextureT::difuse);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// load specular textures
	specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, \
	TextureT::specular);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	// create the mesh
    Mesh ret = Mesh(vertices, indices, textures, loadMaterial(material));

	// process bones
	for (u_int32_t i = 0; i < mesh->mNumBones; ++i) {
        boneIndex = 0;
        boneName = mesh->mBones[i]->mName.data;

		// if the bone don't exist yet
        if (_boneMap.find(boneName) == _boneMap.end()) {
            boneIndex = _actBoneId;
            ++_actBoneId;
            _boneInfo[_actBoneId] = BoneInfo();
        }
        else {
            boneIndex = _boneMap[boneName];
        }

        _boneMap[boneName] = boneIndex;
        _boneInfo[boneIndex].boneOffset = aiToMat4(mesh->mBones[i]->mOffsetMatrix);

		// add boneId ad weight to the mesh
		for (u_int32_t j = 0; j < mesh->mBones[i]->mNumWeights; ++j) {
			vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
			weight = mesh->mBones[i]->mWeights[j].mWeight;
			if (weight <= 0.1)
				continue;
			ret.addBoneData(boneIndex, weight, vertexID);
		}
	}

	ret.setupMesh();
	return ret;
}

std::vector<Texture>	Model::loadMaterialTextures(aiMaterial *mat, \
aiTextureType type, TextureT textType) {
    std::vector<Texture>	textures;
	aiString				str;
	Texture					texture;
	bool					skip;

    for (u_int32_t i = 0; i < mat->GetTextureCount(type); ++i) {
        mat->GetTexture(type, i, &str);

		skip = false;

		// verify if the texture has been loaded already
		for (u_int32_t j = 0; j < _texturesLoaded.size(); ++j) {
			if (std::strcmp(_texturesLoaded[j].path.data(), str.C_Str()) == 0) {
				textures.push_back(_texturesLoaded[j]);
				skip = true;
				break;
			}
		}

		// if not, load it
		if (!skip) {
			texture.id = textureFromFile(str.C_Str(), _directory);
			texture.type = textType;
			texture.path = str.C_Str();
			textures.push_back(texture);
			// save to _texturesLoaded array to skip duplicate textures loading later
			_texturesLoaded.push_back(texture);
		}
    }
    return textures;
}

// send cube data to draw a cube at the bones origin later
void	Model::sendCubeData() {
    glGenVertexArrays(1, &_cubeVao);
    glGenBuffers(1, &_cubeVbo);

    glBindBuffer(GL_ARRAY_BUFFER, _cubeVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Model::_cubeData), Model::_cubeData, GL_STATIC_DRAW);

    glBindVertexArray(_cubeVao);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(7);

	_cubeShader.use();
	// set cube material
	Material material;
	_cubeShader.setBool("material.diffuse.isTexture", false);
	_cubeShader.setVec3("material.diffuse.color", material.diffuse);
	_cubeShader.setBool("material.specular.isTexture", false);
	_cubeShader.setVec3("material.specular.color", material.specular);
	_cubeShader.setFloat("material.shininess", material.shininess);
}

const char* Model::AssimpError::what() const throw() {
    return ("Assimp failed to load the model!");
}
Shader					&Model::getShader() const { return _shader; }
Shader					&Model::getCubeShader() const { return _cubeShader; }
std::vector<Mesh>		Model::getMeshes() const { return _meshes; }
std::string				Model::getDirectory() const { return _directory; }
std::vector<Texture>	Model::getTexturesLoaded() const { return _texturesLoaded; }
mat::Mat4				&Model::getModel() { return _model; }
mat::Mat4				&Model::getModelScale() { return _modelScale; }
const mat::Mat4			&Model::getModel() const { return _model; }
const mat::Mat4			&Model::getModelScale() const { return _modelScale; }
mat::Vec3				Model::getMinPos() const { return _minPos; }
mat::Vec3				Model::getMaxPos() const { return _maxPos; }
std::map<std::string, int>	Model::getBoneMap() const { return _boneMap; }
std::array<Model::BoneInfo, MAX_BONES>	Model::getBoneInfo() const { return _boneInfo; }
std::array<float, MAX_BONES * 16>	Model::getBoneInfoUniform() const { return _boneInfoUniform; }
u_int32_t				Model::getActBoneId() const { return _actBoneId; }
mat::Mat4				Model::getGlobalTransform() const { return _globalTransform; }
mat::Mat4				Model::getGlobalInverseTransform() const { return _globalInverseTransform; }
u_int32_t				Model::getCubeVbo() const { return _cubeVbo; }
u_int32_t				Model::getCubeVao() const { return _cubeVao; }
bool					&Model::isDrawMesh() { return _drawMesh; }
bool					Model::isDrawMesh() const { return _drawMesh; }
bool					&Model::isDrawCube() { return _drawCube; }
bool					Model::isDrawCube() const { return _drawCube; }