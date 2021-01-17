#include "CodeWidget.h"

using namespace widgetMPS;

CodeWidget::CaseOption CodeWidget::caseStyle(styleCase sc, flag code) {
    widgetMPS::CodeWidget::CaseOption caseOption;
    caseOption.code = code;
    caseOption.backgroundTrue = QColor(Qt::green);
    caseOption.backgroundFalse = QColor(Qt::red);
    caseOption.foregroundTrue = QColor(Qt::black);
    caseOption.foregroundFalse = QColor(Qt::white);
    caseOption.foregroundReadTrue = QColor(Qt::darkGray);
    caseOption.foregroundReadFalse = QColor(Qt::lightGray);
    caseOption.fontTrue.setFamily("FontAwesome");
    caseOption.fontFalse.setFamily("FontAwesome");
    caseOption.fontTrue.setPointSize(FontSize);
    caseOption.fontFalse.setPointSize(FontSize);
    switch (sc) {
    case Attribuable:
        caseOption.textTrue = "\uf00c";
        caseOption.textFalse = "\uf00d";
        break;
    case Visible:
        caseOption.textTrue = "\uf06e";
        caseOption.textFalse = "\uf070";
        break;
    }
    return caseOption;
}

CodeWidget::CodeWidget(const Cases & cases, int cote, QWidget * parent)
    : QWidget(parent), m_cote(cote), m_cases(cases) {
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}

void CodeWidget::mousePressEvent(QMouseEvent *event) {
    if(!m_readOnly && event->button() == Qt::LeftButton)
        setCode(m_code ^ m_cases.at(static_cast<szt>(event->x()/m_cote)).code);
}

void CodeWidget::paintEvent(QPaintEvent * /*event*/) {
    QPainter painter(this);
    QPen pen;
    pen.setColor(QGuiApplication::palette().color(QPalette::Active,QPalette::WindowText));
    pen.setWidth(2);
    QPen penText;
    QRect rect(0,0,m_cote,m_cote);
    for(auto iter = m_cases.cbegin(); iter != m_cases.cend(); ++iter, rect.translate(m_cote,0)) {
        painter.setPen(pen);
        if(m_code.test(iter->code)) {
            painter.setBrush(iter->backgroundTrue);
            painter.drawRoundedRect(rect,25.0,25.0,Qt::RelativeSize);
            penText.setBrush(m_readOnly ? iter->foregroundReadTrue : iter->foregroundTrue);
            painter.setPen(penText);
            painter.setFont(iter->fontTrue);
            painter.drawText(rect,Qt::AlignCenter,iter->textTrue);
        }
        else {
            painter.setBrush(iter->backgroundFalse);
            painter.drawRoundedRect(rect,25.0,25.0,Qt::RelativeSize);
            penText.setBrush(m_readOnly ? iter->foregroundReadFalse : iter->foregroundFalse);
            painter.setPen(penText);
            painter.setFont(iter->fontFalse);
            painter.drawText(rect,Qt::AlignCenter,iter->textFalse);
        }
    }
}

void CodeWidget::setCode(flag code) {
    m_code = code;
    update();
    emit codeChanged(m_code);
}
