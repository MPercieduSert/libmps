/*Auteur: PERCIE DU SERT Maxime
 *Date: 29/11/2019
 */
#ifndef TYPE_MPS_H
#define TYPE_MPS_H

//! \ingroup groupe_attribut_entity
//! Type des attributs
namespace mps {
//! Type des identifiants.
using idt = unsigned;
//! Type des tailles et indices de veteur.
using szt = long unsigned;
//! Type des enumération.
using enumt = unsigned;
//! Type des numéros non signé.
using numt = unsigned;
//! Type de position dans les entités.
using post = unsigned;
//! Type de l'identifiant des entités.
using entidt = unsigned;
class flag {
public:
    //! Type de la valuer du drapeau.
    using flag_type = enumt;
protected:
    flag_type m_value = 0;        //! Entier codant 32 booléen.
public:
    //! Constructeur par defaut.
    flag() = default;

    //! Constructeur.
    flag(flag_type value) noexcept : m_value(value) {}

    //! Teste si la valeur est nulle.
    bool operator !() const noexcept
        {return !m_value;}

    //! Revoie un drapeaux correspondant à l'intersection.
    flag operator &(flag_type value) const noexcept
        {return m_value &value;}

    //! Revoie un drapeaux correspondant à l'intersection.
    flag operator &(flag f) const noexcept
        {return m_value &f.m_value;}

    //! Revoie un drapeaux correspondant à l'union.
    flag operator | (flag_type value) const noexcept
        {return m_value | value;}

    //! Revoie un drapeaux correspondant à l'union.
    flag operator | (flag f) const noexcept
        {return m_value | f.m_value;}

    //! Revoie un drapeaux correspondant à un ou exclusif.
    flag operator ^ (flag_type value) const noexcept
        {return m_value ^ value;}

    //! Revoie un drapeaux correspondant à un ou exclusif.
    flag operator ^ (flag f) const noexcept
        {return m_value ^ f.m_value;}

    //! Revoie un drapeaux correspondant à la négation.
    flag operator ~ () const noexcept
        {return ~m_value;}

    //! Revoie un drapeaux correspondant à l'intersection.
    flag &operator &= (flag_type value) noexcept {
        m_value &= value;
        return *this;}

    //! Revoie un drapeaux correspondant à l'intersection.
    flag &operator &= (flag f) noexcept {
        m_value &= f.m_value;
        return *this;}

    //! Revoie un drapeaux correspondant à l'union.
    flag &operator |= (flag_type value) noexcept {
        m_value |= value;
        return *this;
    }

    //! Revoie un drapeaux correspondant à l'union.
    flag &operator |= (flag f) noexcept {
        m_value |= f.m_value;
        return *this;
    }

    //! Revoie un drapeaux correspondant à un ou exclusif.
    flag operator ^= (flag_type value) noexcept {
        m_value ^= value;
        return *this;
    }

    //! Revoie un drapeaux correspondant à un ou exclusif.
    flag operator ^= (flag f) noexcept {
        m_value ^= f.m_value;
        return *this;
    }

    //! Conversion.
    operator flag_type() const noexcept
        {return m_value;}

    //! Conversion.
    operator bool() const noexcept
        {return m_value;}

    //! Conversion.
    flag &operator =(flag_type value) noexcept {
        m_value = value;
        return *this;
    }

    //! Test d'égalité.
    bool operator ==(const flag &value) const
        {return m_value == value.m_value;}

    //! Test d'égalité.
    bool operator ==(flag_type  value) const
        {return m_value == value;}

    //! Test de différence.
    bool operator !=(const flag &value) const
        {return m_value != value.m_value;}

    //! Test de différence.
    bool operator !=(flag_type  value) const
        {return m_value != value;}

    //! Mutateur de la valeur.
    void set_value(flag_type value) noexcept
        {m_value = value;}

    //! Mutateur de la valeur.
    void set_value(flag_type value, bool ok) noexcept
        {m_value = ok ? m_value | value : m_value ^ value;}

    //! Teste si l'intersection n'est pas vide.
    bool test(flag_type value) const noexcept
        {return m_value & value;}

    //! Accesseur de la valeur.
    flag_type value() const noexcept
        {return m_value;}
};
}
#endif // TYPE_MPS_H
