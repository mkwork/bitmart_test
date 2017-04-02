#include "ImagesLoader.h"
#include <QMutexLocker>
#include <QDir>
#include <QPixmap>

namespace {
static const char* FILTERS="*.png;*.jpg;*.jpeg";
}


ImagesLoader::ImagesLoader(QObject *parent) : QObject(parent),
    load_(false), cancel_(false), stop_(false)
{
    setAutoDelete(false);
}

ImagesLoader::~ImagesLoader()
{
    stop();
}

void ImagesLoader::run()
{
    forever {
        QMutexLocker l_(&lock_);

        while(paths_.empty() && !load_ && !stop_)
            event_.wait(&lock_);;

        if (stop_) break;

        if (cancel_) {
            paths_.clear();
        }

        if (load_) {
            processLoad();
        }

        if (!paths_.empty()) {
            processPath(paths_.dequeue());
            if (paths_.empty())
                emit finished();
        }

    }
}

void ImagesLoader::load(const QString &path)
{
    QMutexLocker l_(&lock_);
    root_ = path;
    load_ = true;
    event_.wakeAll();
}

void ImagesLoader::cancel()
{
    QMutexLocker l_(&lock_);
    cancel_ = true;
    event_.wakeAll();
}

void ImagesLoader::stop()
{
    QMutexLocker l_(&lock_);
    stop_ = true;
    event_.wakeAll();
}

void ImagesLoader::addPath(const QString &path)
{
    paths_.append(path);
}

void ImagesLoader::processLoad()
{
    QDir dir(root_);
    QStringList filters = QString(FILTERS).split(';');
    dir.setNameFilters(filters);
    QStringList entries = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);

    emit count(entries.count());

    foreach (QString entry , entries) {
        addPath(root_ + "/" + entry);
    }

    load_ = false;
    cancel_ = false;
}

void ImagesLoader::processPath(const QString &path)
{
    QPixmap pixmap(path);
    emit ready(pixmap);
}
