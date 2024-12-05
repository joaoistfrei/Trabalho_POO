#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>

class ChessBoard : public QWidget {
public:
    ChessBoard() {
        QGridLayout* layout = new QGridLayout(this);
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                QPushButton* button = new QPushButton();
                button->setFixedSize(60, 60);
                layout->addWidget(button, row, col);
            }
        }
        setLayout(layout);
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ChessBoard board;
    board.show();
    return app.exec();
}