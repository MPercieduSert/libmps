/*Auteur: PERCIE DU SERT Maxime
 *Date: 15/09/2020
 */
#ifndef COLONNESFORMODEL_H
#define COLONNESFORMODEL_H

#include "AbstractColonnesModel.h"

namespace modelMPS {
//////////////////////////////////////////////////////// BaseColonne /////////////////////////////////////////
/*! \ingroup groupeModel
 * \brief Classe template générique d'une colonne.
 */
template<class Read, class Find, class Write, class Vec> class TempBaseColonne : public AbstractColonnesModel::AbstractColonne {
protected:
    Read m_get;                                 //!< Fonction lisant la donnée du model.
    Write m_set;                                //!< Fonction modifiant la donnée du model.
    Find m_test;                                //!< Fonction lisant la donnée du model pour les tests de recherche.
    Vec & m_vec;                                //!< Référence sur le vecteur de donnée.
public:
    //! Constructeur.
    TempBaseColonne(const QString & name, Qt::ItemFlags flags, int type,
                 Vec & vec,
                 Read get,
                 Find test,
                 Write set)
        : AbstractColonne(name,flags,type), m_get(get), m_set(set), m_test(test), m_vec(vec) {}

    //! Accesseur de la donnée d'indice id dans la colonne.
    QVariant data(szt id, int role) const override
        {return m_get(m_vec[id], role);}

    //! Accesseut de la donnée pour le test de recherche.
    QVariant dataTest(szt id) const
        {return m_test(m_vec[id]);}

    //! Mutateur de la donnée d'indice id dans la colonne.
    bool setData(szt id, const QVariant & value,  int role) override
        {return m_set(m_vec[id],value,role);}
};

template<class Read, class Write, class Vec> using BaseColonne = TempBaseColonne<QVariant (*)(Read, int),
                                                                                 QVariant (*)(Read),
                                                                                 bool (*)(Write,const QVariant &,int),
                                                                                 Vec>;
template<class T> using VectorRefColonne = BaseColonne<const T&,T&,std::vector<T>>;
template<class T> using VectorValColonne = BaseColonne<T,T,std::vector<T>>;
template<class T> using VectorPtrColonne = BaseColonne<const T&,T&, conteneurMPS::VectorPtr<T>>;
template<class T> using VectorListColonne = BaseColonne<const std::list<T>&,std::list<T>&,std::vector<std::list<T>>>;

//////////////////////////////////////////////////////////// BoolColonne ////////////////////////////////
/*! \ingroup groupeModel
 * \brief Classe Abstraite mère des colonnes de type booléen.
 */
class AbstractBoolColonne : public AbstractColonnesModel::AbstractColonne {
protected:
    QString m_trueLabel;                //!< Label du booléen vraie.
    QString m_falseLabel;               //!< Label du booléen faux.
public:
    //! Constructeur.
    AbstractBoolColonne(const QString & name, Qt::ItemFlags flags, int type,
                const QString & trueLabel = QString(), const QString & falseLabel = QString())
        : AbstractColonne(name,flags,type),
          m_trueLabel(trueLabel), m_falseLabel(falseLabel) {}

    //! Destructeur.
    ~AbstractBoolColonne() override;

    //! Accesseur du label pour faux.
    const QString & falseLabel() const
        {return m_falseLabel;}

    //! Mutateur du label pour faux.
    void setFalseLabel(const QString & falseLabel)
        {m_falseLabel = falseLabel;}

    //! Mutateur du label pour vraie.
    void setTrueLabel(const QString & trueLabel)
        {m_trueLabel = trueLabel;}

