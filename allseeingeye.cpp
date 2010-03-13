// AllSeeingEye Copyright 2010 Ben Karel <eschew@gmail.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "allseeingeye.h"
#include "ui_allseeingeye.h"

#include <QFile>
#include <QFileDialog>
#include <QImage>
#include <QByteArray>
#include <QTextStream>
#include <QThread>

#include <iostream>

AllSeeingEye::AllSeeingEye(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AllSeeingEye)
{
    ui->setupUi(this);

    connect(&watcher, SIGNAL(watchedFileChanged()), this, SLOT(watchedFileChanged()));
}

AllSeeingEye::~AllSeeingEye() {
    delete ui;
}

void AllSeeingEye::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void AllSeeingEye::on_pushButton_clicked()
{
    QFileDialog::Options options;
    QString dir;
    QString path = QFileDialog::getOpenFileName(ui->pushButton,
                                                "Choose File to Watch",
                                                dir,
                                                "All Files (*)",
                                                NULL,
                                                options);
    setWatchedPath(path);
}

void AllSeeingEye::setWatchedPath(QString path) {
    watcher.setWatchedPath(path);
    QMainWindow::setWindowTitle("AllSeeingEye watching " + path);
}

void AllSeeingEye::watchedFileChanged() {
    // We update after a short delay because, on Win32 with large files,
    // we can sometimes get a fileChanged event before the app that modified
    // the file we're interested in has a chance to close the file. Thus,
    // without the delay, we would be unable to open large, just-updated files.
    QTimer::singleShot(200, this, SLOT(updateViewWithContents()));
}

void AllSeeingEye::updateViewWithContents() {
    QImage img(watcher.getWatchedPath());
    bool isValidImage = img.valid(QPoint(0,0));

    if (isValidImage) {
        ui->tabWidget->setCurrentIndex(1);
        ui->imageLabel->setPixmap(QPixmap::fromImage(img));
    } else {
        ui->tabWidget->setCurrentIndex(0);
        QFile file(watcher.getWatchedPath());

        if (!file.exists()) {
            ui->plainTextEdit->setPlainText("");
            return;
        } else if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            ui->plainTextEdit->setPlainText("Error! File not readable!");
            return;
        }
        QTextStream in(&file);

        in.setCodec("UTF-8");
        in.setAutoDetectUnicode(true);

        ui->plainTextEdit->setPlainText(in.readAll());

        file.close();
    }
}
