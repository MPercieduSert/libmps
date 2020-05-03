/*Auteur: PERCIE DU SERT Maxime
 *Date: 24/04/2020
 */
#ifndef FENFLAGS_H
#define FENFLAGS_H

#include <QFlags>

namespace fenMPS {

//! Enumeration des actions
enum actionFlags {NoActions = 0x0,
                 CopierAction = 0x1,
                 CollerAction = 0x2,
                 CouperAction = 0x4,
                 EffacerAction = 0x8,
                 EditerActions = 0xF,
                 SauverAction = 0x10,
                 ChercherAction = 0x12};

Q_DECLARE_FLAGS(Action,actionFlags)
}
Q_DECLARE_OPERATORS_FOR_FLAGS(fenMPS::Action)
#endif // FENFLAGS_H
