#ifndef FOX_RIVER_WORD_ASSEMBLER_H
#define FOX_RIVER_WORD_ASSEMBLER_H

#include "CppJieba/MPSegment.hpp"
#include "CppJieba/KeywordExtractor.hpp"
#include "deps/tinyxml2/tinyxml2.h"

namespace FoxRiver
{
    const size_t KEYWORD_TOP_N = 20;
    const size_t TIME_DICT_COLUMN_NUM = 2;
    
    using namespace CppJieba;


    struct CityInfo
    {
        string code;
        string id;
        string name;
    };
    struct LocationInfo
    {
        string id;
        string name;
        string cityId;
    };

    inline ostream& operator << (ostream& os, const CityInfo& cityInfo)
    {
        return os << string_format("{code: %s, id:%s, name: %s}", cityInfo.code.c_str(), cityInfo.id.c_str(), cityInfo.name.c_str());
    }
    
    typedef unordered_map<string, const CityInfo*> CityInfoIndexType;
    typedef unordered_map<string, const LocationInfo*> LocationInfoIndexType;

    struct KeyInfo
    {
        string time;
        CityInfo cityInfo;
        LocationInfo locationInfo;
    };

    inline ostream& operator << (ostream& os, const KeyInfo& keyInfo)
    {
        return os << string_format("{\"time\": \"%s\", \"cityName\": \"%s\", \"cityId\": \"%s\", \"cityCode\": \"%s\", \"locationId\": \"%s\", \"locationName\":\"%s\", \"locationCityId\":\"%s\"}", keyInfo.time.c_str(), keyInfo.cityInfo.name.c_str(), keyInfo.cityInfo.id.c_str(), keyInfo.cityInfo.code.c_str(), keyInfo.locationInfo.id.c_str(), keyInfo.locationInfo.name.c_str(), keyInfo.locationInfo.cityId.c_str());
    }


    typedef unordered_map<string, int> TimeMapType;

    class WordAssembler
    {
        private:
            KeywordExtractor _keywordExtractor;
            TimeMapType _timeMap;
            vector<CityInfo> _cityInfos;
            CityInfoIndexType _cityNameIndex;
            vector<LocationInfo> _locationInfos;
            LocationInfoIndexType _locationNameIndex;
            
