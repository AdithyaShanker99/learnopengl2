#include"../Headers/Mesh.h"
#include "../Headers/HeightMap.h"

float fov = 45.0;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov+=yoffset;
}

int width = 2160;
int height = 2160;


heightMap heightMap("../Resources/HeightMaps/Heightmap.png");


std::vector<glm::vec3> v = heightMap.createVertices();
std::vector<glm::vec3> n = heightMap.createNormals(v);
std::vector<glm::vec2> t = heightMap.createTextureCoords(v);
std::vector<unsigned int> indicesMesh = heightMap.createIndices();


std::vector<Vertex> verticesMesh = heightMap.createVerts(v, n, t);

// Vertices coordinates
Vertex vertices[] =
{ 
	// Base of the pyramid
	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3( 1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3( 1.0f, 0.0f,  1.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
	// Apex of the pyramid
	Vertex{glm::vec3( 0.0f, 1.0f,  0.0f), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f,  1.0f, 0.0f), glm::vec2(0.5f, 0.5f)}
};

// Indices for vertices order
GLuint indices[] =
{
	// Base
	0, 1, 2,
	0, 2, 3,
	// Sides
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};


Vertex lightVertices[] =
{ //     COORDINATES     //
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};


int main()
{
    
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    GLFWwindow* window = glfwCreateWindow(width, height, "I made this", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "FAILED TO CREATE WINDOW" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);

    gladLoadGL();

    glViewport(0, 0, width, height);

    Texture textures[]
	{
		Texture("../Resources/Textures/Obsidian_diffuse.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture( "../Resources/Textures/Obsidian_spec.png","specular", 1, GL_RED, GL_UNSIGNED_BYTE),
		Texture("../Resources/Textures/planks.png", "diffuse", 2, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture( "../Resources/Textures/planks_spec.png","specular", 3, GL_RED, GL_UNSIGNED_BYTE)
	};

    Shader shaderProgram("../Resources/Shaders/default.vert", "../Resources/Shaders/default.frag");

    std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	// Create floor mesh
	Mesh floor(verticesMesh, indicesMesh, tex);



    // Shader for light cube
	Shader lightShader("../Resources/Shaders/light.vert", "../Resources/Shaders/light.frag");
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	// Create light mesh
	Mesh light(lightVerts, lightInd, tex);


    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
    //GLuint vId = glGetUniformLocation(shaderProgram.ID, "velocity");



    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f,0.9f,1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f,10.0f,0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);


	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


    glEnable(GL_DEPTH_TEST);



    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 3.0f));
    glfwSetScrollCallback(window, scroll_callback);
    // glfwSetKeyCallback();

    while (!glfwWindowShouldClose(window))
    {
        
        
        glClearColor(0.75f, 0.5f, 0.75f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.Activate();

        
        camera.Inputs(window);
        camera.updateMatrix(fov, 0.1f, 1000.0f);
        // Draws different meshes
		floor.Draw(shaderProgram, camera);
		light.Draw(lightShader, camera);

        glfwSwapBuffers(window);


        glfwPollEvents();
    }

    // Delete all the objects we've created
	shaderProgram.Delete();
	lightShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}




