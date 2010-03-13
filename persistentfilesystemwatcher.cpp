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

#include "persistentfilesystemwatcher.h"

#include <QFile>
#include <QStringList>

PersistentFileSystemWatcher::PersistentFileSystemWatcher()
{
    setPollInterval(1000);

    connect(&fswatcher, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));    
    connect(&fstimer, SIGNAL(timeout()), this, SLOT(on_pollFilePath()));
}

bool fileExists(const QString& path) {
    QFile f(path);
    return f.exists();
}

void PersistentFileSystemWatcher::setPollInterval(int ms) {
    this->fsPollInterval = ms;
}

QString PersistentFileSystemWatcher::getWatchedPath() {
    return filepath;
}

void PersistentFileSystemWatcher::setWatchedPath(const QString& path) {
    if (path.isEmpty()) { return; }

    // We only want to watch one file at a time.
    if (!this->filepath.isEmpty()) {
        this->fswatcher.removePath(filepath);
    }

    this->filepath = path;
    fswatcher.addPath(filepath);

    // Pretend the file changed so clients will immediately handle this file
    fileChanged(filepath);
}

void PersistentFileSystemWatcher::fileChanged(const QString& path) {
    if (fileExists(path)) {
        // The file system watcher seems to randomly drop paths
        // after one or two fileChanged events. Oh well.
        if (fswatcher.files().isEmpty()) {
            fswatcher.addPath(path);
        }

        emit watchedFileChanged();
    } else {
        // The file was deleted, and fswatcher will no longer watch the path.
        // Instead, we'll switch to polling the file until it reappears.
        emit watchedFileDeleted();
        emit watchedFileChanged();

        fstimer.start(this->fsPollInterval);
    }
}

void PersistentFileSystemWatcher::on_pollFilePath() {
    if (fileExists(this->filepath)) {
        fstimer.stop();
        emit watchedFileCreated();
        // Will in turn emit watchedFileChanged
        fileChanged(this->filepath);
    }
}
