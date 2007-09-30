//
//    Copyright 2007 Andreas Pfeiffer
//
/*
 *  This file is part of iAIDA.
 *
 *  iAIDA is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as 
 *  published by the Free Software Foundation, either version 3 of 
 *  the License.
 *
 *  iAIDA is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public 
 *  License along with iAIDA.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
   \file File containing general documentation about the package
 */



/**

\mainpage

A plotter (loosely based on an early verison of the AIDA 3.x interfaces)
built on top of the Grace application
(<a href="http://plasma-gate.weizmann.ac.il/Grace">http://plasma-gate.weizmann.ac.il/Grace</a>).
Grace is based directly on Motif (LessTif is fine) so it is not embeddable as
a Qt widget. The interface in this version is a clunky command language
which I have hidden inside the AidaNative::Grace class. Future versions 
will (they claim)
have a programmatic interface in C++, which might even be OO. The plotter was
written to allow an implementation of the AIDA interfaces based on Grace
for use inside Lizard. When the interfaces stabilise I will write a wrapper
based on this package (first version done Oct 8th 2002 MS).

 */

   // This page left intentionally blank ;-)
