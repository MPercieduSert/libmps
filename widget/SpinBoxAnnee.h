/*Auteur: PERCIE DU SERT Maxime
 *Date: 11/09/2018
 */
#ifndef SPINBOXANNEE_H
#define SPINBOXANNEE_H

#include <QAbstractSpinBox>
#include <QDate>
#include <QLineEdit>

/*! \defgroup groupeWidget Widgets
 * \brief Ensemble de widgets spécialisés.
 */

/*! \ingroup groupeWidget
 * \brief Espace de noms des widgets personnalisé.
 */
namespace widgetMPS {

/*! \ingroup groupeWidget
 * \brief Spin Box pour les années scolaires.
 */
class SpinBoxAnnee : public QAbstractSpinBox
{
    Q_OBJECT
protected:
    int m_value;    //! Valeur courante

public:
    //! Constructeur.
    explicit SpinBoxAnnee(int value = QDate::currentDate().year(), QWidget *parent = nullptr);

    //! Destructeur pr default.
    ~SpinBoxAnnee() override = default;

    //! Retourne la valeur selectionnée.
    int value() const         {return m_value;}

    //! incrémente de steps fois la valeur.
    void stepBy(int steps) override;

protected:
    QAbstractSpinBox::StepEnabled stepEnabled() const override;

public slots:
    //! Affiche la valeur.
    void printValue();
};
}
#endif // SPINBOXANNEE_H
