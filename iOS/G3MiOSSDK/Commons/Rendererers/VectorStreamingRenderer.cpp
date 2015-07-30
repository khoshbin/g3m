//
//  VectorStreamingRenderer.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 7/30/15.
//
//

#include "VectorStreamingRenderer.hpp"
#include "Context.hpp"
#include "IDownloader.hpp"
#include "IJSONParser.hpp"
#include "Sector.hpp"
#include "JSONObject.hpp"
#include "JSONArray.hpp"
#include "JSONNumber.hpp"
#include "JSONString.hpp"


/*

 http://localhost:8080/server-mapboo/public/VectorialStreaming/GEONames-PopulatedPlaces_LOD/features?node=&properties=name|population|featureClass|featureCode

 http://localhost:8080/server-mapboo/public/VectorialStreaming/GEONames-PopulatedPlaces_LOD

 */

VectorStreamingRenderer::Node::~Node() {
  delete _sector;
  delete _averagePosition;
}

Sector* VectorStreamingRenderer::GEOJSONUtils::parseSector(const JSONArray* json) {
  const double lowerLat = json->getAsNumber(0)->value();
  const double lowerLon = json->getAsNumber(1)->value();
  const double upperLat = json->getAsNumber(2)->value();
  const double upperLon = json->getAsNumber(3)->value();

  return new Sector(Geodetic2D::fromDegrees(lowerLat, lowerLon),
                    Geodetic2D::fromDegrees(upperLat, upperLon));
}

Geodetic2D* VectorStreamingRenderer::GEOJSONUtils::parseGeodetic2D(const JSONArray* json) {
  const double lat = json->getAsNumber(0)->value();
  const double lon = json->getAsNumber(1)->value();

  return new Geodetic2D(Angle::fromDegrees(lat), Angle::fromDegrees(lon));
}

VectorStreamingRenderer::Node* VectorStreamingRenderer::GEOJSONUtils::parseNode(const JSONObject* json) {
  const std::string id              = json->getAsString("id")->value();
  Sector*           sector          = GEOJSONUtils::parseSector( json->getAsArray("sector") );
  int               featuresCount   = (int) json->getAsNumber("featuresCount")->value();
  Geodetic2D*       averagePosition = GEOJSONUtils::parseGeodetic2D( json->getAsArray("averagePosition") );

  std::vector<std::string> children;
  const JSONArray* childrenJSON = json->getAsArray("children");
  for (int i = 0; i < childrenJSON->size(); i++) {
    children.push_back( childrenJSON->getAsString(i)->value() );
  }

  return new Node(id, sector, featuresCount, averagePosition, children);
}

VectorStreamingRenderer::MetadataParserAsyncTask::~MetadataParserAsyncTask() {
  delete _buffer;

  delete _sector;
  delete _averagePosition;

  for (size_t i = 0; i < _rootNodes.size(); i++) {
    Node* node = _rootNodes[i];
    delete node;
  }
}

void VectorStreamingRenderer::MetadataParserAsyncTask::runInBackground(const G3MContext* context) {

  const JSONBaseObject* jsonBaseObject = IJSONParser::instance()->parse(_buffer);

  delete _buffer;
  _buffer = NULL;

  if (jsonBaseObject == NULL) {
    _parsingError = true;
    return;
  }

  const JSONObject* jsonObject = jsonBaseObject->asObject();
  if (jsonObject == NULL) {
    _parsingError = true;
  }
  else {
    _sector          = GEOJSONUtils::parseSector( jsonObject->getAsArray("sector") );
    _featuresCount   = (long long) jsonObject->getAsNumber("featuresCount")->value();
    _averagePosition = GEOJSONUtils::parseGeodetic2D( jsonObject->getAsArray("averagePosition") );
    _nodesCount      = (int) jsonObject->getAsNumber("featuresCount")->value();
    _minNodeDepth    = (int) jsonObject->getAsNumber("minNodeDepth")->value();
    _maxNodeDepth    = (int) jsonObject->getAsNumber("maxNodeDepth")->value();

    const JSONArray* rootNodesJSON = jsonObject->getAsArray("rootNodes");
    for (int i = 0; i < rootNodesJSON->size(); i++) {
      Node* node = GEOJSONUtils::parseNode( rootNodesJSON->getAsObject(i) );
      _rootNodes.push_back(node);
    }
  }

  delete jsonBaseObject;
}

void VectorStreamingRenderer::MetadataParserAsyncTask::onPostExecute(const G3MContext* context) {
  if (_parsingError) {
    _vectorSet->errorParsingMetadata();
  }
  else {
    _vectorSet->parsedMetadata(_sector,
                               _featuresCount,
                               _averagePosition,
                               _nodesCount,
                               _minNodeDepth,
                               _maxNodeDepth,
                               _rootNodes);
    _sector          = NULL; // moved ownership to pointCloud
    _averagePosition = NULL; // moved ownership to pointCloud
    _rootNodes.clear();      // moved ownership to pointCloud
  }
}



