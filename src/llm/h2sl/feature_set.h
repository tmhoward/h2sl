/**
 * @file    feature_set.h
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
 * The interface for a class used to describe a set of features
 */

#ifndef H2SL_FEATURE_SET_H
#define H2SL_FEATURE_SET_H

#include <iostream>
#include <vector>
#include <libxml/tree.h>

#include <h2sl/grounding.h>
#include <h2sl/feature.h>
#include <h2sl/feature_product.h>

namespace h2sl {
  class Feature_Set {
  public:
    Feature_Set();
    virtual ~Feature_Set();
    Feature_Set( const Feature_Set& other );
    Feature_Set& operator=( const Feature_Set& other );

    void indices( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context, std::vector< unsigned int >& indices, std::vector< Feature* >& features, const std::vector< bool >& evaluateFeatureTypes );
    void indices( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context, std::vector< unsigned int >& indices, std::vector< std::pair< std::vector< Feature* >, unsigned int > >& weightedFeatures, const std::vector< bool >& evaluateFeatureTypes );
    void evaluate( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context, const std::vector< bool >& evaluateFeatureTypes );

    virtual void to_xml( const std::string& filename )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual void from_xml( const std::string& filename );
    virtual void from_xml( xmlNodePtr root );

    unsigned int size( void )const;

    inline std::vector< Feature_Product* >& feature_products( void ){ return _feature_products; };
    inline const std::vector< Feature_Product* >& feature_products( void )const{ return _feature_products; };

  protected:
    std::vector< Feature_Product* > _feature_products;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const Feature_Set& other );
}

#endif /* H2SL_FEATURE_SET_H */
