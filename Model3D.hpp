#pragma once

class Model3D {
public:

	GLuint VAO, VBO;

	float fPos_x;
	float fPos_y;
	float fPos_z;   //position of object

	float fScale_x;
	float fScale_y;
	float fScale_z; //scale of object

	float vAxis_x;
	float vAxis_y;
	float vAxis_z; //for vertical rotation

	float hAxis_x;
	float hAxis_y;
	float hAxis_z; //for horizontal rotation

	float fXtheta_mod;
	float fYtheta_mod;

	float fScale_mod;

public:
	Model3D() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		this->fPos_x = 0.0f;
		this->fPos_y = 0.0f;
		this->fPos_z = -2.0f;

		this->fScale_x = 1.0f;
		this->fScale_y = 1.0f;
		this->fScale_z = 1.0f;

		this->vAxis_x = 1.0f;
		this->vAxis_y = 0.0f;
		this->vAxis_z = 0.0f;

		this->hAxis_x = 0.0f;
		this->hAxis_y = 1.0f;
		this->hAxis_z = 0.0f;

		this->fXtheta_mod = 0.0f;
		this->fYtheta_mod = 0.0f;

		this->fScale_mod = 1.0f;

		this->identity_matrix = glm::mat4(1.0f);

		this->loadTexture();
	}

private:
	GLuint texture;

private:
	std::vector<GLfloat> fullVertexData;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t>material;
	tinyobj::attrib_t attributes;
	glm::mat4 identity_matrix;
	glm::mat4 transformation_matrix;

public:
	void spawn() {
		this->loadTexture();

		this->loadObject();

		this->buildVertexData();

		this->loadVertexData();
	}

	void update(GLuint shaderProg) {
		this->transformationMatrix(shaderProg);
	}

	void draw() {
		glDrawArrays(GL_TRIANGLES, 0, this->fullVertexData.size() / 8);
	}

private:

	void loadTexture() {
		int img_width, img_height, colorChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* tex_bytes = stbi_load("3D/Cattus.png",
			&img_width,
			&img_height,
			&colorChannels,
			0);

		glGenTextures(1, &this->texture);

		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, this->texture);

		glTexImage2D(GL_TEXTURE_2D,
			0,             //texture 0
			GL_RGBA,       //target color format of the texture // change to RGBA for png, RGB for jpg
			img_width,   //texture width
			img_height, //texture height
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			tex_bytes);       //loaded texture in bytes

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(tex_bytes);
		glEnable(GL_DEPTH_TEST);
	}

	void loadObject() {
		std::string path = "3D/SwordCat.obj";
		std::string warning, error;

		bool success = tinyobj::LoadObj(
			&this->attributes,
			&this->shapes,
			&this->material,
			&warning,
			&error,
			path.c_str()
		);

		//if (success == true) {
		//	std::cout << "Success loading Object" << std::endl;
		//}
		//else {
		//	std::cout << "Failed loading Object" << std::endl;
		//}
	}

	void buildVertexData() {
		for (int i = 0; i < this->shapes[0].mesh.indices.size(); i++) {
			tinyobj::index_t vData = this->shapes[0].mesh.indices[i];

			this->fullVertexData.push_back(this->attributes.vertices[(vData.vertex_index * 3)]);
			this->fullVertexData.push_back(this->attributes.vertices[(vData.vertex_index * 3) + 1]);
			this->fullVertexData.push_back(this->attributes.vertices[(vData.vertex_index * 3) + 2]);

			this->fullVertexData.push_back(this->attributes.normals[(vData.normal_index * 3)]);
			this->fullVertexData.push_back(this->attributes.normals[(vData.normal_index * 3) + 1]);
			this->fullVertexData.push_back(this->attributes.normals[(vData.normal_index * 3) + 2]);

			this->fullVertexData.push_back(this->attributes.texcoords[(vData.texcoord_index * 2)]);
			this->fullVertexData.push_back(this->attributes.texcoords[(vData.texcoord_index * 2) + 1]);

			//if (i > this->shapes[0].mesh.indices.size()) {
			//	std::cout << i << std::endl;
			//}
		}

	}

	void loadVertexData() {
		glBufferData(GL_ARRAY_BUFFER,
			sizeof(GLfloat) * this->fullVertexData.size(),
			this->fullVertexData.data(),
			GL_DYNAMIC_DRAW);

		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			8 * sizeof(float),
			(void*)0
		);

		glEnableVertexAttribArray(0);

		GLintptr normPtr = 3 * sizeof(float);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			8 * sizeof(float),
			(void*)normPtr
		);

		glEnableVertexAttribArray(1);

		GLintptr uvPtr = 6 * sizeof(float);
		glVertexAttribPointer(
			2,
			2,
			GL_FLOAT,
			GL_FALSE,
			8 * sizeof(float),
			(void*)uvPtr
		);

		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}


private:

	void transformationMatrix(GLuint shaderProg) {

		this->transformation_matrix = glm::translate(this->identity_matrix,
			glm::vec3(this->fPos_x, this->fPos_y, this->fPos_z));

		this->transformation_matrix = glm::scale(this->transformation_matrix,
			glm::vec3(this->fScale_x, this->fScale_y, this->fScale_z));

		this->transformation_matrix = glm::rotate(this->transformation_matrix,
			glm::radians(this->fYtheta_mod),
			glm::vec3(this->hAxis_x, this->hAxis_y, this->hAxis_z));

		this->transformation_matrix = glm::rotate(this->transformation_matrix,
			glm::radians(this->fXtheta_mod),
			glm::vec3(this->vAxis_x, this->vAxis_y, this->vAxis_z));

		glUseProgram(shaderProg);
		unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");

		glUniformMatrix4fv(transformLoc,
			1,
			GL_FALSE,
			glm::value_ptr(this->transformation_matrix));
	}
};