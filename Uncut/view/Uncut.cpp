#include "Uncut.h"

#include "library/LibraryView.h"
#include <qtimer.h>


Uncut::Uncut(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
    libModel = new LibModel();
    LibraryView* libView = ui.libFrame->findChild<LibraryView*>("libView");
    libView->setModel(libModel);

    libController = new LibController(libModel);

    QObject::connect(ui.actionimport, &QAction::triggered, libController, &LibController::onItemImport);
    QObject::connect(libView, &LibraryView::filesDropped, libController, &LibController::onFilesDropped);

    QTimer::singleShot(0, this, SLOT(onLoad()));
}

Uncut::~Uncut()
{
    delete libModel;
    delete libController;
}

void Uncut::onLoad()
{
    /*QImage img;
    img.load("res/einBild.jpg");
    for (int i = 0; i < 100; ++i) {
        libModel->addItem(new LibItemData(img, "hullo"));
    }*/
}

