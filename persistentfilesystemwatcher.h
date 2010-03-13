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

#ifndef PERSISTENTFILESYSTEMWATCHER_H
#define PERSISTENTFILESYSTEMWATCHER_H

#include <QString>
#include <QFileSystemWatcher>
#include <QTimer>

class PersistentFileSystemWatcher : public QObject
{   
    Q_OBJECT

public:
    PersistentFileSystemWatcher();
    void setWatchedPath(const QString& path);
    QString getWatchedPath();
    void setPollInterval(int ms); ///< defaults to 1 second

signals:
    void watchedFileChanged(); ///< Emitted after any change, including deletion
    void watchedFileDeleted(); ///< Emitted before watchedFileChanged() on deletion
    void watchedFileCreated(); ///< Emitted before watchedFileChanged() on creation

private slots:
    void on_pollFilePath();
    void fileChanged(const QString& path);

private:
    QString filepath;
    QFileSystemWatcher fswatcher;
    QTimer fstimer;
    int fsPollInterval;
 };

#endif // PERSISTENTFILESYSTEMWATCHER_H
