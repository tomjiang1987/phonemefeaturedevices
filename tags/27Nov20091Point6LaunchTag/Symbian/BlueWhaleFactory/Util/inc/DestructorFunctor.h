/**
 * Copyright (c) 2004-2008 Blue Whale Systems Ltd. All Rights Reserved. 
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER 
 *  
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License version 
 * 2 only, as published by the Free Software Foundation.  
 *  
 * This software is provided "as is," and the copyright holder makes no representations or warranties, express or
 * implied, including but not limited to warranties of merchantability or fitness for any particular purpose or that the
 * use of this software or documentation will not infringe any third party patents, copyrights, trademarks or other
 * rights.
 * 
 * The copyright holder will not be liable for any direct, indirect special or consequential damages arising out of any
 * use of this software or documentation.
 * 
 * See the GNU  General Public License version 2 for more details 
 * (a copy is included at /legal/license.txt).  
 *  
 * You should have received a copy of the GNU General Public License 
 * version 2 along with this work; if not, write to the Free Software 
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 
 * 02110-1301 USA  
 *  
 * Please contact Blue Whale Systems Ltd, Suite 123, The LightBox
 * 111 Power Road, Chiswick, London, W4 5PY, United Kingdom or visit 
 * www.bluewhalesystems.com if you need additional 
 * information or have any questions.  
 */ 


#ifndef __DESTRUCTOR_FUNCTOR_H__
#define __DESTRUCTOR_FUNCTOR_H__


/**
 * This is the destructor functor which is used to destroy
 * DATA items added to the hashtable.
 * This default implementation does nothing.
 * If you'd like some destruct action to take place,
 * override this action by implementing a template specialization
 * for your DATA class type.
 */
template <class DATA> class TDestructorFunctor
{
public:
	/**
	 * This one does nothing -- implement a template specialization 
	 * of this method for your own DATA type to implement needed cleanup.
	 */
	static void Destroy( DATA /* aData */)
	{
	}
};
#ifdef __GCCE__
class MStateChangePlugin;
class CSmtpParserNode;
class CImapParserNode;
class MMessage;
class CEikMenuPaneItem;
template class TDestructorFunctor<HBufC *>;
template class TDestructorFunctor<HBufC8 *>;
template class TDestructorFunctor<MUnknown *>;
template class TDestructorFunctor<MStateChangePlugin*>;
template class TDestructorFunctor<CSmtpParserNode*>;
template class TDestructorFunctor<CImapParserNode*>;
template class TDestructorFunctor<MMessage*>;
template class TDestructorFunctor<CEikMenuPaneItem*>;
#endif

#endif /* __DESTRUCTOR_FUNCTOR_H__ */
