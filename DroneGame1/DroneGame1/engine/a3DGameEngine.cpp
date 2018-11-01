#include "a3DGameEngine.h"

a3DGameEngine::a3DGameEngine() : aGameEngine()
{
	//std::cout << "It's a3DGameEngine" << std::endl;

}

a3DGameEngine::a3DGameEngine(const a3DGameEngine & copy) : aGameEngine(copy)
{
}

a3DGameEngine::~a3DGameEngine()
{
}

void a3DGameEngine::processKeys()
{

	float spinXinc = 0, spinYinc = 0, spinZinc = 0;
	if (input_manager->isInputActivated(VK_UP))
	{
		spinXinc = 0.01f;
	}
	if (input_manager->isInputActivated(VK_DOWN))
	{
		spinXinc = -0.01f;
	}
	if (input_manager->isInputActivated(VK_LEFT))
	{
		spinYinc = 0.01f;
	}
	if (input_manager->isInputActivated(VK_RIGHT))
	{
		spinYinc = -0.01f;
	}
	if (input_manager->isInputActivated(VK_SPACE))
	{
		spinZinc = 0.01f;
	}
	if (input_manager->isInputActivated(VK_SHIFT))
	{
		spinZinc = -0.01f;
	}
	float speed = 100.0f;

	glm::mat4 matrixX, matrixXY;

	//rotation about the local x axis
	q = glm::angleAxis(spinXinc*speed, glm::vec3(objectRotation[0][0], objectRotation[0][1], objectRotation[0][2]));
	matrixX = glm::mat4_cast(q) * objectRotation;

	//EXAMPLE FOR ACCESSING USING A 1D array
	const float *pSource = (const float*)glm::value_ptr(matrixX);
	//rotation about the local y axis
	q = glm::angleAxis(spinYinc*speed, glm::vec3(pSource[4], pSource[5], pSource[6]));
	matrixXY = glm::mat4_cast(q) * matrixX;

	//EXAMPLE ACCESSING WITH 2D GLM structure.
	//rotation about the local z axis
	q = glm::angleAxis(spinZinc*speed, glm::vec3(matrixXY[2][0], matrixXY[2][1], matrixXY[2][2]));
	objectRotation = glm::mat4_cast(q) * matrixXY;


}

void a3DGameEngine::draw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glUseProgram(myShader->handle());  // use the shader

	amount += temp;
	if (amount > 1.0f || amount < -0.5f)
		temp = -temp;
	//amount = 0;
	glUniform1f(glGetUniformLocation(myShader->handle(), "displacement"), amount);

	GLuint matLocation = glGetUniformLocation(myShader->handle(), "ProjectionMatrix");
	glUniformMatrix4fv(matLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

	glm::mat4 viewingMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, -50));
	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);

	glUniform4fv(glGetUniformLocation(myShader->handle(), "LightPos"), 1, LightPos);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "light_ambient"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "light_diffuse"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "light_specular"), 1, Light_Specular);

	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_ambient"), 1, Material_Ambient);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_diffuse"), 1, Material_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_specular"), 1, Material_Specular);
	glUniform1f(glGetUniformLocation(myShader->handle(), "material_shininess"), Material_Shininess);


	//DRAW THE MODEL
	modelViewMatrix = viewingMatrix * objectRotation;

	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &modelViewMatrix[0][0]);


	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(modelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

	model.drawElementsUsingVBO(myShader);



	glUseProgram(myBasicShader->handle());  // use the shader
	glUniformMatrix4fv(glGetUniformLocation(myBasicShader->handle(), "ProjectionMatrix"), 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(myBasicShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &modelViewMatrix[0][0]);

	//model.drawBoundingBox(myBasicShader);
	//model.drawOctreeLeaves(myBasicShader);

	glUseProgram(myShader->handle());  // use the shader

	modelViewMatrix = glm::translate(viewingMatrix, glm::vec3(20, 0, 0));

	normalMatrix = glm::inverseTranspose(glm::mat3(modelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

	//Pass the uniform for the modelview matrix - in this case just "r"
	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &modelViewMatrix[0][0]);
	modelbox.drawElementsUsingVBO(myShader);

	glFlush();

}

void a3DGameEngine::init()
{

	glClearColor(1.0, 1.0, 1.0, 0.0);						//sets the clear colour to yellow
														//glClear(GL_COLOR_BUFFER_BIT) in the display function
														//will clear the buffer to this colour
	glEnable(GL_DEPTH_TEST);

	myShader = new Shader;
	//if(!myShader->load("BasicView", "Assets/glslfiles/basicTransformationsWithDisplacement.vert", "Assets/glslfiles/basicTransformationsWithDisplacement.frag"))
	if (!myShader->load("BasicView", "Assets/glslfiles/basicTransformations.vert", "Assets/glslfiles/basicTransformations.frag"))
	{
		cout << "failed to load shader" << endl;
	}

	myBasicShader = new Shader;
	if (!myBasicShader->load("Basic", "Assets/glslfiles/basic.vert", "Assets/glslfiles/basic.frag"))
	{
		cout << "failed to load shader" << endl;
	}

	glUseProgram(myShader->handle());  // use the shader

	glEnable(GL_TEXTURE_2D);

	cout << " loading model " << endl;
	if (objLoader.loadModel("Assets/models/tardis_1.obj", model))//returns true if the model is loaded, puts the model in the model parameter
	{
		cout << " model loaded " << endl;

		//if you want to translate the object to the origin of the screen,
		//first calculate the centre of the object, then move all the vertices
		//back so that the centre is on the origin.
		//model.calcCentrePoint();
		//model.centreOnZero();


		model.calcVertNormalsUsingOctree();  //the method will construct the octree if it hasn't already been created.


		//turn on VBO by setting useVBO to true in threeDmodel.cpp default constructor - only permitted on 8 series cards and higher
		model.scale(3.0f);
		model.initDrawElements();
		model.initVBO(myShader);
		model.deleteVertexFaceData();

	}
	else
	{
		cout << " model failed to load " << endl;
	}

	if (objLoader.loadModel("Assets/models/box.obj", modelbox))//returns true if the model is loaded, puts the model in the model parameter
	{
		cout << " model loaded " << endl;

		//if you want to translate the object to the origin of the screen,
		//first calculate the centre of the object, then move all the vertices
		//back so that the centre is on the origin.
		modelbox.calcCentrePoint();
		modelbox.centreOnZero();


		modelbox.calcVertNormalsUsingOctree();  //the method will construct the octree if it hasn't already been created.


		//turn on VBO by setting useVBO to true in threeDmodel.cpp default constructor - only permitted on 8 series cards and higher
		modelbox.initDrawElements();
		modelbox.initVBO(myShader);
		modelbox.deleteVertexFaceData();

	}
	else
	{
		cout << " model failed to load " << endl;
	}

}

void a3DGameEngine::update()
{

	//collisions
	//physics
	//constraints

}

void a3DGameEngine::resize(int width, int height)
{
	this->screenWidth = width; 
	this->screenHeight = height;

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	//Set the projection matrix
	this->projectionMatrix = glm::perspective(60.0f, (GLfloat)width / (GLfloat)height, 1.0f, 200.0f);

}
