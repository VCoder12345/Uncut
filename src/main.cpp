#include "view/Uncut.h"
#include <QtWidgets/QApplication>
#include <model/library/LibModel.h>
#include <controller/library/LibController.h>
#include <controller/preview/PreviewController.h>
#include <controller/timeline/TlController.h>
#include <model/timeline/TlModel.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Uncut w;

    LibModel* libModel = new LibModel();
    LibController* libController = new LibController(w, libModel);
    PreviewController* previewController = new PreviewController(w, libModel);
    TlModel* tlModel = new TlModel();
    TlController* tlController = new TlController(w, tlModel);


    w.show();
    int ret = a.exec();
    qDebug() << "deleting models and controllers";
    delete libModel;
    delete libController;
    delete previewController;
    delete tlModel;
    delete tlController;

    return ret;
}
