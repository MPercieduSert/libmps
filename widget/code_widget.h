/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/12/2020
 */
#ifndef CODE_WIDGET_H
#define CODE_WIDGET_H

#include <QGuiApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include "type_mps.h"

namespace mps {
namespace widget {
/*! \ingroup groupe_Widget
 *\brief Widget de visialisation et de modifiaction d'un code.
 */
class code_widget : public QWidget {
    Q_OBJECT
public:
    //! Options de représentation d'une case.
    struct option_case {
        flag code;                      //! Code la case
        QBrush background_false;
        QBrush background_true;
        QBrush foreground_false;
        QBrush foreground_true;
        QBrush foregroundRead_false;
        QBrush foregroundRead_true;
        QFont font_false;
        QFont font_true;
        QString text_false;
        QString text_true;};
    //! Options de représentation d'un drapeaux.
    struct option_code {
        std::vector<option_case> cases;                   //!< Référence au mode de représantation d'un code.
        std::function<flag(flag, flag)> transition;       //!< Fonction de transition de code.
    };
    //! Style des cases préconfigurer.
    enum style_case {
        Visible,
        Attribuable};
protected:
    int m_cote;                           //!< Taille des coté des carrés.
    flag m_code;                          //!< Le code représenté.
    const option_code & m_option;         //!< Options de représentation du code.
    bool m_read_only = false;             //!< Le widget est-il en lecture seule.

public:
    //! Taille.
    enum {Cote_Size = 25, Font_Size = 12};
    //! Constructeur.
    code_widget(const option_code &option, int cote = Cote_Size, QWidget *parent = nullptr);

    //! Style d'une case.
    static option_case case_style(style_case sc, flag code);

    //! Accesseur du code.
    flag code() const noexcept
        {return m_code;}

    //! Accesseur de read_only.
    bool read_only() const noexcept
        {return  m_read_only;}

    //! Mutateur du code.
    void set_code(flag code);

    //! Mutateur de la cote.
    void set_cote(int cote) {
        m_cote = cote;
        adjustSize();
    }

    //! Accesseur de read_only.
    void set_read_only(bool bb) noexcept {
        m_read_only = bb;
        repaint();
    }

    //! Taille du widget
    QSize sizeHint() const override
        {return QSize(m_cote *static_cast<int>(m_option.cases.size()),m_cote);}
signals:
    //! Signal emit lors du changment de code.
    void code_changed(mps::flag code);
protected:
    //! Gestionnaire de click de souris.
    void mousePressEvent(QMouseEvent *event) override;

    //! Dessine le widget.
    void paintEvent(QPaintEvent *event) override;
};
}}
#endif // CODE_WIDGET_H
