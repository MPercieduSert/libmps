/*Auteur: PERCIE DU SERT Maxime
 *Date: 15/09/2020
 */
#ifndef COLONNESFORMODEL_H
#define COLONNESFORMODEL_H

#include "abstract_colonnes_model.h"

namespace mps {
namespace model_base {
//////////////////////////////////////////////////////// base_colonne /////////////////////////////////////////
/*! \ingroup groupe_model
 *\brief Classe template générique d'une colonne.
 */
template<class Read, class Find, class Write, class Vec> class base_colonne_temp
        : public abstract_colonnes_model::abstract_colonne {
public:
    using data_type = Vec;
protected:
    Read m_get;         //!< Fonction lisant la donnée du model.
    Write m_set;        //!< Fonction modifiant la donnée du model.
    Find m_test;        //!< Fonction lisant la donnée du model pour les tests de recherche.
    data_type &m_vec;   //!< Référence sur le vecteur de donnée.
public:
    //! Constructeur.
    base_colonne_temp(const QString &name, Qt::ItemFlags flags, int type,
                 data_type &vec,
                 const Read &get,
                 const Find &test,
                 const Write &set)
        : abstract_colonne(name,flags,type), m_get(get), m_set(set), m_test(test), m_vec(vec) {}

    //! Accesseur de la donnée d'indice id dans la colonne.
    QVariant data(szt id, int role) const override
        {return m_get(m_vec[id], role);}

    //! Accesseut de la donnée pour le test de recherche.
    QVariant data_test(szt id) const
        {return m_test(m_vec[id]);}

    //! Mutateur de la donnée d'indice id dans la colonne.
    bool set_data(szt id, const QVariant &value,  int role) override
        {return m_set(m_vec[id],value,role);}
};

template<class Read, class Write, class Vec> using base_colonne = base_colonne_temp<QVariant (*)(Read, int),
                                                                                 QVariant (*)(Read),
                                                                                 bool (*)(Write,const QVariant &,int),
                                                                                 Vec>;
template<class T> using vector_list_colonne = base_colonne<const std::list<T>&, std::list<T>&, std::vector<std::list<T>>>;
template<class T> using vector_ptr_colonne = base_colonne<const T&, T&, vector_ptr<T>>;
template<class T> using vector_ref_colonne = base_colonne<const T&, T&, std::vector<T>>;
template<class T> using vector_val_colonne = base_colonne<T, T, std::vector<T>>;

//////////////////////////////////////////////////////////// bool_colonne ////////////////////////////////
/*! \ingroup groupe_model
 *\brief Classe Abstraite mère des colonnes de type booléen.
 */
class abstract_bool_colonne : public abstract_colonnes_model::abstract_colonne {
protected:
    QString m_false_label;               //!< Label du booléen faux.
    QString m_true_label;                //!< Label du booléen vraie.
public:
    //! Constructeur.
    abstract_bool_colonne(const QString &name, Qt::ItemFlags flags, int type,
                const QString &true_label = QString(), const QString &false_label = QString())
        : abstract_colonne(name,flags,type),
          m_false_label(false_label), m_true_label(true_label) {}

    //! Destructeur.
    ~abstract_bool_colonne() override;

    //! Accesseur du label pour faux.
    const QString &false_label() const
        {return m_false_label;}

    //! Mutateur du label pour faux.
    void set_false_label(const QString &false_label)
        {m_false_label = false_label;}

    //! Mutateur du label pour vraie.
    void set_true_label(const QString &true_label)
        {m_true_label = true_label;}

