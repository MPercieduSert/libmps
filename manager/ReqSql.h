/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */

#ifndef REQSQL_H
#define REQSQL_H

#include <QDate>
#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include "AbstractManager.h"

/*! \ingroup groupeManager
 * \brief Macro rappelant les méthodes de ReqSql dans les classes patrons.
 */
#define USING_REQSQL(T) using T::exec; \
using T::finish; \
using T::next; \
using T::prepare; \
using T::toInt;

namespace bdd {
    //! Mot clé sql.
    enum wordSql{Create,
                NotNull,
                Constraint,
                Unique,
                Foreign,
                Ref};
}

/*! \ingroup groupeManager
 * \brief Classe mère des liens contenant la référence sur l'objet de requète ouvert sur le base de donnée.
 */
class ReqSql
{
protected:
    static QSqlQuery * m_requete;  //!< Référence vers la requète employée dans le manageur.

public:
    CONSTR_DEFAUT(ReqSql)
    DESTR_VIDE(ReqSql)

    //! Lance une execption contenant le message d'erreur de la derniere requète SQL exéctuter.
    void affError() const;

    //! Renvoie une QString correspondant au type.
    static QString typeAttributSqlString(bdd::TypeAttributBdd n);

    QString lastQuery() const
        {return m_requete->lastQuery();}

    //! Modifie le pointeur vers l'objet requête.
    static void setRequete(QSqlQuery * req)
        {m_requete = req;}

    //! Renvoie une QString correspondant au type.
    static QString wordSqlString(bdd::wordSql n);

protected:
    //! Transmet une valeur à la requète.
    template<class T> void bindValue(int n, const T & value)
        {m_requete->bindValue(n,value);}

    //! Exécute la dernière requète préparée et lance une execption si celle-ci n'est pas valide.
    void exec()
    {
        m_requete->exec();
        affError();
    }

    //! Exécute la requéte SQL donnée en argument et lance une execption si celle-ci n'est pas valide.
    void exec(const QString & requete)
    {
        m_requete->exec(requete);
        affError();
    }

    //! Exécute la dernière requète préparée et lance une execption si celle-ci n'est pas valide.
    void execFinish()
    {
        exec();
        finish();
    }

    //! Exécute la requéte SQL donnée en argument et lance une execption si celle-ci n'est pas valide.
    void execFinish(const QString & requete)
    {
        exec(requete);
        finish();
    }

    //! Termine la requète.
    void finish()
        {m_requete->finish();}

    //! Place la requète sur la position suivante.
    bool next()
        {return m_requete->next();}

    //! Prepare la requète SQL.
    void prepare(const QString & requete)
        {m_requete->prepare(requete);}

    //! Récupère la nième valeur de la requète au type T.
    template<class T> inline T value(int n = 0) const;

    //! Convertit un entier en QVariant, en remplaçant 0 par QVariant(QVariant::Int).
    template<class T> QVariant zeroToNull(T n) const
        {return n != 0 ? n : QVariant(QVariant::Int);}
};

template<class T> inline T ReqSql::value(int n) const
    {return m_requete->value(n).value<T>();}

template<> inline QVariant ReqSql::value<QVariant>(int n) const
    {return m_requete->value(n);}

#endif // REQSQL_H