void VectorStreamingRenderer::MetadataDownloadListener::onDownload(const URL& url,
                                                                   IByteBuffer* buffer,
                                                                   bool expired) {
  if (_verbose) {
#ifdef C_CODE
    ILogger::instance()->logInfo("Downloaded metadata for \"%s\" (bytes=%ld)",
                                 _vectorSet->getName().c_str(),
                                 buffer->size());
#endif
#ifdef JAVA_CODE
    ILogger.instance().logInfo("Downloaded metadata for \"%s\" (bytes=%d)", _vectorSet.getName(), buffer.size());
#endif
  }

  _threadUtils->invokeAsyncTask(new MetadataParserAsyncTask(_vectorSet, buffer, _verbose),
                                true);
}

void VectorStreamingRenderer::MetadataDownloadListener::onError(const URL& url) {
  _vectorSet->errorDownloadingMetadata();
}

void VectorStreamingRenderer::MetadataDownloadListener::onCancel(const URL& url) {
  // do nothing
}

void VectorStreamingRenderer::MetadataDownloadListener::onCanceledDownload(const URL& url,
                                                                           IByteBuffer* buffer,
                                                                           bool expired) {
  // do nothing
}

void VectorStreamingRenderer::VectorSet::errorDownloadingMetadata() {
  _downloadingMetadata = false;
  _errorDownloadingMetadata = true;
}

void VectorStreamingRenderer::VectorSet::errorParsingMetadata() {
  _downloadingMetadata = false;
  _errorParsingMetadata = true;
}

void VectorStreamingRenderer::VectorSet::parsedMetadata(Sector* sector,
                                                        long long featuresCount,
                                                        Geodetic2D* averagePosition,
                                                        int nodesCount,
                                                        int minNodeDepth,
                                                        int maxNodeDepth,
                                                        const std::vector<Node*>& rootNodes) {
  _downloadingMetadata = false;

  if (_verbose) {
    ILogger::instance()->logInfo("Parsed metadata for \"%s\"", _name.c_str());
  }

#warning Diego at work!
}

void VectorStreamingRenderer::VectorSet::initialize(const G3MContext* context) {
  _downloadingMetadata = true;
  _errorDownloadingMetadata = false;
  _errorParsingMetadata = false;

  const URL metadataURL(_serverURL, _name);

  if (_verbose) {
    ILogger::instance()->logInfo("Downloading metadata for \"%s\"", _name.c_str());
  }

  context->getDownloader()->requestBuffer(metadataURL,
                                          _downloadPriority,
                                          _timeToCache,
                                          _readExpired,
                                          new VectorStreamingRenderer::MetadataDownloadListener(this,
                                                                                                context->getThreadUtils(),
                                                                                                _verbose),
                                          true);
}

RenderState VectorStreamingRenderer::VectorSet::getRenderState(const G3MRenderContext* rc) {
  if (_downloadingMetadata) {
    return RenderState::busy();
  }

  if (_errorDownloadingMetadata) {
    return RenderState::error("Error downloading metadata of \"" + _name + "\" from \"" + _serverURL.getPath() + "\"");
  }

  if (_errorParsingMetadata) {
    return RenderState::error("Error parsing metadata of \"" + _name + "\" from \"" + _serverURL.getPath() + "\"");
  }

  return RenderState::ready();
}


VectorStreamingRenderer::~VectorStreamingRenderer() {
  for (size_t i = 0; i < _vectorSetsSize; i++) {
    VectorSet* vectorSet = _vectorSets[i];
    delete vectorSet;
  }

  //  _glState->_release();
  //  delete _timer;

#ifdef JAVA_CODE
  super.dispose();
#endif
}

void VectorStreamingRenderer::onChangedContext() {
  for (int i = 0; i < _vectorSetsSize; i++) {
    VectorSet* vectorSet = _vectorSets[i];
    vectorSet->initialize(_context);
  }
}

RenderState VectorStreamingRenderer::getRenderState(const G3MRenderContext* rc) {
  _errors.clear();
  bool busyFlag  = false;
  bool errorFlag = false;

  for (int i = 0; i < _vectorSetsSize; i++) {
    VectorSet* vectorSet = _vectorSets[i];
    const RenderState childRenderState = vectorSet->getRenderState(rc);

    const RenderState_Type childRenderStateType = childRenderState._type;

    if (childRenderStateType == RENDER_ERROR) {
      errorFlag = true;

      const std::vector<std::string> childErrors = childRenderState.getErrors();
#ifdef C_CODE
      _errors.insert(_errors.end(),
                     childErrors.begin(),
                     childErrors.end());
#endif
#ifdef JAVA_CODE
      _errors.addAll(childErrors);
#endif
    }
    else if (childRenderStateType == RENDER_BUSY) {
      busyFlag = true;
    }
  }

  if (errorFlag) {
    return RenderState::error(_errors);
  }
  else if (busyFlag) {
    return RenderState::busy();
  }
  else {
    return RenderState::ready();
  }
}


void VectorStreamingRenderer::addVectorSet(const URL& serverURL,
                                           const std::string& name,
                                           long long downloadPriority,
                                           const TimeInterval& timeToCache,
                                           bool readExpired,
                                           bool verbose) {
  VectorSet* vectorSet = new VectorSet(serverURL,
                                       name,
                                       downloadPriority,
                                       timeToCache,
                                       readExpired,
                                       verbose);
  if (_context != NULL) {
    vectorSet->initialize(_context);
  }
  _vectorSets.push_back(vectorSet);
  _vectorSetsSize = _vectorSets.size();
}


void VectorStreamingRenderer::render(const G3MRenderContext* rc,
                                     GLState* glState) {
#warning Diego at work!
}
