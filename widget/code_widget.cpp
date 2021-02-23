#include "code_widget.h"

using namespace mps;
using namespace widget;

code_widget::option_case code_widget::case_style(style_case sc, flag code) {
    widget::code_widget::option_case option;
    option.code = code;
    option.background_true = QColor(Qt::green);
    option.background_false = QColor(Qt::red);
    option.foreground_true = QColor(Qt::black);
    option.foreground_false = QColor(Qt::white);
    option.foregroundRead_true = QColor(Qt::darkGray);
    option.foregroundRead_false = QColor(Qt::lightGray);
    option.font_true.setFamily("FontAwesome");
    option.font_false.setFamily("FontAwesome");
    option.font_true.setPointSize(Font_Size);
    option.font_false.setPointSize(Font_Size);
    switch (sc) {
    case Attribuable:
        option.text_true = "\uf00c";
        option.text_false = "\uf00d";
        break;
    case Visible:
        option.text_true = "\uf06e";
        option.text_false = "\uf070";
        break;
    }
    return option;
}

code_widget::code_widget(const vec_option_case &cases, int cote, QWidget *parent)
    : QWidget(parent), m_cote(cote), m_cases(cases) {
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}

void code_widget::mousePressEvent(QMouseEvent *event) {
    if(!m_read_only &&event->button() == Qt::LeftButton)
        set_code(m_code ^ m_cases.at(static_cast<szt>(event->x()/m_cote)).code);
}

void code_widget::paintEvent(QPaintEvent */*event*/) {
    QPainter painter(this);
    QPen pen;
    pen.setColor(QGuiApplication::palette().color(QPalette::Active,QPalette::WindowText));
    pen.setWidth(2);
    QPen pen_text;
    QRect rect(0,0,m_cote,m_cote);
    for(auto iter = m_cases.cbegin(); iter != m_cases.cend(); ++iter, rect.translate(m_cote,0)) {
        painter.setPen(pen);
        if(m_code.test(iter->code)) {
            painter.setBrush(iter->background_true);
            painter.drawRoundedRect(rect,25.0,25.0,Qt::RelativeSize);
            pen_text.setBrush(m_read_only ? iter->foregroundRead_true : iter->foreground_true);
            painter.setPen(pen_text);
            painter.setFont(iter->font_true);
            painter.drawText(rect,Qt::AlignCenter,iter->text_true);
        }
        else {
            painter.setBrush(iter->background_false);
            painter.drawRoundedRect(rect,25.0,25.0,Qt::RelativeSize);
            pen_text.setBrush(m_read_only ? iter->foregroundRead_false : iter->foreground_false);
            painter.setPen(pen_text);
            painter.setFont(iter->font_false);
            painter.drawText(rect,Qt::AlignCenter,iter->text_false);
        }
    }
}

void code_widget::set_code(flag code) {
    m_code = code;
    update();
    emit code_changed(m_code);
}
