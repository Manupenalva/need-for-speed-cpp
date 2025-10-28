#include <QApplication>

#include "editor_game.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    EditorGame editorGame;
    editorGame.show();
    app.exec();
    return 0;
}
