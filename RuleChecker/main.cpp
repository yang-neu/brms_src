#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <QProcess>
#include <QQmlContext>
#include <qqml.h>
#include <QDir>
//#include <iostream>
#include <QFontDatabase>

#include "BrmsAdaptor.h"

void loadDefaultFont();
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    loadDefaultFont();
    QTranslator *trApp = new QTranslator(0);
    trApp->load("RuleChecker_ja_JP.qm",".");
    app.installTranslator(trApp);

    BrmsAdaptor brms;

    qmlRegisterUncreatableType<BrmsAdaptor>("hmiapp", 1, 0, "BrmsAdaptor", "BrmsAdaptor: Cannot be created in QML");
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("brms", &brms);
    engine.load(QUrl(QStringLiteral("qrc:/qml/profile01SuddenAccel.qml")));
    brms.init(argc, argv);
    brms.start();

    return app.exec();
}
void loadDefaultFont()
{
    bool found = false;

    //std::cout.flush();
	QString defaultFontPath = QDir::cleanPath("../share/hmi/fonts/vl-gothic-regular_1.800.ttf");
    if(!defaultFontPath.isEmpty())
    {
        int fId = QFontDatabase::addApplicationFont(defaultFontPath);
        QStringList availableFamilies = QFontDatabase::applicationFontFamilies(fId);
       // std::cout << "Loading default application font from: " << qPrintable(defaultFontPath) << std::endl;
       // std::cout << "Available font families: " << std::endl;
        for(int i = 0; i < availableFamilies.count(); i++)
        {
            //std::cout << "\t" << qPrintable(availableFamilies.at(i)) << std::endl;
        }
        if(availableFamilies.count() > 0)
        {
            found = true;
            QFont dFont(availableFamilies.at(0));
            QGuiApplication::setFont(dFont);
        }
        else
        {
           // std::cout << "Unable to find any font family in the specified file" << std::endl;
        }
    }

    if(!found)
    {
      //std::cout << "Default application font was not specified, system default will be used" << std::endl;
   }
  // std::cout<< "Default application font: " << qPrintable(QGuiApplication::font().family()) << std::endl;
}
