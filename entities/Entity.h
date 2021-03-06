/*Auteur: PERCIE DU SERT Maxime
 *Date: 02/08/2018
 */
#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <stdexcept>

#include "AttributMultiple.h"
#include "infoEntity.h"

// Macro pour les entités
/*! \defgroup groupeMacroEntity Macro de base des entités
 * \ingroup groupeEntity
 * \brief Ensemble des Macros des entités.
 */

/*! \ingroup groupeMacroEntity
 * \brief Macro définissant les menbre static d'une entité.
 */
#define MEMBRE_STATIC(ENTITY) /*! \brief Renvoie le nom de l'entité.*/ \
    QString name() const override {return Name();} \
    /*! \brief Renvoie le nom de l'entité.*/ \
    static QString Name() {return QString(#ENTITY);} \
    using EAID::VerifEntity; \
    /*! Convertit la référence entity en une référence de type ENTITY, aprés vérification.*/ \
    static ENTITY * Convert(Entity *entity) { \
        if(VerifEntity(entity)) return static_cast<ENTITY*>(entity); \
        else throw entityMPS::ConvertEntityException(QString("static ").append(Name()).append(" * Convert(Entity *)"),*entity,Name());} \
    /*! Convertit la référence constante entity en une référence constante de type ENTITY, aprés vérification.*/ \
    static ENTITY & Convert(Entity & entity) { \
        if(VerifEntity(entity)) return static_cast<ENTITY &>(entity); \
        else throw entityMPS::ConvertEntityException(QString("static ").append(Name()).append(" & Convert(Entity &)"),entity,Name());} \
    /*! Convertit le pointeur entity en un pointeur de type ENTITY, aprés vérification.*/ \
    static const ENTITY & Convert(const Entity & entity) { \
        if(VerifEntity(entity)) return  static_cast<const ENTITY &>(entity); \
        else throw entityMPS::ConvertEntityException(QString("static const ").append(Name()).append(" & Convert(const Entity &)"),entity,Name());}

/*! \ingroup groupeMacroEntity
 * \brief Macro a placer au début de la déclaration d'une entité.
 */
#define BASE_ENTITY(ENTITY) \
    CONSTR_DEFAUT(ENTITY) \
    CONSTR_AFFECT_DEFAUT(ENTITY) \
    ~ENTITY() override; \
    MEMBRE_STATIC(ENTITY)

/*! \ingroup groupeMacroEntity
 * \brief Macro a placer au début de la déclaration d'une entité abstraite.
 */
#define BASE_ENTITY_ABS(ENTITY) \
    CONSTR_DEFAUT(ENTITY) \
    CONSTR_AFFECT_DEFAUT(ENTITY) \
    ~ENTITY() override {}


namespace entityMPS {
/*! \ingroup groupeEntity
 * \brief Classe abstraite de base des entités.
 *
 * Lorsque l'on crée une nouvelle entité il faut :
 *  - créer une classe fille de Entity (ou créer un synonyme d'une classe prédéfinie à l'étape suivante).
 *  - Implémenter les différents constructeurs.
 *  - Placer la macro BASE_ENTITY(nom de la classe).
 *  - Créer si nécessaire une classe Unique Sql pour l'entité.
 *  - Rentrer les informations SQL pour faire le lien avec les entités correspondantes dans la base de donnée.
 *
 *  - Ajouter un manager de l'entité dans Managers (membre, def_get (dans .h) et manager_tab (dans .cpp)).
 */
class Entity : public amps::IdAttribut {
public:
    //! Position des attributs.
    enum Position {Id, NbrAtt};
    //! Identifiant de l'entité.
    //enum IdentifiantEntity{ID = infoEntity::entityBaseId::entityId};

    BASE_ENTITY_ABS(Entity)

    //! Renvoie une chaine de caractère contenant les noms, les validités et les valeurs des attributs.
    QString affiche() const override;

    //! Retourne un QVariant contenant la donnée souhaitée ou un QVariant nulle si num est invalide.
    virtual QVariant data(post pos) const
        {return pos == Id ? id() : QVariant();}

    //! Teste si l'entité est nouvelle ou si elle provient de la base de donnée en regardant si elle poséde un identifiant non-nul.
    bool isNew() const
        {return id() == 0;}

    //! Renvoie l'identifiant du type de l'entité.
    virtual entidt idEntity() const = 0;

    //! Renvoie le nom de l'entité.
    virtual QString name() const = 0;

    //! Retourne un le nom de l'attribut d'indice num.
    virtual QString nameAttribut(post pos) const = 0;

    //! Renvoie le nombre d'attribut de l'entité.
    virtual post nbrAtt() const = 0;

    //! Mutateur de l'attribut d'indice i.
    virtual void setData(post i, const QVariant & value) = 0;

    //! Mutateur de l'attribut de nom attr.
    virtual bool hydrate(const QString & attr, const QVariant & value);

    //! Mutateur de l'attribut de nom attr.
    virtual bool hydrate(std::vector<std::pair<QString,QVariant>> & values);

    //! Modifient les valeurs des attributs de l'entité avec celles des attributs de entity.
    virtual void operator << (const Entity & entity) = 0;

    //! Opérateur testant l'égalité de deux entités, c'est-à-dire l'égalité de tous les attributs.
    virtual bool operator == (const Entity & entity) const
        {return id() == entity.id();}

    //! Retourne la position de l'attribut de nom attr (retourne NbrAtt si attr est nom de aucun attribut.
    post position(const QString & attr) const;

protected:
    //! Remplace les attributs de l'entité par celle de l'entité transmise, sauf l'identifiant.
    void set(const Entity & entity)
        {setId(entity.id());}
};

class EntityException : public std::exception {
protected:
    QString m_txt;
    mutable std::string m_string;
public:
    //! Constructeur par default.
    EntityException() = default;

    //! Constructeur.
    EntityException(const QString & txt, const Entity & entity)
        : m_txt(txt) {
        m_txt.append("\nErreur sur l'entité :\n");
        m_txt.append(entity.affiche());
    }

    //!Destructeur.
    ~EntityException() override;

    //! Renvoie un pointeur sur la chaine expliquant l'exception.
    const char * what() const noexcept override {
        m_string = m_txt.toStdString();
        return m_string.data();
    }
};

class ConvertEntityException : public EntityException {
public:
    //! Constructeur par default.
    ConvertEntityException() = default;

    //! Constructeur.
    ConvertEntityException(const QString & txt, const Entity & entity, const QString & name) {
        m_txt = txt;
        m_txt.append("\nErreur de conversion d'entité:\n");
        m_txt.append("Une entrée de type ").append(entity.name()).append(" ne peut-être convertie en ").append(name).append(".\n");
        m_txt.append("Tentative de conversion de l'entrée:\n");
        m_txt.append(entity.affiche());
    }

    //!Destructeur.
    ~ConvertEntityException();
};

class InvalideEntityException : public EntityException {
public:
    //! Constructeur par default.
    InvalideEntityException() = default;

    //! Constructeur.
    InvalideEntityException(const QString & txt, const Entity & entity) {
        m_txt = txt;
        m_txt.append("\nErreur de validité de l'entrée:\n");
        m_txt.append(entity.affiche());
    }

    //!Destructeur.
    ~InvalideEntityException();
};

/*! \ingroup groupeEntity
 * \brief Classe abstraite de base des entités contenant les attributs.
 */
template <class Attribut> class EntityAttributs : public Attributs<Entity,Attribut> {
public:
    enum {NbrAtt = Attribut::NbrAtt + Entity::NbrAtt,      //!< Nombre d'attribut de l'entité.
         };

    using Attributs<Entity,Attribut>::setId;
    CONSTR_AFFECT_DEFAUT(EntityAttributs)

    //!Constructeur.
    EntityAttributs<Attribut>(idt id = 0)
        {setId(id);}

//    //! Mutateur de l'attribut de nom attr.
//    bool hydrate(const QString & attr, const QVariant & value) override;

    //! Renvoie le nombre d'attribut de l'entité.
    post nbrAtt() const override
        {return NbrAtt;}

    //! Retourne un le nom de l'attribut d'indice num.
    QString nameAttribut(post pos) const override
        {return pos < NbrAtt ? Attributs<Entity,Attribut>::attributName(pos) : QString();}

    //! Retourne un vecteur contenant les noms des attributs.
    static std::vector<QString> namesAttributs() {
        std::vector<QString> vect(NbrAtt);
        for (post i =0; i < NbrAtt; ++i) {
            vect[i] = Attributs<Entity,Attribut>::nameAttribut(i);
        }
        return vect;
    }

    //! Retourne un le nom de l'attribut d'indice num.
    static QString NameAttribut(post pos)
        {return pos < NbrAtt ? Attributs<Entity,Attribut>::NameAttribut(pos) : QString();}  

    //! Retourne un vecteur contenant les noms des attributs.
    static std::vector<QString> NamesAttributs() {
        std::vector<QString> vect(NbrAtt);
        for (post i =0; i < NbrAtt; ++i) {
            vect[i] = Attributs<Entity,Attribut>::NameAttribut(i);
        }
        return vect;
    }

//    //! Retourne la position de l'attribut de nom attr (retourne NbrAtt si attr est nom de aucun attribut.
//    post position(const QString & attr) const override;

    //! Mutateur de l'attribut d'indice i.
    void setData(post i, const QVariant & value) {
        if(i < NbrAtt)
            setDataP(i, value);
    }

    //! Modifient les valeurs des attributs de l'entité avec celles des attributs de entity.
    void operator << (const EntityAttributs<Attribut> & entity)
        {set(entity);}

protected:
    using Attributs<Entity,Attribut>::setDataP;
};

/*! \ingroup groupeEntity
 * \brief Classe abstraite de base des entités implémenté les membres utilisant l'identifiant du type de l'entité.
 */
template <entidt IDM, class Attribut> class EntityID : public EntityAttributs<Attribut> {
public:
    enum {ID = IDM,         //!< Identifiant de l'attribut.
         };

    using EntAtt = EntityAttributs<Attribut>;
    using EntityAttributs<Attribut>::EntityAttributs;
    using EntAtt::name;
    using EntAtt::operator ==;

protected:
    using EntAtt::set;

public:
    CONSTR_DEFAUT(EntityID)
    CONSTR_AFFECT_DEFAUT(EntityID)

    //! Convertit la référence entity en une référence de type ENTITY, aprés vérification.
    static EntityID<IDM,Attribut> * Convert(Entity *entity) {
        if(VerifEntity(entity))
            return static_cast<EntityID<IDM,Attribut>*>(entity);
        else
            throw ConvertEntityException(QString("static EntityID<IDM,Attribut> * Convert(Entity *)"),*entity,
                                         QString("EntityID<").append(QString::number(IDM)).append(",Attribut>"));
    }

    //! Convertit la référence constante entity en une référence constante de type ENTITY, aprés vérification.
    static EntityID<IDM,Attribut> & Convert(Entity & entity) {
        if(VerifEntity(entity))
            return *(static_cast<EntityID<IDM,Attribut>*>(&entity));
        else
            throw ConvertEntityException(QString("static EntityID<IDM,Attribut> & Convert(Entity &)"),entity,
                                         QString("EntityID<").append(QString::number(IDM)).append(",Attribut>"));
    }

    //*! Convertit le pointeur entity en un pointeur de type ENTITY, aprés vérification.
    static const EntityID<IDM,Attribut> & Convert(const Entity & entity) {
        if(VerifEntity(entity))
            return  *(static_cast<const EntityID<IDM,Attribut>*>(&entity));
        else
            throw ConvertEntityException(QString("static const EntityID<IDM,Attribut> & Convert(const Entity &)"),entity,
                                         QString("EntityID<").append(QString::number(IDM)).append(",Attribut>"));;
    }

    //! Renvoie l'identifiant du type de l'entité.
    entidt idEntity() const override
        {return ID;}

    //! Test si le pointeur entity est aussi un pointeur de ce type d'entité.
    static bool VerifEntity(Entity * entity)
        {return ID == entity->idEntity();}

    //! Test si la référence entity est aussi une référence de ce type d'entité.
    static bool VerifEntity(const Entity & entity)
        {return ID == entity.idEntity();}

    //! Opérateur d'égalité.
    bool operator == (const Entity & entity) const
        {return VerifEntity(entity) && EntityAttributs<Attribut>::operator == (Convert(entity));}

    //! Opérateur de différence.
    bool operator != (const Entity & entity) const
        {return ! operator== (entity);}

    //! Modifient les valeurs des attributs de l'entité avec celles des attributs de entity
    //! qui doit être de même type que la première opérande.
    void operator << (const Entity & entity)
        {set(Convert(entity));}
};

template <entidt IDM, class... Attribut> using EntityIDs = EntityID<IDM,Attributs<Attribut...>>;
}
#endif // ENTITY_H
