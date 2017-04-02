#ifndef IMAGESLOADER_H
#define IMAGESLOADER_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <QPixmap>
#include <QRunnable>

class ImagesLoader : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit ImagesLoader(QObject *parent = 0);
    ~ImagesLoader();
    void run();

signals:
    void count(int count);
    void ready(QPixmap pixmap);
    void finished();
public slots:
    void load(const QString& path);
    void cancel();
    void stop();

private:
    void addPath(const QString& path);
    void processLoad();
    void processPath(const QString& path);

    bool load_;
    bool cancel_;
    bool stop_;

    QString root_;
    QQueue<QString> paths_;
    QMutex lock_;
    QWaitCondition event_;

};

#endif // IMAGESLOADER_H
