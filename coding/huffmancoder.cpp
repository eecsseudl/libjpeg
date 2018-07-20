/*************************************************************************

    This project implements a complete(!) JPEG (10918-1 ITU.T-81) codec,
    plus a library that can be used to encode and decode JPEG streams. 
    It also implements ISO/IEC 18477 aka JPEG XT which is an extension
    towards intermediate, high-dynamic-range lossy and lossless coding
    of JPEG. In specific, it supports ISO/IEC 18477-3/-6/-7/-8 encoding.

    Copyright (C) 2012-2018 Thomas Richter, University of Stuttgart and
    Accusoft.

    This program is available under two licenses, GPLv3 and the ITU
    Software licence Annex A Option 2, RAND conditions.

    For the full text of the GPU license option, see README.license.gpl.
    For the full text of the ITU license option, see README.license.itu.
    
    You may freely select beween these two options.

    For the GPL option, please note the following:

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*************************************************************************/
/*
** This class implements an encoder for a single group of bits in a huffman
** decoder.
**
** $Id: huffmancoder.cpp,v 1.11 2017/06/02 21:17:40 thor Exp $
**
*/

/// Includes
#include "std/string.hpp"
#include "coding/huffmancoder.hpp"
///

/// HuffmanCoder::HuffmanCoder
HuffmanCoder::HuffmanCoder(const UBYTE *lengths,const UBYTE *symbols)
{
  int i;
  ULONG value = 0; // current code value.
  UBYTE cnt   = 0;

  memset(m_ucBits,0,sizeof(m_ucBits));
  memset(m_usCode,0,sizeof(m_usCode));

  for(i = 0;i < 16;i++) {
    UBYTE j = lengths[i]; // j = number of codes of size i + 1.
    if (j) {
      do {
        UBYTE symbol = symbols[cnt];
        assert(m_ucBits[symbol] == 0);
        m_ucBits[symbol] = i + 1; // size in bits
        m_usCode[symbol] = value;
        value++;                  // next code
        // Note: This test also checks that there is no code with all 1 bits set
        // This is what Annex K.2 says, and the introduction of Annex C
        // enforces. There is, from a pure coding perspective, no need to
        // enforce this rule, i.e. a all-1 codeword would still create a
        // valid prefix-free code.
        assert(value < (1UL << (i + 1))); // Overflow?
        cnt++;                    // next entry in the table.
      } while(--j);
    }
    value <<= 1; // shift another bit in.
  }
}
///
