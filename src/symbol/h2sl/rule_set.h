/**
 * @file    rule_set.h
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
 * The interface for a class used to describe a set of rules
 */

#ifndef H2SL_RULE_SET_H
#define H2SL_RULE_SET_H

#include <iostream>
#include <vector>

#include "h2sl/rule.h"

namespace h2sl {
  class Rule_Set: public Rule {
  public:
    Rule_Set( const std::vector< Rule* >& rules = std::vector< Rule* >() );
    Rule_Set( xmlNodePtr root, World* world );
    virtual ~Rule_Set();
    Rule_Set( const Rule_Set& other );
    Rule_Set& operator=( const Rule_Set& other );
    virtual Rule_Set* dup( void )const;

    virtual bool matches_class_name( const std::string& arg )const{ return ( arg == "rule_set" ); };
    virtual void scrape_grounding( const World * world, std::vector< std::string >& classNames, std::map< std::string, std::vector< std::string > >& stringTypes, std::map< std::string, std::vector< int > >& intTypes )const;

    void clear( void );
 
    virtual std::string evaluate_cv( const h2sl::Grounding* rule )const;

    virtual void to_xml( const std::string& filename )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual void from_xml( const std::string& filename );
    virtual void from_xml( xmlNodePtr root, World* world );

    inline std::vector< Rule* >& rules( void ){ return _rules; };
    inline const std::vector< Rule* >& rules( void )const{ return _rules; };

    static std::string class_name( void ){ return "rule_set"; };

  protected:
    std::vector< Rule* > _rules;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Rule_Set& other );
}

#endif /* H2SL_RULE_SET_H */
