#include <QApplication>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <cmath>
#include <vector>

class SphereWidget : public QOpenGLWidget, protected QOpenGLFunctions {
private:
    QOpenGLShaderProgram program;
    GLuint VAO, VBO, EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    void generateSphere(float radius, int stacks, int slices) {
        for (int i = 0; i <= stacks; ++i) {
            float stackAngle = M_PI / 2 - i * M_PI / stacks; // From pi/2 to -pi/2
            float xy = radius * cosf(stackAngle); // r * cos(u)
            float z = radius * sinf(stackAngle);  // r * sin(u)

            for (int j = 0; j <= slices; ++j) {
                float sliceAngle = j * 2 * M_PI / slices; // From 0 to 2pi
                float x = xy * cosf(sliceAngle);          // x = r * cos(u) * cos(v)
                float y = xy * sinf(sliceAngle);          // y = r * cos(u) * sin(v)

                // Vertex positions
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                // Normals (for lighting calculations, here they're the same as the positions)
                vertices.push_back(x / radius);
                vertices.push_back(y / radius);
                vertices.push_back(z / radius);
            }
        }

        for (int i = 0; i < stacks; ++i) {
            for (int j = 0; j < slices; ++j) {
                unsigned int first = (i * (slices + 1)) + j;
                unsigned int second = first + slices + 1;

                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }
    }

protected:
    void initializeGL() override {
        initializeOpenGLFunctions();

        // Generate sphere data
        generateSphere(1.0f, 40, 40);

        // Create buffers and arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Vertex positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Vertex normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        // Shader program
        program.addShaderFromSourceCode(QOpenGLShader::Vertex, R"(
            #version 330 core
            layout (location = 0) in vec3 position;
            layout (location = 1) in vec3 normal;
            out vec3 fragNormal;
            out vec3 fragPosition;
            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;
            void main() {
                fragNormal = mat3(transpose(inverse(model))) * normal;
                fragPosition = vec3(model * vec4(position, 1.0));
                gl_Position = projection * view * model * vec4(position, 1.0);
            }
        )");

        program.addShaderFromSourceCode(QOpenGLShader::Fragment, R"(
            #version 330 core
            in vec3 fragNormal;
            in vec3 fragPosition;
            out vec4 FragColor;
            uniform vec3 lightPos;
            uniform vec3 viewPos;
            uniform vec3 lightColor;
            uniform vec3 objectColor;
            void main() {
                // Ambient
                float ambientStrength = 0.1;
                vec3 ambient = ambientStrength * lightColor;

                // Diffuse
                vec3 norm = normalize(fragNormal);
                vec3 lightDir = normalize(lightPos - fragPosition);
                float diff = max(dot(norm, lightDir), 0.0);
                vec3 diffuse = diff * lightColor;

                // Specular
                float specularStrength = 0.5;
                vec3 viewDir = normalize(viewPos - fragPosition);
                vec3 reflectDir = reflect(-lightDir, norm);
                float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
                vec3 specular = specularStrength * spec * lightColor;

                vec3 result = (ambient + diffuse + specular) * objectColor;
                FragColor = vec4(result, 1.0);
            }
        )");

        program.link();
    }

    void resizeGL(int w, int h) override {
        glViewport(0, 0, w, h);
    }

    void paintGL() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        program.bind();

        QMatrix4x4 model, view, projection;
        model.rotate(30.0f, 1.0f, 0.0f, 0.0f);
        view.translate(0.0f, 0.0f, -5.0f);
        projection.perspective(45.0f, float(width()) / height(), 0.1f, 100.0f);

        program.setUniformValue("model", model);
        program.setUniformValue("view", view);
        program.setUniformValue("projection", projection);
        program.setUniformValue("lightPos", QVector3D(2.0f, 2.0f, 2.0f));
        program.setUniformValue("viewPos", QVector3D(0.0f, 0.0f, 5.0f));
        program.setUniformValue("lightColor", QVector3D(1.0f, 1.0f, 1.0f));
        program.setUniformValue("objectColor", QVector3D(1.0f, 0.5f, 0.3f));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        program.release();
    }

    void cleanup() {
        makeCurrent();
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        doneCurrent();
    }

public:
    ~SphereWidget() {
        cleanup();
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    SphereWidget widget;
    widget.resize(800, 600);
    widget.setWindowTitle("3D Sphere");
    widget.show();

    return app.exec();
}
