/*Auteur: PERCIE DU SERT Maxime
 *Date: 23/07/2018
 */
#ifndef MACRO_MPS_H
#define MACRO_MPS_H

/*! \defgroup groupeMacroBase Macro de base
 *\brief Ensemble des macros de base.
 */
//! \ingroup groupeMacro
//! Macro déclarant par défaut les constructeurs et les opérateurs de copie et de déplacement.
#define CONSTR_AFFECT_DEFAUT(T) /*! Constructeur de recopie.*/ \
    T(const T &) = default; \
    /*! Constructeur de déplacement.*/ \
    T(T &&) = default; \
    /*! Affectation par copie.*/ \
    T&operator = (const T &) = default; \
    /*! Affectation par déplacement.*/ \
    T&operator = (T &&) = default;

//! \ingroup groupeMacro
//! Macro déclarant le constructeur par défaut.
#define CONSTR_DEFAUT(T) /*! Constructeur par deffaut.*/ \
    T() = default;

//! \ingroup groupeMacro
//! Macro déclarant le destructeur vide.
#define DESTR_VIDE(T) /*! Destructeur vide.*/ \
    ~T() = default;

//! \ingroup groupeMacro
//! Macro définisant le destructeur vide.
#define DESTR_VIDE_DEF(T) T::~T() = default;

#endif // MACROLIBMPS_H
