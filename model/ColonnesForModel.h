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
public:
    using Data_type = Vec;
protected:
    Read m_get;                                 //!< Fonction lisant la donnée du model.
    Write m_set;                                //!< Fonction modifiant la donnée du model.
    Find m_test;                                //!< Fonction lisant la donnée du model pour les tests de recherche.
    Data_type & m_vec;                                //!< Référence sur le vecteur de donnée.
public:
    //! Constructeur.
    TempBaseColonne(const QString & name, Qt::ItemFlags flags, int type,
                 Data_type & vec,
                 const Read & get,
                 const Find & test,
                 const Write & set)
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
template<class T> using VectorListColonne = BaseColonne<const std::list<T>&, std::list<T>&, std::vector<std::list<T>>>;
template<class T> using VectorPtrColonne = BaseColonne<const T&, T&, conteneurMPS::VectorPtr<T>>;
template<class T> using VectorRefColonne = BaseColonne<const T&, T&, std::vector<T>>;
template<class T> using VectorValColonne = BaseColonne<T, T, std::vector<T>>;

//////////////////////////////////////////////////////////// BoolColonne ////////////////////////////////
/*! \ingroup groupeModel
 * \brief Classe Abstraite mère des colonnes de type booléen.
 */
class AbstractBoolColonne : public AbstractColonnesModel::AbstractColonne {
protected:
    QString m_falseLabel;               //!< Label du booléen faux.
    QString m_trueLabel;                //!< Label du booléen vraie.
public:
    //! Constructeur.
    AbstractBoolColonne(const QString & name, Qt::ItemFlags flags, int type,
                const QString & trueLabel = QString(), const QString & falseLabel = QString())
        : AbstractColonne(name,flags,type),
          m_falseLabel(falseLabel), m_trueLabel(trueLabel) {}

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
template<class Read, class Write, class Vec> class TempBoolColonne : public AbstractBoolColonne  {
public:
    using Data_type = Vec;
protected:
    Read m_get;         //!< Fonction lisant la donnée du model.
    Write m_set;        //!< Fonction modifiant la donnée du model.
    Data_type & m_vec;        //!< Référence sur le vecteur de donnée.

public:
    //! Constructeur.
    TempBoolColonne(const QString & name, Qt::ItemFlags flags, int type,
                Data_type & vec,
                const Read & get,
                const Write & set,
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

template<class Read, class Write, class Vec> using BoolColonne = TempBoolColonne<bool (*)(Read),void (*)(Write,bool),Vec>;
template<class T> using VectorListBoolColonne = BoolColonne<const std::list<T>&, std::list<T>&, std::vector<std::list<T>>>;
template<class T> using VectorPtrBoolColonne = BoolColonne<const T&,T&, conteneurMPS::VectorPtr<T>>;
template<class T> using VectorRefBoolColonne = BoolColonne<const T&,T&,std::vector<T>>;
template<class T> using VectorValBoolColonne = BoolColonne<T,T,std::vector<T>>;


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
        return Colonne::setData(id,value,role);
    }
};

// Id
template<class Read, class Find, class Write, class Vec> using TempBaseIdColonne = IdAdaptColonne<TempBaseColonne<Read, Find, Write, Vec>>;
template<class Read, class Write, class Vec> using BaseIdColonne = IdAdaptColonne<BaseColonne<Read,Write,Vec>>;
template<class T> using VectorPtrIdColonne = IdAdaptColonne<VectorPtrColonne<T>>;
template<class T> using VectorRefIdColonne = IdAdaptColonne<VectorRefColonne<T>>;
template<class T> using VectorValIdColonne = IdAdaptColonne<VectorValColonne<T>>;
// BoolId
template<class Read, class Write, class Vec> using TempBoolIdColonne = IdAdaptColonne<TempBoolColonne<Read, Write, Vec>>;
template<class Read, class Write, class Vec> using BoolIdColonne = IdAdaptColonne<BoolColonne<Read, Write, Vec>>;
template<class T> using VectorRefBoolIdColonne = IdAdaptColonne<VectorRefBoolColonne<T>>;
template<class T> using VectorPtrBoolIdColonne = IdAdaptColonne<VectorPtrBoolColonne<T>>;
template<class T> using VectorValBoolIdColonne = IdAdaptColonne<VectorValBoolColonne<T>>;


////////////////////////////////////////////////////////// ColorAdapter ///////////////////////////////////////
/*! \ingroup groupeModel
 * \brief Adaptateur de colonne pour les couleur.
 */
template<class Back, class Fore, class Colonne> class TempColorAdaptColonne : public Colonne {
protected:
    Back m_back;                        //!< Fonction donnant l'indice de rendu du fond.
    Fore m_fore;                        //!< Fonction donnant l'indice de rendu du texte.
    std::vector<QBrush> m_backVec;      //!< Vecteur des rendus du fond.
    std::vector<QBrush> m_foreVec;      //!< Vecteur des rendus du texte.
    using Colonne::m_vec;
public:
    //! Constructeur.
    template<class... Args> TempColorAdaptColonne(const QString & name, Qt::ItemFlags flags, int type,
                                                  typename Colonne::Data_type & vec,
                                                  const Back & back, const Fore & fore, const Args & ... args)
        : Colonne(name, flags, type, vec, args...), m_back(back), m_fore(fore) {}

    //! Constructeur.
    template<class... Args> TempColorAdaptColonne(const QString & name, Qt::ItemFlags flags, int type,
                                                  typename Colonne::Data_type & vec,
                                                  const Back & back, const Fore & fore,
                                                  const std::vector<QBrush> & backVec, const std::vector<QBrush> & foreVec,
                                                  Args && ... args)
        : Colonne(name,flags,type,vec,args...), m_back(back), m_fore(fore), m_backVec(backVec), m_foreVec(foreVec) {}

    //! Accesseur du vecteur des rendus du fond.
    const std::vector<QBrush> & backGroundVector() const
        {return m_backVec;}

    //! Accesseur du vecteur des rendus du fond.
    std::vector<QBrush> & backGroundVector()
        {return m_backVec;}

    //! Accesseur de la donnée d'indice id dans la colonne.
    QVariant data(szt id, int role) const override;

    //! Accesseur du vecteur des rendus du texte
    const std::vector<QBrush> & foreGroundVector() const
        {return m_foreVec;}

    //! Accesseur du vecteur des rendus du texte
    std::vector<QBrush> & foreGroundVector()
        {return m_foreVec;}

    //! Mutateur du vecteur des rendus du fond.
    void setBackGroundVector(const std::vector<QBrush> & vec)
        {m_backVec = vec;}

    //! Mutateur du vecteur des rendus du fond.
    void setForeGroundVector(const std::vector<QBrush> & vec)
        {m_foreVec = vec;}
};

template<class Back, class Fore, class Colonne> QVariant TempColorAdaptColonne<Back, Fore, Colonne>::data(szt id, int role) const {
    if(role == Qt::BackgroundRole) {
        auto num = m_back(m_vec[id]);
        if(num < m_backVec.size())
            return m_backVec[num];
        else
            return QVariant();
    }
    if(role == Qt::ForegroundRole) {
        auto num = m_fore(m_vec[id]);
        if(num < m_foreVec.size())
            return m_foreVec[num];
        else
            return QVariant();
    }
    return Colonne::data(id,role);
}

template<class Read, class Colonne> using ColorAdaptColonne = TempColorAdaptColonne<szt(*)(Read), szt(*)(Read), Colonne>;

// Color
template<class Back, class Fore, class Read, class Find, class Write, class Vec>
    using TempBaseColorColonne = TempColorAdaptColonne<Back, Fore, TempBaseColonne<Read, Find, Write, Vec>>;
template<class Read, class Write, class Vec>
    using BaseColorColonne = ColorAdaptColonne<Read, BaseColonne<Read,Write,Vec>>;
template<class T> using VectorListColorColonne = ColorAdaptColonne<const std::list<T>&, VectorListColonne<T>>;
template<class T> using VectorPtrColorColonne = ColorAdaptColonne<const T&, VectorPtrColonne<T>>;
template<class T> using VectorRefColorColonne = ColorAdaptColonne<const T&, VectorRefColonne<T>>;
template<class T> using VectorValColorColonne = ColorAdaptColonne<T,VectorValColonne<T>>;
// ColorId
template<class Back, class Fore, class Read, class Find, class Write, class Vec>
    using TempBaseColorIdColonne = TempColorAdaptColonne<Back, Fore, TempBaseIdColonne<Read, Find, Write, Vec>>;
template<class Read, class Write, class Vec>
    using BaseColorIdColonne = ColorAdaptColonne<Read, BaseIdColonne<Read,Write,Vec>>;
template<class T> using VectorPtrColorIdColonne = ColorAdaptColonne<const T&, VectorPtrIdColonne<T>>;
template<class T> using VectorRefColorIdColonne = ColorAdaptColonne<const T&, VectorRefIdColonne<T>>;
template<class T> using VectorValColorIdColonne = ColorAdaptColonne<T,VectorValIdColonne<T>>;
// BoolColor
template<class Back, class Fore, class Read, class Write, class Vec>
    using TempBoolColorColonne = TempColorAdaptColonne<Back, Fore, TempBoolColonne<Read, Write, Vec>>;
template<class Read, class Write, class Vec>
    using BoolColorColonne = ColorAdaptColonne<Read, BoolColonne<Read, Write, Vec>>;
template<class T> using VectorListBoolColorColonne = ColorAdaptColonne<const std::list<T>&, VectorListBoolColonne<T>>;
template<class T> using VectorPtrBoolColorColonne = ColorAdaptColonne<const T&, VectorPtrBoolColonne<T>>;
template<class T> using VectorRefBoolColorColonne = ColorAdaptColonne<const T&,VectorRefBoolColonne<T>>;
template<class T> using VectorValBoolColorColonne = ColorAdaptColonne<T, VectorValBoolColonne<T>>;
// BoolColorId
template<class Back, class Fore, class Read, class Write, class Vec>
    using TempBoolColorIdColonne = TempColorAdaptColonne<Back, Fore, TempBoolIdColonne<Read, Write, Vec>>;
template<class Read, class Write, class Vec>
    using BoolColorIdColonne = ColorAdaptColonne<Read, BoolIdColonne<Read, Write, Vec>>;
template<class T> using VectorPtrBoolColorIdColonne = ColorAdaptColonne<const T&, VectorPtrBoolIdColonne<T>>;
template<class T> using VectorRefBoolColorIdColonne = ColorAdaptColonne<const T&,VectorRefBoolIdColonne<T>>;
template<class T> using VectorValBoolColorIdColonne = ColorAdaptColonne<T, VectorValBoolIdColonne<T>>;

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

// HeterogeneTaille
template<class Read, class Find, class Write, class Vec>
    using TempBaseHeterogeneTailleColonne= HeterogeneTailleAdaptColonne<TempBaseColonne<Read, Find, Write, Vec>>;
template<class Read, class Write, class Vec> using BaseHeterogeneTailleColonne = HeterogeneTailleAdaptColonne<BaseColonne<Read,Write,Vec>>;
template<class T> using VectorListHeterogeneTailleColonne = HeterogeneTailleAdaptColonne<VectorListColonne<T>>;
template<class T> using VectorPtrHeterogeneTailleColonne = HeterogeneTailleAdaptColonne<VectorPtrColonne<T>>;
template<class T> using VectorRefHeterogeneTailleColonne = HeterogeneTailleAdaptColonne<VectorRefColonne<T>>;
template<class T> using VectorValHeterogeneTailleColonne = HeterogeneTailleAdaptColonne<VectorValColonne<T>>;
// HeterogeneTailleId
template<class Read, class Find, class Write, class Vec>
    using TempBaseHeterogeneTailleIdColonne = HeterogeneTailleAdaptColonne<TempBaseIdColonne<Read, Find, Write, Vec>>;
template<class Read, class Write, class Vec>
    using BaseHeterogeneTailleIdColonne = HeterogeneTailleAdaptColonne<BaseIdColonne<Read,Write,Vec>>;
template<class T> using VectorPtrHeterogeneTailleIdColonne = HeterogeneTailleAdaptColonne<VectorPtrIdColonne<T>>;
template<class T> using VectorRefHeterogeneTailleIdColonne = HeterogeneTailleAdaptColonne<VectorRefIdColonne<T>>;
template<class T> using VectorValHeterogeneTailleIdColonne = HeterogeneTailleAdaptColonne<VectorValIdColonne<T>>;
// HeterogeneTailleColor
template<class Back, class Fore, class Read, class Find, class Write, class Vec>
    using TempBaseHeterogeneTailleColorColonne = HeterogeneTailleAdaptColonne<TempBaseColorColonne<Back, Fore, Read, Find, Write, Vec>>;
template<class Read, class Write, class Vec>
    using BaseHeterogeneTailleColorColonne = HeterogeneTailleAdaptColonne<BaseColorColonne<Read,Write,Vec>>;
template<class T> using VectorListHeterogeneTailleColorColonne = HeterogeneTailleAdaptColonne<VectorListColorColonne<T>>;
template<class T> using VectorPtrHeterogeneTailleColorColonne = HeterogeneTailleAdaptColonne<VectorPtrColorColonne<T>>;
template<class T> using VectorRefHeterogeneTailleColorColonne = HeterogeneTailleAdaptColonne<VectorRefColorColonne<T>>;
template<class T> using VectorValHeterogeneTailleColorColonne = HeterogeneTailleAdaptColonne<VectorValColorColonne<T>>;
// HeterogeneTailleColorId
template<class Back, class Fore, class Read, class Find, class Write, class Vec>
    using TempBaseHeterogeneTailleColorIdColonne = HeterogeneTailleAdaptColonne<TempBaseColorIdColonne<Back, Fore, Read, Find, Write, Vec>>;
template<class Read, class Write, class Vec>
    using BaseHeterogeneTailleColorIdColonne = HeterogeneTailleAdaptColonne<BaseColorIdColonne<Read,Write,Vec>>;
template<class T> using VectorPtrHeterogeneTailleColorIdColonne = HeterogeneTailleAdaptColonne<VectorPtrColorIdColonne<T>>;
template<class T> using VectorRefHeterogeneTailleColorIdColonne = HeterogeneTailleAdaptColonne<VectorRefColorIdColonne<T>>;
template<class T> using VectorValHeterogeneTailleColorIdColonne = HeterogeneTailleAdaptColonne<VectorValColorIdColonne<T>>;
// BooleHeterogeneTaille
template<class Read, class Write, class Vec>
    using TempBoolHeterogeneTailleColonne= HeterogeneTailleAdaptColonne<TempBoolColonne<Read, Write, Vec>>;
template<class Read, class Write, class Vec> using BoolHeterogeneTailleColonne = HeterogeneTailleAdaptColonne<BoolColonne<Read,Write,Vec>>;
template<class T> using VectorListBoolHeterogeneTailleColonne = HeterogeneTailleAdaptColonne<VectorListBoolColonne<T>>;
template<class T> using VectorPtrBoolHeterogeneTailleColonne = HeterogeneTailleAdaptColonne<VectorPtrBoolColonne<T>>;
template<class T> using VectorRefBoolHeterogeneTailleColonne = HeterogeneTailleAdaptColonne<VectorRefBoolColonne<T>>;
template<class T> using VectorValBoolHeterogeneTailleColonne = HeterogeneTailleAdaptColonne<VectorValBoolColonne<T>>;
// BoolHeterogeneTailleId
template<class Read, class Write, class Vec>
    using TempBoolHeterogeneTailleIdColonne = HeterogeneTailleAdaptColonne<TempBoolIdColonne<Read, Write, Vec>>;
template<class Read, class Write, class Vec>
    using BoolHeterogeneTailleIdColonne = HeterogeneTailleAdaptColonne<BoolIdColonne<Read,Write,Vec>>;
template<class T> using VectorPtrBoolHeterogeneTailleIdColonne = HeterogeneTailleAdaptColonne<VectorPtrBoolIdColonne<T>>;
template<class T> using VectorRefBoolHeterogeneTailleIdColonne = HeterogeneTailleAdaptColonne<VectorRefBoolIdColonne<T>>;
template<class T> using VectorValBoolHeterogeneTailleIdColonne = HeterogeneTailleAdaptColonne<VectorValBoolIdColonne<T>>;
// HeterogeneTailleColor
template<class Back, class Fore, class Read, class Write, class Vec>
    using TempBoolHeterogeneTailleColorColonne = HeterogeneTailleAdaptColonne<TempBoolColorColonne<Back, Fore, Read, Write, Vec>>;
template<class Read, class Write, class Vec>
    using BoolHeterogeneTailleColorColonne = HeterogeneTailleAdaptColonne<BoolColorColonne<Read,Write,Vec>>;
template<class T> using VectorListBoolHeterogeneTailleColorColonne = HeterogeneTailleAdaptColonne<VectorListBoolColorColonne<T>>;
template<class T> using VectorPtrBoolHeterogeneTailleColorColonne = HeterogeneTailleAdaptColonne<VectorPtrBoolColorColonne<T>>;
template<class T> using VectorRefBoolHeterogeneTailleColorColonne = HeterogeneTailleAdaptColonne<VectorRefBoolColorColonne<T>>;
template<class T> using VectorValBoolHeterogeneTailleColorColonne = HeterogeneTailleAdaptColonne<VectorValBoolColorColonne<T>>;
// HeterogeneTailleColorId
template<class Back, class Fore, class Read, class Write, class Vec>
    using TempBoolHeterogeneTailleColorIdColonne = HeterogeneTailleAdaptColonne<TempBoolColorIdColonne<Back, Fore, Read, Write, Vec>>;
template<class Read, class Write, class Vec>
    using BoolHeterogeneTailleColorIdColonne = HeterogeneTailleAdaptColonne<BoolColorIdColonne<Read,Write,Vec>>;
template<class T> using VectorPtrBoolHeterogeneTailleColorIdColonne = HeterogeneTailleAdaptColonne<VectorPtrBoolColorIdColonne<T>>;
template<class T> using VectorRefBoolHeterogeneTailleColorIdColonne = HeterogeneTailleAdaptColonne<VectorRefBoolColorIdColonne<T>>;
template<class T> using VectorValBoolHeterogeneTailleColorIdColonne = HeterogeneTailleAdaptColonne<VectorValBoolColorIdColonne<T>>;
}
#endif // COLONNESFORMODEL_H
