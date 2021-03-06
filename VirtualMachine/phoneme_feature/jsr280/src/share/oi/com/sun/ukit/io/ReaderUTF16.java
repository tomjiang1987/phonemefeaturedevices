/*
 * Copyright  1990-2008 Sun Microsystems, Inc. All Rights Reserved. 
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER 
 *  
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License version 
 * 2 only, as published by the Free Software Foundation. 
 *  
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License version 2 for more details (a copy is 
 * included at /legal/license.txt). 
 *  
 * You should have received a copy of the GNU General Public License 
 * version 2 along with this work; if not, write to the Free Software 
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 
 * 02110-1301 USA 
 *  
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa 
 * Clara, CA 95054 or visit www.sun.com if you need additional 
 * information or have any questions.
 */

package com.sun.ukit.io;

import java.io.Reader;
import java.io.InputStream;
import java.io.IOException;
import java.io.EOFException;

/**
 * UTF-16 encoded stream reader.
 */
public class ReaderUTF16
	extends Reader
{
	private InputStream is;
	private char        bo;

	private byte[]      buff;
	private int         bidx = 0;
	private int         bcnt = 0;

	/**
	 * Constructor.
	 *
	 * Byte order argument can be: 'l' for little-endian or 'b' for big-endian.
	 *
	 * @param is A byte input stream.
	 * @param bo A byte order in the input stream. 
	 */
	public ReaderUTF16(InputStream is, char bo)
	{
		switch (bo) {
		case 'l':
			break;

		case 'b':
			break;

		default:
			throw new IllegalArgumentException("");
		}
		this.bo = bo;
		this.is = is;
		buff    = new byte[128];
	}

	/**
	 * Reads characters into a portion of an array.
	 *
	 * @param cbuf Destination buffer.
	 * @param off Offset at which to start storing characters.
	 * @param len Maximum number of characters to read.
	 * @exception IOException If any IO errors occur.
	 */
	public int read(char[] cbuf, int off, int len)
		throws IOException
	{
		int  num = 0;

		if (bo == 'b') {
			while (num < len) {
				if (bidx >= bcnt) {
					if ((bcnt = is.read(buff, 0, buff.length)) < 0)
						return (num != 0)? num: -1;
					bidx = 0;
				}

				cbuf[off++] = (char)((buff[bidx++] << 8) | (getch() & 0xff));
				num++;
			}
		} else {
			while (num < len) {
				if (bidx >= bcnt) {
					if ((bcnt = is.read(buff, 0, buff.length)) < 0)
						return (num != 0)? num: -1;
					bidx = 0;
				}

				cbuf[off++] = (char)((buff[bidx++] & 0xff) | (getch() << 8));
				num++;
			}
		}
		return num;
	}

	/**
	 * Reads a single character.
	 *
	 * @return The character read, as an integer in the range 0 to 65535 
	 *	(0x0000-0xffff), or -1 if the end of the stream has been reached.
	 * @exception IOException If any IO errors occur.
	 */
	public int read()
		throws IOException
	{
		int  val;
		if ((val = is.read()) < 0)
			return -1;
		if (bo == 'b') {
			val = (char)((val << 8) | (is.read() & 0xff));
		} else {
			val = (char)((is.read() << 8) | (val & 0xff));
		}
		return val;
	}

	/**
	 * Closes the stream.
	 *
	 * @exception IOException If any IO errors occur.
	 */
	public void close()
		throws IOException
	{
		is.close();
	}

	/**
	 * Reads next character from the stream buffer.
	 *
	 * @exception IOException If any IO errors occur.
	 */
	private char getch()
		throws IOException
	{
		if (bidx >= bcnt) {
			if ((bcnt = is.read(buff, 0, buff.length)) <= 0)
				throw new EOFException();
			bidx = 0;
		}
		return (char)buff[bidx++];
	}
}
