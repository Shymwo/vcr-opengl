#include "ship.h"

//Statek
#define svNumber 37707
//#define svNumber 1037328
#define svNumber2 8532

vector<vec3> verticess;
vector<vec3> normalss;
vector<vec2> uvs;
vec3 sVertices[svNumber];
vec3 sNormals[svNumber];
vec2 sTexCoords[svNumber];

vector<vec3> verticess2;
vector<vec3> normalss2;
vector<vec2> uvs2;
vec3 pVertices[svNumber2];
vec3 pNormals[svNumber2];
vec2 pTexCoords[svNumber2];

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
			    glm::vec3 normal;
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

void loadShip() {
	loadOBJ("medieval.obj", verticess, normalss, uvs);
	for (int i=0; i<svNumber;i++) {
		sVertices[i] = verticess[i];
		sNormals[i] = normalss[i];
		sTexCoords[i] = uvs[i];
	}
}

void loadPaddle() {
	loadOBJ("paddle.obj", verticess2, normalss2, uvs2);
	for (int i=0; i<svNumber2;i++) {
		pVertices[i] = verticess2[i];
		pNormals[i] = normalss2[i];
		pTexCoords[i] = uvs2[i];
	}
}

void drawShip() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3,GL_FLOAT,0,sVertices);
	glNormalPointer(GL_FLOAT,0,sNormals);
	glTexCoordPointer(2,GL_FLOAT,0,sTexCoords);
	glDrawArrays(GL_TRIANGLES,0,svNumber);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void drawPaddle() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3,GL_FLOAT,0,pVertices);
	glNormalPointer(GL_FLOAT,0,pNormals);
	glTexCoordPointer(2,GL_FLOAT,0,pTexCoords);
	glDrawArrays(GL_TRIANGLES,0,svNumber2);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
