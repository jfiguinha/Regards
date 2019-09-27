//========================================================================
//
// Link.cc
//
// Copyright 1996-2003 Glyph & Cog, LLC
//
//========================================================================

//========================================================================
//
// Modified under the Poppler project - http://poppler.freedesktop.org
//
// All changes made under the Poppler project to this file are licensed
// under GPL version 2 or later
//
// Copyright (C) 2006, 2008 Pino Toscano <pino@kde.org>
// Copyright (C) 2007, 2010, 2011 Carlos Garcia Campos <carlosgc@gnome.org>
// Copyright (C) 2008 Hugo Mercier <hmercier31@gmail.com>
// Copyright (C) 2008-2010, 2012-2014, 2016-2019 Albert Astals Cid <aacid@kde.org>
// Copyright (C) 2009 Kovid Goyal <kovid@kovidgoyal.net>
// Copyright (C) 2009 Ilya Gorenbein <igorenbein@finjan.com>
// Copyright (C) 2012 Tobias Koening <tobias.koenig@kdab.com>
// Copyright (C) 2018 Klarälvdalens Datakonsult AB, a KDAB Group company, <info@kdab.com>. Work sponsored by the LiMux project of the city of Munich
// Copyright (C) 2018 Intevation GmbH <intevation@intevation.de>
// Copyright (C) 2018 Adam Reichold <adam.reichold@t-online.de>
// Copyright (C) 2019 Oliver Sander <oliver.sander@tu-dresden.de>
//
// To see a description of the changes please see the Changelog file that
// came with your tarball or type make ChangeLog if you are building from git
//
//========================================================================

#include <config.h>

#include <stddef.h>
#include <string.h>
#include "goo/gmem.h"
#include "goo/GooString.h"
#include "Error.h"
#include "Object.h"
#include "Array.h"
#include "Dict.h"
#include "Link.h"
#include "Sound.h"
#include "FileSpec.h"
#include "Rendition.h"
#include "Annot.h"

//------------------------------------------------------------------------
// LinkAction
//------------------------------------------------------------------------
LinkAction::LinkAction() : nextActionList(nullptr) {
}

LinkAction::~LinkAction() {
  if (nextActionList) {
    for (auto entry : *nextActionList) {
      delete entry;
    }
    delete nextActionList;
  }
}

LinkAction *LinkAction::parseDest(const Object *obj) {
  LinkAction *action;

  action = new LinkGoTo(obj);
  if (!action->isOk()) {
    delete action;
    return nullptr;
  }
  return action;
}

LinkAction *LinkAction::parseAction(const Object *obj, const GooString *baseURI)
{
    std::set<int> seenNextActions;
    return parseAction(obj, baseURI, &seenNextActions);
}

