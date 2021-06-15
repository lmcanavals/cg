#ifndef __MESH_H__
#define __MESH_H__

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture {
	u32         id;
	std::string type;
	std::string path;
};

class Mesh {

public:
	std::vector<Vertex>  vertices;
	std::vector<u32>     indices;
	std::vector<Texture> textures;

	u32 Vao;

	Mesh(std::vector<Vertex> vertices, std::vector<u32> indices,
			std::vector<Texture> textures)
			: vertices(vertices), indices(indices), textures(textures) {
		setupMesh();
	}

	void Draw(Shader* shader) {
		u32 diffuseNr  = 1;
		u32 specularNr = 1;
		u32 normalNr   = 1;
		u32 heightNr   = 1;

		for (u32 i = 0; i < textures.size(); ++i) {
			glActiveTexture(GL_TEXTURE0 + i);
			std::string number;
			std::string name = textures[i].type;
			if (name == "texture_diffuse") {
				number = std::to_string(diffuseNr++);
			} else if (name == "texture_specular") {
				number = std::to_string(specularNr++);
			} else if (name == "texture_normal") {
				number = std::to_string(normalNr++);
			} else if (name == "texture_height") {
				number = std::to_string(heightNr++);
			}
			glUniform1i(glGetUniformLocation(shader->pid, (name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		
		glBindVertexArray(Vao);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}

private:

	u32 vbo, ebo;

	void setupMesh() {
		glGenVertexArrays(1, &Vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindVertexArray(Vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex),
		             &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(u32),
		             &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		                     (void*)offsetof(Vertex, Normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		                     (void*)offsetof(Vertex, TexCoords));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		                     (void*)offsetof(Vertex, Tangent));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		                     (void*)offsetof(Vertex, Bitangent));

		glBindVertexArray(0);
	}

};

#endif
