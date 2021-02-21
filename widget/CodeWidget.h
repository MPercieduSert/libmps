/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/12/2020
 */
#ifndef CODEWIDGET_H
#define CODEWIDGET_H

#include <QGuiApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include <type_mps.h>

namespace widgetMPS {
using namespace type_mps;
/*! \ingroup groupeWidget
 *\brief Widget de visialisation et de modifiaction d'un code.
 */
class CodeWidget : public QWidget {
    Q_OBJECT
public:
    //! Options de représentation.
    struct CaseOption {
        flag code;
        QBrush backgroundFalse;
        QBrush backgroundTrue;
        QBrush foregroundFalse;
        QBrush foregroundTrue;
        QBrush foregroundReadFalse;
        QBrush foregroundReadTrue;
        QFont fontFalse;
        QFont fontTrue;
        QString textFalse;
        QString textTrue;};
    //! Type de représentation d'un drapeaux.
    using Cases = std::vector<CaseOption>;
    //! Style des cases préconfigurer.
    enum styleCase {
        Visible,
        Attribuable};
protected:
    int m_cote;                     //!< Taille des coté des carrés.
    const Cases &m_cases;          //!< Référence au mode de représantation d'un code.
    flag m_code;                    //!< Le code représenté.
    bool m_readOnly = false;        //!< Le widget est-il en lecture seule.

public:
    //! Taille.
    enum {CoteSize = 25, FontSize = 12};
    //! Constructeur.
    CodeWidget(const Cases &cases, int cote = CoteSize, QWidget *parent = nullptr);

    //! Style d'une case.
    static CaseOption caseStyle(styleCase sc, flag code);

    //! Accesseur du code.
    flag code() const noexcept
        {return m_code;}

    //! Accesseur de readOnly.
    bool readOnly() const noexcept
        {return  m_readOnly;}

    //! Mutateur du code.
    void set_code(flag code);

    //! Mutateur de la cote.
    void setCote(int cote) {
        m_cote = cote;
        adjustSize();
    }

    //! Accesseur de readOnly.
    void setReadOnly(bool bb) noexcept {
        m_readOnly = bb;
        repaint();
    }

    //! Taille du widget
    QSize sizeHint() const override
        {return QSize(m_cote *static_cast<int>(m_cases.size()),m_cote);}
signals:
    //! Signal emit lors du changment de code.
    void codeChanged(flag code);
protected:
    //! Gestionnaire de click de souris.
    void mousePressEvent(QMouseEvent *event) override;

    //! Dessine le widget.
    void paintEvent(QPaintEvent *event) override;
};
}
#endif // CodeWIDGET_H
