
std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
std::vector< glm::vec3 > temp_vertices;
std::vector< glm::vec2 > temp_uvs;
std::vector< glm::vec3 > temp_normals;

bool loadOBJ(
             const char * path,
             std::vector < glm::vec3 > & out_vertices,
             std::vector < glm::vec2 > & out_uvs,
             std::vector < glm::vec3 > & out_normals
             )
