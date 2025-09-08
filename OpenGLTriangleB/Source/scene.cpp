#include "Scene.h"


unsigned int Scene::scrWidth = 0;
unsigned int Scene::scrHeight = 0;



void Scene::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    // update variables
    Scene::scrWidth = width;
    Scene::scrHeight = height;
}

Scene::Scene(int glfwVersionMajor, int glfwVersionMinor, const char* title, unsigned int scrWidth, unsigned int scrHeight)
    : glfwVersionMajor(glfwVersionMajor), glfwVersionMinor(glfwVersionMinor), title(title), activeCamera(-1), currentId("BANANA")
{
    Scene::scrHeight = scrHeight;
    Scene::scrWidth = scrWidth;

    defaultFBO = FramebufferObject(scrWidth, scrHeight, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    setWindowColor(0.1f, 0.15f, 0.15f, 1.0f);
}

bool Scene::init()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glfwVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glfwVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(scrWidth, scrHeight, title, NULL, NULL);
    {
        if (window == NULL)
        {
            return false;
        }
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed To Initialize GLAD" << std::endl;

        glfwTerminate;
        return false;

    }

    glViewport(0, 0, scrWidth,scrHeight);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    //Keyboard register
    glfwSetKeyCallback(window, Keyboard::keyCallback);

    //Mouse Register
    glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);

    glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);

    glfwSetScrollCallback(window, Mouse::mouseWheelCallback);


    glEnable(GL_DEPTH_TEST);

    //blending for text textures
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_STENCIL_TEST);

    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);



    //V-Synq
    glfwSwapInterval(1);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    models = avl_createEmptyRoot(strkeycmp);
    instances = trie::Trie<RigidBody*>(trie::ascii_lowercase);

    //init the oct tree
    octree = new Octree::node(BoundingRegion(glm::vec3(-16.0f), glm::vec3(16.0f)));


}

void Scene::prepare(Box& box, std::vector<Shader> shaders)
{
    octree->update(box);

    for (Shader s : shaders)
    {
        //To do later 
    }
}


void Scene::processInput(float dt)
{
    if (activeCamera != -1 && activeCamera < cameras.size())
    {
        //active camera exists?

        double dx = Mouse::getDX(), dy = Mouse::getDY();
        if (dx != 0 || dy != 0)
        {

        }
    }
}