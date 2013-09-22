#include "ship.h"

//Statek
#define svNumber 1037328
vector<vec3> verticess;
vector<vec2> uvs;
vec3 sVertices[svNumber];
vec2 sTexCoords[svNumber];

// load OBJ model file
bool loadOBJ(const char * path, vector <vec3> &out_vertices, vector <vec2> &out_uvs){
	vector< unsigned int > vertexIndices, uvIndices;
	vector< vec3 > temp_vertices;
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
			}else if ( strcmp( lineHeader, "f" ) == 0 ){
    				string vertex1, vertex2, vertex3;
    				unsigned int vertexIndex[3], uvIndex[3];
    				int matches = fscanf(file, "%d/%d %d/%d %d/%d \n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
    				if (matches != 6){
        				printf("File can't be read : ( Try exporting with other options\n");
        				return false;
    				}
    				vertexIndices.push_back(vertexIndex[0]);
    				vertexIndices.push_back(vertexIndex[1]);
    				vertexIndices.push_back(vertexIndex[2]);
    				uvIndices    .push_back(uvIndex[0]);
    				uvIndices    .push_back(uvIndex[1]);
    				//uvIndices    .push_back(uvIndex[2]);
			}
	}
	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){
		unsigned int vertexIndex = vertexIndices[i];
		vec3 vertex = temp_vertices[ vertexIndex-1 ];
		out_vertices.push_back(vertex);
	}
	for( unsigned int i=0; i<uvIndices.size(); i++ ){
		unsigned int uvIndex = uvIndices[i];
		vec2 uv = temp_uvs[ uvIndex-1 ];
		out_uvs.push_back(uv);
	}

	return true;
}

void loadShip() {
	loadOBJ("boat.obj", verticess, uvs);
//	sVertices = new vec3[verticess.size()];
//	sTexCoords = new vec3[uvs.size()];
	for (int i=0; i<svNumber;i++) {
		sVertices[i] = verticess[i];
		sTexCoords[i] = uvs[i];
	}
}

void drawShip() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3,GL_FLOAT,0,sVertices);
	glTexCoordPointer(2,GL_FLOAT,0,sTexCoords);
	glDrawArrays(GL_TRIANGLES,0,svNumber);
}
