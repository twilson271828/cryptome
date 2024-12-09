#include <QApplication>
#include <QOpenGLContext>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QMatrix4x4>
#include <QtMath>

class sphere : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core {
    Q_OBJECT
    

public:
    sphere(QWidget *parent = nullptr) : QOpenGLWidget(parent), angle(0.0f) {

        
        // Timer to update rotation
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [this]() {
            angle += 1.0f;
            if (angle >= 360.0f)
                angle -= 360.0f;
            update();
        });
        timer->start(16); // ~60 FPS
    }

protected:
    void initializeGL() override {
        initializeOpenGLFunctions();

        // Enable depth testing
        glEnable(GL_DEPTH_TEST);

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

        // Render sphere
        drawSphere(1.0f, 30, 30);
    }

private:
    GLuint vao;
    float angle;
    QMatrix4x4 projection;

    void drawSphere(float radius, int slices, int stacks) {
        for (int i = 0; i < stacks; ++i) {
            float lat0 = M_PI * (-0.5f + float(i) / stacks);
            float z0 = radius * sin(lat0);
            float zr0 = radius * cos(lat0);

            float lat1 = M_PI * (-0.5f + float(i + 1) / stacks);
            float z1 = radius * sin(lat1);
            float zr1 = radius * cos(lat1);

        glBegin(GL_QUAD_STRIP);
            for (int j = 0; j <= slices; ++j) {
                float lng = 2 * M_PI * float(j) / slices;
                float x = cos(lng);
                float y = sin(lng);

                glNormal3f(x * zr0, y * zr0, z0);
                glVertex3f(x * zr0, y * zr0, z0);
                glNormal3f(x * zr1, y * zr1, z1);
                glVertex3f(x * zr1, y * zr1, z1);
            }
        glEnd();
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    sphere window;
    window.resize(800, 600);
    window.show();

    return app.exec();
}
