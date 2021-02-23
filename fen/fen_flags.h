/*Auteur: PERCIE DU SERT Maxime
 *Date: 24/04/2020
 */
#ifndef FEN_FLAGS_H
#define FEN_FLAGS_H

#include "type_mps.h"

/*! \defgroup groupe_fen Fenêtre
 *\brief Ensemble de classes des fenêtres.
 */

namespace mps {
/*! \ingroup groupe_fen
 *\brief Espace de noms des fenêtres.
 */
namespace fenetre {

//! Enumeration des actions
enum action_flags : flag::flag_type {No_Action = 0x0,
                 Copier_Action = 0x1,
                 Coller_Action = 0x2,
                 Couper_Action = 0x4,
                 Effacer_Action = 0x8,
                 Supprimer_Action = 0x10,
                 Editer_Action = 0x1F,
                 Sauver_Action = 0x20,
                 Sauver_Comme_Action = 0x40,
                 Chercher_action = 0x80};
}}
#endif // FEN_FLAGS_H
