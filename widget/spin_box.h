/*Auteur: PERCIE DU SERT Maxime
 *Date: 11/10/2020
 */
#ifndef SPIN_BOX_H
#define SPIN_BOX_H

#include <cmath>
#include <QDate>
#include <QLineEdit>
#include <QList>
#include <QSpinBox>
#include "num_to_texte.h"

/*! \defgroup groupe_Widget Widgets
 *\brief Ensemble de widgets spécialisés.
 */
namespace mps {
/*! \ingroup groupe_Widget
 *\brief Espace de noms des widgets personnalisé.
 */
namespace widget {
/*! \ingroup groupe_Widget
 *\brief Spin Box pour les années scolaires.
 */
class spin_box_annee : public QAbstractSpinBox {
    Q_OBJECT
protected:
    int m_value;    //! Valeur courante
public:
    //! Constructeur.
    explicit spin_box_annee(int value = QDate::currentDate().year(), QWidget *parent = nullptr);

    //! Destructeur pr default.
    ~spin_box_annee() override = default;

    //! Retourne la valeur selectionnée.
    int value() const noexcept  {return m_value;}

    //! incrémente de steps fois la valeur.
    void stepBy(int steps) override;

protected:
    //! Autorise les variations à la hause et à la baisse.
    QAbstractSpinBox::StepEnabled stepEnabled() const noexcept override
        {return StepUpEnabled|StepDownEnabled;}

public slots:
    //! Affiche la valeur.
    void print_value();
};

/*! \ingroup groupe_Widget
 *\brief Spin Box affichant un multiple d'une décimale donné (inverse d'un entier fixé).
 */
class spin_box_decimale : public QSpinBox {
    Q_OBJECT
protected:
    int m_decimale = 1;                         //!< L'inverse de l'atome.
    int m_precision;                    //!< Nombre de chiffre après.
    QRegularExpressionValidator m_valide;   //!< Validateur de l'expression.
public:
    using QSpinBox::setValue;

    //! Constructeur.
    spin_box_decimale(QWidget *parent = nullptr)
        : QSpinBox(parent)
    {set_precision(1);}

    //! Constructeur.
    spin_box_decimale(int decimale, int precision = 1, QWidget *parent = nullptr)
        : QSpinBox(parent) {
        set_decimale(decimale);
        set_precision(precision);
    }

    //! Acceseur de l'inverse de l'atome.
    int decimale() const noexcept
        {return m_decimale;}

    //! Accesseur du maximum en double.
    double maximum_double() const
        {return static_cast<double>(maximum()) / m_decimale;}

    //! Acceseur du minimum en double.
    double minimum_double() const
        {return static_cast<double>(minimum()) / m_decimale;}

    //! Mutateur de l'inverse de l'atome.
    void set_decimale(int decimale) {
        if(decimale > 0) {
            auto val = static_cast<int>(std::lround(static_cast<double>(value()) / m_decimale *decimale));
            auto max = static_cast<int>(std::lround(static_cast<double>(maximum()) / m_decimale *decimale));
            m_decimale = decimale;
            setMaximum(max);
            setValue(val);
        }
        else
            throw std::invalid_argument("spin_box_decimale::set_decimale(int) : l'argument doit être strictement positif");
    }

    //! Mutateur du maximum en double.
    void set_maximum_double(double max)
        {setMaximum(static_cast<int>(std::lround(max *m_decimale)));}

    //! Mutateur du minimum en double.
    void set_minimum_double(double max)
        {setMinimum(static_cast<int>(std::lround(max *m_decimale)));}

    //! Mutateur de la présicion.
    void set_precision(int precision) {
        if(precision >= 0) {
            m_precision = precision;
            if(m_precision == 0)
                m_valide.setRegularExpression(QRegularExpression(QString("\\d*")));
            else
                m_valide.setRegularExpression(QRegularExpression(QString("\\d*.?\\d{0,")
                                                                 .append(QString::number(m_precision)).append("}")));
            lineEdit()->setText(textFromValue(value()));
        }
    }

    //! Mutateur de la valeur en double.
    void set_value_double(double value)
        {setValue(static_cast<int>(std::lround(value *m_decimale)));}

    //! Teste la validité d'un QString pour le style courant.
    QValidator::State validate(QString &input, int &pos) const override
        {return m_valide.validate(input,pos);}