LinkAction *LinkAction::parseAction(const Object *obj, const GooString *baseURI,
                                    std::set<int> *seenNextActions) {
  LinkAction *action;

  if (!obj->isDict()) {
      error(errSyntaxWarning, -1, "parseAction: Bad annotation action for URI '{0:s}'",
            baseURI ? baseURI->c_str() : "NULL");
      return nullptr;
  }

  Object obj2 = obj->dictLookup("S");

  // GoTo action
  if (obj2.isName("GoTo")) {
    Object obj3 = obj->dictLookup("D");
    action = new LinkGoTo(&obj3);

  // GoToR action
  } else if (obj2.isName("GoToR")) {
    Object obj3 = obj->dictLookup("F");
    Object obj4 = obj->dictLookup("D");
    action = new LinkGoToR(&obj3, &obj4);

  // Launch action
  } else if (obj2.isName("Launch")) {
    action = new LinkLaunch(obj);

  // URI action
  } else if (obj2.isName("URI")) {
    Object obj3 = obj->dictLookup("URI");
    action = new LinkURI(&obj3, baseURI);

  // Named action
  } else if (obj2.isName("Named")) {
    Object obj3 = obj->dictLookup("N");
    action = new LinkNamed(&obj3);

  // Movie action
  } else if (obj2.isName("Movie")) {
    action = new LinkMovie(obj);

  // Rendition action
  } else if (obj2.isName("Rendition")) {
    action = new LinkRendition(obj);

  // Sound action
  } else if (obj2.isName("Sound")) {
    action = new LinkSound(obj);

  // JavaScript action
  } else if (obj2.isName("JavaScript")) {
    Object obj3 = obj->dictLookup("JS");
    action = new LinkJavaScript(&obj3);

  // Set-OCG-State action
  } else if (obj2.isName("SetOCGState")) {
    action = new LinkOCGState(obj);

  // Hide action
  } else if (obj2.isName("Hide")) {
    action = new LinkHide(obj);

  // unknown action
  } else if (obj2.isName()) {
    action = new LinkUnknown(obj2.getName());

  // action is missing or wrong type
  } else {
    error(errSyntaxWarning, -1, "parseAction: Unknown annotation action object: URI = '{0:s}'",
          baseURI ? baseURI->c_str() : "NULL");
    action = nullptr;
  }

  if (action && !action->isOk()) {
    delete action;
    return nullptr;
  }

  if (!action) {
    return nullptr;
  }

  // parse the next actions
  const Object nextObj = obj->dictLookup("Next");
  std::vector<LinkAction*> *actionList = nullptr;
  if (nextObj.isDict()) {

    // Prevent circles in the tree by checking the ref against used refs in
    // our current tree branch.
    const Object &nextRefObj = obj->dictLookupNF("Next");
    if (nextRefObj.isRef()) {
        const Ref ref = nextRefObj.getRef();
        if (!seenNextActions->insert(ref.num).second) {
            error(errSyntaxWarning, -1, "parseAction: Circular next actions detected.");
            return action;
        }
    }

    actionList = new std::vector<LinkAction*>();
    actionList->reserve(1);
    actionList->push_back(parseAction(&nextObj, nullptr, seenNextActions));
  } else if (nextObj.isArray()) {
    const Array *a = nextObj.getArray();
    const int n = a->getLength();
    actionList = new std::vector<LinkAction*>();
    actionList->reserve(n);
    for (int i = 0; i < n; ++i) {
      const Object obj3 = a->get(i);
      if (!obj3.isDict()) {
        error(errSyntaxWarning, -1, "parseAction: Next array does not contain only dicts");
        continue;
      }

      // Similar circle check as above.
      const Object &obj3Ref = a->getNF(i);
      if (obj3Ref.isRef()) {
          const Ref ref = obj3Ref.getRef();
          if (!seenNextActions->insert(ref.num).second) {
              error(errSyntaxWarning, -1, "parseAction: Circular next actions detected in array.");
              delete actionList;
              return action;
          }
      }

      actionList->push_back(parseAction(&obj3, nullptr, seenNextActions));
    }
  }

  action->setNextActions(actionList);

  return action;
}

const std::vector<LinkAction*> *LinkAction::nextActions() const {
  return nextActionList;
}

void LinkAction::setNextActions(std::vector<LinkAction*> *actions) {
  delete nextActionList;
  nextActionList = actions;
}

//------------------------------------------------------------------------
// LinkDest
//------------------------------------------------------------------------