    //! Accesseur du label pour vraie.
    const QString &true_label() const
        {return m_true_label;}
};

/*! \ingroup groupe_model
 *\brief Classe mère des colonnes de type booléen.
 */
template<class Read, class Write, class Vec> class bool_colonne_temp : public abstract_bool_colonne  {
public:
    using data_type = Vec;
protected:
    Read m_get;         //!< Fonction lisant la donnée du model.
    Write m_set;        //!< Fonction modifiant la donnée du model.
    data_type &m_vec;   //!< Référence sur le vecteur de donnée.

public:
    //! Constructeur.
    bool_colonne_temp(const QString &name, Qt::ItemFlags flags, int type,
                data_type &vec,
                const Read &get,
                const Write &set,
                const QString &true_label = QString(), const QString &false_label = QString())
        : abstract_bool_colonne(name,flags,type,true_label,false_label),
          m_get(get), m_set(set), m_vec(vec) {}

    //! Accesseur de la donnée d'indice id dans la colonne.
    QVariant data(szt id, int role) const override {
        if(role == Qt::CheckStateRole)
            return m_get(m_vec[id]) ? Qt::Checked : Qt::Unchecked;
        if(role == Qt::DisplayRole)
            return m_get(m_vec[id]) ? m_true_label : m_false_label;
        return QVariant();
    }

    //! Accesseut de la donnée pour le test de recherche.
    QVariant data_test(szt id) const
        {return m_get(m_vec[id]);}

    //! Mutateur de la donnée d'indice id dans la colonne.
    bool set_data(szt id, const QVariant &value,  int role) override {
        if(role == Qt::CheckStateRole) {
            m_set(m_vec[id],value.toBool());
            return true;}
        return false;
    }
};

template<class Read, class Write, class Vec> using bool_colonne = bool_colonne_temp<bool (*)(Read),void (*)(Write,bool),Vec>;
template<class T> using vector_list_bool_colonne = bool_colonne<const std::list<T>&, std::list<T>&, std::vector<std::list<T>>>;
template<class T> using vector_ptr_bool_colonne = bool_colonne<const T&,T&, vector_ptr<T>>;
template<class T> using vector_ref_bool_colonne = bool_colonne<const T&,T&, std::vector<T>>;
template<class T> using vector_val_bool_colonne = bool_colonne<T,T, std::vector<T>>;

////////////////////////////////////////////////////////// IdAdapter ///////////////////////////////////////
/*! \ingroup groupe_model
 *\brief Adaptateur de colonne pour les identifiants des entités.
 */
template<class Colonne> class id_adapt_colonne : public Colonne {
protected:
    using Colonne::m_vec;
public:
    //! Constructeur.
    using Colonne::Colonne;

    //! Accesseur de la donnée d'indice id dans la colonne.
    QVariant data(szt id, int role) const override {
        if(role == abstract_colonnes_model::Id_Role)
            return m_vec[id].id();
        return Colonne::data(id,role);
    }


    //! Mutateur de la donnée d'indice id dans la colonne.
    bool set_data(szt id, const QVariant &value,  int role) override {
        if(role == abstract_colonnes_model::Id_Role) {
            m_vec[id].set_id(value.toUInt());
            return true;
        }
        return Colonne::set_data(id,value,role);
    }
};

// Id
template<class Read, class Find, class Write, class Vec> using base_id_colonne_temp
    = id_adapt_colonne<base_colonne_temp<Read, Find, Write, Vec>>;
template<class Read, class Write, class Vec> using base_id_colonne = id_adapt_colonne<base_colonne<Read,Write,Vec>>;
template<class T> using vector_ptr_id_colonne = id_adapt_colonne<vector_ptr_colonne<T>>;
template<class T> using vector_ref_id_colonne = id_adapt_colonne<vector_ref_colonne<T>>;
template<class T> using vector_val_id_colonne = id_adapt_colonne<vector_val_colonne<T>>;
// BoolId
template<class Read, class Write, class Vec> using bool_id_colonne_temp = id_adapt_colonne<bool_colonne_temp<Read, Write, Vec>>;
template<class Read, class Write, class Vec> using bool_id_colonne = id_adapt_colonne<bool_colonne<Read, Write, Vec>>;
template<class T> using vector_ref_bool_id_colonne = id_adapt_colonne<vector_ref_bool_colonne<T>>;
template<class T> using vector_ptr_bool_id_colonne = id_adapt_colonne<vector_ptr_bool_colonne<T>>;
template<class T> using vector_val_bool_id_colonne = id_adapt_colonne<vector_val_bool_colonne<T>>;

////////////////////////////////////////////////////////// ColorAdapter ///////////////////////////////////////
/*! \ingroup groupe_model
 *\brief Adaptateur de colonne pour les couleur.
 */
template<class Back, class Fore, class Colonne> class color_adapt_colonne_temp : public Colonne {
protected:
    Back m_back;                        //!< Fonction donnant l'indice de rendu du fond.
    Fore m_fore;                        //!< Fonction donnant l'indice de rendu du texte.
    std::vector<QBrush> m_back_vec;      //!< Vecteur des rendus du fond.
    std::vector<QBrush> m_fore_vec;      //!< Vecteur des rendus du texte.
    using Colonne::m_vec;
public:
    //! Constructeur.
    template<class... Args> color_adapt_colonne_temp(const QString &name, Qt::ItemFlags flags, int type,
                                                  typename Colonne::data_type &vec,
                                                  const Back &back, const Fore &fore, const Args &... args)
        : Colonne(name, flags, type, vec, args...), m_back(back), m_fore(fore) {}

    //! Constructeur.
    template<class... Args> color_adapt_colonne_temp(const QString &name, Qt::ItemFlags flags, int type,
                                                  typename Colonne::data_type &vec,
                                                  const Back &back, const Fore &fore,
                                                  const std::vector<QBrush> &back_vec, const std::vector<QBrush> &fore_vec,
                                                  Args &&... args)
        : Colonne(name,flags,type,vec,args...), m_back(back), m_fore(fore), m_back_vec(back_vec), m_fore_vec(fore_vec) {}

    //! Accesseur du vecteur des rendus du fond.
    const std::vector<QBrush> &back_ground_vector() const
        {return m_back_vec;}

    //! Accesseur du vecteur des rendus du fond.
    std::vector<QBrush> &back_ground_vector()
        {return m_back_vec;}

    //! Accesseur de la donnée d'indice id dans la colonne.
    QVariant data(szt id, int role) const override;

    //! Accesseur du vecteur des rendus du texte
    const std::vector<QBrush> &fore_ground_vector() const
        {return m_fore_vec;}

    //! Accesseur du vecteur des rendus du texte
    std::vector<QBrush> &fore_ground_vector()
        {return m_fore_vec;}

    //! Mutateur du vecteur des rendus du fond.
    void set_back_ground_vector(const std::vector<QBrush> &vec)
        {m_back_vec = vec;}

    //! Mutateur du vecteur des rendus du fond.
    void set_fore_ground_vector(const std::vector<QBrush> &vec)
        {m_fore_vec = vec;}
};

template<class Back, class Fore, class Colonne>
        QVariant color_adapt_colonne_temp<Back, Fore, Colonne>::data(szt id, int role) const {
    if(role == Qt::BackgroundRole) {
        auto num = m_back(m_vec[id]);
        if(num < m_back_vec.size())
            return m_back_vec[num];
        else
            return QVariant();
    }
    if(role == Qt::ForegroundRole) {
        auto num = m_fore(m_vec[id]);
        if(num < m_fore_vec.size())
            return m_fore_vec[num];
        else
            return QVariant();
    }
    return Colonne::data(id,role);
}

template<class Read, class Colonne> using color_adapt_colonne = color_adapt_colonne_temp<szt(*)(Read), szt(*)(Read), Colonne>;

// Color
template<class Back, class Fore, class Read, class Find, class Write, class Vec> using base_color_colonne_temp
    = color_adapt_colonne_temp<Back, Fore, base_colonne_temp<Read, Find, Write, Vec>>;
template<class Read, class Write, class Vec> using base_color_colonne
    = color_adapt_colonne<Read, base_colonne<Read,Write,Vec>>;
template<class T> using vector_list_color_colonne = color_adapt_colonne<const std::list<T>&, vector_list_colonne<T>>;
template<class T> using vector_ptr_color_colonne = color_adapt_colonne<const T&, vector_ptr_colonne<T>>;
template<class T> using vector_ref_color_colonne = color_adapt_colonne<const T&, vector_ref_colonne<T>>;
template<class T> using vector_val_color_colonne = color_adapt_colonne<T,vector_val_colonne<T>>;
// ColorId
template<class Back, class Fore, class Read, class Find, class Write, class Vec> using base_color_id_colonne_temp
    = color_adapt_colonne_temp<Back, Fore, base_id_colonne_temp<Read, Find, Write, Vec>>;
template<class Read, class Write, class Vec> using base_color_id_colonne
    = color_adapt_colonne<Read, base_id_colonne<Read,Write,Vec>>;
template<class T> using vector_ptr_color_id_colonne = color_adapt_colonne<const T&, vector_ptr_id_colonne<T>>;
template<class T> using vector_ref_color_id_colonne = color_adapt_colonne<const T&, vector_ref_id_colonne<T>>;
template<class T> using vector_val_color_id_colonne = color_adapt_colonne<T,vector_val_id_colonne<T>>;
// BoolColor
template<class Back, class Fore, class Read, class Write, class Vec>
    using bool_color_colonne_temp = color_adapt_colonne_temp<Back, Fore, bool_colonne_temp<Read, Write, Vec>>;
template<class Read, class Write, class Vec>
    using bool_color_colonne = color_adapt_colonne<Read, bool_colonne<Read, Write, Vec>>;
template<class T> using vector_list_bool_color_colonne = color_adapt_colonne<const std::list<T>&, vector_list_bool_colonne<T>>;
template<class T> using vector_ptr_bool_color_colonne = color_adapt_colonne<const T&, vector_ptr_bool_colonne<T>>;
template<class T> using vector_ref_bool_color_colonne = color_adapt_colonne<const T&,vector_ref_bool_colonne<T>>;
template<class T> using vector_val_bool_color_colonne = color_adapt_colonne<T, vector_val_bool_colonne<T>>;
// BoolColorId
template<class Back, class Fore, class Read, class Write, class Vec> using bool_color_id_colonne_temp
    = color_adapt_colonne_temp<Back, Fore, bool_id_colonne_temp<Read, Write, Vec>>;
template<class Read, class Write, class Vec> using bool_color_id_colonne
    = color_adapt_colonne<Read, bool_id_colonne<Read, Write, Vec>>;
template<class T> using vector_ptr_bool_color_id_colonne = color_adapt_colonne<const T&, vector_ptr_bool_id_colonne<T>>;
template<class T> using vector_ref_bool_color_id_colonne = color_adapt_colonne<const T&,vector_ref_bool_id_colonne<T>>;
template<class T> using vector_val_bool_color_id_colonne = color_adapt_colonne<T, vector_val_bool_id_colonne<T>>;

///////////////////////////////////////////////////// heterogene_taille_adapt_colonne //////////////////
/*! \ingroup groupe_model
 *\brief Adaptateur de colonne pour un tableau de taille hétérogéne.
 */
template<class Colonne> class heterogene_taille_adapt_colonne : public Colonne {
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
    QVariant data_test(szt id) const override {
        if(id < m_vec.size())
            return Colonne::data_test(id);
        else
            return QVariant();
    }

    //! Mutateur de la donnée d'indice id dans la colonne.
    bool set_data(szt id, const QVariant &value,  int role) override {
        if(id >= m_vec.size())
            m_vec.resize(id + 1);
        return Colonne::set_data(id,value,role);
    }
};

// HeterogeneTaille
template<class Read, class Find, class Write, class Vec> using base_heterogene_taille_colonne_temp
    = heterogene_taille_adapt_colonne<base_colonne_temp<Read, Find, Write, Vec>>;
template<class Read, class Write, class Vec> using base_heterogene_taille_colonne
    = heterogene_taille_adapt_colonne<base_colonne<Read,Write,Vec>>;
template<class T> using vector_list_heterogene_taille_colonne = heterogene_taille_adapt_colonne<vector_list_colonne<T>>;
template<class T> using vector_ptr_heterogene_taille_colonne = heterogene_taille_adapt_colonne<vector_ptr_colonne<T>>;
template<class T> using vector_ref_heterogene_taille_colonne = heterogene_taille_adapt_colonne<vector_ref_colonne<T>>;
template<class T> using vector_val_heterogene_taille_colonne = heterogene_taille_adapt_colonne<vector_val_colonne<T>>;
// HeterogeneTaille_Id
template<class Read, class Find, class Write, class Vec> using base_heterogene_taille_id_colonne_temp
    = heterogene_taille_adapt_colonne<base_id_colonne_temp<Read, Find, Write, Vec>>;
template<class Read, class Write, class Vec> using base_heterogene_taille_id_colonne
    = heterogene_taille_adapt_colonne<base_id_colonne<Read,Write,Vec>>;
template<class T> using vector_ptr_heterogene_taille_id_colonne = heterogene_taille_adapt_colonne<vector_ptr_id_colonne<T>>;
template<class T> using vector_ref_heterogene_taille_id_colonne = heterogene_taille_adapt_colonne<vector_ref_id_colonne<T>>;
template<class T> using vector_val_heterogene_taille_id_colonne = heterogene_taille_adapt_colonne<vector_val_id_colonne<T>>;
// HeterogeneTailleColor
template<class Back, class Fore, class Read, class Find, class Write, class Vec> using base_heterogene_taille_color_colonne_temp
    = heterogene_taille_adapt_colonne<base_color_colonne_temp<Back, Fore, Read, Find, Write, Vec>>;
template<class Read, class Write, class Vec> using base_heterogene_taille_color_colonne
    = heterogene_taille_adapt_colonne<base_color_colonne<Read,Write,Vec>>;
template<class T> using vector_list_heterogene_taille_color_colonne
    = heterogene_taille_adapt_colonne<vector_list_color_colonne<T>>;
template<class T> using vector_ptr_heterogene_taille_color_colonne = heterogene_taille_adapt_colonne<vector_ptr_color_colonne<T>>;
template<class T> using vector_ref_heterogene_taille_color_colonne = heterogene_taille_adapt_colonne<vector_ref_color_colonne<T>>;
template<class T> using vector_val_heterogene_taille_color_colonne = heterogene_taille_adapt_colonne<vector_val_color_colonne<T>>;
// HeterogeneTailleColorId
template<class Back, class Fore, class Read, class Find, class Write, class Vec> using base_heterogene_taille_color_id_colonne_temp
    = heterogene_taille_adapt_colonne<base_color_id_colonne_temp<Back, Fore, Read, Find, Write, Vec>>;
template<class Read, class Write, class Vec> using base_heterogene_taille_color_id_colonne
    = heterogene_taille_adapt_colonne<base_color_id_colonne<Read,Write,Vec>>;
template<class T> using vector_ptr_heterogene_taille_color_id_colonne
    = heterogene_taille_adapt_colonne<vector_ptr_color_id_colonne<T>>;
template<class T> using vector_ref_heterogene_taille_color_id_colonne
    = heterogene_taille_adapt_colonne<vector_ref_color_id_colonne<T>>;
template<class T> using vector_val_heterogene_taille_color_id_colonne
    = heterogene_taille_adapt_colonne<vector_val_color_id_colonne<T>>;
// BooleHeterogeneTaille
template<class Read, class Write, class Vec> using bool_heterogene_taille_colonne_temp
    = heterogene_taille_adapt_colonne<bool_colonne_temp<Read, Write, Vec>>;
template<class Read, class Write, class Vec> using bool_heterogene_taille_colonne
    = heterogene_taille_adapt_colonne<bool_colonne<Read,Write,Vec>>;
template<class T> using vector_list_bool_heterogene_taille_colonne = heterogene_taille_adapt_colonne<vector_list_bool_colonne<T>>;
template<class T> using vector_ptr_bool_heterogene_taille_colonne = heterogene_taille_adapt_colonne<vector_ptr_bool_colonne<T>>;
template<class T> using vector_ref_bool_heterogene_taille_colonne = heterogene_taille_adapt_colonne<vector_ref_bool_colonne<T>>;
template<class T> using vector_val_bool_heterogene_taille_colonne = heterogene_taille_adapt_colonne<vector_val_bool_colonne<T>>;
// BoolHeterogeneTaille_Id
template<class Read, class Write, class Vec> using bool_heterogene_taille_id_colonne_temp
    = heterogene_taille_adapt_colonne<bool_id_colonne_temp<Read, Write, Vec>>;
template<class Read, class Write, class Vec> using bool_heterogene_taille_id_colonne
    = heterogene_taille_adapt_colonne<bool_id_colonne<Read,Write,Vec>>;
template<class T> using vector_ptr_bool_heterogene_taille_id_colonne
    = heterogene_taille_adapt_colonne<vector_ptr_bool_id_colonne<T>>;
template<class T> using vector_ref_bool_heterogene_taille_id_colonne
    = heterogene_taille_adapt_colonne<vector_ref_bool_id_colonne<T>>;
template<class T> using vector_val_bool_heterogene_taille_id_colonne
    = heterogene_taille_adapt_colonne<vector_val_bool_id_colonne<T>>;
// HeterogeneTailleColor
template<class Back, class Fore, class Read, class Write, class Vec> using bool_heterogene_taille_color_colonne_temp
    = heterogene_taille_adapt_colonne<bool_color_colonne_temp<Back, Fore, Read, Write, Vec>>;
template<class Read, class Write, class Vec> using bool_heterogene_taille_color_colonne
    = heterogene_taille_adapt_colonne<bool_color_colonne<Read,Write,Vec>>;
template<class T> using vector_list_bool_heterogene_taille_color_colonne
    = heterogene_taille_adapt_colonne<vector_list_bool_color_colonne<T>>;
template<class T> using vector_ptr_bool_heterogene_taille_color_colonne
    = heterogene_taille_adapt_colonne<vector_ptr_bool_color_colonne<T>>;
template<class T> using vector_ref_bool_heterogene_taille_color_colonne
    = heterogene_taille_adapt_colonne<vector_ref_bool_color_colonne<T>>;
template<class T> using vector_val_bool_heterogene_taille_color_colonne
    = heterogene_taille_adapt_colonne<vector_val_bool_color_colonne<T>>;
// HeterogeneTailleColorId
template<class Back, class Fore, class Read, class Write, class Vec> using bool_heterogene_taille_color_id_colonne_temp
    = heterogene_taille_adapt_colonne<bool_color_id_colonne_temp<Back, Fore, Read, Write, Vec>>;
template<class Read, class Write, class Vec> using bool_heterogene_taille_color_id_colonne
    = heterogene_taille_adapt_colonne<bool_color_id_colonne<Read,Write,Vec>>;
template<class T> using vector_ptr_bool_heterogene_taille_color_id_colonne
    = heterogene_taille_adapt_colonne<vector_ptr_bool_color_id_colonne<T>>;
template<class T> using vector_ref_bool_heterogene_taille_color_id_colonne
    = heterogene_taille_adapt_colonne<vector_ref_bool_color_id_colonne<T>>;
template<class T> using vector_val_bool_heterogene_taille_color_id_colonne
    = heterogene_taille_adapt_colonne<vector_val_bool_color_id_colonne<T>>;
}}
#endif // COLONNESFORMODEL_H
