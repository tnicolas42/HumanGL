#include "Model.hpp"
#include <limits>

Model::Model(const char *path)
: _minPos(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()),
  _maxPos(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min()),
  _model(mat::Mat4(1.0f)),
  _modelScale(1) {
	loadModel(path);
}

Model::Model(Model const &src) {
	*this = src;
}

Model::~Model() {
	free(_boneInfoUniform);
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
		_globalTransform = rhs.getGlobalTransform();
	}
	return *this;
}

void	Model::draw(Shader &shader) {
	glUniformMatrix4fv(glGetUniformLocation(shader.id, "bones"), MAX_BONES, GL_TRUE, _boneInfoUniform);
	for (auto &mesh : _meshes)
		mesh.draw(shader);
}

void	Model::loadModel(std::string path) {
	Assimp::Importer	import;
	const aiScene		*scene;

	scene = import.ReadFile(path, \
	aiProcess_Triangulate | \
	aiProcess_FlipUVs | \
	aiProcess_GenNormals | \
	aiProcess_GenUVCoords | \
	aiProcess_LimitBoneWeights);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		throw Model::AssimpError();
	}
	_directory = path.substr(0, path.find_last_of('/'));

	_globalTransform = aiToMat4(scene->mRootNode->mTransformation);  // get global transform
	processNode(scene->mRootNode, scene);
	initScale();
	setBonesTransform(scene->mRootNode, _globalTransform);

	_boneInfoUniform = static_cast<float*>(malloc(sizeof(float) * MAX_BONES * 16));
	for (u_int32_t i=0; i < MAX_BONES; ++i) {
		for (u_int32_t j=0; j < 16; ++j) {
			_boneInfoUniform[i*16 + j] = _boneInfo[i].finalTransformation.getData()[j];
			// std::cout << _boneInfoUniform[i*16 + j] << " ";  // 1/2 show all bones matrix
		}
		// std::cout << "\n";  // 2/2 show all bones matrix
	}
	calcModelMatrix();
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

void	Model::setBonesTransform(aiNode *node, mat::Mat4 parentTransform) {
	mat::Mat4 newParent = parentTransform;
	// std::cout << node->mName.data << "\n";  // print node name
	if (_boneMap.find(node->mName.data) != _boneMap.end()) { // if there is a bone (same name as the node)
		BoneInfo &bone = _boneInfo[_boneMap[node->mName.data]];
		bone.finalTransformation = parentTransform * bone.boneOffset;
		newParent = bone.finalTransformation;
		bone.finalTransformation[0][3] *= _modelScale;
		bone.finalTransformation[1][3] *= _modelScale;
		bone.finalTransformation[2][3] *= _modelScale;
		// std::cout << bone.finalTransformation << "\n";  // print transformation
	}

	// recursion with each of its children
	for (u_int32_t i = 0; i < node->mNumChildren; ++i) {
		setBonesTransform(node->mChildren[i], newParent);
	}
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

// init the object scale
void	Model::initScale() {
	float		maxDiff;
	// calculate scale
	maxDiff = _maxPos.x - _minPos.x;
	if (maxDiff < _maxPos.y - _minPos.y)
		maxDiff = _maxPos.y - _minPos.y;
	if (maxDiff < _maxPos.z - _minPos.z)
		maxDiff = _maxPos.z - _minPos.z;
	maxDiff /= 2;
	_modelScale = 1.0f / maxDiff;
}

// calculate the model matrix to scale and center the Model
void	Model::calcModelMatrix() {
	mat::Vec3	transl;

	_model = mat::Mat4(1.0f);

	// apply the scale
	_model = _model.scale(mat::Vec3(_modelScale, _modelScale, _modelScale));

	// calculate the translation
	transl.x = -((_minPos.x + _maxPos.x) / 2);
	transl.y = -((_minPos.y + _maxPos.y) / 2);
	transl.z = -((_minPos.z + _maxPos.z) / 2);
	// verification due to float precision
	transl.x = _modelScale * ((transl.x < 0.00001f && transl.x > -0.00001f) ? 0.0f : transl.x);
	transl.y = _modelScale * ((transl.y < 0.00001f && transl.y > -0.00001f) ? 0.0f : transl.y);
	transl.z = _modelScale * ((transl.z < 0.00001f && transl.z > -0.00001f) ? 0.0f : transl.z);
	// apply the translation
	_model = _model.translate(transl);
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
			// std::cout << boneName << "\n";  // show all bones names
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

const char* Model::AssimpError::what() const throw() {
    return ("Assimp failed to load the model!");
}

std::vector<Mesh>		Model::getMeshes() const { return _meshes; }
std::string				Model::getDirectory() const { return _directory; }
std::vector<Texture>	Model::getTexturesLoaded() const { return _texturesLoaded; }
mat::Mat4				Model::getModel() const { return _model; }
mat::Vec3				Model::getMinPos() const { return _minPos; }
mat::Vec3				Model::getMaxPos() const { return _maxPos; }
float					Model::getModelScale() const { return _modelScale; }
std::map<std::string, int>	Model::getBoneMap() const { return _boneMap; }
std::array<Model::BoneInfo, MAX_BONES>	Model::getBoneInfo() const { return _boneInfo; }
float					*Model::getBoneInfoUniform() const { return _boneInfoUniform; }
u_int32_t				Model::getActBoneId() const { return _actBoneId; }
mat::Mat4				Model::getGlobalTransform() const { return _globalTransform; }