LinkDest::LinkDest(const Array *a) {
  // initialize fields
  left = bottom = right = top = zoom = 0;
  changeLeft = changeTop = changeZoom = false;
  ok = false;

  // get page
  if (a->getLength() < 2) {
    error(errSyntaxWarning, -1, "Annotation destination array is too short");
    return;
  }
  const Object &obj0 = a->getNF(0);
  if (obj0.isInt()) {
    pageNum = obj0.getInt() + 1;
    pageIsRef = false;
  } else if (obj0.isRef()) {
    pageRef = obj0.getRef();
    pageIsRef = true;
  } else {
    error(errSyntaxWarning, -1, "Bad annotation destination");
    return;
  }

  // get destination type
  Object obj1 = a->get(1);

  // XYZ link
  if (obj1.isName("XYZ")) {
    kind = destXYZ;
    if (a->getLength() < 3) {
      changeLeft = false;
    } else {
      Object obj2 = a->get(2);
      if (obj2.isNull()) {
	changeLeft = false;
      } else if (obj2.isNum()) {
	changeLeft = true;
	left = obj2.getNum();
      } else {
	error(errSyntaxWarning, -1, "Bad annotation destination position");
	return;
      }
    }
    if (a->getLength() < 4) {
      changeTop = false;
    } else {
      Object obj2 = a->get(3);
      if (obj2.isNull()) {
	changeTop = false;
      } else if (obj2.isNum()) {
	changeTop = true;
	top = obj2.getNum();
      } else {
	error(errSyntaxWarning, -1, "Bad annotation destination position");
	return;
      }
    }
    if (a->getLength() < 5) {
      changeZoom = false;
    } else {
      Object obj2 = a->get(4);
      if (obj2.isNull()) {
	changeZoom = false;
      } else if (obj2.isNum()) {
	zoom = obj2.getNum();
	changeZoom = (zoom == 0) ? false : true;
      } else {
	error(errSyntaxWarning, -1, "Bad annotation destination position");
	return;
      }
    }

  // Fit link
  } else if (obj1.isName("Fit")) {
    kind = destFit;

  // FitH link
  } else if (obj1.isName("FitH")) {
    kind = destFitH;
    if (a->getLength() < 3) {
      changeTop = false;
    } else {
      Object obj2 = a->get(2);
      if (obj2.isNull()) {
	changeTop = false;
      } else if (obj2.isNum()) {
	changeTop = true;
	top = obj2.getNum();
      } else {
	error(errSyntaxWarning, -1, "Bad annotation destination position");
	kind = destFit;
      }
    }

  // FitV link
  } else if (obj1.isName("FitV")) {
    if (a->getLength() < 3) {
      error(errSyntaxWarning, -1, "Annotation destination array is too short");
      return;
    }
    kind = destFitV;
    Object obj2 = a->get(2);
    if (obj2.isNull()) {
      changeLeft = false;
    } else if (obj2.isNum()) {
      changeLeft = true;
      left = obj2.getNum();
    } else {
      error(errSyntaxWarning, -1, "Bad annotation destination position");
      kind = destFit;
    }

  // FitR link
  } else if (obj1.isName("FitR")) {
    if (a->getLength() < 6) {
      error(errSyntaxWarning, -1, "Annotation destination array is too short");
      return;
    }
    kind = destFitR;
    Object obj2 = a->get(2);
    if (obj2.isNum()) {
      left = obj2.getNum();
    } else {
      error(errSyntaxWarning, -1, "Bad annotation destination position");
      kind = destFit;
    }
    obj2 = a->get(3);
    if (obj2.isNum()) {
      bottom = obj2.getNum();
    } else {
      error(errSyntaxWarning, -1, "Bad annotation destination position");
      kind = destFit;
    }
    obj2 = a->get(4);
    if (obj2.isNum()) {
      right = obj2.getNum();
    } else {
      error(errSyntaxWarning, -1, "Bad annotation destination position");
      kind = destFit;
    }
    obj2 = a->get(5);
    if (obj2.isNum()) {
      top = obj2.getNum();
    } else {
      error(errSyntaxWarning, -1, "Bad annotation destination position");
      kind = destFit;
    }

  // FitB link
  } else if (obj1.isName("FitB")) {
    kind = destFitB;

  // FitBH link
  } else if (obj1.isName("FitBH")) {
    if (a->getLength() < 3) {
      error(errSyntaxWarning, -1, "Annotation destination array is too short");
      return;
    }
    kind = destFitBH;
    Object obj2 = a->get(2);
    if (obj2.isNull()) {
      changeTop = false;
    } else if (obj2.isNum()) {
      changeTop = true;
      top = obj2.getNum();
    } else {
      error(errSyntaxWarning, -1, "Bad annotation destination position");
      kind = destFit;
    }

  // FitBV link
  } else if (obj1.isName("FitBV")) {
    if (a->getLength() < 3) {
      error(errSyntaxWarning, -1, "Annotation destination array is too short");
      return;
    }
    kind = destFitBV;
    Object obj2 = a->get(2);
    if (obj2.isNull()) {
      changeLeft = false;
    } else if (obj2.isNum()) {
      changeLeft = true;
      left = obj2.getNum();
    } else {
      error(errSyntaxWarning, -1, "Bad annotation destination position");
      kind = destFit;
    }

  // unknown link kind
  } else {
    error(errSyntaxWarning, -1, "Unknown annotation destination type");
  }

  ok = true;
  return;
}

