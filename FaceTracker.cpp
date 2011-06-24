#include "FaceTracker.h"

#include <QtDebug>
#include <QImage>
#include <QMetaType>

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>

cv::Mat QImage2CvMat(const QImage &img)
{
    int cvType;

    switch (img.format()) {
    case QImage::Format_Indexed8:
        cvType = CV_8UC1;
        break;
    case QImage::Format_RGB888:
        cvType = CV_8UC3;
        break;
    default:
        qCritical() << "Error: unsupported conversion from QImage to cv::Mat";
        return cv::Mat();
    }

    // using this constructor:
    // Mat(int _rows, int _cols, int _type, void* _data, size_t _step=AUTO_STEP);
    return cv::Mat(img.height(), img.width(), cvType, (void*)img.bits(), img.bytesPerLine());
}

static bool s_greyTableInit = false;
static QVector<QRgb> s_greyTable;

QImage CvMat2QImage(const cv::Mat& cvmat)
{
    int height = cvmat.rows;
    int width = cvmat.cols;

    if (cvmat.depth() == CV_8U && cvmat.channels() == 3) {
        QImage img((const uchar*)cvmat.data, width, height, cvmat.step.p[0], QImage::Format_RGB888);
        return img.rgbSwapped();
    } else if (cvmat.depth() == CV_8U && cvmat.channels() == 1) {
        if (!s_greyTableInit) {
            for (int i = 0; i < 256; i++){
                s_greyTable.push_back(qRgb(i, i, i));
            }
        }
        QImage img((const uchar*)cvmat.data, width, height, cvmat.step.p[0], QImage::Format_Indexed8);
        img.setColorTable(s_greyTable);
        return img;
    } else {
        qWarning() << "Image cannot be converted.";
        return QImage();
    }
}

struct FaceTracker::Private {
    cv::CascadeClassifier * m_cvDetector;

    Private()
        : m_cvDetector(0)
    {}

    ~Private() {
        if (m_cvDetector)
            delete m_cvDetector;
    }
};

FaceTracker::FaceTracker(QObject * parent)
    : QObject(parent)
    , m_private( new Private )
{
    // set up the OpenCV detector
    m_private->m_cvDetector = new cv::CascadeClassifier;
    Q_ASSERT( m_private->m_cvDetector->load("lbpcascade_frontalface.xml") );
    qRegisterMetaType< QList<QRect> >("QList<QRect>");
}

FaceTracker::~FaceTracker() {
    delete m_private;
}

bool larger(const QRect& a, const QRect& b)
{
    return a.width()*a.height() > b.width()*b.height();
}

void FaceTracker::detect(const QImage& frame)
{
    QList<QRect> faces;
    // wrap the frame
    cv::Mat cvFrame0 = QImage2CvMat(frame), cvFrame;
    if (cvFrame0.channels() == 3) {
        // RGB -> grey scale
        cv::cvtColor(cvFrame0, cvFrame, CV_RGB2GRAY);
    } else {
        Q_ASSERT(cvFrame0.channels() == 1);
        cvFrame = cvFrame0;
    }

    // detect...
    std::vector<cv::Rect> rects;

    int maxSize = frame.height() * 2 / 3;

    m_private->m_cvDetector->detectMultiScale( cvFrame, rects, 1.1, 3,
                                    cv::CascadeClassifier::FIND_BIGGEST_OBJECT
                                    | cv::CascadeClassifier::DO_ROUGH_SEARCH
                                    | cv::CascadeClassifier::DO_CANNY_PRUNING,
                                    cv::Size(10, 10), cv::Size(maxSize, maxSize));
    // convert the results to qt rects
    foreach(cv::Rect r, rects)
        faces.push_back( QRect(r.x, r.y, r.width, r.height) );

    //qSort(faces.begin(), faces.end(), larger);
    QImage grey = CvMat2QImage(cvFrame);
    emit facesDetected(grey, faces);
}

