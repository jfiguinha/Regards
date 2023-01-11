#include "StdAfx.h"
#include "Camera.h"
#include <iostream>
/*
    RawSpeed - RAW file decoder.

    Copyright (C) 2009 Klaus Post

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

    http://www.klauspost.com
*/

namespace RawSpeed {

Camera::Camera(xmlDocPtr doc, xmlNodePtr cur) {
  xmlChar *key;
  key = xmlGetProp(cur, (const xmlChar *)"make");
  if (!key)
    ThrowCME("Camera XML Parser: \"make\" attribute not found.");
  make = string((const char*)key);
  xmlFree(key);

  key = xmlGetProp(cur, (const xmlChar *)"model");
  if (!key)
    ThrowCME("Camera XML Parser: \"model\" attribute not found.");
  model = string((const char*)key);
  xmlFree(key);

  supported = true;
  key = xmlGetProp(cur, (const xmlChar *)"supported");
  if (key) {
    string s = string((const char*)key);
    if (s.compare("no") == 0)
      supported = false;
    xmlFree(key);
  }

  key = xmlGetProp(cur, (const xmlChar *)"mode");
  if (key) {
    mode = string((const char*)key);
    xmlFree(key);
  } else {
    mode = string("");
  }

  key = xmlGetProp(cur, (const xmlChar *)"decoder_version");
  if (key) {
    decoderVersion = getAttributeAsInt(cur, cur->name, "decoder_version");
    xmlFree(key);
  } else {
    decoderVersion = 0;
  }

  cur = cur->xmlChildrenNode;
  while (cur != NULL) {
    parseCameraChild(doc, cur);
    cur = cur->next;
  }
}

Camera::Camera( const Camera* camera, uint32 alias_num)
{
  if (alias_num >= camera->aliases.size())
    ThrowCME("Camera: Internal error, alias number out of range specified.");

  make = camera->make;
  model = camera->aliases[alias_num];
  mode = camera->mode;
  cfa = camera->cfa;
  supported = camera->supported;
  cropSize = camera->cropSize;
  cropPos = camera->cropPos;
  decoderVersion = camera->decoderVersion;
  for (uint32 i = 0; i < camera->blackAreas.size(); i++) {
    blackAreas.push_back(camera->blackAreas[i]);
  }
  for (uint32 i = 0; i < camera->sensorInfo.size(); i++) {
    sensorInfo.push_back(camera->sensorInfo[i]);
  }
  map<string,string>::const_iterator mi = camera->hints.begin();
  for (; mi != camera->hints.end(); ++mi) {
    hints.insert(make_pair((*mi).first, (*mi).second));
  }
}

Camera::~Camera(void) {
}

void Camera::parseCameraChild(xmlDocPtr doc, xmlNodePtr cur) {

  if (!xmlStrcmp(cur->name, (const xmlChar *) "CFA")) {    
    if (2 != getAttributeAsInt(cur, cur->name, "width") || 2 != getAttributeAsInt(cur, cur->name, "height")) {
      supported = FALSE;
    } else {
      cur = cur->xmlChildrenNode;
      while (cur != NULL) {
        parseCFA(doc, cur);
        cur = cur->next;
      }
    }
    return;
  }
  if (!xmlStrcmp(cur->name, (const xmlChar *) "Crop")) {
    cropPos.x = getAttributeAsInt(cur, cur->name, "x");
    cropPos.y = getAttributeAsInt(cur, cur->name, "y");

    if (cropPos.x < 0)
      ThrowCME("Negative X axis crop specified in camera %s %s", make.c_str(), model.c_str());
    if (cropPos.y < 0)
      ThrowCME("Negative Y axis crop specified in camera %s %s", make.c_str(), model.c_str());

    cropSize.x = getAttributeAsInt(cur, cur->name, "width");
    cropSize.y = getAttributeAsInt(cur, cur->name, "height");
    return;
  }

  if (!xmlStrcmp(cur->name, (const xmlChar *) "Sensor")) {
    parseSensorInfo(doc, cur);
    return;
  }

  if (!xmlStrcmp(cur->name, (const xmlChar *) "BlackAreas")) {
    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
      parseBlackAreas(doc, cur);
      cur = cur->next;
    }
    return;
  }

