//
// DocumentFragment.cpp
//
// $Id: //poco/1.2/XML/src/DocumentFragment.cpp#1 $
//
// Library: XML
// Package: DOM
// Module:  DOM
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/DOM/DocumentFragment.h"


namespace Poco {
namespace XML {


const XMLString DocumentFragment::NODE_NAME = toXMLString("#document-fragment");


DocumentFragment::DocumentFragment(Document* pOwnerDocument): 
	AbstractContainerNode(pOwnerDocument)
{
}


DocumentFragment::DocumentFragment( Document* pOwnerDocument, const DocumentFragment& fragment): 
	AbstractContainerNode(pOwnerDocument, fragment)
{
}


DocumentFragment::~DocumentFragment()
{
}


const XMLString& DocumentFragment::nodeName() const
{
	return NODE_NAME;
}


unsigned short DocumentFragment::nodeType() const
{
	return Node::DOCUMENT_FRAGMENT_NODE;
}


Node* DocumentFragment::copyNode(bool deep, Document* pOwnerDocument) const
{
	DocumentFragment* pClone = new DocumentFragment(pOwnerDocument, *this);
	if (deep)
	{
		Node* pCur = firstChild();
		while (pCur)
		{
			pClone->appendChild(static_cast<AbstractNode*>(pCur)->copyNode(deep, pOwnerDocument))->release();
			pCur = pCur->nextSibling();
		}
	}
	return pClone;
}


} } // namespace Poco::XML
