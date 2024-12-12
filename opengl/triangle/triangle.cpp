#include <QApplication>
#include <QOpenGLContext>
//#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QTimer>
#include <QMatrix4x4>
#include <QtMath>

class triangle : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core {
    Q_OBJECT
    

public:
    triangle(QWidget *parent = nullptr) : QOpenGLWidget(parent), angle(0.0f) {

        
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

        // Render triangle
        draw(1.0f, 30, 30);
    }

private:
    GLuint vao;
    float angle;
    QMatrix4x4 projection;

    void draw(float radius, int slices, int stacks) {
        }
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
