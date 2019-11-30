/*Auteur: PERCIE DU SERT Maxime
 *Date: 12/09/2018
 */
#ifndef SPINBOXNUMEXCLU_H
#define SPINBOXNUMEXCLU_H

#include <QAbstractSpinBox>
#include <QLineEdit>
#include <QList>

namespace widgetMPS {
/*! \ingroup groupeWidget
 * \brief Spin Box affichant des entier strictement positif n'étant pas dans la liste transmise.
 */
class SpinBoxNumExclu : public QAbstractSpinBox
{
    Q_OBJECT
protected:
    int m_value;            //!< Valeur courante.
    QList<int> m_liste;     //!< Liste des valeurs excluses.

public:
    //! Constructeur.
    explicit SpinBoxNumExclu(const QList<int> &liste = QList<int>(), QWidget *parent = nullptr);
    //! Destructeur par default.
    ~SpinBoxNumExclu() override = default;
    //! Mutateur de la liste des valeurs à exclure.
    void setListe(const QList<int> &liste);
    //! Méthode virtuelle d'incrémentation.
    void stepBy(int steps) override;
    //! Accesseur de la valeur courante.
    int value() const         {return m_value;}

protected:
    //! Méthode virtuelle des drapeaux.
    QAbstractSpinBox::StepEnabled stepEnabled() const override;

signals:
    //! Signal de changement de la valeur courante.
    void valueChanged(int newValue);

public slots:
    //! Slot d'affichage des valeurs.
    void printValue();
};
}
#endif // SPINBOXNUMEXCLU_H
