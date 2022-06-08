#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QScopedPointer>
#include <poppler/qt5/poppler-qt5.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_selectPdfBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("PDF (*.pdf)"));
    if (!fileName.isEmpty()) {
        ui->PdfLineEdit->setText(fileName);
    }
}

void Widget::on_selectOutputBtn_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        ui->OutputLineEdit->setText(dir);
    }
}

static bool pdf2images(const QString &pdfFileName, const QString &imageDir, QString *err)
{
    QScopedPointer<Poppler::Document> doc(Poppler::Document::load(pdfFileName));
    if (!doc.get()) {
        if (err != nullptr) *err = "loading error";
        return false;
    }
    if (doc->isLocked()) {
        if (err != nullptr) *err = "encrypted document";
        return false;
    }
    doc->setRenderHint(Poppler::Document::Antialiasing);
    doc->setRenderHint(Poppler::Document::TextAntialiasing);
    int count = doc->numPages();
    for (int i = 0; i < count; ++i) {
        QScopedPointer<Poppler::Page> p(doc->page(i));
        if (!p.get()) {
            if (err != nullptr) *err = "NULL page";
            return false;
        }
        QImage img = p->renderToImage();
        if (img.isNull()) {
            if (err != nullptr) *err = "rendering failed";
            return false;
        }
        if (!img.save(imageDir + "/" + QString::number(i) + ".png", "png")) {
            if (err != nullptr) *err = "saving to file failed";
            return false;
        }
    }
    return true;
}

void Widget::on_convertBtn_clicked()
{
    QString error;
    bool ok = pdf2images(ui->PdfLineEdit->text(), ui->OutputLineEdit->text(), &error);
    if (ok) {
        QMessageBox::information(this, "INFO", "convert ok!");
    } else {
        QMessageBox::warning(this, "WARN", error);
    }
}

