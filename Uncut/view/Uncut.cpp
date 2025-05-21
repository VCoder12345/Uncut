#include "Uncut.h"

#include "library/LibraryView.h"
#include <qtimer.h>
#include <controller/preview/PreviewController.h>
#include "preview/PreviewView.h"


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
    QObject::connect(libView, &LibraryView::itemSelected, libController, &LibController::onItemSelected);

    previewController = new PreviewController(ui.previewFrame->findChild<PreviewView*>("previewView"));
    connect(libModel, &LibModel::itemSelected, previewController, &PreviewController::onLibItemSelected);
    QPushButton* playBtn = ui.previewFrame->findChild<QPushButton*>("playBtn");
    connect(playBtn, &QPushButton::clicked, previewController, &PreviewController::onPlayBtnClicked);

    QTimer::singleShot(0, this, SLOT(onLoad()));
}

Uncut::~Uncut()
{
    delete libModel;
    delete libController;
    delete previewController;
}

void Uncut::onLoad()
{
    libController->importItemFromFile("D:/Video-Schnitt/Schule/Inception_Filmmusik/MVI_0130.MP4");
    libController->importItemFromFile("D:/Video-Schnitt/Schule/Inception_Filmmusik/2022-06-05 17-06-42.mov");
    libController->importItemFromFile("D:/Video-Schnitt/Schule/Inception_Filmmusik/MVI_0150.MP4");

    /*QImage img;
    img.load("res/einBild.jpg");
    for (int i = 0; i < 100; ++i) {
        libModel->addItem(new LibItemData(img, "hullo"));
    }*/
}

