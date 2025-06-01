#include "Uncut.h"

#include <qtimer.h>

Uncut::Uncut(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
    QTimer::singleShot(0, this, SLOT(onLoad()));
}

Uncut::~Uncut()
{
}

QPushButton* Uncut::getPlayBtn() {
return ui.previewFrame->findChild<QPushButton*>("playBtn");
}

LibraryView* Uncut::getLibView()
{
    return ui.libFrame->findChild<LibraryView*>("libView");
}

QAction* Uncut::getImportAction()
{
    return ui.actionimport;
}

PreviewView* Uncut::getPreviewView()
{
    return ui.previewFrame->findChild<PreviewView*>("previewView");
}

TlView* Uncut::getTlView()
{
    return ui.tlView;
}

void Uncut::onLoad()
{
   /* libController->importItemFromFile("D:/Video-Schnitt/Schule/Inception_Filmmusik/MVI_0130.MP4");
    libController->importItemFromFile("D:/Video-Schnitt/Schule/Inception_Filmmusik/2022-06-05 17-06-42.mov");
    libController->importItemFromFile("D:/Video-Schnitt/Schule/Inception_Filmmusik/MVI_0150.MP4");*/

    /*QImage img;
    img.load("res/einBild.jpg");
    for (int i = 0; i < 100; ++i) {
        libModel->addItem(new LibItemData(img, "hullo"));
    }*/
}

