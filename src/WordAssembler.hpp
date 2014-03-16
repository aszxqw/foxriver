#ifndef FOX_RIVER_WORD_ASSEMBLER_H
#define FOX_RIVER_WORD_ASSEMBLER_H

#include "CppJieba/MPSegment.hpp"
#include "CppJieba/KeywordExtractor.hpp"

namespace FoxRiver
{
    const size_t KEYWORD_TOP_N = 5;
    using namespace CppJieba;
    class WordAssembler
    {
        private:
            KeywordExtractor _keywordExtractor;
        public:
            WordAssembler(const string& dictPath, const string& hmmPath, const string& idfPath, const string& stopwordPath): _keywordExtractor(dictPath, hmmPath, idfPath, stopwordPath){}
            ~WordAssembler()
            {}
        public:
            bool run(const string& sentence, string& res) const
            {
                vector<string> words;
                if(!_keywordExtractor.extract(sentence, words, KEYWORD_TOP_N))
                {
                    LogError("[%s] extract failed.", sentence.c_str());
                    return false;
                }

                return true;
            }
        public:
            
    };
}

#endif