    //! Accesseur de la valeur en double.
    double value_double() const
        {return static_cast<double>(value()) / m_decimale;}

protected:
    //! Réimplémentation de la manière d'afficher les valeur.
    QString textFromValue(int value) const override
        {return QString::number(static_cast<double>(value)/m_decimale,'f',m_precision);}

    //! Réimplémentation de la manière de lire les valeur.
    int valueFromText(const QString &text) const override
        {return static_cast<int>(std::lround(text.toDouble() *m_decimale));}
};

/*! \ingroup groupe_Widget
 *\brief Spin Box affichant un entier positif sous diférent style
 *(arabe, romain, lettre minuscule, majsucule, grec minuscule, grec Majuscule.
 */
class spin_box_lettre : public QSpinBox {
    Q_OBJECT
protected:
    divers::num_to_string m_numToText;  //!< Convertisseur.
public:
    //! Constructeur.
    spin_box_lettre(enumt style = divers::num_to_string::Arabe_Style, QWidget *parent = nullptr)
        : QSpinBox(parent), m_numToText(style) {
        setMinimum(0);
    }

    //! Mutateur du style d'affichage des numéros.
    void set_style(enumt style) {
        m_numToText.set_style(style);
        lineEdit()->setText(textFromValue(value()));
    }

    //! Acceseur du style d'affichage des numéros.
    enumt style() const noexcept
        {return m_numToText.style();}

    //! Teste la validité d'un QString pour le style courant.
    QValidator::State validate(QString &input, int &pos) const override
        {return m_numToText.validate(input,pos);}

protected:
    //! Réimplémentation de la manière d'afficher les valeur.
    QString textFromValue(int value) const override
        {return m_numToText.texte(value);}

    //! Réimplémentation de la manière de lire les valeur.
    int valueFromText(const QString &text) const override
        {return m_numToText.num(text);}
};

/*! \ingroup groupe_Widget
 *\brief Spin Box affichant des entier strictement positif n'étant pas dans la liste transmise.
 */
class spin_box_num_exclu : public QAbstractSpinBox {
    Q_OBJECT
protected:
    bool m_min_visible = false;  //!< Minimum visible.
    int m_minimum = -1;         //!< Valeur minimal.
    int m_offset = 0;           //!< Différence entre la valeur affichée et la valeur courante.
    int m_value = m_minimum;    //!< Valeur courante.
    std::list<int> m_list;      //!< Liste des valeurs excluses.

public:
    //! Constructeur.
    explicit spin_box_num_exclu(const std::list<int> &liste = std::list<int>(), QWidget *parent = nullptr);

    //! Destructeur par default.
    ~spin_box_num_exclu() override = default;

    //! Efface la liste des numéro exclus.
    void clear_list()
        {m_list.clear();}

    //! Accesseur de la lists des valeurs à exclure.
    const std::list<int> list() const
        {return m_list;}

    //! Accesseur du minimum.
    int minimum() const noexcept
        {return m_minimum;}

    //! Accesseur de la visibilité du minimum.
    bool minimum_visible() const noexcept
        {return m_min_visible;}

    //! Accesseur de l'offset.
    int offset() const noexcept
        {return m_offset;}

    //! Mutateur de la liste des valeurs à exclure.
    void set_list(const std::list<int> &list);

    //! Mutateur du minimum.
    void set_minimum(int min) {
        m_minimum = min;
        if(m_value < m_minimum)
            set_value(m_minimum);
    }

    //! Mutateur de la visibilité du minimum.
    void set_minimum_visible(bool bb){
        m_min_visible = bb;
        print_value();
    }

    //! Mutateur de l'offset.
    void set_offset(int off){
        m_offset = off;
        print_value();
    }

    //! Mutateur de la valeur courante.
    void set_value(int val);

    //! Méthode virtuelle d'incrémentation.
    void stepBy(int steps) override;

    //! Accesseur de la valeur courante.
    int value() const noexcept     {return m_value;}

protected:
    //! Méthode virtuelle des drapeaux.
    QAbstractSpinBox::StepEnabled stepEnabled() const override
        {return (StepUpEnabled| StepDownEnabled);}

signals:
    //! Signal de changement de la valeur courante.
    void value_changed(int newValue);

public slots:
    //! Slot d'affichage des valeurs.
    void print_value();
};
}}
#endif // SPIN_BOX_H
