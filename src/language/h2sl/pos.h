/**
 * @file    pos.h
 * @author  Thomas M. Howard (tmhoward@csail.mit.edu)
 *          Matthew R. Walter (mwalter@csail.mit.edu)
 * @version 1.0
 *
 * @section LICENSE
 *
 * This file is part of h2sl.
 *
 * Copyright (C) 2014 by the Massachusetts Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <http://www.gnu.org/licenses/gpl-2.0.html> or write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * @section DESCRIPTION
 *
 * The interface for a class used to represent different parts of speech
 */

#ifndef H2SL_POS_H
#define H2SL_POS_H

#include <iostream>
#include <cstring>

namespace h2sl {
  typedef enum {
    POS_UNKNOWN,
    POS_CC,
    POS_CD,
    POS_DT,
    POS_EX,
    POS_FW,
    POS_IN,
    POS_JJ,
    POS_JJR,
    POS_JJS,
    POS_LS,
    POS_MD,
    POS_NN,
    POS_NNS,
    POS_NNP,
    POS_NNPS,
    POS_PDT,
    POS_POS,
    POS_PRP,
    POS_PRP$,
    POS_RB,
    POS_RBR,
    POS_RBS,
    POS_RP,
    POS_SYM,
    POS_TO,
    POS_UH,
    POS_VB,
    POS_VBD,
    POS_VBG,
    POS_VBN,
    POS_VBP,
    POS_VBZ,
    POS_WDT,
    POS_WP,
    POS_WP$,
    POS_WRB,
    NUM_POS_TAGS
  } pos_t;

  inline std::string pos_t_to_std_string( const pos_t& pos ){
    switch( pos ){
    case( POS_CC ):
      return "CC";
    case( POS_CD ):
      return "CD";
    case( POS_DT ):
      return "DT";
    case( POS_EX ):
      return "EX";
    case( POS_FW ):
      return "FW";
    case( POS_IN ):
      return "IN";
    case( POS_JJ ):
      return "JJ";
    case( POS_JJR ):
      return "JJR";
    case( POS_JJS ):
      return "JJS";
    case( POS_LS ):
      return "LS";
    case( POS_MD ):
      return "MD";
    case( POS_NN ):
      return "NN";
    case( POS_NNS ):
      return "NNS";
    case( POS_NNP ):
      return "NNP";
    case( POS_NNPS ):
      return "NNPS";
    case( POS_PDT ):
      return "PDT";
    case( POS_POS ):
      return "POS";
    case( POS_PRP ):
      return "PRP";
    case( POS_PRP$ ):
      return "PRPD";
    case( POS_RB ):
      return "RB";
    case( POS_RBR ):
      return "RBR";
    case( POS_RBS ):
      return "RBS";
    case( POS_RP ):
      return "RP";
    case( POS_SYM ):
      return "SYM";
    case( POS_TO ):
      return "TO";
    case( POS_UH ):
      return "UH";
    case( POS_VB ):
      return "VB";
    case( POS_VBD ):
      return "VBD";
    case( POS_VBG ):
      return "VBG";
    case( POS_VBN ):
      return "VBN";
    case( POS_VBP ):
      return "VBP";
    case( POS_VBZ ):
      return "VBZ";
    case( POS_WDT ):
      return "WDT";
    case( POS_WP ):
      return "WP";
    case( POS_WP$ ):
      return "WPD";
    case( POS_WRB ):
      return "WRB";
    case( NUM_POS_TAGS ):
    default:
      return "POS_UNKNOWN";
    };
  };
  
  inline pos_t pos_t_from_std_string( const std::string& arg ){
    for( unsigned int i = 0; i < NUM_POS_TAGS; i++ ){
      if( strcmp( arg.c_str(), pos_t_to_std_string( ( pos_t )( i ) ).c_str() ) == 0 ){
        return ( pos_t )( i );
      }
    }
    return POS_UNKNOWN;
  }; 
}

#endif /* H2SL_POS_H */