LinkDest::LinkDest(const LinkDest *dest) {
  kind = dest->kind;
  pageIsRef = dest->pageIsRef;
  if (pageIsRef)
    pageRef = dest->pageRef;
  else
    pageNum = dest->pageNum;
  left = dest->left;
  bottom = dest->bottom;
  right = dest->right;
  top = dest->top;
  zoom = dest->zoom;
  changeLeft = dest->changeLeft;
  changeTop = dest->changeTop;
  changeZoom = dest->changeZoom;
  ok = true;
}

//------------------------------------------------------------------------
// LinkGoTo
//------------------------------------------------------------------------

LinkGoTo::LinkGoTo(const Object *destObj) {
  dest = nullptr;
  namedDest = nullptr;

  // named destination
  if (destObj->isName()) {
    namedDest = new GooString(destObj->getName());
  } else if (destObj->isString()) {
    namedDest = destObj->getString()->copy();

  // destination dictionary
  } else if (destObj->isArray()) {
    dest = new LinkDest(destObj->getArray());
    if (!dest->isOk()) {
      delete dest;
      dest = nullptr;
    }

  // error
  } else {
    error(errSyntaxWarning, -1, "Illegal annotation destination");
  }
}

LinkGoTo::~LinkGoTo() {
  if (dest)
    delete dest;
  if (namedDest)
    delete namedDest;
}

//------------------------------------------------------------------------
// LinkGoToR
//------------------------------------------------------------------------

LinkGoToR::LinkGoToR(Object *fileSpecObj, Object *destObj) {
  fileName = nullptr;
  dest = nullptr;
  namedDest = nullptr;

  // get file name
  Object obj1 = getFileSpecNameForPlatform (fileSpecObj);
  if (obj1.isString()) {
    fileName = obj1.getString()->copy();
  }

  // named destination
  if (destObj->isName()) {
    namedDest = new GooString(destObj->getName());
  } else if (destObj->isString()) {
    namedDest = destObj->getString()->copy();

  // destination dictionary
  } else if (destObj->isArray()) {
    dest = new LinkDest(destObj->getArray());
    if (!dest->isOk()) {
      delete dest;
      dest = nullptr;
    }

  // error
  } else {
    error(errSyntaxWarning, -1, "Illegal annotation destination");
  }
}

LinkGoToR::~LinkGoToR() {
  if (fileName)
    delete fileName;
  if (dest)
    delete dest;
  if (namedDest)
    delete namedDest;
}


//------------------------------------------------------------------------
// LinkLaunch
//------------------------------------------------------------------------

LinkLaunch::LinkLaunch(const Object *actionObj) {

  fileName = nullptr;
  params = nullptr;

  if (actionObj->isDict()) {
    Object obj1 = actionObj->dictLookup("F");
    if (!obj1.isNull()) {
      Object obj3 = getFileSpecNameForPlatform (&obj1);
      if (obj3.isString()) {
	fileName = obj3.getString()->copy();
      }
    } else {
#ifdef _WIN32
      obj1 = actionObj->dictLookup("Win");
#else
      //~ This hasn't been defined by Adobe yet, so assume it looks
      //~ just like the Win dictionary until they say otherwise.
      obj1 = actionObj->dictLookup("Unix");
#endif
      if (obj1.isDict()) {
	Object obj2 = obj1.dictLookup("F");
	Object obj3 = getFileSpecNameForPlatform (&obj2);
	if (obj3.isString()) {
	  fileName = obj3.getString()->copy();
	}
	obj2 = obj1.dictLookup("P");
	if (obj2.isString()) {
	  params = obj2.getString()->copy();
	}
      } else {
	error(errSyntaxWarning, -1, "Bad launch-type link action");
      }
    }
  }
}

