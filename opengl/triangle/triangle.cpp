#include <QApplication>
#include <QOpenGLContext>
//#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QTimer>
#include <QMatrix4x4>
#include <QtMath>

#include <iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";



class triangle : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core {
    Q_OBJECT
    
private:  
    unsigned int fragmentShader;
    unsigned int vertexShader;
    unsigned int shaderProgram;
    float vertices[];

public:
    triangle(QWidget *parent = nullptr) : QOpenGLWidget(parent) {

          // build and compile our shader program
        // ------------------------------------
        // vertex shader
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];

    vertices = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    }; 

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    
     // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
     
    }

protected:
    void initializeGL() override {
        initializeOpenGLFunctions();

        // Enable depth testing
        glEnable(GL_DEPTH_TEST);
      
        // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    

        // Generate a Vertex Array Object
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
    }

    void resizeGL(int w, int h) override {
        glViewport(0, 0, w, h);
        projection.setToIdentity();
        projection.perspective(45.0f, float(w) / float(h), 0.1f, 100.0f);
    }

    void paintGL() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        QMatrix4x4 model;
        model.translate(0.0f, 0.0f, -5.0f);
        model.rotate(angle, 0.0f, 1.0f, 0.0f);

        QMatrix4x4 mvp = projection * model;

        // Set up basic rendering (no shaders in this example, uses fixed pipeline)
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(mvp.constData());

        // Render triangle
        draw(1.0f, 30, 30);
    }

private:
    GLuint vao;
    float angle;
    QMatrix4x4 projection;

    void draw(float radius, int slices, int stacks) {
        }
    };


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    triangle window;
    window.resize(800, 600);
    window.show();

    return app.exec();
}
#include "triangle.moc"
