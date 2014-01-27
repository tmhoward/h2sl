/**
 * @file    word.h
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
 * The interface for a class used to describe a word
 */

#ifndef H2SL_WORD_H
#define H2SL_WORD_H

#include <iostream>
#include <libxml/tree.h>

#include <h2sl/pos.h>

namespace h2sl {
  class Word {
  public:
    Word( const pos_t& pos = POS_UNKNOWN, const std::string& text = "na" );
    virtual ~Word();
    Word( const Word& other );
    Word& operator=( const Word& other );
    bool operator==( const Word& other )const;
    bool operator!=( const Word& other )const;

    void to_xml( const std::string& filename )const;
    void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    void from_xml( const std::string& filename );
    void from_xml( xmlNodePtr root );

    inline pos_t& pos( void ){ return _pos; };
    inline const pos_t& pos( void )const{ return _pos; };
    inline std::string& text( void ){ return _text; };
    inline const std::string& text( void )const{ return _text; }; 

  protected:
    pos_t _pos;
    std::string _text;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Word& other );
}

#endif /* H2SL_WORD_H */
