#ifndef FOX_RIVER_WORD_ASSEMBLER_H
#define FOX_RIVER_WORD_ASSEMBLER_H

#include "CppJieba/MPSegment.hpp"
#include "CppJieba/KeywordExtractor.hpp"

namespace FoxRiver
{
    const size_t KEYWORD_TOP_N = 5;
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

    class WordAssembler
    {
        private:
            KeywordExtractor _keywordExtractor;
            set<string> _locationSet;
        public:
            WordAssembler(const string& dictPath, const string& hmmPath, const string& idfPath, const string& stopwordPath , const string& locationDictPath): _keywordExtractor(dictPath, hmmPath, idfPath, stopwordPath)
            {
                _loadSet(locationDictPath, _locationSet);
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

                res << keyInfo;
                return true;
            }
        private:
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
