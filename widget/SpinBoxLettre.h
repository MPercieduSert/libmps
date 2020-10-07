/*Auteur: PERCIE DU SERT Maxime
 *Date: 30/09/2020
 */
#ifndef SPINBOXLETTRE_H
#define SPINBOXLETTRE_H

#include <QLineEdit>
#include <QSpinBox>
#include "NumToTexte.h"

namespace widgetMPS {
/*! \ingroup groupeWidget
 * \brief Spin Box affichant des entier positif sous diférent style (arabe, romain, lettre minuscule, majsucule, grec minuscule, grec Majuscule.
 */
class SpinBoxLettre : public QSpinBox {
    Q_OBJECT
protected:
    diversMPS::NumToTexte m_numToText;  //!< Convertisseur.
public:
    //! Constructeur.
    SpinBoxLettre(unsigned style = diversMPS::NumToTexte::Arabe, QWidget * parent = nullptr)
        : QSpinBox(parent), m_numToText(style) {
        setMinimum(0);
    }

    //! Mutateur du style d'affichage des numéros.
    void setStyle(unsigned style) {
        m_numToText.setStyle(style);
        lineEdit()->setText(textFromValue(value()));
    }

    //! Acceseur du style d'affichage des numéros.
    unsigned style() const
        {return m_numToText.style();}

    //! Teste la validité d'un QString pour le style courant.
    QValidator::State validate(QString &input, int & pos) const override
        {return m_numToText.validate(input,pos);}

protected:
    //! Réimplémentation de la manière d'afficher les valeur.
    QString textFromValue(int value) const override
        {return m_numToText.texte(value);}

    //! Réimplémentation de la manière de lire les valeur.
    int valueFromText(const QString & text) const override
        {return m_numToText.num(text);}
};
}
#endif // SPINBOXLETTRE_H
