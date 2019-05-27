#include <QtCore>
#include <QtGui>
#include "ihm/fenetre.h"

int main(int argc, char* argv[])
{
    QApplication app(argc,argv);

    //Traduite les fenêtre usuelles
    QTranslator translator;
    translator.load(QString("qt_") + QLocale::system().name().section('_', 0, 0), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&translator);

    //Application en soit
    fenetre f;
    f.show();
    return app.exec();
}