        public:
            const vector<CityInfo>& getCityInfos() const
            {
                return _cityInfos;
            }
            const CityInfoIndexType& getCityNameIndex() const 
            {
                return _cityNameIndex;
            }
        public:
            WordAssembler(const string& dictPath, const string& hmmPath, const string& idfPath, const string& stopwordPath , const string& timeDictPath, const string& cityDictPath, const string& locationDictPath): _keywordExtractor(dictPath, hmmPath, idfPath, stopwordPath)
            {
                //_loadSet(locationDictPath, _locationSet);
                _loadTimeMap(timeDictPath, _timeMap);
                _loadCityDict(cityDictPath, _cityInfos, _cityNameIndex);
                _loadLocationDict(locationDictPath, _locationInfos, _locationNameIndex);
            }
            ~WordAssembler()
            {}
        public:
            bool run(const string& sentence, string& res) const
            {
                KeyInfo keyInfo;
                vector<string> words;
                if(!_keywordExtractor.extract(sentence, words, KEYWORD_TOP_N))
                {
                    LogError("[%s] extract failed.", sentence.c_str());
                    return false;
                }

                _findCityInfo(words, keyInfo.cityInfo);
                _findLocationInfo(words, keyInfo.locationInfo);
                _findTime(words,keyInfo.time);

                res << keyInfo;
                return true;
            }
            bool convertTime(const string& commonTime, string& timeStr) const
            {
                TimeMapType::const_iterator citer = _timeMap.find(commonTime);
                if(_timeMap.end() == citer)
                {
                    return false;
                }
                int dis = citer->second;
                time_t t;
                time(&t);
                t += dis * 24 * 3600;
                timeStr.resize(9);
                strftime((char*)(timeStr.c_str()), timeStr.size(), "%Y%m%d", localtime(&t));
                return true;
            }
        private:
            void _loadTimeMap(const string& filePath, TimeMapType& mp) const
            {
                ifstream ifs(filePath.c_str());
                if(!ifs)
                {
                    LogFatal("open [%s] failed.", filePath.c_str());
                    assert(false);
                }
                mp.clear();
                
                string line;
                vector<string> buf;
                for(size_t lineno = 0; getline(ifs, line); lineno++)
                {
                    if(!split(line, buf, " ") || buf.size() != TIME_DICT_COLUMN_NUM)
                    {
                        LogError("line[%u:%s]", lineno, line.c_str());
                        continue;
                    }
                    
                    mp[buf[0]] = atoi(buf[1].c_str());
                }
                assert(mp.size());
            }
        private:
            void _loadCityDict(const string& filePath, vector<CityInfo>& cityInfos, CityInfoIndexType& cityNameIndex) const
            {
                tinyxml2::XMLDocument doc;
                tinyxml2::XMLError xmlError = doc.LoadFile(filePath.c_str());
                assert(xmlError == tinyxml2::XML_SUCCESS);
                tinyxml2::XMLElement * element = NULL;
                tinyxml2::XMLElement * elementTmp = NULL;
                const char* str = NULL;
                element = doc.FirstChildElement("CityDetails");
                assert(element);
                element = element->FirstChildElement("CityDetail");
                assert(element);
                CityInfo cityInfo;
                cityInfos.clear();
                cityNameIndex.clear();
                while(element)
                {
                    elementTmp = element->FirstChildElement("CityCode");
                    assert(elementTmp);
                    str = elementTmp->GetText();
                    if(str)
                    {
                        cityInfo.code = str;
                    }
                    else
                    {
                        cityInfo.code.clear();
                    }
                    
                    elementTmp = element->FirstChildElement("City");
                    assert(elementTmp);
                    str = elementTmp->GetText();
                    assert(str);
                    cityInfo.id = str;
                    
                    elementTmp = element->FirstChildElement("CityName");
                    assert(elementTmp);
                    str = elementTmp->GetText();
                    assert(str);
                    cityInfo.name = str;

                    cityInfos.push_back(cityInfo);
                    
                    element = element->NextSiblingElement("CityDetail");
                }
                for(size_t i = 0; i < cityInfos.size(); i ++)
                {
                    cityNameIndex[cityInfos[i].name] = &(cityInfos[i]);
                }
                LogDebug("load [%u] cityInfos ", cityInfos.size());
            }
            void _loadLocationDict(const string& filePath, vector<LocationInfo>& locInfos, LocationInfoIndexType& locNameIndex) const
            {
                tinyxml2::XMLDocument doc;
                tinyxml2::XMLError xmlError = doc.LoadFile(filePath.c_str());
                assert(xmlError == tinyxml2::XML_SUCCESS);
                tinyxml2::XMLElement * element = NULL;
                tinyxml2::XMLElement * elementTmp = NULL;
                const char* str = NULL;
                element = doc.FirstChildElement("LocationDetails");
                assert(element);
                element = element->FirstChildElement("LocationDetail");
                assert(element);
                LocationInfo locInfo;
                locInfos.clear();
                locNameIndex.clear();
                while(element)
                {
                    elementTmp = element->FirstChildElement("Location");
                    assert(elementTmp);
                    str = elementTmp->GetText();
                    assert(str);
                    locInfo.id = str;
                    
                    elementTmp = element->FirstChildElement("LocationName");
                    assert(elementTmp);
                    str = elementTmp->GetText();
                    assert(str);
                    locInfo.name = str;

                    elementTmp = element->FirstChildElement("LocationCity");
                    assert(elementTmp);
                    str = elementTmp->GetText();
                    assert(str);
                    locInfo.cityId = str;

                    locInfos.push_back(locInfo);
                    
                    element = element->NextSiblingElement("LocationDetail");
                }
                for(size_t i = 0; i < locInfos.size(); i ++)
                {
                    locNameIndex[locInfos[i].name] = &(locInfos[i]);
                }
                LogDebug("load [%u] locInfos ", locInfos.size());
            }
            bool _findTime(const vector<string>& words, string & timeStr) const
            {
                for(size_t i = 0; i < words.size(); i ++)
                {
                    if(convertTime(words[i], timeStr))
                    {
                        return true;
                    }
                }
                return false;
            }
            bool _findCityInfo(const vector<string>& words, CityInfo & cityInfo) const
            {
                CityInfoIndexType::const_iterator citer;
                for(size_t i = 0; i < words.size(); i++)
                {
                    citer = _cityNameIndex.find(words[i]);
                    if(_cityNameIndex.end() != citer)
                    {

                        cityInfo.name = citer->second->name;
                        cityInfo.id = citer->second->id;
                        cityInfo.code = citer->second->code;
                        return true;
                    }
                }
                return false;
            }
            bool _findLocationInfo(const vector<string>& words, LocationInfo & locationInfo) const
            {
                LocationInfoIndexType::const_iterator citer;
                for(size_t i = 0; i < words.size(); i++)
                {
                    citer = _locationNameIndex.find(words[i]);
                    if(_locationNameIndex.end() != citer)
                    {
                        locationInfo.name = citer->second->name;
                        locationInfo.id = citer->second->id;
                        locationInfo.cityId = citer->second->cityId;
                        return true;
                    }
                }
                return false;
            }
            //bool _findLocation(const vector<string>& words, string & location) const
            //{
            //    for(size_t i = 0; i < words.size(); i ++)
            //    {
            //        if(isIn(_locationSet, words[i]))
            //        {
            //            location = words[i];
            //            return true;
            //        }
            //    }
            //    return false;
            //}
            
    };
}

#endif
