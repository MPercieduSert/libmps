/*Auteur: PERCIE DU SERT Maxime
 *Date: 11/10/2020
 */
#ifndef SPINBOX_H
#define SPINBOX_H

#include <cmath>
#include <QDate>
#include <QLineEdit>
#include <QList>
#include <QSpinBox>
#include "NumToTexte.h"

/*! \defgroup groupeWidget Widgets
 * \brief Ensemble de widgets spécialisés.
 */

/*! \ingroup groupeWidget
 * \brief Espace de noms des widgets personnalisé.
 */
namespace widgetMPS {
using namespace typeMPS;
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
    int value() const noexcept  {return m_value;}

    //! incrémente de steps fois la valeur.
    void stepBy(int steps) override;

protected:
    QAbstractSpinBox::StepEnabled stepEnabled() const noexcept override;

public slots:
    //! Affiche la valeur.
    void printValue();
};

/*! \ingroup groupeWidget
 * \brief Spin Box affichant un multiple d'une décimale donné (inverse d'un entier fixé).
 */
class SpinBoxDecimale : public QSpinBox {
    Q_OBJECT
protected:
    int m_decimale = 1;                         //!< L'inverse de l'atome.
    int m_precision;                    //!< Nombre de chiffre après.
    QRegularExpressionValidator m_valide;   //!< Validateur de l'expression.
public:
    using QSpinBox::setValue;

    //! Constructeur.
    SpinBoxDecimale(QWidget * parent = nullptr)
        : QSpinBox(parent)
    {setPrecision(1);}

    //! Constructeur.
    SpinBoxDecimale(int decimale, int precision = 1, QWidget * parent = nullptr)
        : QSpinBox(parent) {
        setDecimale(decimale);
        setPrecision(precision);
    }

    //! Acceseur de l'inverse de l'atome.
    int decimale() const noexcept
        {return m_decimale;}

    //! Accesseur du maximum en double.
    double maximumDouble() const
        {return static_cast<double>(maximum()) / m_decimale;}

    //! Acceseur du minimum en double.
    double minimumDouble() const
        {return static_cast<double>(minimum()) / m_decimale;}

    //! Mutateur de l'inverse de l'atome.
    void setDecimale(int decimale) {
        if(decimale > 0) {
            auto val = static_cast<int>(std::lround(static_cast<double>(value()) / m_decimale * decimale));
            auto max = static_cast<int>(std::lround(static_cast<double>(maximum()) / m_decimale * decimale));
            m_decimale = decimale;
            setMaximum(max);
            setValue(val);
        }
        else
            throw std::invalid_argument("SpinBoxDecimale::setDecimale(int) : l'argument doit être strictement positif");
    }

    //! Mutateur du maximum en double.
    void setMaximumDouble(double max)
        {setMaximum(static_cast<int>(std::lround(max * m_decimale)));}

    //! Mutateur du minimum en double.
    void setMinimumDouble(double max)
        {setMinimum(static_cast<int>(std::lround(max * m_decimale)));}

    //! Mutateur de la présicion.
    void setPrecision(int precision) {
        if(precision >= 0) {
            m_precision = precision;
            if(m_precision == 0)
                m_valide.setRegularExpression(QRegularExpression(QString("\\d*")));
            else
                m_valide.setRegularExpression(QRegularExpression(QString("\\d*.?\\d{0,").append(QString::number(m_precision)).append("}")));
            lineEdit()->setText(textFromValue(value()));
        }
    }

    //! Mutateur de la valeur en double.
    void setValueDouble(double value)
        {setValue(static_cast<int>(std::lround(value * m_decimale)));}

    //! Teste la validité d'un QString pour le style courant.
    QValidator::State validate(QString &input, int & pos) const override
        {return m_valide.validate(input,pos);}

    //! Accesseur de la valeur en double.
    double valueDouble() const
        {return static_cast<double>(value()) / m_decimale;}

protected:
    //! Réimplémentation de la manière d'afficher les valeur.
    QString textFromValue(int value) const override
        {return QString::number(static_cast<double>(value)/m_decimale,'f',m_precision);}

    //! Réimplémentation de la manière de lire les valeur.
    int valueFromText(const QString & text) const override
        {return static_cast<int>(std::lround(text.toDouble() * m_decimale));}
};

/*! \ingroup groupeWidget
 * \brief Spin Box affichant un entier positif sous diférent style
 * (arabe, romain, lettre minuscule, majsucule, grec minuscule, grec Majuscule.
 */
class SpinBoxLettre : public QSpinBox {
    Q_OBJECT
protected:
    diversMPS::NumToTexte m_numToText;  //!< Convertisseur.
public:
    //! Constructeur.
    SpinBoxLettre(enumt style = diversMPS::NumToTexte::Arabe, QWidget * parent = nullptr)
        : QSpinBox(parent), m_numToText(style) {
        setMinimum(0);
    }

    //! Mutateur du style d'affichage des numéros.
    void setStyle(enumt style) {
        m_numToText.setStyle(style);
        lineEdit()->setText(textFromValue(value()));
    }

    //! Acceseur du style d'affichage des numéros.
    enumt style() const noexcept
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

/*! \ingroup groupeWidget
 * \brief Spin Box affichant des entier strictement positif n'étant pas dans la liste transmise.
 */
class SpinBoxNumExclu : public QAbstractSpinBox {
    Q_OBJECT
protected:
    bool m_minVisible = false;  //!< Minimum visible.
    int m_minimum = -1;         //!< Valeur minimal.
    int m_offset = 0;           //!< Différence entre la valeur affichée et la valeur courante.
    int m_value = m_minimum;    //!< Valeur courante.
    std::list<int> m_list;      //!< Liste des valeurs excluses.

public:
    //! Constructeur.
    explicit SpinBoxNumExclu(const std::list<int> &liste = std::list<int>(), QWidget *parent = nullptr);

    //! Destructeur par default.
    ~SpinBoxNumExclu() override = default;

    //! Efface la liste des numéro exclus.
    void clearList()
        {m_list.clear();}

    //! Accesseur de la lists des valeurs à exclure.
    const std::list<int> list() const
        {return m_list;}

    //! Accesseur du minimum.
    int minimum() const noexcept
        {return m_minimum;}

    //! Accesseur de la visibilité du minimum.
    bool minimumVisible() const noexcept
        {return m_minVisible;}

    //! Accesseur de l'offset.
    int offset() const noexcept
        {return m_offset;}

    //! Mutateur de la liste des valeurs à exclure.
    void setList(const std::list<int> &list);

    //! Mutateur du minimum.
    void setMinimum(int min) {
        m_minimum = min;
        if(m_value < m_minimum)
            setValue(m_minimum);
    }

    //! Mutateur de la visibilité du minimum.
    void setMinimumVisible(bool bb){
        m_minVisible = bb;
        printValue();
    }

    //! Mutateur de l'offset.
    void setOffset(int off){
        m_offset = off;
        printValue();
    }

    //! Mutateur de la valeur courante.
    void setValue(int val);

    //! Méthode virtuelle d'incrémentation.
    void stepBy(int steps) override;

    //! Accesseur de la valeur courante.
    int value() const noexcept     {return m_value;}

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
#endif // SPINBOX_H
