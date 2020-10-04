#include "NumToTexte.h"

using namespace diversMPS;

const std::map<int,QString> NumToTexte::m_romain{   {1000,"M"},
                                                    {900,"CM"},
                                                    {500,"D"},
                                                    {400,"CD"},
                                                    {100,"C"},
                                                    {90,"XC"},
                                                    {50,"L"},
                                                    {40,"XL"},
                                                    {10,"X"},
                                                    {9,"IX"},
                                                    {5,"V"},
                                                    {4,"IV"},
                                                    {1,"I"}};

void NumToTexte::setStyle(uint style){
    m_style = style;
    switch (style) {
    case Arabe:
        m_toString = arabe;
        break;
    case Majuscule:
        m_toString = majuscule;
        break;
    case Romain:
        m_toString = romain;
        break;
    case Minuscule:
        m_toString = minuscule;
        break;
    case GrecMinuscule:
        m_toString = grecMinuscule;
        break;
    case GrecMajuscule:
        m_toString = grecMajuscule;
        break;
    default:
        m_toString = arabe;
    }
}

QString NumToTexte::romain(int num) {
    num += 1;
    QString str;
    for (auto iter = m_romain.cbegin(); num; ++iter) {
        while (num >= iter->first) {
            str.append(iter->second);
            num -= iter->first;
        }
    }
    return str;
}
