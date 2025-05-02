#include "progressmessagebox.h"
// #include "utils.h"

ProgressMessageBox::ProgressMessageBox(const QString& title, const QStringList& barLabels)
    : QDialog(), m_checkPixmap(loadSvgIcon(":/icons/check.svg")), m_layout(new QVBoxLayout)
{
    this -> setWindowTitle(title);

    setLayout(m_layout);

    // For each requested label, create a row with QLabel + QProgressBar
    for (const QString& lbl : barLabels) {
        auto* row = new QWidget;
        auto* hbox = new QHBoxLayout(row);

        QLabel* label = new QLabel(lbl, row);
        QProgressBar* prog = new QProgressBar(row);
        prog->setFixedSize(300, 20);

        auto* spinner = new QQuickWidget(this);

        QSurfaceFormat fmt;
        fmt.setAlphaBufferSize(8);
        spinner->setFormat(fmt);
        spinner->setClearColor(Qt::transparent);
        spinner->setAttribute(Qt::WA_TranslucentBackground);
        spinner->setAttribute(Qt::WA_AlwaysStackOnTop);
        spinner->setResizeMode(QQuickWidget::SizeRootObjectToView);
        spinner->engine()->addImportPath("qrc:/icons");
        spinner->setSource(QUrl("qrc:/icons/spinner.qml"));
        spinner->setFixedSize(20, 20);
        spinner->setClearColor(Qt::transparent);

        auto* checkIcon = new QLabel();
        checkIcon->setFixedSize(20, 20);
        checkIcon->hide(); // start hidden

        hbox->addWidget(label);
        hbox->addWidget(prog);
        hbox->addWidget(spinner);
        hbox->addWidget(checkIcon);

        m_layout->addWidget(row);

        m_bars.append(prog);
        m_spinners.append(spinner);
        m_checks.append(checkIcon);
    }
}

void ProgressMessageBox::setProgress(int index, int value) {
    if (index < 0 || index >= m_bars.size()) return;

    m_bars[index]->setValue(value);

    if (value >= 100) {
        m_spinners[index]->hide();
        m_checks[index]->setPixmap(m_checkPixmap);
        m_checks[index]->show();
    } else {
        m_spinners[index]->show();
        m_checks[index]->hide();
    }
}

QPixmap ProgressMessageBox::loadSvgIcon(const QString &resourcePath) {
    QSvgRenderer renderer(resourcePath);
    QSize targetSize(24, 24);
    QPixmap pm(targetSize);
    pm.fill(Qt::transparent);

    QPainter p(&pm);
    // Render the SVG to fill the entire pixmap rect
    renderer.render(&p, pm.rect());

    qreal dpr = qApp->primaryScreen()->devicePixelRatio();
    pm.setDevicePixelRatio(dpr);

    return pm;
}
