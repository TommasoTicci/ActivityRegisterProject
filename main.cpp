#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QPushButton button("PLACEHOLDER", nullptr);
    button.resize(400, 200);
    button.show();
    return QApplication::exec();
}
