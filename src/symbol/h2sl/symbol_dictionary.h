/**
 * @file    symbol_dictionary.h
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
 * A symbol dictionary for loading the symbolic representation in DCG
 */

#ifndef H2SL_SYMBOL_DICTIONARY_H
#define H2SL_SYMBOL_DICTIONARY_H

#include <iostream>
#include <vector>
#include <map>
#include <libxml/tree.h>

#include "h2sl/grounding_set.h"

namespace h2sl {
  class Symbol_Dictionary {
  public:
    Symbol_Dictionary( const std::map< std::string, std::vector< std::string > >& classNames = std::map< std::string, std::vector< std::string > >(), const std::map< std::string, std::vector< std::string > >& stringTypes = std::map< std::string, std::vector< std::string > >(), const std::map< std::string, std::vector< int > >& intTypes = std::map< std::string, std::vector< int > >() );
    Symbol_Dictionary( const std::string& filename );
    virtual ~Symbol_Dictionary();
    Symbol_Dictionary( const Symbol_Dictionary& other, const std::string& symbolType = "all" );
    Symbol_Dictionary& operator=( const Symbol_Dictionary& other );
    bool operator==( const Symbol_Dictionary& other )const;
    bool operator!=( const Symbol_Dictionary& other )const;
  
    virtual bool has_class_name( const std::string& className )const;

    virtual bool from_xml( const std::string& file );
    virtual bool from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    inline std::map< std::string, std::vector< std::string > >& class_names( void ){ return _class_names; };
    inline const std::map< std::string, std::vector< std::string > >& class_names( void )const{ return _class_names; };
    inline std::map< std::string, std::vector< std::string > >& string_types( void ){ return _string_types; };
    inline const std::map< std::string, std::vector< std::string > >& string_types( void )const{ return _string_types; };
    inline std::map< std::string, std::vector< int > >& int_types( void ){ return _int_types; };
    inline const std::map< std::string, std::vector< int > >& int_types( void )const{ return _int_types; };
  
  protected:
    std::map< std::string, std::vector< std::string > > _class_names;
    std::map< std::string, std::vector< std::string > > _string_types;
    std::map< std::string, std::vector< int > > _int_types;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Symbol_Dictionary& other );
}

#endif /* H2SL_SYMBOL_DICTIONARY_H */