LinkLaunch::~LinkLaunch() {
  if (fileName)
    delete fileName;
  if (params)
    delete params;
}

//------------------------------------------------------------------------
// LinkURI
//------------------------------------------------------------------------

LinkURI::LinkURI(const Object *uriObj, const GooString *baseURI) {
  const GooString *uri2;
  int n;
  char c;

  uri = nullptr;
  if (uriObj->isString()) {
    uri2 = uriObj->getString();
    n = (int)strcspn(uri2->c_str(), "/:");
    if (n < uri2->getLength() && uri2->getChar(n) == ':') {
      // "http:..." etc.
      uri = uri2->copy();
    } else if (!uri2->cmpN("www.", 4)) {
      // "www.[...]" without the leading "http://"
      uri = new GooString("http://");
      uri->append(uri2);
    } else {
      // relative URI
      if (baseURI) {
	uri = baseURI->copy();
	if (uri->getLength() > 0) {
	  c = uri->getChar(uri->getLength() - 1);
	  if (c != '/' && c != '?') {
	    uri->append('/');
	  }
	}
	if (uri2->getChar(0) == '/') {
	  uri->append(uri2->c_str() + 1, uri2->getLength() - 1);
	} else {
	  uri->append(uri2);
	}
      } else {
	uri = uri2->copy();
      }
    }
  } else {
    error(errSyntaxWarning, -1, "Illegal URI-type link");
  }
}

LinkURI::~LinkURI() {
  if (uri)
    delete uri;
}

//------------------------------------------------------------------------
// LinkNamed
//------------------------------------------------------------------------

LinkNamed::LinkNamed(const Object *nameObj) {
  name = nullptr;
  if (nameObj->isName()) {
    name = new GooString(nameObj->getName());
  }
}

LinkNamed::~LinkNamed() {
  if (name) {
    delete name;
  }
}

//------------------------------------------------------------------------
// LinkMovie
//------------------------------------------------------------------------

LinkMovie::LinkMovie(const Object *obj) {
  annotRef = Ref::INVALID();
  annotTitle = nullptr;

  const Object &annotationObj = obj->dictLookupNF("Annotation");
  if (annotationObj.isRef()) {
    annotRef = annotationObj.getRef();
  }

  Object tmp = obj->dictLookup("T");
  if (tmp.isString()) {
    annotTitle = tmp.getString()->copy();
  }

  if ((annotTitle == nullptr) && (annotRef == Ref::INVALID())) {
    error(errSyntaxError, -1,
	  "Movie action is missing both the Annot and T keys");
  }

  tmp = obj->dictLookup("Operation");
  if (tmp.isName()) {
    const char *name = tmp.getName();
    
    if (!strcmp(name, "Play")) {
      operation = operationTypePlay;
    }
    else if (!strcmp(name, "Stop")) {
      operation = operationTypeStop;
    }
    else if (!strcmp(name, "Pause")) {
      operation = operationTypePause;
    }
    else if (!strcmp(name, "Resume")) {
      operation = operationTypeResume;
    }
  }
}

LinkMovie::~LinkMovie() {
  if (annotTitle) {
    delete annotTitle;
  }
}

//------------------------------------------------------------------------
// LinkSound
//------------------------------------------------------------------------

