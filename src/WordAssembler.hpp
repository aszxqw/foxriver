#ifndef FOX_RIVER_WORD_ASSEMBLER_H
#define FOX_RIVER_WORD_ASSEMBLER_H

#include "CppJieba/MPSegment.hpp"
#include "CppJieba/KeywordExtractor.hpp"

namespace FoxRiver
{
    const size_t KEYWORD_TOP_N = 5;
    const size_t TIME_DICT_COLUMN_NUM = 2;
    
    using namespace CppJieba;


    struct KeyInfo
    {
        string time;
        string location;
    };

    ostream& operator << (ostream& os, const KeyInfo& keyInfo)
    {
        return os << string_format("{\"time\": \"%s\", \"location\": \"%s\"}", keyInfo.time.c_str(), keyInfo.location.c_str());
    }

    typedef unordered_map<string, int> TimeMapType;

    class WordAssembler
    {
        private:
            KeywordExtractor _keywordExtractor;
            TimeMapType _timeMap;
            set<string> _locationSet;
        public:
            WordAssembler(const string& dictPath, const string& hmmPath, const string& idfPath, const string& stopwordPath , const string& timeDictPath, const string& locationDictPath): _keywordExtractor(dictPath, hmmPath, idfPath, stopwordPath)
            {
                _loadSet(locationDictPath, _locationSet);
                _loadTimeMap(timeDictPath, _timeMap);
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

                _findLocation(words, keyInfo.location);
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
            void _loadSet(const string& filePath, set<string>& st) const
            {
                ifstream ifs(filePath.c_str());
                if(!ifs)
                {
                    LogFatal("open [%s] failed.", filePath.c_str());
                    assert(false);
                }
                st.clear();
                string line;
                while(getline(ifs, line))
                {
                    st.insert(line);
                }
                assert(st.size());
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
            bool _findLocation(const vector<string>& words, string & location) const
            {
                for(size_t i = 0; i < words.size(); i ++)
                {
                    if(isIn(_locationSet, words[i]))
                    {
                        location = words[i];
                        return true;
                    }
                }
                return false;
            }
            
    };
}

#endif