  if (!xmlStrcmp(cur->name, (const xmlChar *) "Aliases")) {
    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
      parseAlias(doc, cur);
      cur = cur->next;
    }
    return;
  }

  if (!xmlStrcmp(cur->name, (const xmlChar *) "Hints")) {
    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
      parseHint(doc, cur);
      cur = cur->next;
    }
    return;
  }
}

void Camera::parseCFA(xmlDocPtr doc, xmlNodePtr cur) {
  xmlChar *key;
  if (!xmlStrcmp(cur->name, (const xmlChar *) "Color")) {
    int x = getAttributeAsInt(cur, cur->name, "x");
    if (x < 0 || x > 1) {
      ThrowCME("Invalid x coordinate in CFA array of in camera %s %s", make.c_str(), model.c_str());
    }

    int y = getAttributeAsInt(cur, cur->name, "y");
    if (y < 0 || y > 1) {
      ThrowCME("Invalid y coordinate in CFA array of in camera %s %s", make.c_str(), model.c_str());
    }

    key = xmlNodeListGetString(doc, cur->children, 1);
    if (!xmlStrcmp(key, (const xmlChar *) "GREEN"))
      cfa.setColorAt(iPoint2D(x, y), CFA_GREEN);
    else if (!xmlStrcmp(key, (const xmlChar *) "RED"))
      cfa.setColorAt(iPoint2D(x, y), CFA_RED);
    else if (!xmlStrcmp(key, (const xmlChar *) "BLUE"))
      cfa.setColorAt(iPoint2D(x, y), CFA_BLUE);

    xmlFree(key);
  }
}

void Camera::parseBlackAreas(xmlDocPtr doc, xmlNodePtr cur) {
  if (!xmlStrcmp(cur->name, (const xmlChar *) "Vertical")) {

    int x = getAttributeAsInt(cur, cur->name, "x");
    if (x < 0) {
      ThrowCME("Invalid x coordinate in vertical BlackArea of in camera %s %s", make.c_str(), model.c_str());
    }

    int w = getAttributeAsInt(cur, cur->name, "width");
    if (w < 0) {
      ThrowCME("Invalid width in vertical BlackArea of in camera %s %s", make.c_str(), model.c_str());
    }

    blackAreas.push_back(BlackArea(x, w, true));

  } else if (!xmlStrcmp(cur->name, (const xmlChar *) "Horizontal")) {

    int y = getAttributeAsInt(cur, cur->name, "y");
    if (y < 0) {
      ThrowCME("Invalid y coordinate in horizontal BlackArea of in camera %s %s", make.c_str(), model.c_str());
    }

    int h = getAttributeAsInt(cur, cur->name, "height");
    if (h < 0) {
      ThrowCME("Invalid width in horizontal BlackArea of in camera %s %s", make.c_str(), model.c_str());
    }

    blackAreas.push_back(BlackArea(y, h, false));

  }
}

int Camera::StringToInt(const xmlChar *in, const xmlChar *tag, const char* attribute) {
  int i;

#if defined(__unix__) || defined(__APPLE__) || defined(__MINGW32__)
  if (EOF == sscanf((const char*)in, "%d", &i))
#else
  if (EOF == sscanf_s((const char*)in, "%d", &i))
#endif
    ThrowCME("Error parsing attribute %s in tag %s, in camera %s %s.", attribute, tag, make.c_str(), model.c_str());

  return i;
}

vector<int> Camera::MultipleStringToInt(const xmlChar *in, const xmlChar *tag, const char* attribute) {
  int i;
  vector<int> ret;
  vector<string> v = split_string(string((const char*)in), ' ');

  for (uint32 j = 0; j < v.size(); j++) {
#if defined(__unix__) || defined(__APPLE__) || defined(__MINGW32__)
    if (EOF == sscanf(v[j].c_str(), "%d", &i))
#else
    if (EOF == sscanf_s(v[j].c_str(), "%d", &i))
#endif
      ThrowCME("Error parsing attribute %s in tag %s, in camera %s %s.", attribute, tag, make.c_str(), model.c_str());
    ret.push_back(i);
  }
  return ret;
}


