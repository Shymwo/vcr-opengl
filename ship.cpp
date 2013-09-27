#include "ship.h"

//Statek

vector<vec3> verticess;
vector<vec3> normalss;
vector<vec2> uvs;

GLuint bufVertices;
GLuint bufNormals;
GLuint bufTexCoords;

vector<vec3> verticess2;
vector<vec3> normalss2;
vector<vec2> uvs2;

GLuint bufVertices2;
GLuint bufNormals2;
GLuint bufTexCoords2;

// load OBJ model file
bool loadOBJ(const char * path, vector <vec3> &out_vertices, vector <vec3> &out_normals, vector <vec2> &out_uvs){
	vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	vector< vec3 > temp_vertices;
	vector< vec3 > temp_normals;
	vector< vec2 > temp_uvs;

	FILE * file = fopen(path, "r");
	if( file == NULL ){
    		printf("Impossible to open the file !\n");
    		return false;
	}

	while( 1 ){

    		char lineHeader[128];
    		// read the first word of the line
    		int res = fscanf(file, "%s", lineHeader);
    		if (res == EOF)
        		break; // EOF = End Of File. Quit the loop.

    		// else : parse lineHeader

			if ( strcmp( lineHeader, "v" ) == 0 ){
    				vec3 vertex;
    				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
    				temp_vertices.push_back(vertex);
			}else if ( strcmp( lineHeader, "vt" ) == 0 ){
    				vec2 uv;
   				 fscanf(file, "%f %f\n", &uv.x, &uv.y);
    				temp_uvs.push_back(uv);
			}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			    vec3 normal;
			    fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			    temp_normals.push_back(normal);
			}else if ( strcmp( lineHeader, "f" ) == 0 ){
			    std::string vertex1, vertex2, vertex3;
			    unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			    int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			    if (matches != 9){
			        printf("File can't be read by our simple parser : ( Try exporting with other options\n");
			        return false;
			    }
			    vertexIndices.push_back(vertexIndex[0]);
			    vertexIndices.push_back(vertexIndex[1]);
			    vertexIndices.push_back(vertexIndex[2]);
			    uvIndices    .push_back(uvIndex[0]);
			    uvIndices    .push_back(uvIndex[1]);
			    uvIndices    .push_back(uvIndex[2]);
			    normalIndices.push_back(normalIndex[0]);
			    normalIndices.push_back(normalIndex[1]);
			    normalIndices.push_back(normalIndex[2]);
			}
	}
	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){
		unsigned int vertexIndex = vertexIndices[i];
		vec3 vertex = temp_vertices[ vertexIndex-1 ];
		out_vertices.push_back(vertex);
	}
	for( unsigned int i=0; i<normalIndices.size(); i++ ){
		unsigned int normalIndex = normalIndices[i];
		vec3 normal = temp_normals[ normalIndex-1 ];
		out_normals.push_back(normal);
	}
	for( unsigned int i=0; i<uvIndices.size(); i++ ){
		unsigned int uvIndex = uvIndices[i];
		vec2 uv = temp_uvs[ uvIndex-1 ];
		out_uvs.push_back(uv);
	}

	return true;
}

void loadShip(char* filename) {
	loadOBJ(filename, verticess, normalss, uvs);

	glGenBuffers(1,&bufVertices);
	glBindBuffer(GL_ARRAY_BUFFER,bufVertices);
	glBufferData(GL_ARRAY_BUFFER, verticess.size()*sizeof(vec3), &verticess[0], GL_STATIC_DRAW);

	glGenBuffers(1,&bufNormals);
	glBindBuffer(GL_ARRAY_BUFFER,bufNormals);
	glBufferData(GL_ARRAY_BUFFER, normalss.size()*sizeof(vec3), &normalss[0], GL_STATIC_DRAW);

	glGenBuffers(1,&bufTexCoords);
	glBindBuffer(GL_ARRAY_BUFFER,bufTexCoords);
	glBufferData(GL_ARRAY_BUFFER, uvs.size()*sizeof(vec2), &uvs[0], GL_STATIC_DRAW);

}

void loadPaddle(char* filename) {
	loadOBJ(filename, verticess2, normalss2, uvs2);

	glGenBuffers(1,&bufVertices2);
	glBindBuffer(GL_ARRAY_BUFFER,bufVertices2);
	glBufferData(GL_ARRAY_BUFFER, verticess2.size()*sizeof(vec3), &verticess2[0], GL_STATIC_DRAW);

	glGenBuffers(1,&bufNormals2);
	glBindBuffer(GL_ARRAY_BUFFER,bufNormals2);
	glBufferData(GL_ARRAY_BUFFER, normalss2.size()*sizeof(vec3), &normalss2[0], GL_STATIC_DRAW);

	glGenBuffers(1,&bufTexCoords2);
	glBindBuffer(GL_ARRAY_BUFFER,bufTexCoords2);
	glBufferData(GL_ARRAY_BUFFER, uvs2.size()*sizeof(vec2), &uvs2[0], GL_STATIC_DRAW);
}

void drawShip() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, bufVertices);
	glVertexPointer( 3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, bufNormals);
	glNormalPointer( GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, bufTexCoords);
	glTexCoordPointer( 2, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	glDrawArrays(GL_TRIANGLES,0,verticess.size());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void drawPaddle() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, bufVertices2);
	glVertexPointer( 3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, bufNormals2);
	glNormalPointer( GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, bufTexCoords2);
	glTexCoordPointer( 2, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	glDrawArrays(GL_TRIANGLES,0,verticess2.size());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void deleteShipBuffers() {
	glDeleteBuffers(1,&bufVertices);
	glDeleteBuffers(1,&bufNormals);
	glDeleteBuffers(1,&bufTexCoords);

	glDeleteBuffers(1,&bufVertices2);
	glDeleteBuffers(1,&bufNormals2);
	glDeleteBuffers(1,&bufTexCoords2);
}