LinkSound::LinkSound(const Object *soundObj) {
  volume = 1.0;
  sync = false;
  repeat = false;
  mix = false;
  sound = nullptr;
  if (soundObj->isDict())
  {
    // volume
    Object tmp = soundObj->dictLookup("Volume");
    if (tmp.isNum()) {
      volume = tmp.getNum();
    }
    // sync
    tmp = soundObj->dictLookup("Synchronous");
    if (tmp.isBool()) {
      sync = tmp.getBool();
    }
    // repeat
    tmp = soundObj->dictLookup("Repeat");
    if (tmp.isBool()) {
      repeat = tmp.getBool();
    }
    // mix
    tmp = soundObj->dictLookup("Mix");
    if (tmp.isBool()) {
      mix = tmp.getBool();
    }
    // 'Sound' object
    tmp = soundObj->dictLookup("Sound");
    sound = Sound::parseSound(&tmp);
  }
}

LinkSound::~LinkSound() {
  delete sound;
}

//------------------------------------------------------------------------
// LinkRendition
//------------------------------------------------------------------------

LinkRendition::LinkRendition(const Object *obj) {
  operation = NoRendition;
  media = nullptr;
  js = nullptr;
  int operationCode = -1;

  screenRef = Ref::INVALID();

  if (obj->isDict()) {
    Object tmp = obj->dictLookup("JS");
    if (!tmp.isNull()) {
      if (tmp.isString()) {
        js = new GooString(tmp.getString());
      } else if (tmp.isStream()) {
        Stream *stream = tmp.getStream();
	js = new GooString();
	stream->fillGooString(js);
      } else {
        error(errSyntaxWarning, -1, "Invalid Rendition Action: JS not string or stream");
      }
    }

    tmp = obj->dictLookup("OP");
    if (tmp.isInt()) {
      operationCode = tmp.getInt();
      if (!js && (operationCode < 0 || operationCode > 4)) {
        error(errSyntaxWarning, -1, "Invalid Rendition Action: unrecognized operation valued: {0:d}", operationCode);
      } else {
        // retrieve rendition object
        renditionObj = obj->dictLookup("R");
        if (renditionObj.isDict()) {
          media = new MediaRendition(&renditionObj);
	} else if (operationCode == 0 || operationCode == 4) {
          error(errSyntaxWarning, -1, "Invalid Rendition Action: no R field with op = {0:d}", operationCode);
	  renditionObj.setToNull();
	}

	const Object &anObj = obj->dictLookupNF("AN");
	if (anObj.isRef()) {
	  screenRef = anObj.getRef();
	} else if (operation >= 0 && operation <= 4) {
	  error(errSyntaxWarning, -1, "Invalid Rendition Action: no AN field with op = {0:d}", operationCode);
	}
      }

      switch (operationCode) {
        case 0:
          operation = PlayRendition;
          break;
        case 1:
          operation = StopRendition;
          break;
        case 2:
          operation = PauseRendition;
          break;
        case 3:
          operation = ResumeRendition;
          break;
        case 4:
          operation = PlayRendition;
          break;
      }
    } else if (!js) {
      error(errSyntaxWarning, -1, "Invalid Rendition action: no OP or JS field defined");
    }
  }
}

LinkRendition::~LinkRendition() {
  delete js;
  delete media;
}


//------------------------------------------------------------------------
// LinkJavaScript
//------------------------------------------------------------------------

LinkJavaScript::LinkJavaScript(Object *jsObj) {
  js = nullptr;

  if (jsObj->isString()) {
    js = new GooString(jsObj->getString());
  }
  else if (jsObj->isStream()) {
    Stream *stream = jsObj->getStream();
    js = new GooString();
    stream->fillGooString(js);
  }
}

LinkJavaScript::~LinkJavaScript() {
  if (js) {
    delete js;
  }
}

Object LinkJavaScript::createObject(XRef *xref, const GooString &js)
{
  Dict *linkDict = new Dict(xref);
  linkDict->add("S", Object(objName, "JavaScript"));
  linkDict->add("JS", Object(js.copy()));

  return Object(linkDict);
}