    //! Accesseur du label pour vraie.
    const QString & trueLabel() const
        {return m_trueLabel;}
};

/*! \ingroup groupeModel
 * \brief Classe mère des colonnes de type booléen.
 */
template<class Read, class Write, class Vec> class BoolColonne : public AbstractBoolColonne  {
protected:
    bool (*m_get)(Read);           //!< Fonction lisant la donnée du model.
    void (*m_set)(Write,bool);     //!< Fonction modifiant la donnée du model.
    Vec & m_vec;                   //!< Référence sur le vecteur de donnée.

public:
    //! Constructeur.
    BoolColonne(const QString & name, Qt::ItemFlags flags, int type,
                Vec & vec,
                bool (*get)(Read),
                void (*set)(Write,bool),
                const QString & trueLabel = QString(), const QString & falseLabel = QString())
        : AbstractBoolColonne(name,flags,type,trueLabel,falseLabel),
          m_get(get), m_set(set), m_vec(vec) {}

    //! Accesseur de la donnée d'indice id dans la colonne.
    QVariant data(szt id, int role) const override {
        if(role == Qt::CheckStateRole)
            return m_get(m_vec[id]) ? Qt::Checked : Qt::Unchecked;
        if(role == Qt::DisplayRole)
            return m_get(m_vec[id]) ? m_trueLabel : m_falseLabel;
        return QVariant();
    }

    //! Accesseut de la donnée pour le test de recherche.
    QVariant dataTest(szt id) const
        {return m_get(m_vec[id]);}

    //! Mutateur de la donnée d'indice id dans la colonne.
    bool setData(szt id, const QVariant & value,  int role) override {
        if(role == Qt::CheckStateRole) {
            m_set(m_vec[id],value.toBool());
            return true;}
        return false;
    }
};

template<class T> using VectorRefBoolColonne = BoolColonne<const T&,T&,std::vector<T>>;
template<class T> using VectorValBoolColonne = BoolColonne<T,T,std::vector<T>>;
template<class T> using VectorPtrBoolColonne = BoolColonne<const T&,T&, conteneurMPS::VectorPtr<T>>;

////////////////////////////////////////////////////////// IdAdapter ///////////////////////////////////////
/*! \ingroup groupeModel
 * \brief Adaptateur de colonne pour les identifiants des entités.
 */
template<class Colonne> class IdAdaptColonne : public Colonne {
protected:
    using Colonne::m_vec;
public:
    //! Constructeur.
    using Colonne::Colonne;

    //! Accesseur de la donnée d'indice id dans la colonne.
    QVariant data(szt id, int role) const override {
        if(role == AbstractColonnesModel::IdRole)
            return m_vec[id].id();
        return Colonne::data(id,role);
    }


    //! Mutateur de la donnée d'indice id dans la colonne.
    bool setData(szt id, const QVariant & value,  int role) override {
        if(role == AbstractColonnesModel::IdRole) {
            m_vec[id].setId(value.toUInt());
            return true;
        }
        return Colonne::setData(id,value,role);}
};

template<class Read, class Find, class Write, class Vec> using TempBaseIdColonne = IdAdaptColonne<TempBaseColonne<Read, Find, Write, Vec>>;
template<class Read, class Write, class Vec> using BaseIdColonne = IdAdaptColonne<BaseColonne<Read,Write,Vec>>;
template<class T> using VectorRefIdColonne = IdAdaptColonne<VectorRefColonne<T>>;
template<class T> using VectorValIdColonne = IdAdaptColonne<VectorValColonne<T>>;
template<class T> using VectorPtrIdColonne = IdAdaptColonne<VectorPtrColonne<T>>;
template<class T> using VectorRefBoolIdColonne = IdAdaptColonne<VectorRefBoolColonne<T>>;
template<class T> using VectorValBoolIdColonne = IdAdaptColonne<VectorValBoolColonne<T>>;
template<class T> using VectorPtrBoolIdColonne = IdAdaptColonne<VectorPtrBoolColonne<T>>;

///////////////////////////////////////////////////// HeterogeneTailleAdaptColonne //////////////////
/*! \ingroup groupeModel
 * \brief Adaptateur de colonne pour un tableau de taille hétérogéne.
 */
template<class Colonne> class HeterogeneTailleAdaptColonne : public Colonne {
protected:
    using Colonne::m_vec;
public:
    //! Constructeur.
    using Colonne::Colonne;

    //! Accesseur de la donnée d'indice id dans la colonne.
    QVariant data(szt id, int role) const override {
        if(id < m_vec.size())
            return Colonne::data(id,role);
        else
            return QVariant();
    }

    //! Accesseut de la donnée pour le test de recherche.
    QVariant dataTest(szt id) const override {
        if(id < m_vec.size())
            return Colonne::dataTest(id);
        else
            return QVariant();
    }

    //! Mutateur de la donnée d'indice id dans la colonne.
    bool setData(szt id, const QVariant & value,  int role) override {
        if(id >= m_vec.size())
            m_vec.resize(id + 1);
        return Colonne::setData(id,value,role);
    }
};
template<class Read, class Find, class Write, class Vec> using TempBaseHeterogeneTailleColonne
                                                                = HeterogeneTailleAdaptColonne<TempBaseColonne<Read, Find, Write, Vec>>;
template<class Read, class Write, class Vec> using BaseHeterogeneTailleColonne = HeterogeneTailleAdaptColonne<BaseColonne<Read,Write,Vec>>;
template<class T> using VectorRefHeterogeneTailleColonne = HeterogeneTailleAdaptColonne<VectorRefColonne<T>>;
template<class T> using VectorValHeterogeneTailleColonne = HeterogeneTailleAdaptColonne<VectorValColonne<T>>;
template<class T> using VectorListHeterogeneTailleColonne = HeterogeneTailleAdaptColonne<VectorListColonne<T>>;
template<class T> using VectorPtrHeterogeneTailleColonne = HeterogeneTailleAdaptColonne<VectorPtrColonne<T>>;
template<class Read, class Find, class Write, class Vec> using TempBaseHeterogeneTailleIdColonne
                                                                = HeterogeneTailleAdaptColonne<TempBaseIdColonne<Read, Find, Write, Vec>>;
template<class Read, class Write, class Vec> using BaseHeterogeneTailleIdColonne
                                                        = HeterogeneTailleAdaptColonne<BaseIdColonne<Read,Write,Vec>>;
template<class T> using VectorRefHeterogeneTailleIdColonne = HeterogeneTailleAdaptColonne<VectorRefIdColonne<T>>;
template<class T> using VectorValHeterogeneTailleIdColonne = HeterogeneTailleAdaptColonne<VectorValIdColonne<T>>;
template<class T> using VectorPtrHeterogeneTailleIdColonne = HeterogeneTailleAdaptColonne<VectorPtrIdColonne<T>>;
}
#endif // COLONNESFORMODEL_H
