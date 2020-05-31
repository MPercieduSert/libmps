/*Auteur: PERCIE DU SERT Maxime
 *Date: 29/11/2019
 */
#ifndef TYPEMPS_H
#define TYPEMPS_H

//! \ingroup groupeAttributEntity
//! Type des attributs
namespace typeMPS {
using idt = unsigned;
using codeType = unsigned;
using szt = unsigned;

class flag {
public:
    //! Type de la valuer du drapeau.
    using flag_type = unsigned;
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
    flag operator & (flag_type value) const noexcept
        {return m_value & value;}

    //! Revoie un drapeaux correspondant à l'union.
    flag operator | (flag_type value) const noexcept
        {return m_value | value;}

    //! Revoie un drapeaux correspondant à un ou exclusif.
    flag operator ^ (flag_type value) const noexcept
        {return m_value ^ value;}

    //! Revoie un drapeaux correspondant à un ou exclusif.
    flag operator ~ () const noexcept
        {return ~m_value;}

    //! Revoie un drapeaux correspondant à l'intersection.
    flag & operator &= (flag_type value) noexcept {
        m_value &= value;
        return *this;}

    //! Revoie un drapeaux correspondant à l'union.
    flag & operator |= (flag_type value) noexcept {
        m_value |= value;
        return *this;
    }

    //! Revoie un drapeaux correspondant à un ou exclusif.
    flag operator ^= (flag_type value) noexcept {
        m_value ^= value;
        return *this;
    }

    //! Conversion.
    operator flag_type() const noexcept
        {return m_value;}

    //! Conversion.
    flag & operator =(flag_type value) noexcept {
        m_value = value;
        return *this;
    }

    //! Mutateur de la valeur.
    void setValue(flag_type value) noexcept
        {m_value = value;}

    //! Teste si l'intersection n'est pas vide.
    bool test(flag_type value) const noexcept
        {return m_value & value;}

    //! Accesseur de la valeur.
    flag_type value() const noexcept
        {return m_value;}
};
}

#endif // TYPEMPS_H