//------------------------------------------------------------------------
// LinkOCGState
//------------------------------------------------------------------------
LinkOCGState::LinkOCGState(const Object *obj) {
  stateList = new std::vector<StateList*>();
  preserveRB = true;

  Object obj1 = obj->dictLookup("State");
  if (obj1.isArray()) {
    StateList *stList = nullptr;

    for (int i = 0; i < obj1.arrayGetLength(); ++i) {
      const Object &obj2 = obj1.arrayGetNF(i);
      if (obj2.isName()) {
        if (stList)
	  stateList->push_back(stList);

	const char *name = obj2.getName();
	stList = new StateList();
	stList->list = new std::vector<Ref*>();
	if (!strcmp (name, "ON")) {
	  stList->st = On;
	} else if (!strcmp (name, "OFF")) {
	  stList->st = Off;
	} else if (!strcmp (name, "Toggle")) {
	  stList->st = Toggle;
	} else {
	  error(errSyntaxWarning, -1, "Invalid name '{0:s}' in OCG Action state array", name);
	  delete stList;
	  stList = nullptr;
	}
      } else if (obj2.isRef()) {
        if (stList) {
	  Ref ocgRef = obj2.getRef();
	  Ref *item = new Ref();
	  *item = ocgRef;
	  stList->list->push_back(item);
	} else {
	  error(errSyntaxWarning, -1, "Invalid OCG Action State array, expected name instead of ref");
	}
      } else {
        error(errSyntaxWarning, -1, "Invalid item in OCG Action State array");
      }
    }
    // Add the last group
    if (stList)
      stateList->push_back(stList);
  } else {
    error(errSyntaxWarning, -1, "Invalid OCGState action");
    delete stateList;
    stateList = nullptr;
  }

  obj1 = obj->dictLookup("PreserveRB");
  if (obj1.isBool()) {
    preserveRB = obj1.getBool();
  }
}

LinkOCGState::~LinkOCGState() {
  if (stateList) {
    for (auto entry : *stateList) {
      delete entry;
    }
    delete stateList;
  }
}

LinkOCGState::StateList::~StateList() {
  if (list) {
    for (auto entry : *list) {
      delete entry;
    }
    delete list;
  }
}

//------------------------------------------------------------------------
// LinkHide
//------------------------------------------------------------------------

LinkHide::LinkHide(const Object *hideObj) {
  targetName = nullptr;
  show = false; // Default

  if (hideObj->isDict()) {
      const Object targetObj = hideObj->dictLookup("T");
      if (targetObj.isString()) {
	targetName = targetObj.getString()->copy();
      }
      const Object shouldHide = hideObj->dictLookup("H");
      if (shouldHide.isBool()) {
	show = !shouldHide.getBool();
      }
  }
}

LinkHide::~LinkHide() {
  delete targetName;
}

//------------------------------------------------------------------------
// LinkUnknown
//------------------------------------------------------------------------

LinkUnknown::LinkUnknown(const char *actionA) {
  action = new GooString(actionA);
}

LinkUnknown::~LinkUnknown() {
  delete action;
}

//------------------------------------------------------------------------
// Links
//------------------------------------------------------------------------

Links::Links(Annots *annots) {
  int size;
  int i;

  links = nullptr;
  size = 0;
  numLinks = 0;

  if (!annots)
    return;

  for (i = 0; i < annots->getNumAnnots(); ++i) {
    Annot *annot = annots->getAnnot(i);

    if (annot->getType() != Annot::typeLink)
      continue;

    if (numLinks >= size) {
      size += 16;
      links = (AnnotLink **)greallocn(links, size, sizeof(AnnotLink *));
    }
    annot->incRefCnt();
    links[numLinks++] = static_cast<AnnotLink *>(annot);
  }
}

Links::~Links() {
  int i;

  for (i = 0; i < numLinks; ++i)
    links[i]->decRefCnt();

  gfree(links);
}

LinkAction *Links::find(double x, double y) const {
  int i;

  for (i = numLinks - 1; i >= 0; --i) {
    if (links[i]->inRect(x, y)) {
      return links[i]->getAction();
    }
  }
  return nullptr;
}

bool Links::onLink(double x, double y) const {
  int i;

  for (i = 0; i < numLinks; ++i) {
    if (links[i]->inRect(x, y))
      return true;
  }
  return false;
}
