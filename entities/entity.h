/*Auteur: PERCIE DU SERT Maxime
 *Date: 02/08/2018
 */
#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <stdexcept>
#include "attribut_multiple.h"
#include "info_entity.h"

// Macro pour les entités
/*! \defgroup groupe_macro_entity Macro de base des entités
 *\ingroup groupe_entity
 *\brief Ensemble des Macros des entités.
 */

/*! \ingroup groupe_macro_entity
 *\brief Macro définissant les menbre static d'une entité.
 */
#define MEMBRE_STATIC(ENTITY) /*! \brief Renvoie le nom de l'entité.*/ \
    QString name() const override {return Name();} \
    /*! \brief Renvoie le nom de l'entité.*/ \
    static QString Name() {return QString(#ENTITY);} \
    using eaid::Verif_entity; \
    /*! Convertit la référence entity en une référence de type ENTITY, aprés vérification.*/ \
    static ENTITY *Convert(entity *ent) { \
        if(Verif_entity(ent)) return static_cast<ENTITY*>(ent); \
        else throw mps::entities::convert_entity_exception(QString("static ").append(Name()) \
                                                       .append(" *Convert(entity *)"),*ent,Name());} \
    /*! Convertit la référence constante entity en une référence constante de type ENTITY, aprés vérification.*/ \
    static ENTITY &Convert(entity &ent) { \
        if(Verif_entity(ent)) return static_cast<ENTITY &>(ent); \
        else throw mps::entities::convert_entity_exception(QString("static ").append(Name()) \
                                                       .append(" &Convert(entity &)"),ent,Name());} \
    /*! Convertit le pointeur entity en un pointeur de type ENTITY, aprés vérification.*/ \
    static const ENTITY &Convert(const entity &ent) { \
        if(Verif_entity(ent)) return  static_cast<const ENTITY &>(ent); \
        else throw mps::entities::convert_entity_exception(QString("static const ").append(Name()) \
                                                       .append(" &Convert(const entity &)"),ent,Name());}

/*! \ingroup groupe_macro_entity
 *\brief Macro a placer au début de la déclaration d'une entité.
 */
#define BASE_ENTITY(ENTITY) \
    CONSTR_DEFAUT(ENTITY) \
    CONSTR_AFFECT_DEFAUT(ENTITY) \
    ~ENTITY() override; \
    MEMBRE_STATIC(ENTITY)

/*! \ingroup groupe_macro_entity
 *\brief Macro a placer au début de la déclaration d'une entité abstraite.
 */
#define BASE_ENTITY_ABS(ENTITY) \
    CONSTR_DEFAUT(ENTITY) \
    CONSTR_AFFECT_DEFAUT(ENTITY) \
    ~ENTITY() override {}

namespace mps {
namespace entities {
/*! \ingroup groupe_entity
 *\brief Classe abstraite de base des entités.
 *
 *Lorsque l'on crée une nouvelle entité il faut :
 * - créer une classe fille de entity (ou créer un synonyme d'une classe prédéfinie à l'étape suivante).
 * - Implémenter les différents constructeurs.
 * - Placer la macro BASE_ENTITY(nom de la classe).
 * - Créer si nécessaire une classe Unique Sql pour l'entité.
 * - Rentrer les informations SQL pour faire le lien avec les entités correspondantes dans la base de donnée.
 * - Ajouter un manager de l'entité dans Managers (membre, def_get (dans .h) et manager_tab (dans .cpp)).
 */
class entity : public attribut::id_attribut {
public:
    //! position des attributs.
    enum position {Id, Nbr_Att};
    BASE_ENTITY_ABS(entity)

    //! Renvoie une chaine de caractère contenant les noms, les validités et les valeurs des attributs.
    QString affiche() const override;

    //! Retourne un QVariant contenant la donnée souhaitée ou un QVariant nulle si num est invalide.
    virtual QVariant data(post pos) const
        {return pos == Id ? id() : QVariant();}

    //! Teste si l'entité est nouvelle ou si elle provient de la base de donnée en regardant si elle poséde un identifiant non-nul.
    bool is_new() const
        {return id() == 0;}

    //! Renvoie l'identifiant du type de l'entité.
    virtual entidt id_entity() const = 0;

    //! Renvoie le nom de l'entité.
    virtual QString name() const = 0;

    //! Retourne un le nom de l'attribut d'indice num.
    virtual QString attribut_name(post pos) const = 0;

    //! Renvoie le nombre d'attribut de l'entité.
    virtual post nbr_att() const = 0;

    //! Mutateur de l'attribut d'indice i.
    virtual void set_data(post i, const QVariant &value) = 0;

    //! Mutateur de l'attribut de nom attr.
    virtual bool hydrate(const QString &attr, const QVariant &value);

    //! Mutateur de l'attribut de nom attr.
    virtual bool hydrate(std::vector<std::pair<QString,QVariant>> &values);

    //! Modifient les valeurs des attributs de l'entité avec celles des attributs de ent.
    virtual void operator << (const entity &ent) = 0;

    //! Opérateur testant l'égalité de deux entités, c'est-à-dire l'égalité de tous les attributs.
    virtual bool operator == (const entity &ent) const
        {return id() == ent.id();}

    //! Retourne la position de l'attribut de nom attr (retourne Nbr_Att si attr est nom de aucun attribut.
    post position(const QString &attr) const;

protected:
    //! Remplace les attributs de l'entité par celle de l'entité transmise, sauf l'identifiant.
    void set(const entity &ent)
        {set_id(ent.id());}
};

class entity_exception : public std::exception {
protected:
    QString m_txt;
    mutable std::string m_string;
public:
    //! Constructeur par default.
    entity_exception() = default;

    //! Constructeur.
    entity_exception(const QString &txt, const entity &ent)
        : m_txt(txt) {
        m_txt.append("\nErreur sur l'entité :\n");
        m_txt.append(ent.affiche());
    }

    //!Destructeur.
    ~entity_exception() override;

    //! Renvoie un pointeur sur la chaine expliquant l'exception.
    const char *what() const noexcept override {
        m_string = m_txt.toStdString();
        return m_string.data();
    }
};

class convert_entity_exception : public entity_exception {
public:
    //! Constructeur par default.
    convert_entity_exception() = default;

    //! Constructeur.
    convert_entity_exception(const QString &txt, const entity &ent, const QString &name) {
        m_txt = txt;
        m_txt.append("\nErreur de conversion d'entité:\n");
        m_txt.append("Une entrée de type ").append(ent.name()).append(" ne peut-être convertie en ").append(name).append(".\n");
        m_txt.append("Tentative de conversion de l'entrée:\n");
        m_txt.append(ent.affiche());
    }

    //!Destructeur.
    ~convert_entity_exception();
};

class invalide_entity_exception : public entity_exception {
public:
    //! Constructeur par default.
    invalide_entity_exception() = default;

    //! Constructeur.
    invalide_entity_exception(const QString &txt, const entity &ent) {
        m_txt = txt;
        m_txt.append("\nErreur de validité de l'entrée:\n");
        m_txt.append(ent.affiche());
    }

    //!Destructeur.
    ~invalide_entity_exception();
};

/*! \ingroup groupe_entity
 *\brief Classe abstraite de base des entités contenant les attributs.
 */
template <class Attribut> class entity_attributs : public attributs<entity,Attribut> {
public:
    enum {Nbr_Att = Attribut::Nbr_Att + entity::Nbr_Att,      //!< Nombre d'attribut de l'entité.
         };

    using attributs<entity,Attribut>::set_id;
    CONSTR_AFFECT_DEFAUT(entity_attributs)

    //!Constructeur.
    entity_attributs<Attribut>(idt id = 0)
        {set_id(id);}

    //! Renvoie le nombre d'attribut de l'entité.
    post nbr_att() const override
        {return Nbr_Att;}

    //! Retourne le nom de l'attribut d'indice num.
    QString attribut_name(post pos) const override
        {return pos < Nbr_Att ? attributs<entity,Attribut>::multiple_nom(pos) : QString();}

    //! Retourne un vecteur contenant les noms des attributs.
    static std::vector<QString> attribut_names() {
        std::vector<QString> vect(Nbr_Att);
        for (post i =0; i < Nbr_Att; ++i)
            vect[i] = attributs<entity,Attribut>::attribut_name(i);
        return vect;
    }

    //! Retourne un le nom de l'attribut d'indice num.
    static QString Name(post pos)
        {return pos < Nbr_Att ? attributs<entity,Attribut>::Name(pos) : QString();}

    //! Retourne un vecteur contenant les noms des attributs.
    static std::vector<QString> Names() {
        std::vector<QString> vect(Nbr_Att);
        for (post i =0; i < Nbr_Att; ++i)
            vect[i] = attributs<entity,Attribut>::Name(i);
        return vect;
    }

    //! Mutateur de l'attribut d'indice i.
    void set_data(post i, const QVariant &value) {
        if(i < Nbr_Att)
            set_data_v(i, value);
    }

    //! Modifient les valeurs des attributs de l'entité avec celles des attributs de ent.
    void operator << (const entity_attributs<Attribut> &ent)
        {set(ent);}

protected:
    using attributs<entity,Attribut>::set_data_v;
};

/*! \ingroup groupe_entity
 *\brief Classe abstraite de base des entités implémenté les membres utilisant l'identifiant du type de l'entité.
 */
template <entidt IDM, class Attribut> class entity_id : public entity_attributs<Attribut> {
public:
    enum {ID = IDM,         //!< Identifiant de l'attribut.
         };

    using ent_att = entity_attributs<Attribut>;
    using entity_attributs<Attribut>::entity_attributs;
    using ent_att::name;
    using ent_att::operator ==;

protected:
    using ent_att::set;

public:
    CONSTR_DEFAUT(entity_id)
    CONSTR_AFFECT_DEFAUT(entity_id)

    //! Convertit la référence entity en une référence de type ENTITY, aprés vérification.
    static entity_id<IDM,Attribut> *Convert(entity *ent) {
        if(Verif_entity(ent))
            return static_cast<entity_id<IDM,Attribut>*>(ent);
        else
            throw convert_entity_exception(QString("static entity_id<IDM,Attribut> *Convert(entity *)"),*ent,
                                         QString("entity_id<").append(QString::number(IDM)).append(",Attribut>"));
    }

    //! Convertit la référence constante entity en une référence constante de type ENTITY, aprés vérification.
    static entity_id<IDM,Attribut> &Convert(entity &ent) {
        if(Verif_entity(ent))
            return *(static_cast<entity_id<IDM,Attribut>*>(&ent));
        else
            throw convert_entity_exception(QString("static entity_id<IDM,Attribut> &Convert(entity &)"),ent,
                                         QString("entity_id<").append(QString::number(IDM)).append(",Attribut>"));
    }

    //*! Convertit le pointeur entity en un pointeur de type ENTITY, aprés vérification.
    static const entity_id<IDM,Attribut> &Convert(const entity &ent) {
        if(Verif_entity(ent))
            return  *(static_cast<const entity_id<IDM,Attribut>*>(&ent));
        else
            throw convert_entity_exception(QString("static const entity_id<IDM,Attribut> &Convert(const entity &)"),ent,
                                         QString("entity_id<").append(QString::number(IDM)).append(",Attribut>"));;
    }

    //! Renvoie l'identifiant du type de l'entité.
    entidt id_entity() const override
        {return ID;}

    //! Test si le pointeur entity est aussi un pointeur de ce type d'entité.
    static bool Verif_entity(entity *ent)
        {return ID == ent->id_entity();}

    //! Test si la référence entity est aussi une référence de ce type d'entité.
    static bool Verif_entity(const entity &ent)
        {return ID == ent.id_entity();}

    //! Opérateur d'égalité.
    bool operator == (const entity &ent) const
        {return Verif_entity(ent) &&entity_attributs<Attribut>::operator == (Convert(ent));}

    //! Opérateur de différence.
    bool operator != (const entity &ent) const
        {return ! operator== (ent);}

    //! Modifient les valeurs des attributs de l'entité avec celles des attributs de entity
    //! qui doit être de même type que la première opérande.
    void operator << (const entity &ent)
        {set(Convert(ent));}
};

template <entidt IDM, class... Attribut> using entity_ids = entity_id<IDM,attributs<Attribut...>>;
}}
#endif // ENTITY_H
