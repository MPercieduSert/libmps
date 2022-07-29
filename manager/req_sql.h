/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef REQ_SQL_H
#define REQ_SQL_H

#include <QDate>
#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include "abstract_manager.h"

/*! \ingroup groupe_manager
 *\brief Macro rappelant les méthodes de req_sql dans les classes patrons.
 */
#define USING_REQSQL(T) using T::exec; \
using T::finish; \
using T::next; \
using T::prepare; \
using T::toInt;

namespace mps {
namespace b2d {
    //! Mot clé sql.
    enum word_sql{Create,
                Not_null,
                Constraint,
                Unique,
                Foreign,
                Ref};
}

namespace manager {
/*! \ingroup groupe_manager
 *\brief Classe mère des liens contenant la référence sur l'objet de requète ouvert sur le base de donnée.
 */
class req_sql {
protected:
    static QSqlQuery *m_requete;  //!< Pointeur vers la requète employée dans le manageur.
public:
    CONSTR_DEFAUT(req_sql)
    DESTR_VIDE(req_sql)

    //! Lance une execption contenant le message d'erreur de la derniere requète SQL exéctuter.
    void aff_error() const;

    //! Renvoie une QString correspondant au type.
    static QString type_attribut_sql_string(b2d::type_attribut_bdd n);

    QString last_query() const
        {return m_requete->lastQuery();}

    //! Modifie le pointeur vers l'objet requête.
    static void set_requete(QSqlQuery *req)
        {m_requete = req;}

    //! Renvoie une QString correspondant au type.
    static QString word_sql_string(b2d::word_sql n);

protected:
    //! Transmet une valeur à la requète.
    void bind_value(numt n, const QVariant &value)
        {m_requete->bindValue(static_cast<int>(n),value);}

    //! Transmet une valeur à la requète.
    void bind_value(int n, const QVariant &value)
        {m_requete->bindValue(n,value);}

    //! Exécute la dernière requète préparée et lance une execption si celle-ci n'est pas valide.
    void exec() {
        m_requete->exec();
        aff_error();
    }

    //! Exécute la requéte SQL donnée en argument et lance une execption si celle-ci n'est pas valide.
    void exec(const QString &requete) {
        m_requete->exec(requete);
        aff_error();
    }

    //! Exécute la dernière requète préparée et lance une execption si celle-ci n'est pas valide.
    void exec_finish() {
        exec();
        finish();
    }

    //! Exécute la requéte SQL donnée en argument et lance une execption si celle-ci n'est pas valide.
    void exec_finish(const QString &requete) {
        exec(requete);
        finish();
    }

    //! Termine la requète.
    void finish()
        {m_requete->finish();}

    //! Accesseur de l'identifiant.
    idt id(post pos = entity::Id) const
        {return value<idt>(pos);}

    //! Place la requète sur la position suivante.
    bool next()
        {return m_requete->next();}

    //! Prepare la requète SQL.
    void prepare(const QString &requete)
        {m_requete->prepare(requete);}

    //! Récupère la nième valeur de la requète au type T.
    template<class T> inline T value(post n = 0) const;

    //! Convertit un entier en QVariant, en remplaçant 0 par QVariant(QMetaType(QMetaType::Int)).
    template<class T> QVariant zero_to_null(T n) const
        {return n != 0 ? n : QVariant(QMetaType(QMetaType::Int));}
};

template<class T> inline T req_sql::value(post n) const
    {return m_requete->value(static_cast<int>(n)).value<T>();}

template<> inline flag req_sql::value<flag>(post n) const
    {return m_requete->value(static_cast<int>(n)).toUInt();}

template<> inline QVariant req_sql::value<QVariant>(post n) const
    {return m_requete->value(static_cast<int>(n));}
}}
#endif // REQ_SQL_H
