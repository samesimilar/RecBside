/* -*- Mode: C -*- */

/*

A simple Pd external to scan an integer from a string (symbol) input.
Copyright 2024 Michael Spears

version 0.1

Creation arguments:
1: The prefix (text) before the place the integer should appear in the string.
NOTE: This object does not protect against pattern injection via the argument. 

Input:
Left input: The string to scan.

Output:
Left output: The scanned integer (as a float).
Right output: Bang, if no integer was found or there was a mismatch between the prefix 
              and the input string.

Acknowledgments
---------------
PD by Miller Puckette and others.

Guide to writing externals project by IOhannes zmölnig at https://github.com/pure-data/externals-howto

pd-lib-builder project (a helper makefile for Pure Data external libraries by Katja Vetter) at http://github.com/pure-data/pd-lib-builder
---------------

 For information on usage and redistribution, and for a DISCLAIMER OF ALL
 WARRANTIES, see the file, "LICENSE.txt," in this distribution.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 See file LICENSE for further informations on licensing terms.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <m_pd.h>



static t_class *simplescani_class;

typedef struct _simplescani
{
  t_object       x_obj;
  
  t_symbol      *x_prefix; 
  t_outlet      *x_num_outlet;  //-- scanned num outlet
  t_outlet      *x_null_outlet;
} t_simplescani;

static void simplescani_symbol(t_simplescani *x, t_symbol *s)
{
    int num = 0;
    char *pattern;
    size_t prefixLen = strlen(x->x_prefix->s_name);
    pattern = malloc((prefixLen + 2)*sizeof(char));
    sprintf(pattern,"%s%%d",x->x_prefix->s_name);
    
    int c = sscanf(s->s_name, pattern, &num);
    
    if (c == 0) {
      outlet_bang(x->x_null_outlet);
      free(pattern);
      return;
    }
    
    outlet_float(x->x_num_outlet, num);
    
    free(pattern);
}

/*--------------------------------------------------------------------
 * new
 */
static void *simplescani_new(t_symbol *s, int argc, t_atom *argv)
{
    t_simplescani *x = (t_simplescani *)pd_new(simplescani_class);

    x->x_prefix = gensym("");
    
    
    switch(argc){
    default:
    case 1:
      x->x_prefix=atom_getsymbol(argv);
      break;
    case 0:
      break;
    }

    //-- outlets
    x->x_num_outlet = outlet_new(&x->x_obj, &s_float);
    x->x_null_outlet = outlet_new(&x->x_obj, &s_bang);

    return (void *)x;
}

/*--------------------------------------------------------------------
 * free
 */
static void simplescani_free(t_simplescani *x)
{
  
  outlet_free(x->x_num_outlet);
  outlet_free(x->x_null_outlet);
  return;
}

/*--------------------------------------------------------------------
 * setup
 */
void simplescani_setup(void)
{
 
  
  simplescani_class = class_new(gensym("simplescani"),
			    (t_newmethod)simplescani_new,
			    (t_method)simplescani_free,
			    sizeof(t_simplescani),
			    CLASS_DEFAULT,
			    A_GIMME, 0);
  
  class_addsymbol(simplescani_class, (t_method)simplescani_symbol);

}