int Camera::getAttributeAsInt(xmlNodePtr cur , const xmlChar *tag, const char* attribute) {
  xmlChar *key = xmlGetProp(cur, (const xmlChar *)attribute);

  if (!key)
    ThrowCME("Could not find attribute %s in tag %s, in camera %s %s.", attribute, tag, make.c_str(), model.c_str());

  try {
    int i = StringToInt(key, tag, attribute);
    xmlFree(key);
    return i;
  } catch (CameraMetadataException &e) {
    xmlFree(key);
    throw e;
  }

 /* Never actually reachable */
  return 0;
}

void Camera::parseAlias( xmlDocPtr doc, xmlNodePtr cur )
{
  if (!xmlStrcmp(cur->name, (const xmlChar *) "Alias")) {
    cur = cur->xmlChildrenNode;
    while (NULL != cur) {
      if (cur && !xmlStrcmp(cur->name, (const xmlChar *) "text"))
        aliases.push_back(string((const char*)cur->content));
      cur = cur->next;
    }
  }
}

void Camera::parseHint( xmlDocPtr doc, xmlNodePtr cur )
{
  if (!xmlStrcmp(cur->name, (const xmlChar *) "Hint")) {
    xmlChar *key;
    string hint_name, hint_value;
    key = xmlGetProp(cur, (const xmlChar *)"name");
    if (key) {
      hint_name = string((const char*)key);
      xmlFree(key);
    }
    else 
      ThrowCME("CameraMetadata: Could not find name for hint for %s %s camera.", make.c_str(), model.c_str());

    key = xmlGetProp(cur, (const xmlChar *)"value");
    if (key) {
      hint_value = string((const char*)key);
      xmlFree(key);
    }
    else 
      ThrowCME("CameraMetadata: Could not find value for hint %s for %s %s camera.", hint_name.c_str(), make.c_str(), model.c_str());

    hints.insert(make_pair(hint_name, hint_value));
  }
}

void Camera::parseSensorInfo( xmlDocPtr doc, xmlNodePtr cur )
{
  int min_iso = 0;
  int max_iso = 0;
  int black = getAttributeAsInt(cur, cur->name, "black");
  int white = getAttributeAsInt(cur, cur->name, "white");

  xmlChar *key = xmlGetProp(cur, (const xmlChar *)"iso_min");
  if (key) {
    min_iso = StringToInt(key, cur->name, "iso_min");
    xmlFree(key);
  }

  key = xmlGetProp(cur, (const xmlChar *)"iso_max");
  if (key) {
    max_iso = StringToInt(key, cur->name, "iso_max");
    xmlFree(key);
  }
  key = xmlGetProp(cur, (const xmlChar *)"black_colors");
  vector<int> black_colors;
  if (key) {
    black_colors = MultipleStringToInt(key, cur->name, "black_colors");
    xmlFree(key);
  }
  key = xmlGetProp(cur, (const xmlChar *)"iso_list");
  if (key) {
    vector<int> values = MultipleStringToInt(key, cur->name, "iso_list");
    xmlFree(key);
    if (!values.empty()) {
      for (uint32 i = 0; i < values.size(); i++) {
        sensorInfo.push_back(CameraSensorInfo(black, white, values[i], values[i], black_colors));
      }      
    }
  } else {
    sensorInfo.push_back(CameraSensorInfo(black, white, min_iso, max_iso, black_colors));
  }
}

const CameraSensorInfo* Camera::getSensorInfo( int iso )
{
  /* If only one, just return that */
  if (sensorInfo.size() == 1)
    return &sensorInfo[0];

  vector<CameraSensorInfo*> candidates;
  vector<CameraSensorInfo>::iterator i = sensorInfo.begin();
  do 
  {
    if (i->isIsoWithin(iso))
      candidates.push_back(&(*i));
  } while (++i != sensorInfo.end());

  if (candidates.size() == 1)
    return candidates[0];

  vector<CameraSensorInfo*>::iterator j = candidates.begin();
  do 
  {
    if (!(*j)->isDefault())
      return *j;
  } while (++j != candidates.end());
  /* Several defaults??? Just return first */
  return candidates[0];
}

} // namespace RawSpeed
